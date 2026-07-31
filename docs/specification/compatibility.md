# Compatibility policy

Each generated interface emits a deterministic compatibility snapshot containing its ID, version, and fingerprint. The fingerprint includes method IDs, method signatures, ownership, and nullability. A changed fingerprint is ABI-significant and must be reviewed as a compatible append or a major-version change.

Existing method IDs are stable. The current generator assigns source-order IDs when they are omitted; published interfaces should gain explicit method-ID support before reordering methods.

Run `bridgeabi compatibility current.bridge baseline.compat.json` in CI to reject an unreviewed ABI fingerprint change. A snapshot mismatch is reported as `BRG1301`.
