/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <Show.h>

#include <mariana-trench/Assert.h>
#include <mariana-trench/ForwardTaintEnvironment.h>

namespace marianatrench {

namespace {

// TODO(T144485000): In the future, this will only be necessary in the backward
// analysis.
Taint propagate_artificial_sources(Taint taint, Path::Element path_element) {
  // This is called when propagating taint down in an abstract tree.
  taint.append_to_artificial_source_input_paths(path_element);
  return taint;
}

} // namespace

ForwardTaintEnvironment::ForwardTaintEnvironment()
    : taint_(TaintEnvironment::bottom()) {}

ForwardTaintEnvironment::ForwardTaintEnvironment(TaintEnvironment taint)
    : taint_(std::move(taint)) {}

ForwardTaintEnvironment ForwardTaintEnvironment::initial() {
  return ForwardTaintEnvironment();
}

bool ForwardTaintEnvironment::is_bottom() const {
  return taint_.is_bottom();
}

bool ForwardTaintEnvironment::is_top() const {
  return taint_.is_top();
}

bool ForwardTaintEnvironment::leq(const ForwardTaintEnvironment& other) const {
  return taint_.leq(other.taint_);
}

bool ForwardTaintEnvironment::equals(
    const ForwardTaintEnvironment& other) const {
  return taint_.equals(other.taint_);
}

void ForwardTaintEnvironment::set_to_bottom() {
  taint_.set_to_bottom();
}

void ForwardTaintEnvironment::set_to_top() {
  taint_.set_to_top();
}

void ForwardTaintEnvironment::join_with(const ForwardTaintEnvironment& other) {
  mt_if_expensive_assert(auto previous = *this);
  taint_.join_with(other.taint_);
  mt_expensive_assert(previous.leq(*this) && other.leq(*this));
}

void ForwardTaintEnvironment::widen_with(const ForwardTaintEnvironment& other) {
  mt_if_expensive_assert(auto previous = *this);
  taint_.widen_with(other.taint_);
  mt_expensive_assert(previous.leq(*this) && other.leq(*this));
}

void ForwardTaintEnvironment::meet_with(const ForwardTaintEnvironment& other) {
  taint_.meet_with(other.taint_);
}

void ForwardTaintEnvironment::narrow_with(
    const ForwardTaintEnvironment& other) {
  taint_.narrow_with(other.taint_);
}

TaintTree ForwardTaintEnvironment::read(MemoryLocation* memory_location) const {
  return taint_.get(memory_location->root())
      .read(memory_location->path(), propagate_artificial_sources);
}

TaintTree ForwardTaintEnvironment::read(
    MemoryLocation* memory_location,
    const Path& path) const {
  Path full_path = memory_location->path();
  full_path.extend(path);

  return taint_.get(memory_location->root())
      .read(full_path, propagate_artificial_sources);
}

TaintTree ForwardTaintEnvironment::read(
    const MemoryLocationsDomain& memory_locations) const {
  TaintTree taint;
  for (auto* memory_location : memory_locations.elements()) {
    taint.join_with(read(memory_location));
  }
  return taint;
}

TaintTree ForwardTaintEnvironment::read(
    const MemoryLocationsDomain& memory_locations,
    const Path& path) const {
  TaintTree taint;
  for (auto* memory_location : memory_locations.elements()) {
    taint.join_with(read(memory_location, path));
  }

  return taint;
}

void ForwardTaintEnvironment::write(
    MemoryLocation* memory_location,
    TaintTree taint,
    UpdateKind kind) {
  taint_.update(memory_location->root(), [&](const TaintTree& tree) {
    auto copy = tree;
    copy.write(memory_location->path(), std::move(taint), kind);
    return copy;
  });
}

void ForwardTaintEnvironment::write(
    MemoryLocation* memory_location,
    const Path& path,
    TaintTree taint,
    UpdateKind kind) {
  Path full_path = memory_location->path();
  full_path.extend(path);

  taint_.update(memory_location->root(), [&](const TaintTree& tree) {
    auto copy = tree;
    copy.write(full_path, std::move(taint), kind);
    return copy;
  });
}

void ForwardTaintEnvironment::write(
    MemoryLocation* memory_location,
    const Path& path,
    Taint taint,
    UpdateKind kind) {
  Path full_path = memory_location->path();
  full_path.extend(path);

  taint_.update(memory_location->root(), [&](const TaintTree& tree) {
    auto copy = tree;
    copy.write(full_path, std::move(taint), kind);
    return copy;
  });
}

void ForwardTaintEnvironment::write(
    const MemoryLocationsDomain& memory_locations,
    TaintTree taint,
    UpdateKind kind) {
  if (memory_locations.size() > 1) {
    // In practice, only one of the memory location is affected, so we must
    // treat this as a weak update, even if a strong update was requested.
    kind = UpdateKind::Weak;
  }

  for (auto* memory_location : memory_locations.elements()) {
    write(memory_location, taint, kind);
  }
}

void ForwardTaintEnvironment::write(
    const MemoryLocationsDomain& memory_locations,
    Taint taint,
    UpdateKind kind) {
  write(memory_locations, TaintTree(std::move(taint)), kind);
}

void ForwardTaintEnvironment::write(
    const MemoryLocationsDomain& memory_locations,
    const Path& path,
    TaintTree taint,
    UpdateKind kind) {
  if (memory_locations.size() > 1) {
    // In practice, only one of the memory location is affected, so we must
    // treat this as a weak update, even if a strong update was requested.
    kind = UpdateKind::Weak;
  }

  for (auto* memory_location : memory_locations.elements()) {
    write(memory_location, path, taint, kind);
  }
}

void ForwardTaintEnvironment::write(
    const MemoryLocationsDomain& memory_locations,
    const Path& path,
    Taint taint,
    UpdateKind kind) {
  if (memory_locations.size() > 1) {
    // In practice, only one of the memory location is affected, so we must
    // treat this as a weak update, even if a strong update was requested.
    kind = UpdateKind::Weak;
  }

  for (auto* memory_location : memory_locations.elements()) {
    write(memory_location, path, taint, kind);
  }
}

std::ostream& operator<<(
    std::ostream& out,
    const ForwardTaintEnvironment& environment) {
  return out << environment.taint_;
}

} // namespace marianatrench
