#include "bridgeabi/loader.hpp"
#include <regex>
namespace bridge {
bool validate_manifest_json(std::string_view json, manifest_info& output, std::string& error) noexcept {
  try { const std::string text(json); std::smatch match; auto field = [&](const char* name, std::string& value) { const std::regex expression(std::string("\\\"") + name + "\\\"\\s*:\\s*\\\"([^\\\"]+)\\\""); if (!std::regex_search(text, match, expression)) return false; value = match[1].str(); return true; };
    std::string schema; if (!field("id", output.id) || !field("version", output.version) || !field("entryPoint", output.entry_point)) { error = "Manifest requires string fields: id, version, entryPoint"; return false; }
    const std::regex schema_expression("\\\"schemaVersion\\\"\\s*:\\s*([0-9]+)"); if (!std::regex_search(text, match, schema_expression)) { error = "Manifest requires numeric schemaVersion"; return false; } output.schema_version = static_cast<std::uint32_t>(std::stoul(match[1].str())); if (output.schema_version != 1 || output.entry_point != plugin_entry_symbol) { error = "Unsupported manifest schema or entry point"; return false; } return true;
  } catch (...) { error = "Manifest is malformed"; return false; }
}
} // namespace bridge
