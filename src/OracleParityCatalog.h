#pragma once

#include <array>
#include <cstddef>

enum class ParityTab {
    Protecoes = 0,
    Poderes,
    Mundo,
    Conta,
    Overlay,
    Migracao,
};

struct OracleFeatureSpec {
    const char* key;
    const char* label;
    const char* hint;
    int optionIndex;
    bool telemetryPriority;
};

struct OracleActionSpec {
    const char* key;
    const char* label;
    const char* hint;
};

constexpr int kOptionNone = -1;

constexpr std::array<OracleFeatureSpec, 6> kProtectionFeatures = {{
    {"antiAFK", "Anti AFK", "Evita desconexao por inatividade", 3, true},
    {"antiRainDrain", "Anti chuva", "Bloqueia perda de energia na chuva", 1, true},
    {"antiKrill", "Anti krill", "Mitiga comportamento agressivo de krill", 2, true},
    {"invincibility", "Invencibilidade", "Bloqueia dano direto", 0, false},
    {"autoCharge", "Recarga automatica", "Recupera energia automaticamente", 4, true},
    {"infinite_energy", "Energia infinita", "Mantem energia sempre carregada", kOptionNone, false},
}};

constexpr std::array<OracleFeatureSpec, 13> kPowerFeatures = {{
    {"superRun", "Super corrida", "Aumenta velocidade de corrida", 5, true},
    {"superFlight", "Super voo", "Aumenta controle de voo", 7, true},
    {"super_launch", "Super lancamento", "Aumenta impulso inicial", kOptionNone, false},
    {"fast_flap", "Batida rapida", "Aumenta ritmo de batidas", kOptionNone, false},
    {"super_slidey", "Super escorregadio", "Ajusta atrito em superficies", kOptionNone, false},
    {"spell_emitter", "Emissor de feiticos", "Dispara efeito em area", kOptionNone, false},
    {"scooter_mode", "Modo scooter", "Troca locomocao para modo scooter", kOptionNone, false},
    {"rainbow_glow", "Brilho arco-iris", "Aplica brilho colorido", kOptionNone, false},
    {"rainbow_trails", "Rastros arco-iris", "Ativa rastros coloridos", kOptionNone, false},
    {"bubble_trails", "Rastros de bolha", "Ativa rastros de bolha", kOptionNone, false},
    {"unlimited_fireworks", "Fogos ilimitados", "Remove cooldown de fogos", kOptionNone, false},
    {"crawl_mode", "Modo rastejar", "Ativa animacao rastejando", kOptionNone, false},
    {"fake_cape_level", "Nivel falso da capa", "Forca nivel visual da capa", kOptionNone, false},
}};

constexpr std::array<OracleFeatureSpec, 11> kTransformationFeatures = {{
    {"transform_jellyfish", "Transformar em agua-viva", "Transforma em agua-viva", kOptionNone, false},
    {"transform_crab", "Transformar em caranguejo", "Transforma em caranguejo", kOptionNone, false},
    {"transform_broom", "Transformar em vassoura", "Transforma em vassoura", kOptionNone, false},
    {"transform_fish", "Transformar em peixe", "Transforma em peixe", kOptionNone, false},
    {"transform_bird", "Transformar em passaro", "Transforma em passaro", kOptionNone, false},
    {"transform_manta", "Transformar em manta", "Transforma em manta", kOptionNone, false},
    {"transform_butterfly", "Transformar em borboleta", "Transforma em borboleta", kOptionNone, false},
    {"transform_paper", "Transformar em papel", "Transforma em papel amassado", kOptionNone, false},
    {"transform_stone_bust", "Transformar em busto", "Transforma em busto de pedra", kOptionNone, false},
    {"transform_skid_flying", "Transformar em SkyKid voador", "Transforma em SkyKid voador", kOptionNone, false},
    {"transform_skid_masked", "Transformar em SkyKid mascarado", "Transforma em SkyKid mascarado", kOptionNone, false},
}};

