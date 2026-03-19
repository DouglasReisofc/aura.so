param(
    [string[]]$Devices = @(),
    [switch]$SkipBuild,
    [switch]$RestartCanvas,
    [switch]$RestartGame
)

$ErrorActionPreference = "Stop"

$repoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
Set-Location $repoRoot

function Get-ConnectedDevices {
    $lines = adb devices | Select-Object -Skip 1
    $ids = @()
    foreach ($line in $lines) {
        if ($line -match "^([^\s]+)\s+device$") {
            $ids += $Matches[1]
        }
    }
    return $ids
}

if (-not $SkipBuild) {
    Write-Host "==> Build debug"
    & "$repoRoot\gradlew.bat" assembleDebug
}

$libPath = Join-Path $repoRoot "app\build\intermediates\stripped_native_libs\debug\stripDebugDebugSymbols\out\lib\arm64-v8a\libaura.so"
if (-not (Test-Path $libPath)) {
    throw "Nao encontrei libaura.so em: $libPath"
}

if ($Devices.Count -eq 0) {
    $Devices = Get-ConnectedDevices
}

if ($Devices.Count -eq 0) {
    throw "Nenhum device ADB conectado."
}

foreach ($device in $Devices) {
    Write-Host "==> Push para $device"
    & adb -s $device push $libPath /sdcard/Download/libaura.so | Out-Host

    if ($RestartGame) {
        Write-Host "==> Reiniciando jogo em $device"
        $gamePackages = @(
            "com.tgc.sky.android",
            "com.tgc.sky.android.test",
            "com.tgc.sky.android.huawei",
            "com.tgc.sky"
        )
        $targetGame = $null
        foreach ($pkg in $gamePackages) {
            $probe = & adb -s $device shell "pm path $pkg" 2>$null
            if ($probe -match "package:") {
                $targetGame = $pkg
                break
            }
        }
        if ($targetGame) {
            & adb -s $device shell am force-stop $targetGame | Out-Null
            & adb -s $device shell monkey -p $targetGame -c android.intent.category.LAUNCHER 1 | Out-Null
        } else {
            Write-Warning "Nao achei pacote do Sky instalado em $device."
        }
    }

    if ($RestartCanvas) {
        Write-Host "==> Reiniciando Canvas em $device"
        & adb -s $device shell am force-stop git.artdeell.skymodloader | Out-Null
        & adb -s $device shell monkey -p git.artdeell.skymodloader -c android.intent.category.LAUNCHER 1 | Out-Null
    }
}

Write-Host "Concluido. libaura.so atualizada em /sdcard/Download/libaura.so"
