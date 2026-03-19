param(
    [string]$CanvasApkUrl = "https://github.com/XeTrinityz/ThatSkyMod-Android/releases/latest/download/Canvas.apk"
)

$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = Split-Path -Parent $scriptDir
$libDir = Join-Path $repoRoot "app\src\main\cpp\lib"
$outSo = Join-Path $libDir "libciphered.so"
$tmpApk = Join-Path $env:TEMP "aura_canvas_latest.apk"
$tmpExtract = Join-Path $env:TEMP "aura_canvas_extract"

New-Item -ItemType Directory -Force -Path $libDir | Out-Null

if (Test-Path $tmpExtract) {
    Remove-Item -Recurse -Force $tmpExtract
}
New-Item -ItemType Directory -Force -Path $tmpExtract | Out-Null

Write-Host "Downloading Canvas APK..."
curl.exe -L $CanvasApkUrl -o $tmpApk | Out-Null

Write-Host "Extracting libciphered.so..."
tar -xf $tmpApk -C $tmpExtract "lib/arm64-v8a/libciphered.so"

$sourceSo = Join-Path $tmpExtract "lib\arm64-v8a\libciphered.so"
if (!(Test-Path $sourceSo)) {
    throw "Failed to extract libciphered.so from APK."
}

Copy-Item -Force $sourceSo $outSo

$info = Get-Item $outSo
Write-Host ("Done. " + $info.FullName + " (" + $info.Length + " bytes)")

