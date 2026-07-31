# Roadmap

1. **ABI Foundation (complete):** loader, stable primitive layouts, ownership callback, entry-point negotiation, manifest preflight, and greeter integration test.
2. **Interface generation (in progress):** deterministic restricted IDL parser and generated versioned function tables. C++ client/server wrappers, compatibility fixtures, manifest fragments, and other language emitters remain to be added.
3. **Plugin runtime:** registry, capabilities, dependencies, diagnostics, package inspection, and full manifest schema validation.
4. **Hot reload and async:** active-call draining, state handoff, cancellation, and rollback.
5. **Process isolation:** worker process, local IPC, crash handling, and resource policies.
6. **.NET:** NativeAOT-friendly SafeHandle-based bindings and source generation.
