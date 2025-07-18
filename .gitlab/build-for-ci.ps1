param (
    [string]$preset = "CI-Windows",
    [string]$config = "Debug",
    [string]$target = "all"
)

$ErrorActionPreference = "Stop"

cmake --preset $preset || exit 1
cmake --build --parallel --preset $preset --config $config --target $target || exit 1
