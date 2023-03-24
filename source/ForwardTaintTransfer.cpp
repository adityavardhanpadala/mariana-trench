/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <mariana-trench/ArtificialMethods.h>
#include <mariana-trench/CallEffects.h>
#include <mariana-trench/CallGraph.h>
#include <mariana-trench/ClassProperties.h>
#include <mariana-trench/Features.h>
#include <mariana-trench/Fields.h>
#include <mariana-trench/ForwardTaintTransfer.h>
#include <mariana-trench/FulfilledPartialKindState.h>
#include <mariana-trench/Log.h>
#include <mariana-trench/Methods.h>
#include <mariana-trench/MultiSourceMultiSinkRule.h>
#include <mariana-trench/PartialKind.h>
#include <mariana-trench/Positions.h>
#include <mariana-trench/Rules.h>
#include <mariana-trench/TransferCall.h>
#include <mariana-trench/TransformOperations.h>
#include <mariana-trench/TriggeredPartialKind.h>

namespace marianatrench {

bool ForwardTaintTransfer::analyze_default(
    MethodContext* context,
    const IRInstruction* instruction,
    ForwardTaintEnvironment* environment) {
  log_instruction(context, instruction);

  if (instruction->has_dest() || instruction->has_move_result_any()) {
    auto* memory_location = context->memory_factory.make_location(instruction);
    LOG_OR_DUMP(context, 4, "Tainting {} with {{}}", show(memory_location));
    environment->write(
        memory_location, TaintTree::bottom(), UpdateKind::Strong);
  }

  return false;
}

bool ForwardTaintTransfer::analyze_check_cast(
    MethodContext* context,
    const IRInstruction* instruction,
    ForwardTaintEnvironment* environment) {
  log_instruction(context, instruction);
  const auto& aliasing = context->aliasing.get(instruction);

  auto taint = environment->read(
      aliasing.register_memory_locations(instruction->src(0)));

  // Add via-cast feature as configured by the program options.
  auto allowed_features = context->options.allow_via_cast_features();
  if (context->options.emit_all_via_cast_features() ||
      std::find(
          allowed_features.begin(),
          allowed_features.end(),
          instruction->get_type()->str()) != allowed_features.end()) {
    auto features = FeatureMayAlwaysSet::make_always(
        {context->features.get_via_cast_feature(instruction->get_type())});
    taint.map([&features](Taint& sources) {
      sources.add_inferred_features(features);
    });
  }

  LOG_OR_DUMP(context, 4, "Tainting result register with {}", taint);
  environment->write(
      aliasing.result_memory_location(), taint, UpdateKind::Strong);

  return false;
}

bool ForwardTaintTransfer::analyze_iget(
    MethodContext* context,
    const IRInstruction* instruction,
    ForwardTaintEnvironment* environment) {
  log_instruction(context, instruction);

  const auto field_target =
      context->call_graph.resolved_field_access(context->method(), instruction);
  if (!field_target) {
    WARNING_OR_DUMP(
        context,
        3,
        "Unable to resolve access of instance field {}",
        show(instruction->get_field()));
  }

  // Read user defined field model.
  auto declared_field_model =
      field_target ? context->registry.get(field_target->field) : FieldModel();

  if (!declared_field_model.empty()) {
    const auto& aliasing = context->aliasing.get(instruction);
    LOG_OR_DUMP(
        context,
        4,
        "Tainting register {} with {}",
        k_result_register,
        declared_field_model.sources());
    environment->write(
        aliasing.result_memory_locations(),
        declared_field_model.sources(),
        UpdateKind::Weak);
  }

  return false;
}

bool ForwardTaintTransfer::analyze_sget(
    MethodContext* context,
    const IRInstruction* instruction,
    ForwardTaintEnvironment* environment) {
  log_instruction(context, instruction);
  const auto& aliasing = context->aliasing.get(instruction);

  const auto field_target =
      context->call_graph.resolved_field_access(context->method(), instruction);
  if (!field_target) {
    WARNING_OR_DUMP(
        context,
        3,
        "Unable to resolve access of static field {}",
        show(instruction->get_field()));
  }
  auto field_model =
      field_target ? context->registry.get(field_target->field) : FieldModel();
  LOG_OR_DUMP(
      context,
      4,
      "Tainting register {} with {}",
      k_result_register,
      field_model.sources());
  environment->write(
      aliasing.result_memory_location(),
      TaintTree(field_model.sources()),
      UpdateKind::Strong);

  return false;
}

namespace {

void apply_generations(
    MethodContext* context,
    const InstructionAliasResults& aliasing,
    ForwardTaintEnvironment* environment,
    const IRInstruction* instruction,
    const CalleeModel& callee,
    TaintTree& result_taint) {
  LOG_OR_DUMP(
      context,
      4,
      "Processing generations for call to `{}`",
      show(callee.method_reference));

  for (const auto& [root, generations] : callee.model.generations()) {
    switch (root.kind()) {
      case Root::Kind::Return: {
        LOG_OR_DUMP(context, 4, "Tainting invoke result with {}", generations);
        result_taint.join_with(generations);
        break;
      }
      case Root::Kind::Argument: {
        auto parameter_position = root.parameter_position();
        auto register_id = instruction->src(parameter_position);
        LOG_OR_DUMP(
            context,
            4,
            "Tainting register {} with {}",
            register_id,
            generations);
        environment->write(
            aliasing.register_memory_locations(register_id),
            generations,
            UpdateKind::Weak);
        break;
      }
      default:
        mt_unreachable();
    }
  }
}

void apply_add_features_to_arguments(
    MethodContext* context,
    const InstructionAliasResults& aliasing,
    const ForwardTaintEnvironment* previous_environment,
    ForwardTaintEnvironment* new_environment,
    const IRInstruction* instruction,
    const CalleeModel& callee) {
  if (!callee.model.add_via_obscure_feature() &&
      !callee.model.has_add_features_to_arguments()) {
    return;
  }

  for (std::size_t parameter_position = 0,
                   number_parameters = instruction->srcs_size();
       parameter_position < number_parameters;
       parameter_position++) {
    auto parameter = Root(Root::Kind::Argument, parameter_position);
    auto features = FeatureMayAlwaysSet::make_always(
        callee.model.add_features_to_arguments(parameter));
    const auto* position = !features.empty()
        ? context->positions.get(callee.position, parameter, instruction)
        : nullptr;
    if (callee.model.add_via_obscure_feature()) {
      features.add_always(context->features.get("via-obscure"));
    }

    if (features.empty()) {
      continue;
    }

    auto register_id = instruction->src(parameter_position);
    auto memory_locations = aliasing.register_memory_locations(register_id);
    for (auto* memory_location : memory_locations.elements()) {
      auto taint = previous_environment->read(memory_location);
      taint.map([&features, position](Taint& sources) {
        sources.add_inferred_features_and_local_position(features, position);
      });
      // This is using a strong update, since a weak update would turn
      // the always-features we want to add into may-features.
      new_environment->write(
          memory_location, std::move(taint), UpdateKind::Strong);
    }
  }
}

void apply_propagations(
    MethodContext* context,
    const InstructionAliasResults& aliasing,
    const ForwardTaintEnvironment* previous_environment,
    ForwardTaintEnvironment* new_environment,
    const IRInstruction* instruction,
    const CalleeModel& callee,
    const std::vector<std::optional<std::string>>& source_constant_arguments,
    TaintTree& result_taint) {
  LOG_OR_DUMP(
      context,
      4,
      "Processing propagations for call to `{}`",
      show(callee.method_reference));

  for (const auto& [input_path, propagations] :
       callee.model.propagations().elements()) {
    LOG_OR_DUMP(context, 4, "Processing propagations from {}", input_path);
    if (!input_path.root().is_argument()) {
      WARNING_OR_DUMP(
          context,
          2,
          "Ignoring propagation with a return input: {}",
          input_path);
      continue;
    }

    auto input_parameter_position = input_path.root().parameter_position();
    if (input_parameter_position >= instruction->srcs_size()) {
      WARNING(
          2,
          "Model for method `{}` contains a port on parameter {} but the method only has {} parameters. Skipping...",
          input_parameter_position,
          show(callee.method_reference),
          instruction->srcs_size());
      continue;
    }

    auto input_register_id = instruction->src(input_parameter_position);
    auto input_taint_tree = previous_environment->read(
        aliasing.register_memory_locations(input_register_id),
        input_path.path().resolve(source_constant_arguments));

    if (input_taint_tree.is_bottom()) {
      continue;
    }

    // Collapsing the tree here is required for correctness and performance.
    // Propagations can be collapsed, which results in taking the common
    // prefix of the input paths. Because of this, if we don't collapse
    // here, we might build invalid trees. See the end-to-end test
    // `propagation_collapse` for an example.
    // However, collapsing leads to FP with the builder pattern.
    // eg:
    // class A {
    //   private String s1;
    //
    //   public A setS1(String s) {
    //     this.s1 = s;
    //     return this;
    //   }
    // }
    // In this case, collapsing propagations results in entire `this` being
    // tainted. For chained calls, it can lead to FP.
    // `no-collapse-on-propagation` mode is used to prevent such cases.
    // See the end-to-end test `no_collapse_on_propagation` for example.
    if (!callee.model.no_collapse_on_propagation()) {
      LOG_OR_DUMP(context, 4, "Collapsing taint tree {}", input_taint_tree);
      input_taint_tree.collapse_inplace(
          /* transform */ [context](Taint& taint) {
            taint.add_inferred_features(FeatureMayAlwaysSet{
                context->features.get_propagation_broadening_feature()});
          });
    }

    auto position =
        context->positions.get(callee.position, input_path.root(), instruction);

    for (const auto& propagation : propagations.frames_iterator()) {
      LOG_OR_DUMP(
          context,
          4,
          "Processing propagation from {} to {}",
          input_path,
          propagation);

      auto propagation_info =
          transforms::apply_propagation(context, propagation, input_taint_tree);

      auto output_root = propagation_info.propagation_kind->root();
      FeatureMayAlwaysSet features = FeatureMayAlwaysSet::make_always(
          callee.model.add_features_to_arguments(output_root));
      features.add(propagation.features());
      features.add_always(
          callee.model.add_features_to_arguments(input_path.root()));

      propagation_info.output_taint_tree.map(
          [&features, position](Taint& taints) {
            taints.add_inferred_features_and_local_position(features, position);
          });

      for (const auto& [output_path, _] :
           propagation.output_paths().elements()) {
        auto output_path_resolved =
            output_path.resolve(source_constant_arguments);

        switch (output_root.kind()) {
          case Root::Kind::Return: {
            LOG_OR_DUMP(
                context,
                4,
                "Tainting invoke result path {} with {}",
                output_path_resolved,
                propagation_info.output_taint_tree);
            result_taint.write(
                output_path_resolved,
                propagation_info.output_taint_tree,
                UpdateKind::Weak);
            break;
          }
          case Root::Kind::Argument: {
            auto output_parameter_position = output_root.parameter_position();
            auto output_register_id =
                instruction->src(output_parameter_position);
            LOG_OR_DUMP(
                context,
                4,
                "Tainting register {} path {} with {}",
                output_register_id,
                output_path_resolved,
                propagation_info.output_taint_tree);
            new_environment->write(
                aliasing.register_memory_locations(output_register_id),
                output_path_resolved,
                propagation_info.output_taint_tree,
                callee.model.strong_write_on_propagation() ? UpdateKind::Strong
                                                           : UpdateKind::Weak);
            break;
          }
          default:
            mt_unreachable();
        }
      }
    }
  }
}

void create_issue(
    MethodContext* context,
    Taint source,
    Taint sink,
    const Rule* rule,
    const Position* position,
    TextualOrderIndex sink_index,
    std::string_view callee,
    const FeatureMayAlwaysSet& extra_features) {
  std::unordered_set<const Kind*> kinds;
  for (const auto& frame : source.frames_iterator()) {
    kinds.emplace(frame.kind());
  }

  source.add_inferred_features(
      context->class_properties.issue_features(context->method(), kinds));

  sink.add_inferred_features(extra_features);
  auto issue = Issue(
      Taint{std::move(source)},
      Taint{std::move(sink)},
      rule,
      callee,
      sink_index,
      position);
  LOG_OR_DUMP(context, 4, "Found issue: {}", issue);
  context->new_model.add_issue(std::move(issue));
}

// Called when a source is detected to be flowing into a partial sink for a
// multi source rule. The set of fulfilled sinks should be accumulated for
// each argument at a callsite (an invoke instruction).
void check_multi_source_multi_sink_rules(
    MethodContext* context,
    const Kind* source_kind,
    const Taint& source,
    const PartialKind* sink_kind,
    const Taint& sink,
    FulfilledPartialKindState& fulfilled_partial_sinks,
    const MultiSourceMultiSinkRule* rule,
    const Position* position,
    TextualOrderIndex sink_index,
    std::string_view callee,
    const FeatureMayAlwaysSet& extra_features) {
  // Features found by this branch of the multi-source-sink flow. Should be
  // reported as part of the final issue discovered.
  auto features = source.features_joined();
  features.add(sink.features_joined());

  auto issue_sink_frame = fulfilled_partial_sinks.fulfill_kind(
      sink_kind, rule, features, sink, context->kinds);

  if (issue_sink_frame) {
    create_issue(
        context,
        source,
        *issue_sink_frame,
        rule,
        position,
        sink_index,
        callee,
        extra_features);
  } else {
    LOG_OR_DUMP(
        context,
        4,
        "Found source kind: {} flowing into partial sink: {}, rule code: {}",
        *source_kind,
        *sink_kind,
        rule->code());
  }
}

void add_inferred_sink(
    MethodContext* context,
    AccessPath sink_caller_port,
    Taint new_sinks,
    FeatureMayAlwaysSet widen_broadening_features) {
  LOG_OR_DUMP(
      context, 4, "Inferred sink for port {}: {}", sink_caller_port, new_sinks);
  context->new_model.add_inferred_sinks(
      sink_caller_port, new_sinks, widen_broadening_features);
}

void create_sinks(
    MethodContext* context,
    const Taint& sources,
    const Taint& sinks,
    const FeatureMayAlwaysSet& extra_features,
    const FulfilledPartialKindState& fulfilled_partial_sinks) {
  if (sources.is_bottom() || sinks.is_bottom()) {
    return;
  }

  auto partitioned_by_artificial_sources =
      sources.partition_by_kind<bool>([&](const Kind* kind) {
        return kind->discard_transforms() == Kinds::artificial_source();
      });

  auto artificial_sources = partitioned_by_artificial_sources.find(true);
  if (artificial_sources == partitioned_by_artificial_sources.end()) {
    // Sinks are created when artificial sources are found flowing into them.
    // No artificial sources, therefore no sinks.
    return;
  }

  for (const auto& artificial_source :
       artificial_sources->second.frames_iterator()) {
    const auto* artificial_kind = artificial_source.kind();
    mt_assert(artificial_kind != nullptr);

    auto new_sinks = transforms::get_sink_for_artificial_source(
        context, sinks, artificial_kind, fulfilled_partial_sinks);

    const auto callee_port = artificial_source.callee_port();
    auto features = extra_features;
    features.add_always(
        context->previous_model.attach_to_sinks(callee_port.root()));
    features.add(artificial_source.features());

    new_sinks.add_inferred_features(features);

    // local_positions() are specific to the callee position. Normally,
    // combining all of a Taint's local positions would be odd, but this method
    // creates sinks and should always be called for the same call position
    // (where the sink is).
    new_sinks.set_local_positions(artificial_sources->second.local_positions());

    FeatureMayAlwaysSet widen_broadening_features{
        context->features.get_widen_broadening_feature()};
    if (artificial_kind->is<TransformKind>()) {
      // `callee_port` for transformed artificial source includes the `path`
      add_inferred_sink(
          context, callee_port, new_sinks, widen_broadening_features);

      continue;
    }

    const auto input_paths_per_root = artificial_sources->second.input_paths();
    for (const auto& [input_path, _] :
         input_paths_per_root.get(callee_port.root()).elements()) {
      add_inferred_sink(
          context,
          AccessPath(callee_port.root(), input_path),
          new_sinks,
          widen_broadening_features);
    }
  }
}

// Checks if the given sources/sinks fulfill any rule. If so, create an issue.
//
// If fulfilled_partial_sinks is non-null, also checks for multi-source rules
// (partial rules). If a partial rule is fulfilled, this converts a partial
// sink to a triggered sink and accumulates this list of triggered sinks. How
// these sinks should be handled depends on what happens at other sinks/ports
// within the same callsite/invoke. The caller MUST accumulate triggered sinks
// at the callsite then call create_sinks. Regular sinks are also not created in
// this mode.
//
// If fulfilled_partial_sinks is null, regular sinks will be created if an
// artificial source is found to be flowing into a sink.
void check_sources_sinks_flows(
    MethodContext* context,
    const Taint& sources,
    const Taint& sinks,
    const Position* position,
    TextualOrderIndex sink_index,
    std::string_view callee,
    const FeatureMayAlwaysSet& extra_features,
    FulfilledPartialKindState* MT_NULLABLE fulfilled_partial_sinks) {
  if (sources.is_bottom() || sinks.is_bottom()) {
    return;
  }

  auto sources_by_kind = sources.partition_by_kind();
  auto sinks_by_kind = sinks.partition_by_kind();
  for (const auto& [source_kind, source_taint] : sources_by_kind) {
    if (source_kind->discard_transforms() == Kinds::artificial_source()) {
      continue;
    }

    for (const auto& [sink_kind, sink_taint] : sinks_by_kind) {
      // Check if this satisfies any rule. If so, create the issue.
      const auto& rules = context->rules.rules(source_kind, sink_kind);
      for (const auto* rule : rules) {
        create_issue(
            context,
            source_taint,
            sink_taint,
            rule,
            position,
            sink_index,
            callee,
            extra_features);
      }

      // Check if this satisfies any partial (multi-source/sink) rule.
      if (fulfilled_partial_sinks != nullptr) {
        const auto* MT_NULLABLE partial_sink = sink_kind->as<PartialKind>();
        if (partial_sink != nullptr) {
          const auto& partial_rules =
              context->rules.partial_rules(source_kind, partial_sink);
          for (const auto* partial_rule : partial_rules) {
            check_multi_source_multi_sink_rules(
                context,
                source_kind,
                source_taint,
                partial_sink,
                sink_taint,
                *fulfilled_partial_sinks,
                partial_rule,
                position,
                // TODO(T120190935) Add the ability to hold multiple callee
                // ports per issue handle for multi-source multi-sink rules.
                sink_index,
                callee,
                extra_features);
          }
        }
      }
    }
  }

  if (fulfilled_partial_sinks == nullptr) {
    create_sinks(
        context,
        sources,
        sinks,
        extra_features,
        /* fulfilled_partial_sinks */ {});
  }
}

void check_call_flows(
    MethodContext* context,
    const InstructionAliasResults& aliasing,
    const ForwardTaintEnvironment* environment,
    const std::function<std::optional<Register>(ParameterPosition)>&
        get_parameter_register,
    const CalleeModel& callee,
    const std::vector<std::optional<std::string>>& source_constant_arguments,
    const FeatureMayAlwaysSet& extra_features) {
  LOG_OR_DUMP(
      context,
      4,
      "Processing sinks for call to `{}`",
      show(callee.method_reference));

  FulfilledPartialKindState fulfilled_partial_sinks;
  std::vector<std::tuple<Taint, const Taint&>> sources_sinks;

  for (const auto& [port, sinks] : callee.model.sinks().elements()) {
    if (!port.root().is_argument()) {
      continue;
    }

    auto register_id = get_parameter_register(port.root().parameter_position());
    if (!register_id) {
      continue;
    }

    Taint sources =
        environment
            ->read(
                aliasing.register_memory_locations(*register_id),
                port.path().resolve(source_constant_arguments))
            .collapse(/* transform */ [context](Taint& taint) {
              return taint.add_inferred_features_to_real_sources(
                  FeatureMayAlwaysSet{
                      context->features.get_issue_broadening_feature()});
            });
    check_sources_sinks_flows(
        context,
        sources,
        sinks,
        callee.position,
        /* sink_index */ callee.call_index,
        /* callee */ callee.resolved_base_method
            ? callee.resolved_base_method->show()
            : std::string(k_unresolved_callee),
        extra_features,
        &fulfilled_partial_sinks);

    sources_sinks.push_back(
        std::make_tuple(std::move(sources), std::cref(sinks)));
  }

  // Create the sinks, checking at each point, if any partial sinks should
  // become triggered. This must not happen in the loop above because we need
  // the full set of triggered sinks at all positions/port of the callsite.
  //
  // Example: callsite(partial_sink_A, triggered_sink_B).
  // Scenario: triggered_sink_B discovered in check_sources_sinks_flows above
  // when a source flows into the argument.
  //
  // This next loop needs that information to convert partial_sink_A into a
  // triggered sink to be propagated if it is reachable via artifical sources.
  //
  // Outside of multi-source rules, this also creates regular sinks for the
  // method if an artificial source is found flowing into a sink.
  for (const auto& [sources, sinks] : sources_sinks) {
    create_sinks(
        context, sources, sinks, extra_features, fulfilled_partial_sinks);
  }
}

void check_call_flows(
    MethodContext* context,
    const InstructionAliasResults& aliasing,
    const ForwardTaintEnvironment* environment,
    const std::vector<Register>& instruction_sources,
    const CalleeModel& callee,
    const std::vector<std::optional<std::string>>& source_constant_arguments,
    const FeatureMayAlwaysSet& extra_features) {
  check_call_flows(
      context,
      aliasing,
      environment,
      [&instruction_sources](
          ParameterPosition parameter_position) -> std::optional<Register> {
        if (parameter_position >= instruction_sources.size()) {
          return std::nullopt;
        }

        return instruction_sources.at(parameter_position);
      },
      callee,
      source_constant_arguments,
      extra_features);
}

void check_flows_to_array_allocation(
    MethodContext* context,
    const InstructionAliasResults& aliasing,
    ForwardTaintEnvironment* environment,
    const IRInstruction* instruction) {
  if (!context->artificial_methods.array_allocation_kind_used()) {
    return;
  }

  auto* array_allocation_method = context->methods.get(
      context->artificial_methods.array_allocation_method());
  auto* position =
      context->positions.get(context->method(), aliasing.position());
  auto array_allocation_sink = Taint{TaintConfig(
      /* kind */ context->artificial_methods.array_allocation_kind(),
      /* callee_port */ AccessPath(Root(Root::Kind::Argument, 0)),
      /* callee */ nullptr,
      /* field_callee */ nullptr,
      /* call_position */ position,
      /* distance */ 1,
      /* origins */ MethodSet{array_allocation_method},
      /* field_origins */ {},
      /* inferred features */ {},
      /* locally_inferred_features */ {},
      /* user features */ {},
      /* via_type_of_ports */ {},
      /* via_value_of_ports */ {},
      /* canonical_names */ {},
      /* input_paths */ {},
      /* output_paths */ {},
      /* local_positions */ {},
      /* call_info */ CallInfo::Origin)};
  auto array_allocation_index = context->call_graph.array_allocation_index(
      context->method(), instruction);
  for (std::size_t parameter_position = 0,
                   number_parameters = instruction->srcs_size();
       parameter_position < number_parameters;
       parameter_position++) {
    auto register_id = instruction->src(parameter_position);
    Taint sources =
        environment->read(aliasing.register_memory_locations(register_id))
            .collapse();
    // Fulfilled partial sinks ignored. No partial sinks for array allocation.
    check_sources_sinks_flows(
        context,
        sources,
        array_allocation_sink,
        position,
        /* sink_index */ array_allocation_index,
        /* callee */ array_allocation_method->show(),
        /* extra_features */ {},
        /* fulfilled_partial_sinks */ nullptr);
  }
}

void check_artificial_calls_flows(
    MethodContext* context,
    const InstructionAliasResults& aliasing,
    const IRInstruction* instruction,
    ForwardTaintEnvironment* environment,
    const std::vector<std::optional<std::string>>& source_constant_arguments =
        {}) {
  const auto& artificial_callees =
      context->call_graph.artificial_callees(context->method(), instruction);

  for (const auto& artificial_callee : artificial_callees) {
    check_call_flows(
        context,
        aliasing,
        environment,
        [&artificial_callee](
            ParameterPosition parameter_position) -> std::optional<Register> {
          auto found =
              artificial_callee.parameter_registers.find(parameter_position);
          if (found == artificial_callee.parameter_registers.end()) {
            return std::nullopt;
          }

          return found->second;
        },
        get_callee(context, artificial_callee, aliasing.position()),
        source_constant_arguments,
        FeatureMayAlwaysSet::make_always(artificial_callee.features));
  }
}

void check_call_effect_flows(
    MethodContext* context,
    const CalleeModel& callee) {
  const auto& caller_call_effect_sources =
      context->previous_model.call_effect_sources();
  if (caller_call_effect_sources.is_bottom()) {
    return;
  }

  const auto& callee_call_effect_sinks = callee.model.call_effect_sinks();
  if (callee_call_effect_sinks.is_bottom()) {
    return;
  }

  LOG(5,
      "Checking call effect flow in method {} from sources: {} to sinks: {}",
      show(callee.model.method()),
      caller_call_effect_sources,
      callee_call_effect_sinks);

  auto* position = context->positions.get(context->method());
  for (const auto& [effect, sources] : caller_call_effect_sources) {
    const auto& sinks = callee_call_effect_sinks.read(effect);
    check_sources_sinks_flows(
        context,
        // Add the position of the caller to call effect sources.
        sources.attach_position(position),
        sinks,
        callee.position,
        callee.call_index,
        /* sink_index */ callee.resolved_base_method
            ? callee.resolved_base_method->show()
            : std::string(k_unresolved_callee),
        /* extra features */ {},
        /* fulfilled partial sinks */ nullptr);
  }
}

void apply_call_effects(MethodContext* context, const CalleeModel& callee) {
  const auto& callee_call_effect_sinks = callee.model.call_effect_sinks();
  for (const auto& [effect, sinks] : callee_call_effect_sinks) {
    switch (effect.kind()) {
      case CallEffect::Kind::CALL_CHAIN: {
        LOG(5,
            "Add inferred call effect sinks {} for method: {}",
            sinks,
            show(context->method()));

        auto sinks_copy = sinks;
        context->new_model.add_inferred_call_effect_sinks(
            effect, std::move(sinks_copy));

      } break;

      default:
        mt_unreachable();
    }
  }
}

} // namespace

bool ForwardTaintTransfer::analyze_invoke(
    MethodContext* context,
    const IRInstruction* instruction,
    ForwardTaintEnvironment* environment) {
  log_instruction(context, instruction);
  const auto& aliasing = context->aliasing.get(instruction);

  auto source_constant_arguments = get_source_constant_arguments(
      aliasing.register_memory_locations_map(), instruction);
  auto callee = get_callee(
      context,
      instruction,
      aliasing.position(),
      get_source_register_types(context, instruction),
      source_constant_arguments);

  const ForwardTaintEnvironment previous_environment = *environment;
  check_call_flows(
      context,
      aliasing,
      &previous_environment,
      instruction->srcs_vec(),
      callee,
      source_constant_arguments,
      /* extra_features */ {});
  check_call_effect_flows(context, callee);
  apply_call_effects(context, callee);

  TaintTree result_taint;
  apply_add_features_to_arguments(
      context,
      aliasing,
      &previous_environment,
      environment,
      instruction,
      callee);
  apply_propagations(
      context,
      aliasing,
      &previous_environment,
      environment,
      instruction,
      callee,
      source_constant_arguments,
      result_taint);
  apply_generations(
      context, aliasing, environment, instruction, callee, result_taint);

  if (callee.resolved_base_method &&
      callee.resolved_base_method->returns_void()) {
    // No result.
  } else if (
      try_inline_invoke(
          context,
          aliasing.register_memory_locations_map(),
          instruction,
          callee) != nullptr) {
    // Since we are inlining the call, we should NOT write any taint.
    LOG_OR_DUMP(context, 4, "Inlining method call");
  } else {
    auto* memory_location = aliasing.result_memory_location();
    LOG_OR_DUMP(
        context, 4, "Tainting {} with {}", show(memory_location), result_taint);
    environment->write(memory_location, result_taint, UpdateKind::Weak);
  }

  check_artificial_calls_flows(
      context, aliasing, instruction, environment, source_constant_arguments);

  return false;
}

namespace {

void check_flows_to_field_sink(
    MethodContext* context,
    const IRInstruction* instruction,
    const TaintTree& source_taint,
    const Position* position) {
  mt_assert(
      opcode::is_an_sput(instruction->opcode()) ||
      opcode::is_an_iput(instruction->opcode()));

  if (source_taint.is_bottom()) {
    return;
  }

  const auto field_target =
      context->call_graph.resolved_field_access(context->method(), instruction);
  if (!field_target) {
    WARNING_OR_DUMP(
        context,
        3,
        "Unable to resolve access of field {} for instruction opcode {}",
        show(instruction->get_field()),
        instruction->opcode());
    return;
  }
  auto field_model =
      field_target ? context->registry.get(field_target->field) : FieldModel();
  auto sinks = field_model.sinks();
  if (sinks.empty()) {
    return;
  }
  for (const auto& [port, sources] : source_taint.elements()) {
    check_sources_sinks_flows(
        context,
        sources,
        sinks,
        position,
        /* sink_index */ field_target->field_sink_index,
        /* callee */ show(field_target->field),
        /* extra_features */ FeatureMayAlwaysSet(),
        /* fulfilled_partial_sinks */ nullptr);
  }
}

} // namespace

bool ForwardTaintTransfer::analyze_iput(
    MethodContext* context,
    const IRInstruction* instruction,
    ForwardTaintEnvironment* environment) {
  log_instruction(context, instruction);
  const auto& aliasing = context->aliasing.get(instruction);

  auto taint = environment->read(
      aliasing.register_memory_locations(instruction->src(0)));
  auto* position = context->positions.get(
      context->method(),
      aliasing.position(),
      Root(Root::Kind::Return),
      instruction);
  taint.map(
      [position](Taint& sources) { sources.add_local_position(position); });

  check_flows_to_field_sink(context, instruction, taint, position);

  // Store the taint in the memory location(s) representing the field
  auto* field_name = instruction->get_field()->get_name();
  auto target_memory_locations =
      aliasing.register_memory_locations(instruction->src(1));
  bool is_singleton = target_memory_locations.singleton() != nullptr;

  for (auto* memory_location : target_memory_locations.elements()) {
    auto* field_memory_location = memory_location->make_field(field_name);
    auto taint_copy = taint;
    add_field_features(context, taint_copy, field_memory_location);

    LOG_OR_DUMP(
        context,
        4,
        "Tainting {} with {} update kind: {}",
        show(field_memory_location),
        taint_copy,
        (is_singleton ? "Strong" : "Weak"));
    environment->write(
        field_memory_location,
        taint_copy,
        is_singleton ? UpdateKind::Strong : UpdateKind::Weak);
  }

  check_artificial_calls_flows(context, aliasing, instruction, environment);

  return false;
}

bool ForwardTaintTransfer::analyze_sput(
    MethodContext* context,
    const IRInstruction* instruction,
    ForwardTaintEnvironment* environment) {
  log_instruction(context, instruction);
  const auto& aliasing = context->aliasing.get(instruction);

  auto taint = environment->read(
      aliasing.register_memory_locations(instruction->src(0)));
  if (taint.is_bottom()) {
    return false;
  }
  auto* position = context->positions.get(
      context->method(),
      aliasing.position(),
      Root(Root::Kind::Return),
      instruction);
  taint.map(
      [position](Taint& sources) { sources.add_local_position(position); });
  check_flows_to_field_sink(context, instruction, taint, position);
  return false;
}

bool ForwardTaintTransfer::analyze_load_param(
    MethodContext* context,
    const IRInstruction* instruction,
    ForwardTaintEnvironment* environment) {
  log_instruction(context, instruction);
  const auto& aliasing = context->aliasing.get(instruction);

  auto* memory_location = aliasing.result_memory_location_or_null();
  if (memory_location == nullptr) {
    ERROR_OR_DUMP(context, 1, "Failed to deduce the parameter of a load");
    return false;
  }

  auto* parameter_memory_location =
      memory_location->dyn_cast<ParameterMemoryLocation>();
  if (parameter_memory_location == nullptr) {
    ERROR_OR_DUMP(context, 1, "Failed to deduce the parameter of a load");
    return false;
  }

  // Add parameter sources specified in model generators.
  auto root = Root(Root::Kind::Argument, parameter_memory_location->position());
  auto taint = context->previous_model.parameter_sources().read(root);

  // Add the position of the instruction to the parameter sources.
  auto* position = context->positions.get(context->method());
  taint.map([position](Taint& sources) {
    sources = sources.attach_position(position);
  });

  // Introduce an artificial parameter source in order to infer sinks and
  // propagations.
  taint.write(
      Path{}, Taint::artificial_source(AccessPath(root)), UpdateKind::Weak);

  LOG_OR_DUMP(context, 4, "Tainting {} with {}", show(memory_location), taint);
  environment->write(memory_location, std::move(taint), UpdateKind::Strong);

  return false;
}

bool ForwardTaintTransfer::analyze_move(
    MethodContext* context,
    const IRInstruction* instruction,
    ForwardTaintEnvironment* environment) {
  log_instruction(context, instruction);

  // This is a no-op for taint.
  return false;
}

bool ForwardTaintTransfer::analyze_move_result(
    MethodContext* context,
    const IRInstruction* instruction,
    ForwardTaintEnvironment* environment) {
  log_instruction(context, instruction);

  // This is a no-op for taint.
  return false;
}

bool ForwardTaintTransfer::analyze_aget(
    MethodContext* context,
    const IRInstruction* instruction,
    ForwardTaintEnvironment* environment) {
  log_instruction(context, instruction);

  // This is a no-op for taint.
  return false;
}

bool ForwardTaintTransfer::analyze_aput(
    MethodContext* context,
    const IRInstruction* instruction,
    ForwardTaintEnvironment* environment) {
  log_instruction(context, instruction);
  const auto& aliasing = context->aliasing.get(instruction);

  auto taint = environment->read(
      aliasing.register_memory_locations(instruction->src(0)));

  auto features =
      FeatureMayAlwaysSet::make_always({context->features.get("via-array")});
  auto* position = context->positions.get(
      context->method(),
      aliasing.position(),
      Root(Root::Kind::Return),
      instruction);
  taint.map([&features, position](Taint& sources) {
    sources.add_inferred_features_and_local_position(features, position);
  });

  // We use a single memory location for the array and its elements.
  LOG_OR_DUMP(
      context,
      4,
      "Tainting register {} with {}",
      instruction->src(1),
      taint,
      UpdateKind::Weak);
  environment->write(
      aliasing.register_memory_locations(instruction->src(1)),
      taint,
      UpdateKind::Weak);

  return false;
}

bool ForwardTaintTransfer::analyze_new_array(
    MethodContext* context,
    const IRInstruction* instruction,
    ForwardTaintEnvironment* environment) {
  check_flows_to_array_allocation(
      context, context->aliasing.get(instruction), environment, instruction);
  return analyze_default(context, instruction, environment);
}

bool ForwardTaintTransfer::analyze_filled_new_array(
    MethodContext* context,
    const IRInstruction* instruction,
    ForwardTaintEnvironment* environment) {
  check_flows_to_array_allocation(
      context, context->aliasing.get(instruction), environment, instruction);
  return analyze_default(context, instruction, environment);
}

namespace {

static bool analyze_numerical_operator(
    MethodContext* context,
    const IRInstruction* instruction,
    ForwardTaintEnvironment* environment) {
  log_instruction(context, instruction);
  const auto& aliasing = context->aliasing.get(instruction);

  TaintTree taint;
  for (auto register_id : instruction->srcs()) {
    taint.join_with(
        environment->read(aliasing.register_memory_locations(register_id)));
  }

  auto features = FeatureMayAlwaysSet::make_always(
      {context->features.get("via-numerical-operator")});
  auto* position = context->positions.get(
      context->method(),
      aliasing.position(),
      Root(Root::Kind::Return),
      instruction);
  taint.map([&features, position](Taint& sources) {
    sources.add_inferred_features_and_local_position(features, position);
  });

  auto* memory_location = aliasing.result_memory_location();
  LOG_OR_DUMP(context, 4, "Tainting {} with {}", show(memory_location), taint);
  environment->write(memory_location, taint, UpdateKind::Strong);

  return false;
}

} // namespace

bool ForwardTaintTransfer::analyze_unop(
    MethodContext* context,
    const IRInstruction* instruction,
    ForwardTaintEnvironment* environment) {
  return analyze_numerical_operator(context, instruction, environment);
}

bool ForwardTaintTransfer::analyze_binop(
    MethodContext* context,
    const IRInstruction* instruction,
    ForwardTaintEnvironment* environment) {
  return analyze_numerical_operator(context, instruction, environment);
}

bool ForwardTaintTransfer::analyze_binop_lit(
    MethodContext* context,
    const IRInstruction* instruction,
    ForwardTaintEnvironment* environment) {
  return analyze_numerical_operator(context, instruction, environment);
}

namespace {

void add_inferred_propagation(
    MethodContext* context,
    AccessPath input_path,
    const Kind* output_kind,
    PathTreeDomain output_paths,
    FeatureMayAlwaysSet features) {
  auto propagation = PropagationConfig(
      /* input_path */ input_path,
      /* kind */ output_kind,
      /* output_paths */ output_paths,
      /* inferred_features */ features,
      /* locally_inferred_features */ FeatureMayAlwaysSet::bottom(),
      /* user_features */ FeatureSet::bottom());

  LOG_OR_DUMP(context, 4, "Inferred propagation {}", propagation);
  context->new_model.add_inferred_propagation(
      std::move(propagation),
      /* widening_features */
      FeatureMayAlwaysSet{context->features.get_widen_broadening_feature()});
}

// Infer propagations and generations for the output `taint` on port `root`.
void infer_output_taint(
    MethodContext* context,
    Root output_root,
    const PropagationKind* propagation_kind,
    const TaintTree& taint) {
  for (const auto& [output_path, sources] : taint.elements()) {
    auto partitioned_by_artificial_sources =
        sources.partition_by_kind<bool>([&](const Kind* kind) {
          return kind->discard_transforms() == Kinds::artificial_source();
        });

    auto real_sources = partitioned_by_artificial_sources.find(false);
    if (real_sources != partitioned_by_artificial_sources.end()) {
      auto generation = real_sources->second;
      generation.add_inferred_features(FeatureMayAlwaysSet::make_always(
          context->previous_model.attach_to_sources(output_root)));
      auto port = AccessPath(output_root, output_path);
      LOG_OR_DUMP(
          context, 4, "Inferred generation for port {}: {}", port, generation);
      context->new_model.add_inferred_generations(
          std::move(port),
          std::move(generation),
          /* widening_features */
          FeatureMayAlwaysSet{
              context->features.get_widen_broadening_feature()});
    }

    auto artificial_sources = partitioned_by_artificial_sources.find(true);
    if (artificial_sources == partitioned_by_artificial_sources.end()) {
      continue;
    }

    for (const auto& artificial_source :
         artificial_sources->second.frames_iterator()) {
      const auto& input_access_path = artificial_source.callee_port();
      if (input_access_path.root() == output_root) {
        continue;
      }

      const auto* kind_to_propagate =
          transforms::get_propagation_for_artificial_source(
              context, propagation_kind, artificial_source.kind());
      if (kind_to_propagate == nullptr) {
        continue;
      }

      auto features = artificial_source.features();
      features.add_always(context->previous_model.attach_to_propagations(
          input_access_path.root()));
      features.add_always(
          context->previous_model.attach_to_propagations(output_root));

      if (kind_to_propagate->is<TransformKind>()) {
        // `callee_port` for transformed artificial source includes the `path`
        add_inferred_propagation(
            context,
            /* input_path */ input_access_path,
            /* output_kind */ kind_to_propagate,
            /* output_path */
            PathTreeDomain{{output_path, SingletonAbstractDomain()}},
            /* inferred features */ features);

        continue;
      }

      const auto input_paths_per_root =
          artificial_sources->second.input_paths();

      for (const auto& [input_path, _] :
           input_paths_per_root.get(input_access_path.root()).elements()) {
        add_inferred_propagation(
            context,
            /* input_path */ AccessPath{input_access_path.root(), input_path},
            /* output_kind */ kind_to_propagate,
            /* output_path */
            PathTreeDomain{{output_path, SingletonAbstractDomain()}},
            /* inferred features */ features);
      }
    }
  }
}

} // namespace

bool ForwardTaintTransfer::analyze_return(
    MethodContext* context,
    const IRInstruction* instruction,
    ForwardTaintEnvironment* environment) {
  log_instruction(context, instruction);
  const auto& aliasing = context->aliasing.get(instruction);

  auto return_sinks =
      context->previous_model.sinks().read(Root(Root::Kind::Return));

  // Add the position of the instruction to the return sinks.
  auto* position =
      context->positions.get(context->method(), aliasing.position());
  return_sinks.map(
      [position](Taint& sinks) { sinks = sinks.attach_position(position); });
  auto return_index =
      context->call_graph.return_index(context->method(), instruction);

  for (auto register_id : instruction->srcs()) {
    auto memory_locations = aliasing.register_memory_locations(register_id);
    infer_output_taint(
        context,
        Root(Root::Kind::Return),
        context->kinds.local_return(),
        environment->read(memory_locations));

    for (const auto& [path, sinks] : return_sinks.elements()) {
      Taint sources =
          environment->read(memory_locations, path)
              .collapse(
                  /* transform */ [context](Taint& taint) {
                    return taint.add_inferred_features_to_real_sources(
                        FeatureMayAlwaysSet{
                            context->features.get_issue_broadening_feature()});
                  });
      // Fulfilled partial sinks are not expected to be produced here. Return
      // sinks are never partial.
      check_sources_sinks_flows(
          context,
          sources,
          sinks,
          position,
          /* sink_index */ return_index,
          /* callee */ std::string(k_return_callee),
          /* extra_features */ {},
          /* fulfilled_partial_sinks */ nullptr);
    }
  }

  if (!context->method()->is_static()) {
    infer_output_taint(
        context,
        Root(Root::Kind::Argument, 0),
        context->kinds.local_receiver(),
        environment->read(context->memory_factory.make_parameter(0)));
  }

  return false;
}

} // namespace marianatrench
