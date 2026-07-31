# Publishing `BridgeABI.Native`

The release workflow is [publish-nuget.yml](../.github/workflows/publish-nuget.yml). It always builds, tests, packs, and uploads a `.nupkg` artifact. It publishes only when either:

- a GitHub release with a `v<major>.<minor>.<patch>` tag is published; or
- the workflow is manually dispatched with **Publish** selected.

Before publishing, create an API key scoped to `BridgeABI.Native` on NuGet.org and add it to the GitHub repository as the `NUGET_API_KEY` Actions secret. Do not put the key in source control, workflow files, or local project configuration.

Use a tag such as `v0.1.0-preview.2`; the workflow removes the leading `v` for the package version. To retry a failed release after a workflow fix, manually dispatch the workflow with the same version and **Publish** enabled. The package is currently Windows x64 only, so releases should remain prerelease until the supported platform set and Phase 2 API are ready for a stable compatibility commitment.
