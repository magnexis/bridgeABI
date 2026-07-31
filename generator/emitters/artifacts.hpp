#pragma once
#include "model/interface.hpp"
#include <map>
#include <string>
namespace bridgeabi::generator { using artifact_set = std::map<std::string, std::string>; [[nodiscard]] artifact_set emit_artifacts(const interface_definition& definition); [[nodiscard]] std::string fingerprint(const interface_definition& definition); }
