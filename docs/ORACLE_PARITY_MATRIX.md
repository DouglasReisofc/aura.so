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

## Features com layout migrado (logica parcial)
- `superLaunch`
- `fastFlap`
- `superSlidey`
- `quickTeleportMenu`
- `joinRandom`
- `collectGifts`
- `giftAllLight`
- `accountManager`
- `saveAccounts`
- `exportAccounts`
- `relog`

## Overlay nativo (host API)
- `frameRateLimit` via `setFrameRateLimited`
- `gameSpeed` via `setGameSpeed`
- telemetria com backend/profile/runtime
