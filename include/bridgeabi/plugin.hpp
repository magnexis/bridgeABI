#pragma once
#include "bridgeabi/abi.hpp"

namespace bridge {
struct host_api { std::uint32_t struct_size{sizeof(host_api)}; std::uint32_t abi_major{bridge::abi_major}; std::uint32_t abi_minor{bridge::abi_minor}; const allocator* host_allocator{}; };
struct plugin_api {
  std::uint32_t struct_size{sizeof(plugin_api)};
  std::uint32_t abi_major{};
  std::uint32_t abi_minor{};
  string_view plugin_id{};
  version plugin_version{};
  const void* primary_interface{};
  status (BRIDGEABI_CALL *shutdown)(void*) noexcept {};
  void* shutdown_context{};
};
using plugin_entry_fn = status (BRIDGEABI_CALL *)(const host_api*, plugin_api*) noexcept;
inline constexpr const char* plugin_entry_symbol = "bridge_plugin_entry";

// A Phase 1 hand-authored interface table. Phase 2 will generate these tables from IDL.
struct greeter_v1 {
  std::uint32_t struct_size{sizeof(greeter_v1)};
  std::uint32_t abi_major{1};
  std::uint32_t abi_minor{};
  void* instance{};
  status (BRIDGEABI_CALL *greet)(void*, string_view, owned_buffer*) noexcept {};
  void (BRIDGEABI_CALL *retain)(void*) noexcept {};
  void (BRIDGEABI_CALL *release)(void*) noexcept {};
};
} // namespace bridge
