# BridgeABI Phase 1 architecture

`bridgeabi` owns the host-side dynamic module abstraction. A plugin exports exactly one C-linkage symbol, `bridge_plugin_entry`. The host supplies `host_api`; the plugin fills `plugin_api`, whose leading size and ABI version allow compatible append-only extension.

The greeter example demonstrates the key ownership rule: the plugin allocates its reply and provides a `release_fn`; the host calls that callback instead of freeing the memory itself. No C++ class, STL object, exception, or allocator crosses the boundary.

Phase 1 uses a hand-authored `greeter_v1` function table to prove the runtime contract. Phase 2 replaces hand-authored tables with generated C headers, adapters, and C++ wrappers from an IDL.

The Phase 2 generator accepts only a small declarative `.bridgeidl` grammar; it does not attempt to parse arbitrary C++. Generated `string` and `buffer` return values are `owned_buffer` outputs, so the caller releases them through the producer callback. Input strings and buffers are borrowed views for the duration of the call.

Manifest preflight validation currently recognizes required identity/version/entry-point fields in JSON. It is deliberately not a general JSON parser and is not a security boundary; packaging, schema validation, signatures, dependencies, capabilities, hot reload, async, IPC, and bindings remain future phases.
