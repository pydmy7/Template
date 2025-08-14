#!/usr/bin/env pwsh

param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string]$Output_Path
)

if (-not (Test-Path -Path $Output_Path) -or -not (Test-Path -Path $Output_Path -PathType Container)) {
    exit 1
}

$git_hash = git rev-parse HEAD
if (-not $git_hash) {
    Write-Host "Failed to retrieve git hash"
    exit 1
}

$Output_Path = Join-Path -Path $Output_Path -ChildPath "version"
"version: $git_hash" | Set-Content -Encoding UTF8 -Force $Output_Path
