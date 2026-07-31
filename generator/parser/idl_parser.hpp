#pragma once
#include "model/interface.hpp"
#include <string_view>

namespace bridgeabi::generator {
[[nodiscard]] bool parse_idl(std::string_view input, interface_definition& output, std::string& error) noexcept;
} // namespace bridgeabi::generator
