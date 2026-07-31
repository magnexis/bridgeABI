#pragma once
#include "model/interface.hpp"
#include <string>
namespace bridgeabi::generator { [[nodiscard]] bool validate(interface_definition& definition, std::string& error) noexcept; [[nodiscard]] const char* ownership_name(ownership value) noexcept; }
