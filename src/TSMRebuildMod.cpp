#include "AuraHostApi.h"

#include <android/log.h>

#include <array>
#include <string>

namespace {
constexpr const char* kLogTag = "TSMRebuild";
const AuraHostApi* g_hostApi = nullptr;

struct FeatureEntry {
    const char* key;
    const char* label;
    const char* hint;
};

constexpr std::array<FeatureEntry, 6> kBaselineFeatures = {{
    {"superRun", "Super corrida", "Ativa corrida acelerada"},
    {"superFlight", "Super voo", "Ativa melhoria de voo"},
    {"antiAFK", "Anti AFK", "Evita desconexao por inatividade"},
    {"antiRainDrain", "Anti chuva", "Reduz perda de energia na chuva"},
    {"antiKrill", "Anti krill", "Mitiga deteccao de krill"},
    {"autoCharge", "Recarga automatica", "Recupera energia automaticamente"},
}};

const AuraHostApi* ResolveHostApi() {
    if (g_hostApi == nullptr || g_hostApi->abiVersion != AURA_HOST_API_VERSION) {
        return nullptr;
    }
    return g_hostApi;
}

bool LoadFeatureValue(const AuraHostApi* hostApi, const char* key) {
    bool currentValue = false;
    hostApi->getLegacyBoolValue(key, &currentValue);
    hostApi->loadPersistedBool(key, currentValue, &currentValue);
    return currentValue;
}

void SaveFeatureValue(const AuraHostApi* hostApi, const char* key, bool value) {
    hostApi->setLegacyBoolValue(key, value);
    hostApi->savePersistedBool(key, value);
    hostApi->requestLegacyFeatureApply(key, value);
}

void DrawFeatureRow(const AuraHostApi* hostApi, const FeatureEntry& feature) {
    const bool available = hostApi->isLegacyFeatureAvailable(feature.key);
    bool value = LoadFeatureValue(hostApi, feature.key);
    const bool changed = hostApi->toggle(feature.label, &value, feature.hint);
    if (changed && available) {
        SaveFeatureValue(hostApi, feature.key, value);
    }
    if (!available) {
        hostApi->textDisabled("indisponivel para esta build");
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

    hostApi->pumpLegacyFeatureRequests();
    hostApi->setNextWindowDefaultPos(28.0f, 24.0f);
    hostApi->setNextWindowDefaultSize(520.0f, 640.0f);

    if (!hostApi->beginPanelWindow("##tsm_rebuild_core_window", open)) {
        hostApi->endWindow();
        return;
    }

    hostApi->setWindowFontScale(1.12f);
    hostApi->textColored(0.12f, 0.88f, 0.44f, 1.0f, "Nucleo TSM Rebuild");
    hostApi->separator();
    hostApi->textDisabled((std::string("Backend: ") + hostApi->getLegacyBackendStatusLabel()).c_str());
    hostApi->textDisabled((std::string("Perfil: ") + hostApi->getLegacyCompatibilityProfile()).c_str());
    hostApi->spacing();

    for (const auto& feature : kBaselineFeatures) {
        DrawFeatureRow(hostApi, feature);
        hostApi->spacing();
    }

    hostApi->separator();
    if (hostApi->button("Sincronizar estado do oracle")) {
        hostApi->reloadLegacyState();
    }
    hostApi->sameLine();
    if (hostApi->button("Aplicar fila pendente")) {
        hostApi->pumpLegacyFeatureRequests();
    }

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
    "{\"name\":\"libTSMRebuild.so\",\"displayName\":\"Nucleo TSM Rebuild\",\"author\":\"AuraFlow\","
    "\"description\":\"Base reconstruivel do TSM com source completo para manutencao e updates.\","
    "\"majorVersion\":0,\"minorVersion\":0,\"patchVersion\":1,"
    "\"dependencies\":[],\"displaysUI\":true,\"selfManagedUI\":true,"
    "\"githubReleasesUrl\":\"https://api.github.com/repos/DouglasReisofc/aura.so/releases/latest\"}";
