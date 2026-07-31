#include "bridgeabi/loader.hpp"
#include <iostream>
#include <string>
int main() {
  const auto require = [](bool condition, const char* message) { if (!condition) std::cerr << message << '\n'; return condition; };
  bridge::manifest_info info; std::string error;
  if (!require(bridge::validate_manifest_json(R"({"schemaVersion":1,"id":"org.example.test","version":"1.0.0","entryPoint":"bridge_plugin_entry"})", info, error), "Expected valid manifest")) return 1;
  if (!require(info.id == "org.example.test", "Unexpected manifest ID")) return 1;
  if (!require(!bridge::validate_manifest_json(R"({"schemaVersion":2})", info, error), "Expected invalid manifest rejection")) return 1;
  bridge::owned_buffer buffer{}; bridge::release(buffer);
}
