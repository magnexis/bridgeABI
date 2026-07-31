#include "emitters/artifacts.hpp"
#include "compatibility/snapshot.hpp"
#include "parser/idl_parser.hpp"
#include "semantics/validate.hpp"
#include <iostream>
#include <string>

int main() {
  const auto require = [](bool condition, const char* message) { if (!condition) std::cerr << message << '\n'; return condition; };
  constexpr std::string_view source = R"(namespace examples.audio;
@id("org.example.audio")
interface AudioPlugin version 1.2 {
  void initialize(uint32 channels);
  owned string name();
})";
  bridgeabi::generator::interface_definition definition; std::string error;
  if (!require(bridgeabi::generator::parse_idl(source, definition, error), "Expected valid IDL to parse")) return 1;
  if (!require(bridgeabi::generator::validate(definition, error), "Expected valid IDL to validate")) return 1;
  if (!require(definition.name == "AudioPlugin" && definition.major == 1 && definition.methods.size() == 2, "Unexpected normalized interface")) return 1;
  const auto files = bridgeabi::generator::emit_artifacts(definition);
  if (!require(files.size() == 6 && files.contains("c/audioplugin_abi.h"), "Expected generated artifacts")) return 1;
  if (!require(files.at("c/audioplugin_abi.h").find("bridge_owned_buffer") != std::string::npos, "Expected owned result buffer")) return 1;
  const auto same = bridgeabi::generator::compare_snapshot(definition, files.at("compatibility/audioplugin.compat.json"));
  if (!require(same.compatible, "Expected unchanged snapshot compatibility")) return 1;
  definition.methods[0].name = "changed";
  if (!require(!bridgeabi::generator::compare_snapshot(definition, files.at("compatibility/audioplugin.compat.json")).compatible, "Expected changed interface incompatibility")) return 1;
  if (!require(!bridgeabi::generator::parse_idl("interface invalid", definition, error), "Expected invalid IDL rejection")) return 1;
}
