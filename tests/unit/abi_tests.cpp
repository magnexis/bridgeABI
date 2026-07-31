#include "bridgeabi/loader.hpp"
#include <cassert>
#include <string>
int main() {
  bridge::manifest_info info; std::string error;
  assert(bridge::validate_manifest_json(R"({"schemaVersion":1,"id":"org.example.test","version":"1.0.0","entryPoint":"bridge_plugin_entry"})", info, error));
  assert(info.id == "org.example.test");
  assert(!bridge::validate_manifest_json(R"({"schemaVersion":2})", info, error));
  bridge::owned_buffer buffer{}; bridge::release(buffer);
}
