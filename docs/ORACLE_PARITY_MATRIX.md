# Oracle Parity Matrix (TSM)

Referencia: `libTSM.so` (oracle original) e comportamento observado no Canvas.

## Superficie obrigatoria
- Exports:
  - `Start`
  - `InitLate`
- Config:
  - `displayName`: `That Sky Mod`
  - `githubReleasesUrl`: endpoint GitHub Releases

## Features mapeadas nesta fase
| Grupo | Feature key | Option index | Estrategia atual |
| --- | --- | --- | --- |
| Protecoes | `antiAFK` | `3` | oracle-first (`requestLegacyFeatureApply`) com fallback |
| Protecoes | `antiRainDrain` | `1` | oracle-first com fallback |
| Protecoes | `antiKrill` | `2` | oracle-first com fallback |
| Protecoes | `invincibility` | `0` | oracle-first com fallback |
| Protecoes | `autoCharge` | `4` | oracle-first com fallback |
| Poderes | `superRun` | `5` | oracle-first com telemetria de runtime |
| Poderes | `superFlight` | `7` | oracle-first com fallback |
| Protecoes | `infinite_energy` | `-` | chave migrada no layout, aguardando mapeamento de backend |
| Poderes | `super_launch` | `-` | chave migrada no layout, aguardando mapeamento de backend |
| Poderes | `fast_flap` | `-` | chave migrada no layout, aguardando mapeamento de backend |
| Poderes | `super_slidey` | `-` | chave migrada no layout, aguardando mapeamento de backend |
| Poderes | `spell_emitter` | `-` | chave migrada no layout, aguardando mapeamento de backend |
| Poderes | `scooter_mode` | `-` | chave migrada no layout, aguardando mapeamento de backend |
| Poderes | `rainbow_glow` | `-` | chave migrada no layout, aguardando mapeamento de backend |
| Poderes | `rainbow_trails` | `-` | chave migrada no layout, aguardando mapeamento de backend |
| Poderes | `bubble_trails` | `-` | chave migrada no layout, aguardando mapeamento de backend |
| Poderes | `unlimited_fireworks` | `-` | chave migrada no layout, aguardando mapeamento de backend |
| Poderes | `crawl_mode` | `-` | chave migrada no layout, aguardando mapeamento de backend |
| Poderes | `fake_cape_level` | `-` | chave migrada no layout, aguardando mapeamento de backend |

## Features com layout migrado (logica parcial)
- Transformacoes:
  - `transform_jellyfish`
  - `transform_crab`
  - `transform_broom`
  - `transform_fish`
  - `transform_bird`
  - `transform_manta`
  - `transform_butterfly`
  - `transform_paper`
  - `transform_stone_bust`
  - `transform_skid_flying`
  - `transform_skid_masked`
- Mundo / movimento:
  - `teleport_menu`
  - `anti_void`
  - `instant_finish`
  - `join_random`
  - `collect_gifts`
  - `gift_light`
  - `teleport_boost`
  - `movement_profiles`
  - `super_flight_tab`
- Conta / localizacao:
  - `account_export`
  - `account_import`
  - `account_quicksave`
  - `translation_list`
  - `translation_import`
  - `translation_reload`

## Acoes migradas no painel
- Conta:
  - `account_export`
  - `account_import`
  - `account_quicksave`
  - `relog`
- Mundo:
  - `join_random`
  - `collect_gifts`
  - `gift_light`
  - `teleport_menu`
  - `instant_finish`
  - `anti_void`
- Migracao / idioma:
  - `translation_list`
  - `translation_import`
  - `translation_reload`

As acoes seguem trilha em cascata:
1. `invokeLegacyAction`
2. pulso `requestLegacyFeatureApply`
3. pulso `setLegacyBoolValue`
4. pulso `setLegacyBoolOption` quando houver `optionIndex`

Compatibilidade adicional de chave:
- fallback automatico entre `snake_case` e variantes legadas em `camelCase` para aumentar acerto com backend antigo.

## Overlay nativo (host API)
- `frameRateLimit` via `setFrameRateLimited`
- `gameSpeed` via `setGameSpeed`
- telemetria com backend/profile/runtime
