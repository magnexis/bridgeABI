#include "bridgeabi/loader.hpp"
#include <utility>
#if defined(_WIN32)
#include <windows.h>
#else
#include <dlfcn.h>
#endif
namespace bridge {
namespace {
std::string platform_error() {
#if defined(_WIN32)
  return "Windows dynamic loader error " + std::to_string(GetLastError());
#else
  const char* error = dlerror();
  return error ? error : "dynamic loader error";
#endif
}
}
module::~module() { close(); }
module::module(module&& other) noexcept : handle_(std::exchange(other.handle_, nullptr)), plugin_(other.plugin_), loaded_(std::exchange(other.loaded_, false)) { other.plugin_ = {}; }
module& module::operator=(module&& other) noexcept { if (this != &other) { close(); handle_ = std::exchange(other.handle_, nullptr); plugin_ = other.plugin_; loaded_ = std::exchange(other.loaded_, false); other.plugin_ = {}; } return *this; }
load_result module::open(const std::filesystem::path& path, const host_api& host) noexcept {
  close(); load_result result{};
  #if defined(_WIN32)
  handle_ = reinterpret_cast<void*>(LoadLibraryW(path.c_str()));
  #else
  handle_ = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
  #endif
  if (!handle_) { result.error = "Could not load '" + path.string() + "': " + platform_error(); return result; }
  #if defined(_WIN32)
  auto entry = reinterpret_cast<plugin_entry_fn>(GetProcAddress(reinterpret_cast<HMODULE>(handle_), plugin_entry_symbol));
  #else
  auto entry = reinterpret_cast<plugin_entry_fn>(dlsym(handle_, plugin_entry_symbol));
  #endif
  if (!entry) { result.error = "BridgeABI entry point missing: " + platform_error(); close(); return result; }
  plugin_ = {}; plugin_.struct_size = sizeof(plugin_api);
  const status state = entry(&host, &plugin_);
  if (!succeeded(state)) { result.error = "Plugin entry rejected host (status " + std::to_string(static_cast<std::uint32_t>(state.code)) + ")"; close(); return result; }
  if (plugin_.struct_size < offsetof(plugin_api, primary_interface) + sizeof(plugin_.primary_interface) || plugin_.abi_major != abi_major) { result.error = "Plugin returned an incompatible BridgeABI API"; close(); return result; }
  loaded_ = true; result.plugin = plugin_; return result;
}
void module::close() noexcept {
  if (!handle_) return;
  if (loaded_ && plugin_.shutdown) plugin_.shutdown(plugin_.shutdown_context);
#if defined(_WIN32)
  FreeLibrary(reinterpret_cast<HMODULE>(handle_));
#else
  dlclose(handle_);
#endif
  handle_ = nullptr; plugin_ = {}; loaded_ = false;
}
} // namespace bridge
