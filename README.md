# BridgeABI

![BridgeABI logo](assets/brand/bridgeabi-logo.png)

> Stable native interfaces without trusting the C++ ABI.

BridgeABI is an open-source C++ foundation for loading independently compiled native plugins through a deliberately small C-compatible ABI boundary. It does not pass STL types, exceptions, or allocator ownership across module boundaries.

## Status

Phase 2 is underway and includes a deterministic, restricted BridgeABI IDL parser plus a `bridgeabi generate` CLI that emits versioned C++ ABI function tables. The core runtime remains Phase 1: fixed-layout ABI primitives, versioned plugin entry point, owned-buffer release callbacks, a portable loader, manifest preflight validation, and a complete greeter plugin integration test. Windows x64, Linux x64, macOS x64, and macOS arm64 are the intended initial targets.

## Build and run

```sh
cmake -S . -B build -DBRIDGEABI_BUILD_TESTS=ON
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

The integration test loads the separately built greeter library and prints `Hello, BridgeABI`.

Generate a function table from the restricted IDL grammar:

```sh
./build/bridgeabi generate examples/greeter/greeter.bridgeidl --output generated
```

## ABI rules

Generation creates C ABI headers, C++ host/proxy headers, C++ plugin-boundary headers, JSON descriptors, Markdown documentation, and compatibility snapshots. The public boundary uses fixed-width integers, explicitly sized structures, opaque instance pointers, UTF-8 `string_view`, and `owned_buffer` values released by their producer callback. C++ exceptions must be translated to `bridge::status` before returning through the entry point or function table.

In-process plugins are **not sandboxed**: loading one grants it the host process's privileges. Process isolation and signature policy are planned later phases.

See [Architecture](docs/ARCHITECTURE.md) and [roadmap](docs/ROADMAP.md).

## NuGet prerelease package

Build a local Windows x64 `BridgeABI.Native` prerelease package with:

```powershell
./scripts/pack-nuget.ps1
```

This prepares a package but never publishes it. See the package [README](packages/nuget/BridgeABI.Native/README.md) for consumer setup and platform scope.

Publishing is performed by the guarded GitHub Actions workflow described in [NuGet publishing](docs/NUGET-PUBLISHING.md).