constexpr std::array<OracleFeatureSpec, 9> kWorldFeatures = {{
    {"teleport_menu", "Teleporte rapido", "Abre menu de teleporte", kOptionNone, false},
    {"anti_void", "Anti vazio", "Evita queda no vazio", kOptionNone, false},
    {"instant_finish", "Assistente de missao", "Tenta concluir interacoes de missao", kOptionNone, false},
    {"join_random", "Entrar aleatorio", "Tenta entrar em sessao aleatoria", kOptionNone, false},
    {"collect_gifts", "Coletar presentes", "Automatiza coleta de presentes", kOptionNone, false},
    {"gift_light", "Dar toda luz", "Automatiza envio de luz", kOptionNone, false},
    {"teleport_boost", "Impulso de teleporte", "Amplifica deslocamento de teleporte", kOptionNone, false},
    {"movement_profiles", "Perfis de movimento", "Alterna perfis predefinidos de movimento", kOptionNone, false},
    {"super_flight_tab", "Super voo (tab)", "Atalho do bloco de movimento para super voo", kOptionNone, false},
}};

constexpr std::array<OracleFeatureSpec, 3> kLocalizationFeatures = {{
    {"translation_list", "Lista de traducoes", "Gerencia traducoes carregadas", kOptionNone, false},
    {"translation_import", "Importar traducao", "Importa pacote de traducao", kOptionNone, false},
    {"translation_reload", "Recarregar traducoes", "Recarrega traducoes em runtime", kOptionNone, false},
}};

constexpr std::array<OracleFeatureSpec, 3> kAccountFeatures = {{
    {"account_export", "Exportar dados", "Exporta dados de conta", kOptionNone, false},
    {"account_import", "Importar dados", "Importa dados de conta", kOptionNone, false},
    {"account_quicksave", "Salvamento rapido", "Salva conta atual rapidamente", kOptionNone, false},
}};

constexpr std::array<OracleActionSpec, 4> kAccountActions = {{
    {"account_export", "Executar exportacao", "Dispara exportacao imediata"},
    {"account_import", "Executar importacao", "Dispara importacao imediata"},
    {"account_quicksave", "Executar quicksave", "Salva estado atual da conta"},
    {"relog", "Relogar", "Tenta religar sessao atual"},
}};

constexpr std::array<OracleActionSpec, 6> kWorldActions = {{
    {"join_random", "Acao: entrar aleatorio", "Dispara entrada aleatoria"},
    {"collect_gifts", "Acao: coletar presentes", "Dispara coleta de presentes"},
    {"gift_light", "Acao: dar luz", "Dispara envio de luz"},
    {"teleport_menu", "Acao: abrir teleporte", "Dispara abertura do menu de teleporte"},
    {"instant_finish", "Acao: assistente de missao", "Dispara assistente de missao"},
    {"anti_void", "Acao: anti vazio", "Reaplica protecao anti vazio"},
}};

constexpr std::array<OracleActionSpec, 3> kMigrationActions = {{
    {"translation_list", "Acao: lista traducoes", "Abre lista de traducoes"},
    {"translation_import", "Acao: importar traducao", "Dispara fluxo de importacao"},
    {"translation_reload", "Acao: recarregar traducao", "Recarrega traducoes em runtime"},
}};

constexpr std::array<OracleFeatureSpec, 3> kOverlayFeatures = {{
    {"frameRateLimit", "Limitar FPS", "Trava FPS no overlay", kOptionNone, false},
    {"gameSpeed", "Velocidade do jogo", "Speedhack pelo backend CE", kOptionNone, false},
    {"showDebugTelemetry", "Telemetria detalhada", "Exibe estado de backend e erros", kOptionNone, true},
}};

template <typename Fn>
void ForEachFeature(Fn&& fn) {
    for (const auto& feature : kProtectionFeatures) {
        fn(feature, ParityTab::Protecoes);
    }
    for (const auto& feature : kPowerFeatures) {
        fn(feature, ParityTab::Poderes);
    }
    for (const auto& feature : kTransformationFeatures) {
        fn(feature, ParityTab::Poderes);
    }
    for (const auto& feature : kWorldFeatures) {
        fn(feature, ParityTab::Mundo);
    }
    for (const auto& feature : kLocalizationFeatures) {
        fn(feature, ParityTab::Migracao);
    }
    for (const auto& feature : kAccountFeatures) {
        fn(feature, ParityTab::Conta);
    }
    for (const auto& feature : kOverlayFeatures) {
        fn(feature, ParityTab::Overlay);
    }
}
