param(
    [string]$CommitMessage = "",
    [string[]]$Devices = @(),
    [switch]$SkipBuild,
    [switch]$RestartCanvas,
    [switch]$RestartGame
)

$ErrorActionPreference = "Stop"

$repoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
Set-Location $repoRoot

if ([string]::IsNullOrWhiteSpace($CommitMessage)) {
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $CommitMessage = "update: aura.so $timestamp"
}

Write-Host "==> Build + push ADB"
& "$PSScriptRoot\update-dev.ps1" -Devices $Devices -SkipBuild:$SkipBuild -RestartCanvas:$RestartCanvas -RestartGame:$RestartGame

Write-Host "==> Git add/commit/push"
& git add -A

$staged = git diff --cached --name-only
if ([string]::IsNullOrWhiteSpace(($staged -join ""))) {
    Write-Host "Sem alteracoes para commit."
    exit 0
}

& git commit -m $CommitMessage
& git push origin main

Write-Host "Concluido. Commit enviado para origin/main."
