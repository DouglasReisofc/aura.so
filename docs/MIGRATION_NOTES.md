# Migration Notes

## Estado inicial
- Rebuild criado com ABI compativel com host Aura/Canvas.
- Exports suportados:
  - `SetAuraHostApi`
  - `Start`
  - `InitLate`
- Config `.config` embutido para fluxo de updater.
- Layout em abas migrado para espelhar o fluxo do painel original.
- Aplicacao de feature em trilha `oracle-first` com fallback de `optionIndex` quando suportado pelo host.
- Bloco atual migrado:
  - transformacoes do painel original
  - mundo/conta/localizacao por chaves legadas
  - acoes com cascata de execucao e telemetria da ultima acao

## Estrategia
1. Validar estabilidade do shell do modulo.
2. Migrar features do oracle por lotes pequenos.
3. Registrar offsets/perfis por versao de jogo.
4. Publicar release a cada lote validado em device.

## Meta de curto prazo
- Paridade de `superRun`.
- Paridade de `superFlight`.
- Paridade de protecoes basicas.
- Consolidar mapeamento de offsets/chaves para features sem `optionIndex`.
- Promover `libTSMRebuild.so` para `libTSM.so` quando o checklist de estabilidade estiver fechado.
