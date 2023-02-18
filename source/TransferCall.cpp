/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <Show.h>

#include <mariana-trench/Log.h>
#include <mariana-trench/Positions.h>
#include <mariana-trench/TransferCall.h>

namespace marianatrench {

void log_instruction(
    const MethodContext* context,
    const IRInstruction* instruction) {
  LOG_OR_DUMP(context, 4, "Instruction: \033[33m{}\033[0m", show(instruction));
}

std::vector<const DexType * MT_NULLABLE> get_source_register_types(
    const MethodContext* context,
    const IRInstruction* instruction) {
  std::vector<const DexType* MT_NULLABLE> register_types = {};
  for (auto source_register : instruction->srcs()) {
    register_types.push_back(context->types.register_type(
        context->method(), instruction, source_register));
  }
  return register_types;
}

namespace {

std::optional<std::string> register_constant_argument(
    const MemoryLocationEnvironment& memory_location_environment,
    Register register_id) {
  MemoryLocationsDomain memory_locations =
      memory_location_environment.get(register_id);

  if (memory_locations.size() != 1) {
    return std::nullopt;
  }

  MemoryLocation* memory_location = *memory_locations.elements().begin();
  auto* instruction_memory_location =
      memory_location->dyn_cast<InstructionMemoryLocation>();
  if (instruction_memory_location == nullptr) {
    return std::nullopt;
  }

  return instruction_memory_location->get_constant();
}

} // namespace

std::vector<std::optional<std::string>> get_source_constant_arguments(
    const MemoryLocationEnvironment& memory_location_environment,
    const IRInstruction* instruction) {
  std::vector<std::optional<std::string>> constant_arguments = {};
  constant_arguments.reserve(instruction->srcs_size());

  for (auto register_id : instruction->srcs()) {
    constant_arguments.push_back(
        register_constant_argument(memory_location_environment, register_id));
  }

  return constant_arguments;
}

CalleeModel get_callee(
    const MethodContext* context,
    const IRInstruction* instruction,
    const DexPosition* MT_NULLABLE dex_position,
    const std::vector<const DexType * MT_NULLABLE>& source_register_types,
    const std::vector<std::optional<std::string>>& source_constant_arguments) {
  mt_assert(opcode::is_an_invoke(instruction->opcode()));

  auto call_target = context->call_graph.callee(context->method(), instruction);
  if (!call_target.resolved()) {
    WARNING_OR_DUMP(
        context,
        3,
        "Unable to resolve call to `{}`",
        show(instruction->get_method()));
  } else {
    LOG_OR_DUMP(
        context,
        4,
        "Call resolved to `{}`",
        show(call_target.resolved_base_callee()));
  }

  auto* position = context->positions.get(context->method(), dex_position);

  auto model = context->model_at_callsite(
      call_target, position, source_register_types, source_constant_arguments);
  LOG_OR_DUMP(context, 4, "Callee model: {}", model);

  // Avoid copies using `std::move`.
  // https://fb.workplace.com/groups/2292641227666517/permalink/2478196942444277/
  return CalleeModel{
      instruction->get_method(),
      call_target.resolved_base_callee(),
      position,
      call_target.call_index(),
      std::move(model)};
}

CalleeModel get_callee(
    const MethodContext* context,
    const ArtificialCallee& callee,
    const DexPosition* MT_NULLABLE dex_position) {
  const auto* resolved_base_callee = callee.call_target.resolved_base_callee();
  mt_assert(resolved_base_callee != nullptr);

  LOG_OR_DUMP(
      context, 4, "Artificial call to `{}`", show(resolved_base_callee));

  auto* position = context->positions.get(context->method(), dex_position);

  auto model = context->model_at_callsite(
      callee.call_target,
      position,
      /* source_register_types */ {},
      /* source_constant_arguments */ {});
  LOG_OR_DUMP(context, 4, "Callee model: {}", model);

  return CalleeModel{
      resolved_base_callee->dex_method(),
      resolved_base_callee,
      position,
      callee.call_target.call_index(),
      std::move(model)};
}

MemoryLocation* MT_NULLABLE try_inline_invoke(
    const MethodContext* context,
    const MemoryLocationEnvironment& memory_location_environment,
    const IRInstruction* instruction,
    const CalleeModel& callee) {
  auto access_path = callee.model.inline_as().get_constant();
  if (!access_path) {
    return nullptr;
  }

  auto register_id = instruction->src(access_path->root().parameter_position());
  auto memory_locations = memory_location_environment.get(register_id);
  if (memory_locations.size() != 1) {
    return nullptr;
  }

  auto memory_location = *memory_locations.elements().begin();
  for (const auto& field : access_path->path()) {
    mt_assert(field.is_field());
    memory_location = memory_location->make_field(field.name());
  }

  // Only inline if the model does not generate or propagate extra taint.
  if (!callee.model.generations().is_bottom() ||
      !callee.model.propagations().leq(TaintAccessPathTree(
          {{/* input */ *access_path,
            Taint::propagation(PropagationConfig(
                /* input_path */ *access_path,
                /* kind */ context->kinds.local_return(),
                /* output_paths */
                PathTreeDomain{{Path{}, SingletonAbstractDomain()}},
                /* inferred_features */ FeatureMayAlwaysSet(),
                /* user_features */ FeatureSet::bottom()))}})) ||
      callee.model.add_via_obscure_feature() ||
      callee.model.has_add_features_to_arguments()) {
    return nullptr;
  }

  return memory_location;
}

} // namespace marianatrench
