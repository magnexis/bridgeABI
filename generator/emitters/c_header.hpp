#pragma once
#include "model/interface.hpp"
#include <string>
namespace bridgeabi::generator { [[nodiscard]] std::string emit_c_header(const interface_definition& definition); }
