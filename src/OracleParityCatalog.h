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

constexpr int kOptionNone = -1;

constexpr std::array<OracleFeatureSpec, 5> kProtectionFeatures = {{
    {"antiAFK", "Anti AFK", "Evita desconexao por inatividade", 3, true},
    {"antiRainDrain", "Anti chuva", "Bloqueia perda de energia na chuva", 1, true},
    {"antiKrill", "Anti krill", "Mitiga comportamento agressivo de krill", 2, true},
    {"invincibility", "Invencibilidade", "Bloqueia dano direto", 0, false},
    {"autoCharge", "Recarga automatica", "Recupera energia automaticamente", 4, true},
}};

constexpr std::array<OracleFeatureSpec, 5> kPowerFeatures = {{
    {"superRun", "Super corrida", "Aumenta velocidade de corrida", 5, true},
    {"superFlight", "Super voo", "Aumenta controle de voo", 7, true},
    {"superLaunch", "Super lancamento", "Aumenta impulso inicial", kOptionNone, false},
    {"fastFlap", "Batida rapida", "Aumenta ritmo de batidas", kOptionNone, false},
    {"superSlidey", "Super escorregadio", "Ajusta atrito em superficies", kOptionNone, false},
}};

constexpr std::array<OracleFeatureSpec, 4> kWorldFeatures = {{
    {"quickTeleportMenu", "Teleporte rapido", "Abre menu de teleporte", kOptionNone, false},
    {"joinRandom", "Entrar aleatorio", "Tenta entrar em sessao random", kOptionNone, false},
    {"collectGifts", "Coletar presentes", "Automatiza coleta de presentes", kOptionNone, false},
    {"giftAllLight", "Dar toda luz", "Dispara sequencia de gift light", kOptionNone, false},
}};

constexpr std::array<OracleFeatureSpec, 4> kAccountFeatures = {{
    {"accountManager", "Gerenciador de conta", "Fluxo de conta do TSM", kOptionNone, false},
    {"saveAccounts", "Salvar contas", "Armazena sessao local", kOptionNone, false},
    {"exportAccounts", "Exportar contas", "Exporta sessao para arquivo", kOptionNone, false},
    {"relog", "Relogar", "Reinicia sessao da conta ativa", kOptionNone, false},
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
    for (const auto& feature : kWorldFeatures) {
        fn(feature, ParityTab::Mundo);
    }
    for (const auto& feature : kAccountFeatures) {
        fn(feature, ParityTab::Conta);
    }
    for (const auto& feature : kOverlayFeatures) {
        fn(feature, ParityTab::Overlay);
    }
}

