#include "AuraHostApi.h"
#include "OracleParityCatalog.h"

#include <android/log.h>

#include <array>
#include <cstring>
#include <string>

namespace {
constexpr const char* kLogTag = "TSMRebuild";
const AuraHostApi* g_hostApi = nullptr;
ParityTab g_currentTab = ParityTab::Protecoes;
bool g_showOnlyAvailable = false;
bool g_showTelemetry = true;
float g_gameSpeedMultiplier = 1.15f;
bool g_lastActionSuccess = false;
std::string g_lastActionKey = "nenhuma";
std::string g_lastActionMessage = "Sem acao executada nesta sessao";

const AuraHostApi* ResolveHostApi() {
    if (g_hostApi == nullptr || g_hostApi->abiVersion != AURA_HOST_API_VERSION) {
        return nullptr;
    }
    return g_hostApi;
}

const char* TabLabel(ParityTab tab) {
    switch (tab) {
        case ParityTab::Protecoes:
            return "Protecoes";
        case ParityTab::Poderes:
            return "Poderes";
        case ParityTab::Mundo:
            return "Mundo";
        case ParityTab::Conta:
            return "Conta";
        case ParityTab::Overlay:
            return "Overlay";
        case ParityTab::Migracao:
            return "Migracao";
    }
    return "Desconhecido";
}

void TextDisabledLine(const AuraHostApi* hostApi, const char* left, const char* right) {
    std::string line = std::string(left) + right;
    hostApi->textDisabled(line.c_str());
}

void SetLastActionStatus(const char* key, const char* message, bool success) {
    g_lastActionSuccess = success;
    g_lastActionKey = (key != nullptr && key[0] != '\0') ? key : "desconhecida";
    g_lastActionMessage = (message != nullptr && message[0] != '\0') ? message : "Sem detalhes";
}

const OracleFeatureSpec* FindFeatureByKey(const char* key) {
    if (key == nullptr || key[0] == '\0') {
        return nullptr;
    }

    const OracleFeatureSpec* found = nullptr;
    ForEachFeature([&](const OracleFeatureSpec& feature, ParityTab) {
        if (found != nullptr) {
            return;
        }
        if (std::strcmp(feature.key, key) == 0) {
            found = &feature;
        }
    });
    return found;
}

bool IsSpecialOverlayFeature(const char* key) {
    return key != nullptr
        && (std::strcmp(key, "frameRateLimit") == 0
            || std::strcmp(key, "gameSpeed") == 0
            || std::strcmp(key, "showDebugTelemetry") == 0);
}

bool IsFeatureAvailable(const AuraHostApi* hostApi, const OracleFeatureSpec& feature) {
    if (std::strcmp(feature.key, "frameRateLimit") == 0) {
        return true;
    }
    if (std::strcmp(feature.key, "gameSpeed") == 0) {
        return hostApi->isGameSpeedSupported();
    }
    if (std::strcmp(feature.key, "showDebugTelemetry") == 0) {
        return true;
    }
    if (feature.optionIndex != kOptionNone && hostApi->isLegacyBoolOptionAvailable(feature.optionIndex)) {
        return true;
    }
    return hostApi->isLegacyFeatureAvailable(feature.key);
}

bool IsActionLikelyAvailable(const AuraHostApi* hostApi, const OracleActionSpec& action) {
    if (hostApi->isLegacyFeatureAvailable(action.key)) {
        return true;
    }
    const OracleFeatureSpec* feature = FindFeatureByKey(action.key);
    if (feature != nullptr) {
        return IsFeatureAvailable(hostApi, *feature);
    }
    return false;
}

bool LoadFeatureValue(const AuraHostApi* hostApi, const OracleFeatureSpec& feature, bool* outValue) {
    if (outValue == nullptr) {
        return false;
    }

    if (std::strcmp(feature.key, "frameRateLimit") == 0) {
        *outValue = hostApi->isFrameRateLimited();
        return true;
    }
    if (std::strcmp(feature.key, "gameSpeed") == 0) {
        bool persisted = false;
        hostApi->loadPersistedBool("gameSpeed", false, &persisted);
        *outValue = persisted;
        return true;
    }
    if (std::strcmp(feature.key, "showDebugTelemetry") == 0) {
        bool persisted = true;
        hostApi->loadPersistedBool("showDebugTelemetry", true, &persisted);
        *outValue = persisted;
        return true;
    }

    bool value = false;
    if (hostApi->getLegacyBoolValue(feature.key, &value)) {
        *outValue = value;
        return true;
    }
    if (hostApi->loadPersistedBool(feature.key, false, &value)) {
        *outValue = value;
        return true;
    }
    *outValue = false;
    return false;
}

bool ApplyFeatureValue(const AuraHostApi* hostApi, const OracleFeatureSpec& feature, bool enabled) {
    if (std::strcmp(feature.key, "frameRateLimit") == 0) {
        hostApi->setFrameRateLimited(enabled);
        hostApi->savePersistedBool(feature.key, enabled);
        return true;
    }

    if (std::strcmp(feature.key, "gameSpeed") == 0) {
        const float speed = enabled ? g_gameSpeedMultiplier : 1.0f;
        const bool ok = hostApi->setGameSpeed(speed);
        hostApi->savePersistedBool(feature.key, enabled && ok);
        return ok;
    }

    if (std::strcmp(feature.key, "showDebugTelemetry") == 0) {
        g_showTelemetry = enabled;
        hostApi->savePersistedBool(feature.key, enabled);
        return true;
    }

    bool accepted = hostApi->requestLegacyFeatureApply(feature.key, enabled);
    if (!accepted) {
        accepted = hostApi->setLegacyBoolValue(feature.key, enabled);
    }
    if (!accepted && feature.optionIndex != kOptionNone && hostApi->isLegacyBoolOptionAvailable(feature.optionIndex)) {
        accepted = hostApi->setLegacyBoolOption(feature.optionIndex, enabled);
    }
    hostApi->savePersistedBool(feature.key, enabled);
    return accepted;
}

bool ExecuteAction(const AuraHostApi* hostApi, const OracleActionSpec& action) {
    bool success = hostApi->invokeLegacyAction(action.key);
    if (success) {
        SetLastActionStatus(action.key, "Acao aplicada via invokeLegacyAction", true);
        return true;
    }

    success = hostApi->requestLegacyFeatureApply(action.key, true);
    if (success) {
        hostApi->requestLegacyFeatureApply(action.key, false);
        SetLastActionStatus(action.key, "Acao aplicada via pulso requestLegacyFeatureApply", true);
        return true;
    }

    success = hostApi->setLegacyBoolValue(action.key, true);
    if (success) {
        hostApi->setLegacyBoolValue(action.key, false);
        SetLastActionStatus(action.key, "Acao aplicada via pulso setLegacyBoolValue", true);
        return true;
    }

    const OracleFeatureSpec* feature = FindFeatureByKey(action.key);
    if (feature != nullptr && feature->optionIndex != kOptionNone) {
        success = hostApi->setLegacyBoolOption(feature->optionIndex, true);
        if (success) {
            hostApi->setLegacyBoolOption(feature->optionIndex, false);
            SetLastActionStatus(action.key, "Acao aplicada via pulso setLegacyBoolOption", true);
            return true;
        }
    }

    SetLastActionStatus(action.key, "Acao indisponivel para o backend atual", false);
    return false;
}

void DrawFeatureStateTelemetry(const AuraHostApi* hostApi, const OracleFeatureSpec& feature) {
    if (!g_showTelemetry || !feature.telemetryPriority || IsSpecialOverlayFeature(feature.key)) {
        return;
    }
    const char* runtime = hostApi->getLegacyFeatureRuntimeState(feature.key);
    TextDisabledLine(hostApi, "Estado: ", runtime != nullptr ? runtime : "Unknown");
    if (std::strcmp(feature.key, "superRun") == 0) {
        TextDisabledLine(hostApi, "Ultimo evento: ", hostApi->getLastLegacyFeatureTimestamp("superRun"));
        hostApi->textWrapped(hostApi->getLastLegacyFeatureError("superRun"));
    }
}

void DrawFeatureRow(const AuraHostApi* hostApi, const OracleFeatureSpec& feature) {
    const bool available = IsFeatureAvailable(hostApi, feature);
    if (g_showOnlyAvailable && !available) {
        return;
    }

    bool value = false;
    LoadFeatureValue(hostApi, feature, &value);
    std::string label = feature.label;
    if (!available) {
        label += " [indisponivel]";
    }

    const bool changed = hostApi->toggle(label.c_str(), &value, feature.hint);
    if (changed && available) {
        const bool applied = ApplyFeatureValue(hostApi, feature, value);
        if (!applied) {
            __android_log_print(
                ANDROID_LOG_WARN,
                kLogTag,
                "Aplicacao falhou para feature=%s value=%d",
                feature.key,
                value ? 1 : 0
            );
        }
    }
    DrawFeatureStateTelemetry(hostApi, feature);
}

template <std::size_t N>
void DrawFeatureSection(const AuraHostApi* hostApi, const char* title, const std::array<OracleFeatureSpec, N>& features) {
    hostApi->separator();
    hostApi->textColored(0.14f, 0.82f, 0.43f, 1.0f, title);
    for (const auto& feature : features) {
        DrawFeatureRow(hostApi, feature);
    }
}

template <std::size_t N>
void DrawActionSection(const AuraHostApi* hostApi, const char* title, const std::array<OracleActionSpec, N>& actions) {
    hostApi->separator();
    hostApi->textColored(0.86f, 0.76f, 0.18f, 1.0f, title);
    for (const auto& action : actions) {
        const bool likelyAvailable = IsActionLikelyAvailable(hostApi, action);
        if (g_showOnlyAvailable && !likelyAvailable) {
            continue;
        }

        std::string actionLabel = action.label;
        if (!likelyAvailable) {
            actionLabel += " [indisponivel]";
        }
        if (hostApi->button(actionLabel.c_str())) {
            const bool ok = ExecuteAction(hostApi, action);
            __android_log_print(
                ok ? ANDROID_LOG_INFO : ANDROID_LOG_WARN,
                kLogTag,
                "Action %s result=%d",
                action.key,
                ok ? 1 : 0
            );
        }
        hostApi->textDisabled(action.hint);
    }
}

void DrawTabControls(const AuraHostApi* hostApi) {
    constexpr std::array<ParityTab, 6> tabs = {{
        ParityTab::Protecoes,
        ParityTab::Poderes,
        ParityTab::Mundo,
        ParityTab::Conta,
        ParityTab::Overlay,
        ParityTab::Migracao,
    }};

    for (std::size_t i = 0; i < tabs.size(); ++i) {
        const ParityTab tab = tabs[i];
        std::string buttonLabel;
        if (g_currentTab == tab) {
            buttonLabel = std::string("> ") + TabLabel(tab);
        } else {
            buttonLabel = TabLabel(tab);
        }
        if (hostApi->button(buttonLabel.c_str())) {
            g_currentTab = tab;
        }
        if (i + 1 < tabs.size()) {
            hostApi->sameLine();
        }
    }
}

void DrawMigrationPanel(const AuraHostApi* hostApi) {
    TextDisabledLine(hostApi, "Backend: ", hostApi->getLegacyBackendStatusLabel());
    TextDisabledLine(hostApi, "Perfil de compatibilidade: ", hostApi->getLegacyCompatibilityProfile());
    TextDisabledLine(hostApi, "Sky build key: ", hostApi->getSkyBuildKey());

    bool localShowOnlyAvailable = g_showOnlyAvailable;
    if (hostApi->toggle("Mostrar apenas disponiveis", &localShowOnlyAvailable, "Oculta funcoes sem backend mapeado")) {
        g_showOnlyAvailable = localShowOnlyAvailable;
        hostApi->savePersistedBool("showOnlyAvailable", g_showOnlyAvailable);
    }

    bool localTelemetry = g_showTelemetry;
    if (hostApi->toggle("Mostrar telemetria", &localTelemetry, "Mostra estado e ultimo erro do backend")) {
        g_showTelemetry = localTelemetry;
        hostApi->savePersistedBool("showDebugTelemetry", g_showTelemetry);
    }

    if (hostApi->button("Sincronizar estado do oracle")) {
        const bool reloaded = hostApi->reloadLegacyState();
        __android_log_print(ANDROID_LOG_INFO, kLogTag, "reloadLegacyState=%d", reloaded ? 1 : 0);
    }
    hostApi->sameLine();
    if (hostApi->button("Aplicar fila pendente")) {
        hostApi->pumpLegacyFeatureRequests();
    }

    if (hostApi->button("Resetar baseline de migracao")) {
        ForEachFeature([&](const OracleFeatureSpec& feature, ParityTab tab) {
            if (tab == ParityTab::Migracao) {
                return;
            }
            if (IsSpecialOverlayFeature(feature.key)) {
                return;
            }
            ApplyFeatureValue(hostApi, feature, false);
        });
        hostApi->setFrameRateLimited(false);
        hostApi->setGameSpeed(1.0f);
        hostApi->savePersistedBool("gameSpeed", false);
    }

    hostApi->separator();
    hostApi->textColored(
        g_lastActionSuccess ? 0.22f : 0.88f,
        g_lastActionSuccess ? 0.84f : 0.26f,
        g_lastActionSuccess ? 0.34f : 0.26f,
        1.0f,
        g_lastActionSuccess ? "Ultima acao: sucesso" : "Ultima acao: falha"
    );
    TextDisabledLine(hostApi, "Chave: ", g_lastActionKey.c_str());
    hostApi->textWrapped(g_lastActionMessage.c_str());
}

void DrawCurrentTab(const AuraHostApi* hostApi) {
    switch (g_currentTab) {
        case ParityTab::Protecoes:
            DrawFeatureSection(hostApi, "Protecoes", kProtectionFeatures);
            break;
        case ParityTab::Poderes:
            DrawFeatureSection(hostApi, "Poderes", kPowerFeatures);
            DrawFeatureSection(hostApi, "Transformacoes", kTransformationFeatures);
            break;
        case ParityTab::Mundo:
            DrawFeatureSection(hostApi, "Mundo", kWorldFeatures);
            DrawActionSection(hostApi, "Acoes rapidas (Mundo)", kWorldActions);
            break;
        case ParityTab::Conta:
            DrawFeatureSection(hostApi, "Conta", kAccountFeatures);
            DrawActionSection(hostApi, "Acoes de conta", kAccountActions);
            break;
        case ParityTab::Overlay:
            DrawFeatureSection(hostApi, "Overlay", kOverlayFeatures);
            hostApi->separator();
            hostApi->textDisabled("Ajuste manual de velocidade (aplica quando gameSpeed estiver ON)");
            if (hostApi->sliderFloat("##game_speed_slider", &g_gameSpeedMultiplier, 0.5f, 3.0f, "%.2fx")) {
                bool gameSpeedEnabled = false;
                hostApi->loadPersistedBool("gameSpeed", false, &gameSpeedEnabled);
                if (gameSpeedEnabled) {
                    hostApi->setGameSpeed(g_gameSpeedMultiplier);
                }
            }
            break;
        case ParityTab::Migracao:
            DrawFeatureSection(hostApi, "Idioma / Traducoes", kLocalizationFeatures);
            DrawActionSection(hostApi, "Acoes de migracao", kMigrationActions);
            DrawMigrationPanel(hostApi);
            break;
    }
}

void Draw(bool* open) {
    const AuraHostApi* hostApi = ResolveHostApi();
    if (hostApi == nullptr) {
        __android_log_print(ANDROID_LOG_WARN, kLogTag, "Draw skipped: host API missing or ABI mismatch");
        return;
    }

    if (open != nullptr && !*open) {
        return;
    }

    hostApi->loadPersistedBool("showDebugTelemetry", true, &g_showTelemetry);
    hostApi->loadPersistedBool("showOnlyAvailable", false, &g_showOnlyAvailable);

    hostApi->pumpLegacyFeatureRequests();
    hostApi->setNextWindowDefaultPos(28.0f, 24.0f);
    hostApi->setNextWindowDefaultSize(760.0f, 680.0f);

    if (!hostApi->beginPanelWindow("##tsm_rebuild_core_window", open)) {
        hostApi->endWindow();
        return;
    }

    hostApi->setWindowFontScale(1.12f);
    hostApi->textColored(0.12f, 0.88f, 0.44f, 1.0f, "PAINEL AURA - CORE REBUILD");
    hostApi->separator();
    DrawTabControls(hostApi);
    DrawCurrentTab(hostApi);

    hostApi->endWindow();
}
}  // namespace

extern "C" __attribute__((visibility("default"))) void SetAuraHostApi(const AuraHostApi* hostApi) {
    g_hostApi = hostApi;
}

extern "C" __attribute__((visibility("default"))) void* Start() {
    return reinterpret_cast<void*>(&Draw);
}

extern "C" __attribute__((visibility("default"))) void InitLate() {
    const AuraHostApi* hostApi = ResolveHostApi();
    if (hostApi != nullptr) {
        hostApi->reloadLegacyState();
    }
}

extern "C" __attribute__((used, section(".config"), aligned(1))) const char kTSMRebuildConfig[] =
    "{\"name\":\"libTSMRebuild.so\",\"displayName\":\"That Sky Mod\",\"author\":\"AuraFlow\","
    "\"description\":\"Migracao do nucleo TSM com foco em paridade do oracle original.\","
    "\"majorVersion\":0,\"minorVersion\":3,\"patchVersion\":0,"
    "\"dependencies\":[],\"displaysUI\":true,\"selfManagedUI\":true,"
    "\"githubReleasesUrl\":\"https://api.github.com/repos/DouglasReisofc/aura.so/releases/latest\"}";
