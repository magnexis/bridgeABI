#include "bridgeabi/plugin.hpp"
#include <cstring>
#include <new>

namespace {
using namespace bridge;
struct greeter { std::uint32_t references{1}; };
void BRIDGEABI_CALL retain(void* instance) noexcept { ++static_cast<greeter*>(instance)->references; }
void BRIDGEABI_CALL release_instance(void* instance) noexcept { auto* value = static_cast<greeter*>(instance); if (--value->references == 0) delete value; }
void BRIDGEABI_CALL release_buffer(void*, std::byte* data, std::uint64_t) noexcept { delete[] data; }
status BRIDGEABI_CALL greet(void*, string_view name, owned_buffer* output) noexcept {
  if (!output || (!name.data && name.size != 0) || name.text_encoding != encoding::utf8) return {status_code::invalid_argument};
  constexpr char prefix[] = "Hello, "; const auto byte_count = sizeof(prefix) - 1 + name.size;
  auto* data = new (std::nothrow) std::byte[byte_count]; if (!data) return {status_code::out_of_memory};
  std::memcpy(data, prefix, sizeof(prefix) - 1); if (name.size) std::memcpy(data + sizeof(prefix) - 1, name.data, static_cast<std::size_t>(name.size));
  *output = {data, byte_count, release_buffer, nullptr}; return {};
}
const char plugin_id[] = "org.bridgeabi.examples.greeter";
greeter_v1 table{sizeof(greeter_v1), 1, 0, new greeter{}, greet, retain, release_instance};
status BRIDGEABI_CALL shutdown(void*) noexcept { if (table.instance) { release_instance(table.instance); table.instance = nullptr; } return {}; }
}

extern "C" BRIDGEABI_EXPORT bridge::status BRIDGEABI_CALL bridge_plugin_entry(const bridge::host_api* host, bridge::plugin_api* plugin) noexcept {
  if (!host || !plugin || host->abi_major != bridge::abi_major || plugin->struct_size < sizeof(bridge::plugin_api)) return {bridge::status_code::version_mismatch};
  if (!table.instance) table.instance = new (std::nothrow) greeter{}; if (!table.instance) return {bridge::status_code::out_of_memory};
  *plugin = {sizeof(bridge::plugin_api), bridge::abi_major, bridge::abi_minor, bridge::utf8(plugin_id, sizeof(plugin_id) - 1), {1, 0, 0, 0}, &table, shutdown, nullptr}; return {};
}
