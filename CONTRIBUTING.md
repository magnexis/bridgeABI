# Contributing

BridgeABI treats its low-level ABI as a compatibility promise. Changes to public ABI structures must be append-only, retain fixed-width field types, update layout tests, and document the compatibility impact.

Before opening a pull request, configure with tests enabled, build, and run CTest. Do not add STL types, exceptions, or implicit cross-module deallocation to exported function signatures.
