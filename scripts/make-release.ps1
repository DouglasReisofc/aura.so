param(
    [Parameter(Mandatory=$true)]
    [string]$Tag,
    [Parameter(Mandatory=$true)]
    [string]$Owner,
    [Parameter(Mandatory=$true)]
    [string]$Repo,
    [Parameter(Mandatory=$true)]
    [string]$InputSoPath
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path $InputSoPath)) {
    throw "Arquivo nao encontrado: $InputSoPath"
}

$repoRoot = Split-Path -Parent $PSScriptRoot
$distRoot = Join-Path $repoRoot "dist\tsm-rebuild\$Tag"
New-Item -Path $distRoot -ItemType Directory -Force | Out-Null

$targetLib = Join-Path $distRoot "libTSMRebuild.so"
Copy-Item -Path $InputSoPath -Destination $targetLib -Force

$hash = (Get-FileHash -Algorithm SHA256 $targetLib).Hash.ToLowerInvariant()
$releaseUrl = "https://api.github.com/repos/$Owner/$Repo/releases/latest"

$manifest = @"
{
  "tag": "$Tag",
  "owner": "$Owner",
  "repo": "$Repo",
  "githubReleasesUrl": "$releaseUrl",
  "asset": {
    "name": "libTSMRebuild.so",
    "sha256": "$hash"
  }
}
"@

$manifestPath = Join-Path $distRoot "release-manifest.json"
Set-Content -Path $manifestPath -Value $manifest -NoNewline -Encoding UTF8

Write-Output "Release pronta em: $distRoot"
Write-Output "SHA256: $hash"
