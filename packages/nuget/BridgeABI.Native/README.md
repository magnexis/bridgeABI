# BridgeABI.Native

Native C++ distribution of BridgeABI for Windows x64. It provides headers, the static BridgeABI runtime library, and CMake integration. The canonical logo remains in the source repository; it is intentionally not embedded as a NuGet icon because NuGet package icons are limited to 1 MB.

## Status

`0.1.0-preview.2` is a prerelease. The stable ABI foundation and plugin loader are available; interface generation is still under active Phase 2 development.

## CMake

```cmake
list(APPEND CMAKE_PREFIX_PATH "<nuget-package>/buildTransitive/native")
find_package(BridgeABI CONFIG REQUIRED)
target_link_libraries(my_target PRIVATE BridgeABI::bridgeabi)
```

This package currently ships Windows x64 artifacts only. Linux and macOS packages should be published from their respective release builds under the matching NuGet runtime identifiers.
