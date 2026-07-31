#include "bridgeabi/loader.hpp"
#include <cstring>
#include <iostream>

int main(int argc, char** argv) {
  if (argc != 2) { std::cerr << "usage: bridgeabi_greeter_host <plugin-library>\n"; return 2; }
  bridge::module module; const auto result = module.open(argv[1]);
  if (!result) { std::cerr << result.error << '\n'; return 1; }
  const auto* table = static_cast<const bridge::greeter_v1*>(result.plugin.primary_interface);
  if (!table || table->struct_size < sizeof(bridge::greeter_v1) || table->abi_major != 1 || !table->greet) { std::cerr << "Greeter v1 is unavailable\n"; return 1; }
  constexpr char name[] = "BridgeABI"; bridge::owned_buffer greeting{};
  const auto status = table->greet(table->instance, bridge::utf8(name, sizeof(name) - 1), &greeting);
  if (!bridge::succeeded(status)) { std::cerr << "Greeting failed\n"; return 1; }
  std::cout.write(reinterpret_cast<const char*>(greeting.data), static_cast<std::streamsize>(greeting.size)); std::cout << '\n'; bridge::release(greeting); return 0;
}
