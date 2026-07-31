#pragma once
#include "bridgeabi/plugin.hpp"
#include <filesystem>
#include <string>

namespace bridge {
struct load_result { plugin_api plugin{}; std::string error{}; explicit operator bool() const noexcept { return error.empty(); } };
class module final {
public:
  module() = default; ~module(); module(module&&) noexcept; module& operator=(module&&) noexcept;
  module(const module&) = delete; module& operator=(const module&) = delete;
  [[nodiscard]] load_result open(const std::filesystem::path& path, const host_api& host = {}) noexcept;
  void close() noexcept;
  [[nodiscard]] const plugin_api* plugin() const noexcept { return loaded_ ? &plugin_ : nullptr; }
private: void* handle_{}; plugin_api plugin_{}; bool loaded_{};
};
struct manifest_info { std::string id, version, entry_point; std::uint32_t schema_version{}; };
[[nodiscard]] bool validate_manifest_json(std::string_view json, manifest_info& output, std::string& error) noexcept;
} // namespace bridge
