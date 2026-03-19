# Auditoria de Atualizacao de Repositorios (2026-03-19)

## Objetivo
Verificar fontes mais atuais para tirar o `aura.so` do estado de funcoes `offline`.

## Resultado rapido
- Existem updates recentes de compatibilidade, mas os mais atuais sao **binarios-only** (sem source dos hooks).
- O source de `AutoWax4C` (base do `aura.so`) esta defasado para Sky atual.
- Para source aberto, a melhor trilha recente e `SkyRobinHood` (padroes atualizados em 2025-12-06), mas focado em hooks de gameplay/local, nao no fluxo de credencial AW4C.

## Repositorios chave (estado atual)
- `XeTrinityz/ThatSkyMod-Android`
  - Release mais recente: `0.25.0` (publicada em 2026-02-28).
  - Nota da release: compatibilidade com **Sky V32.5.370662**.
  - Ativo, porem repo de distribuicao (`Canvas.apk` + `libTSM.so`), sem source dos hooks.
- `HugeFrog24/libtibik`
  - Release mais recente: `v1.0.8` (2026-03-17).
  - Historico de hotfixes frequente (inclusive compatibilidade).
  - Tambem binario-only para distribuicao.
- `skyprotocol/Canvas-Open-Source`
  - Ultimo push: 2026-03-17.
  - Serve como base do app/loader Canvas (host), nao como fonte direta dos hooks do seu modulo AW4C.
- `MisterGatto/Canvas-Open-Source-test`
  - Ultimo push: 2026-03-16.
  - Fork espelho, util para diff pontual.
- `artdeell/AutoWax4C`
  - Ultimo push de codigo: 2024-05-11.
  - Base antiga; os patterns de autenticacao atuais do `aura.so` vieram daqui e estao desatualizados.
- `alvindimas05/SkyRobinHood`
  - Release `v1.1.0` em 2025-12-06.
  - Changelog cita: "Fix update patterns to latest Sky version".
  - Source aberto util para estrategia de pattern scanning/hook robusto.

## Diagnostico tecnico para o `aura.so`
- O gargalo atual nao e UI nem carregamento do modulo.
- O gargalo e **aquisicao de credenciais/sessao** no fluxo AW4C:
  - `app/src/main/cpp/main.cpp` usa pattern antigo em `get_gameptr()` e estrutura fixa (`Game[58]`, offsets de UUID/session).
  - Quando esse pattern falha na versao atual do jogo, as funcoes de rede ficam indisponiveis.
- Nao foi encontrado, entre os repositorios analisados, um source publico recente com substituto pronto para esse fluxo AW4C de credenciais.

## Conclusao pratica
- Update util imediato para "funcionar agora":
  - usar o binario mais novo (`libTSM.so`/`libtibik.so`) quando quiser paridade de funcoes sem reconstruir hooks.
- Update util para "manter source propria":
  - migrar o `aura.so` para estrategia com pattern scanning moderno (linha `SkyRobinHood`) e reconstruir o bloco de credenciais do AW4C para Sky 32.x.

## Proxima acao recomendada
1. Tratar `AutoWax4C` como legado e isolar o bloco `get_Auth`.
2. Implementar novo provedor de sessao no `aura.so` (sem depender do pattern antigo de `Game[58]`).
3. Reabilitar funcoes de rede por etapas, validando cada endpoint (`/account/get_currency`, `/account/buy_candle_wax`, etc.).

