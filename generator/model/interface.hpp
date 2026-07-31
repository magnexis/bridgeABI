#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace bridgeabi::generator {
enum class ownership { none, borrowed, owned, transferred, shared, static_lifetime, host_owned, plugin_owned };
struct parameter { std::string type; std::string name; ownership owner{ownership::none}; bool nullable{}; };
struct method { std::string result_type; std::string name; ownership result_owner{ownership::none}; std::uint32_t id{}; std::vector<parameter> parameters; };
struct capability { std::string id; std::uint32_t major{}, minor{}; };
struct interface_definition { std::string name_space; std::string name; std::string id; std::uint32_t major{}; std::uint32_t minor{}; std::string threading; std::vector<capability> capabilities; std::vector<method> methods; };
} // namespace bridgeabi::generator
