# Aura SO

Repositorio dedicado ao nucleo nativo reconstruivel do painel Aura.

## Objetivo
- Recuperar e manter o source da `.so` com controle total.
- Evoluir features por versao sem depender de binario perdido.
- Publicar releases com `libTSMRebuild.so` para consumo no app.

## Estrutura
- `src/TSMRebuildMod.cpp`: implementacao principal do modulo.
- `include/AuraHostApi.h`: contrato ABI com o host (Canvas/Aura).
- `CMakeLists.txt`: build standalone Android.
- `scripts/make-release.ps1`: empacota artefato e manifesto.

## Build (Android NDK)
Exemplo com CMake:

```bash
cmake -S . -B build/android ^
  -DANDROID_ABI=arm64-v8a ^
  -DANDROID_PLATFORM=android-26 ^
  -DCMAKE_TOOLCHAIN_FILE=%ANDROID_NDK_HOME%/build/cmake/android.toolchain.cmake
cmake --build build/android --config Release
```

O artefato final sera `libTSMRebuild.so`.

## Release
Depois do build, execute:

```powershell
./scripts/make-release.ps1 -Tag 0.0.1 -Owner DouglasReisofc -Repo aura.so -InputSoPath .\build\android\libTSMRebuild.so
```

Isso gera `dist/tsm-rebuild/<tag>/` com:
- `libTSMRebuild.so`
- `release-manifest.json` (inclui hash SHA-256 e URL da API GitHub Releases)
