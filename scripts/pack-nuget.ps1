param(
  [string]$Configuration = "Release",
  [string]$Version = "0.1.0-preview.2",
  [string]$OutputDirectory = "artifacts/nuget"
)

$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot
$build = Join-Path $root "build/nuget-win-x64"
$install = Join-Path $build "install"
$library = Join-Path $install "lib/bridgeabi.lib"

cmake -S $root -B $build "-DBRIDGEABI_BUILD_EXAMPLES=OFF" "-DBRIDGEABI_BUILD_TESTS=OFF" "-DBRIDGEABI_BUILD_GENERATOR=OFF" "-DCMAKE_INSTALL_PREFIX=$install"
cmake --build $build --config $Configuration
cmake --install $build --config $Configuration
if (-not (Test-Path -LiteralPath $library)) { throw "Expected Windows x64 library was not produced: $library" }

dotnet pack (Join-Path $root "packages/nuget/BridgeABI.Native/BridgeABI.Native.csproj") --configuration $Configuration --output (Join-Path $root $OutputDirectory) -p:Version=$Version -p:BridgeAbiNativeLibrary=$library -p:BridgeAbiInstallRoot=$install
