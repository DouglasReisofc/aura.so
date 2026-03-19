# Aura.so - Fila de Integracao por Blocos

## Bloco 1 - Build estavel do `.so` (prioridade maxima)
- Objetivo: garantir `assembleDebug` sem erro de link.
- Origem principal:
  - `gxosty/Userlib-SML-NDK-only` (pipeline CMake/NDK)
  - `RomanChamelo/userlib-mod-menu-canvas` (bootstrap minimo)
- Criterio de pronto:
  - gera `libaura.so` com secao `.config` e `.icon`.

## Bloco 2 - Contrato com Canvas ModLoader
- Objetivo: manter `Start`, `Init`, `InitLate`, `Menu` no formato esperado.
- Origem principal:
  - `userlib-mod-menu-canvas`
  - `SkyRobinHood` (organizacao interna do modulo)
- Criterio de pronto:
  - modulo carregado no Canvas sem crash ao habilitar.

## Bloco 3 - Migracao funcional do menu
- Objetivo: migrar funcoes uma a uma sem mudar contrato do loader.
- Origem principal:
  - codigo atual do `aura.so` (AutoWax4C importado)
  - padroes de estrutura do `SkyRobinHood`.
- Criterio de pronto:
  - cada toggle altera comportamento real e possui log de ativacao.

## Bloco 4 - Paridade e validacao
- Objetivo: comparar contra APK alvo e garantir regressao zero no fluxo principal.
- Origem principal:
  - `canvas-rebuild` + artefatos de validacao (manifest/assets/libs/classes).
- Criterio de pronto:
  - smoke test completo sem fechamento inesperado.

