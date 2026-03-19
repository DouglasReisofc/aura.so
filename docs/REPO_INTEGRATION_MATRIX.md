# Aura.so - Matriz de Integracao dos Repositorios (links das imagens)

## Base canonica (rebuild do app Canvas)
- `skyprotocol/Canvas-Open-Source` (`osp-master`)
  - Funcao: base principal do app Android (launcher, bridge Java, CMake nativo `ciphered`).
  - Status: usar como **fonte principal**.
- `MisterGatto/Canvas-Open-Source-test`
  - Funcao: espelho/fork.
  - Status: usar apenas para diff pontual quando faltar algo no upstream.
- Forks `Canvas-Open-Source` de `RomanChamelo`, `Kiojeen`, `gxosty`, `HugeFrog24`, `HungWah2`, `alvindimas05`
  - Funcao: forks secundarios.
  - Status: consultar somente para comparar patches isolados.

## Base canonica (novo modulo nativo `aura.so`)
- `RomanChamelo/userlib-mod-menu-canvas`
  - Funcao: template minimo de userlib para Canvas (estrutura simples para iniciar).
  - Status: boa base de entrada para bootstrap rapido.
- `gxosty/Userlib-SML-NDK-only`
  - Funcao: template NDK/CMake standalone, bom para pipeline de build do `.so`.
  - Status: fonte principal para padronizar build do `aura.so`.
- `alvindimas05/SkyRobinHood`
  - Funcao: arquitetura modular (model/view/controller + shared sources).
  - Status: referencia para organizar codigo interno do `aura.so`.

## Menus/ImGui e historico
- `Polarmods/PolarImGui`, `lukas0x1/PolarImGui`, `Kiojeen/PolarImGui`
  - Funcao: templates/forks de menu ImGui.
  - Status: referencia de UI/estrutura (nao base principal do Canvas).
- `Polarmods/MagiskPolarImGui`
  - Funcao: variante Magisk.
  - Status: referencia secundaria.

## Repositorios auxiliares
- `artdeell/SkySessionPuller`
  - Funcao: ferramenta auxiliar de sessao.
  - Status: nao integrar no app principal por padrao.
- `achcyano/SkyHub`, `HugeFrog24/libtibik`
  - Funcao: repositorios de distribuicao/info (sem source completo util para rebuild).
  - Status: tratar como referencia de release, nao como base de codigo.

## Repositorios fora de escopo para este rebuild
- `RadiatedExodus/*` (conteudo nao Canvas/Sky ModLoader core).
  - Status: ignorar para a fusao do projeto.

## Estrategia de "juntar tudo" sem quebrar
1. Manter `canvas-rebuild` alinhado ao upstream `skyprotocol/Canvas-Open-Source`.
2. Tratar o APK recuperado como alvo de validacao (manifest, libs, assets, classes).
3. Construir `aura.so` em trilha propria usando:
   - bootstrap de `userlib-mod-menu-canvas`;
   - pipeline de build de `Userlib-SML-NDK-only`;
   - modularizacao inspirada em `SkyRobinHood`.
4. Importar blocos de forks apenas por diff comprovado.
5. Registrar cada importacao com origem (repo/commit) para rastreabilidade.

