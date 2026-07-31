#include "emitters/artifacts.hpp"
#include "compatibility/snapshot.hpp"
#include "parser/idl_parser.hpp"
#include "semantics/validate.hpp"
#include <cassert>
#include <string>

int main() {
  constexpr std::string_view source = R"(namespace examples.audio;
@id("org.example.audio")
interface AudioPlugin version 1.2 {
  void initialize(uint32 channels);
  owned string name();
})";
  bridgeabi::generator::interface_definition definition; std::string error;
  assert(bridgeabi::generator::parse_idl(source, definition, error));
  assert(bridgeabi::generator::validate(definition, error));
  assert(definition.name == "AudioPlugin" && definition.major == 1 && definition.methods.size() == 2);
  const auto files = bridgeabi::generator::emit_artifacts(definition);
  assert(files.size() == 6 && files.contains("c/audioplugin_abi.h"));
  assert(files.at("c/audioplugin_abi.h").find("bridge_owned_buffer") != std::string::npos);
  const auto same = bridgeabi::generator::compare_snapshot(definition, files.at("compatibility/audioplugin.compat.json"));
  assert(same.compatible);
  definition.methods[0].name = "changed";
  assert(!bridgeabi::generator::compare_snapshot(definition, files.at("compatibility/audioplugin.compat.json")).compatible);
  assert(!bridgeabi::generator::parse_idl("interface invalid", definition, error));
}
