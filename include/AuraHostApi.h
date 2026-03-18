#pragma once

#ifdef __cplusplus
extern "C" {
#endif

enum {
    AURA_HOST_API_VERSION = 16,
};

typedef struct AuraHostApi {
    int abiVersion;
    void (*setNextWindowDefaultPos)(float x, float y);
    void (*setNextWindowDefaultSize)(float width, float height);
    bool (*beginWindow)(const char* title, bool* open);
    bool (*beginPanelWindow)(const char* id, bool* open);
    bool (*beginCompactPanelWindow)(const char* id, bool* open);
    void (*endWindow)();
    void (*text)(const char* text);
    void (*textWrapped)(const char* text);
    void (*textColored)(float r, float g, float b, float a, const char* text);
    void (*separator)();
    bool (*collapsingHeader)(const char* label, bool defaultOpen);
    bool (*toggle)(const char* label, bool* value, const char* description);
    bool (*button)(const char* label);
    bool (*buttonSized)(const char* label, float width, float height);
    bool (*sliderFloat)(const char* label, float* value, float minValue, float maxValue, const char* format);
    void (*sameLine)();
    void (*spacing)();
    bool (*beginChild)(const char* id, float width, float height, bool border);
    void (*endChild)();
    void (*dummy)(float width, float height);
    void (*setWindowFontScale)(float scale);
    void (*pushStyleColor)(int index, float r, float g, float b, float a);
    void (*popStyleColor)(int count);
    void (*pushStyleVarFloat)(int index, float value);
    void (*pushStyleVarVec2)(int index, float x, float y);
    void (*popStyleVar)(int count);
    void (*setCursorPos)(float x, float y);
    void (*setCursorPosX)(float x);
    void (*setCursorPosY)(float y);
    float (*calcTextWidth)(const char* text);
    float (*getContentRegionAvailWidth)();
    float (*getCursorPosY)();
    int (*featureRowShell)(const char* id, float width, float height, bool enabled);
    bool (*switchControl)(const char* id, bool* value, bool enabled);
    bool (*iconCircleButton)(const char* id, const char* iconName, float diameter, bool selected);
    float (*getWindowWidth)();
    float (*getWindowPosX)();
    float (*getWindowPosY)();
    void (*setWindowPos)(float x, float y);
    float (*getMousePosX)();
    float (*getMousePosY)();
    bool (*isMouseDown)(int button);
    bool (*isMouseClicked)(int button);
    float (*getDisplayWidth)();
    float (*getDisplayHeight)();
    void (*handleCurrentWindowTouchScroll)(float dragThreshold, float speedMultiplier, float scrollbarReserve);
    void (*beginDisabled)(bool disabled);
    void (*endDisabled)();
    void (*textDisabled)(const char* text);
    void (*bulletStatus)(const char* label, bool enabled);
    bool (*isCeserverEnabled)();
    bool (*isGameSpeedSupported)();
    bool (*setGameSpeed)(float speed);
    bool (*isFrameRateLimited)();
    void (*setFrameRateLimited)(bool enabled);
    bool (*isLegacyBoolOptionAvailable)(int optionIndex);
    bool (*getLegacyBoolOption)(int optionIndex);
    bool (*setLegacyBoolOption)(int optionIndex, bool enabled);
    bool (*isSuperRunEnabled)();
    bool (*setSuperRunEnabled)(bool enabled);
    float (*getSuperRunMultiplier)();
    bool (*setSuperRunMultiplier)(float multiplier);
    unsigned int (*getSkyGameVersion)();
    const char* (*getSkyBuildKey)();
    int (*getLegacyBackendState)();
    const char* (*getLegacyBackendStatusLabel)();
    bool (*isLegacyBackendReady)();
    const char* (*getLegacyCompatibilityProfile)();
    bool (*reloadLegacyState)();
    bool (*isLegacyFeatureAvailable)(const char* featureKey);
    const char* (*getLegacyFeatureRuntimeState)(const char* featureKey);
    bool (*getRequestedLegacyBoolValue)(const char* featureKey, bool* outValue);
    bool (*requestLegacyFeatureApply)(const char* featureKey, bool enabled);
    const char* (*getLastLegacyFeatureError)(const char* featureKey);
    const char* (*getLastLegacyFeatureTimestamp)(const char* featureKey);
    bool (*getLegacyBoolValue)(const char* featureKey, bool* outValue);
    bool (*setLegacyBoolValue)(const char* featureKey, bool enabled);
    bool (*getLegacyFloatValue)(const char* featureKey, float* outValue);
    bool (*setLegacyFloatValue)(const char* featureKey, float value);
    bool (*getLegacyIntValue)(const char* featureKey, int* outValue);
    bool (*setLegacyIntValue)(const char* featureKey, int value);
    bool (*invokeLegacyAction)(const char* actionKey);
    bool (*loadPersistedBool)(const char* key, bool defaultValue, bool* outValue);
    bool (*savePersistedBool)(const char* key, bool value);
    void (*pumpLegacyFeatureRequests)();
} AuraHostApi;

#ifdef __cplusplus
}
#endif
