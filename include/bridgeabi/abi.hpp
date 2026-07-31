#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>

#if defined(_WIN32)
  #if defined(BRIDGEABI_BUILDING_LIBRARY)
    #define BRIDGEABI_EXPORT __declspec(dllexport)
  #else
    #define BRIDGEABI_EXPORT __declspec(dllimport)
  #endif
#else
  #define BRIDGEABI_EXPORT __attribute__((visibility("default")))
#endif

#define BRIDGEABI_CALL

namespace bridge {

inline constexpr std::uint32_t abi_major = 1;
inline constexpr std::uint32_t abi_minor = 0;

enum class status_code : std::uint32_t {
  ok = 0, invalid_argument, invalid_state, unsupported, version_mismatch,
  capability_missing, module_load_failure, symbol_not_found,
  plugin_initialization_failure, plugin_execution_failure, out_of_memory,
  internal_failure
};

enum class encoding : std::uint32_t { utf8 = 1, utf16 = 2, utf32 = 3 };

struct bool32 { std::uint32_t value{}; constexpr explicit operator bool() const noexcept { return value != 0; } };
struct version { std::uint16_t major{}, minor{}, patch{}, abi_revision{}; };
struct guid { std::uint8_t bytes[16]{}; };
struct string_view { const char* data{}; std::uint64_t size{}; encoding text_encoding{encoding::utf8}; };
struct buffer_view { const std::byte* data{}; std::uint64_t size{}; };
using release_fn = void (BRIDGEABI_CALL *)(void* context, std::byte* data, std::uint64_t size) noexcept;
struct owned_buffer { std::byte* data{}; std::uint64_t size{}; release_fn release{}; void* release_context{}; };
struct error { status_code code{status_code::ok}; string_view message{}; string_view source{}; bool32 recoverable{}; };
struct status { status_code code{status_code::ok}; error detail{}; };
struct allocator {
  void* context{};
  void* (BRIDGEABI_CALL *allocate)(void*, std::uint64_t, std::uint64_t) noexcept {};
  void (BRIDGEABI_CALL *deallocate)(void*, void*, std::uint64_t, std::uint64_t) noexcept {};
};

[[nodiscard]] constexpr bool succeeded(status value) noexcept { return value.code == status_code::ok; }
[[nodiscard]] constexpr string_view utf8(const char* data, std::uint64_t size) noexcept { return {data, size, encoding::utf8}; }
inline void release(owned_buffer& value) noexcept { if (value.release != nullptr && value.data != nullptr) value.release(value.release_context, value.data, value.size); value = {}; }

static_assert(sizeof(bool32) == 4);
static_assert(sizeof(version) == 8);
static_assert(sizeof(guid) == 16);
static_assert(sizeof(string_view) == (sizeof(void*) + 16));
static_assert(sizeof(buffer_view) == (sizeof(void*) + 8));
static_assert(sizeof(owned_buffer) == sizeof(void*) * 3 + 8);
static_assert(std::numeric_limits<unsigned char>::digits == 8, "BridgeABI requires 8-bit bytes");

} // namespace bridge
