#pragma once
#include "model/interface.hpp"
#include <string>
namespace bridgeabi::generator {
struct compatibility_result { bool compatible{}; std::string message; };
[[nodiscard]] compatibility_result compare_snapshot(const interface_definition& current, std::string_view previous_json) noexcept;
}
