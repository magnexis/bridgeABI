#include "emitters/artifacts.hpp"
#include "compatibility/snapshot.hpp"
#include "parser/idl_parser.hpp"
#include "semantics/validate.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>

namespace { bool write_if_changed(const std::filesystem::path& path, const std::string& content) { std::ifstream in(path, std::ios::binary); const std::string old((std::istreambuf_iterator<char>(in)), {}); if (old == content) return false; std::filesystem::create_directories(path.parent_path()); const auto temporary = path.string() + ".tmp"; { std::ofstream out(temporary, std::ios::binary | std::ios::trunc); out << content; if (!out) throw std::runtime_error("Cannot write output"); } std::filesystem::rename(temporary, path); return true; } }
int main(int argc, char** argv) {
  if (argc < 3 || (std::string_view(argv[1]) != "generate" && std::string_view(argv[1]) != "validate" && std::string_view(argv[1]) != "compatibility")) { std::cerr << "usage: bridgeabi <generate|validate> <interface.bridge> [--output <directory>]\n       bridgeabi compatibility <interface.bridge> <snapshot.json>\n"; return 2; }
  std::ifstream input(argv[2], std::ios::binary); if (!input) { std::cerr << "Cannot read input IDL\n"; return 1; }
  const std::string source((std::istreambuf_iterator<char>(input)), {}); bridgeabi::generator::interface_definition definition; std::string error;
  if (!bridgeabi::generator::parse_idl(source, definition, error) || !bridgeabi::generator::validate(definition, error)) { std::cerr << error << '\n'; return 1; }
  if (std::string_view(argv[1]) == "validate") { std::cout << "Valid: " << definition.name << "\n"; return 0; }
  if (std::string_view(argv[1]) == "compatibility") { if (argc != 4) { std::cerr << "usage: bridgeabi compatibility <interface.bridge> <snapshot.json>\n"; return 2; } std::ifstream snapshot(argv[3], std::ios::binary); const std::string previous((std::istreambuf_iterator<char>(snapshot)), {}); const auto result = bridgeabi::generator::compare_snapshot(definition, previous); std::cout << result.message << '\n'; return result.compatible ? 0 : 1; }
  std::filesystem::path directory = "generated"; if (argc == 5 && std::string_view(argv[3]) == "--output") directory = argv[4]; else if (argc != 3) { std::cerr << "usage: bridgeabi generate <interface.bridge> [--output <directory>]\n"; return 2; }
  try { std::size_t generated{}; std::size_t unchanged{}; for (const auto& [relative, content] : bridgeabi::generator::emit_artifacts(definition)) { if (write_if_changed(directory / relative, content)) ++generated; else ++unchanged; } std::cout << "Generated: " << generated << " files\nUnchanged: " << unchanged << " files\n"; return 0; } catch (const std::exception& exception) { std::cerr << exception.what() << '\n'; return 1; }
}
