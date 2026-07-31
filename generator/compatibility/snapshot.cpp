#include "compatibility/snapshot.hpp"
#include "emitters/artifacts.hpp"
#include <regex>
namespace bridgeabi::generator {
compatibility_result compare_snapshot(const interface_definition& current, std::string_view previous_json) noexcept {
  try { std::smatch match; const std::string text(previous_json); const std::regex id_pattern("\\\"id\\\"\\s*:\\s*\\\"([^\\\"]+)\\\""); const std::regex fingerprint_pattern("\\\"fingerprint\\\"\\s*:\\s*\\\"([^\\\"]+)\\\""); if (!std::regex_search(text, match, id_pattern)) return {false, "BRG1301: Snapshot has no interface identity"}; const auto previous_id = match[1].str(); if (previous_id != current.id) return {false, "BRG1301: Interface identity changed"}; if (!std::regex_search(text, match, fingerprint_pattern)) return {false, "BRG1301: Snapshot has no fingerprint"}; if (match[1].str() == fingerprint(current)) return {true, "Compatible: ABI fingerprint unchanged"}; return {false, "BRG1301: ABI fingerprint changed; require a reviewed compatible append or a major version increase"};
  } catch (...) { return {false, "BRG1301: Invalid compatibility snapshot"}; }
}
} // namespace bridgeabi::generator
