/**
 * @file ProductionModeContext.h
 * @brief Production Implementation of IModeContext
 *
 * Wraps msSystem and msGlobals to provide IModeContext interface.
 * This adapter allows existing code to work through the new interface
 * while maintaining backward compatibility.
 *
 * Part of Phase 2B refactoring - Dependency Injection layer.
 */

#ifndef PRODUCTION_MODE_CONTEXT_H
#define PRODUCTION_MODE_CONTEXT_H

#include "IModeContext.h"

// Forward declarations to avoid circular dependencies
class MagicShifterSystem;
class MagicShifterGlobals;

/**
 * @brief Production implementation adapters
 *
 * These classes wrap existing hardware drivers to implement
 * the new interface layer.
 */

/**
 * @brief LED Controller adapter for MagicShifterLEDs
 */
class LEDControllerAdapter : public ILEDController
{
private:
    class MagicShifterLEDs &leds;

public:
    LEDControllerAdapter(class MagicShifterLEDs &ledsRef);

    void setLED(int index, uint8_t r, uint8_t g, uint8_t b,
               uint8_t brightness) override;
    void fillLEDs(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness) override;
    void updateLEDs() override;
    void fastClear() override;
    void saveBuffer(uint8_t *buffer) override;
    void loadBuffer(uint8_t *buffer) override;
};

/**
 * @brief Button Input adapter for MagicShifterButtons
 */
class ButtonInputAdapter : public IButtonInput
{
private:
    class MagicShifterButtons &buttons;

public:
    ButtonInputAdapter(class MagicShifterButtons &buttonsRef);

    bool isButtonAPressed() override;
    bool isButtonALongPressed() override;
    bool isButtonADoublePressed() override;
    bool isButtonBPressed() override;
    bool isButtonBLongPressed() override;
    bool isButtonBDoublePressed() override;
    bool isPowerButtonPressed() override;
    bool isPowerButtonLongPressed() override;
    void clearAllEvents() override;
    bool isAnyButtonActive() override;
};

/**
 * @brief Sensor Input adapter for MagicShifterAccelerometer
 */
class SensorInputAdapter : public ISensorInput
{
private:
    MagicShifterGlobals &globals;

public:
    SensorInputAdapter(MagicShifterGlobals &globalsRef);

    void readAcceleration(float *destination) override;
    void readMagnetometer(int *destination) override;
    float getAccelX() override;
    float getAccelY() override;
    float getAccelZ() override;
    bool isSensorAvailable() override;
};

/**
 * @brief Simple logger adapter for msSystem.slog()
 */
class SystemLoggerAdapter : public ILogger
{
private:
    MagicShifterSystem &system;

public:
    SystemLoggerAdapter(MagicShifterSystem &systemRef);

    void log(const char *message) override;
    void logln(const char *message) override;
    void error(const char *message) override;
    void warning(const char *message) override;
    void debug(const char *message) override;
};

/**
 * @brief Stub config adapter - returns defaults for now
 *
 * TODO: Implement proper protobuf wrapper in future
 */
class StubConfigAdapter : public IModeConfig
{
public:
    int getInt(const char *key, int defaultValue = 0) override
    {
        return defaultValue;
    }
    float getFloat(const char *key, float defaultValue = 0.0f) override
    {
        return defaultValue;
    }
    bool getBool(const char *key, bool defaultValue = false) override
    {
        return defaultValue;
    }
    bool setInt(const char *key, int value) override { return false; }
    bool setFloat(const char *key, float value) override { return false; }
    bool setBool(const char *key, bool value) override { return false; }
    bool save() override { return false; }
    bool load() override { return false; }
};

/**
 * @brief Production Mode Context
 *
 * Implements IModeContext by wrapping msSystem and msGlobals.
 * This provides the new interface while maintaining full compatibility
 * with existing code.
 */
class ProductionModeContext : public IModeContext
{
private:
    MagicShifterSystem &system;
    MagicShifterGlobals &globals;

    // Adapters
    LEDControllerAdapter ledAdapter;
    ButtonInputAdapter buttonAdapter;
    SensorInputAdapter sensorAdapter;
    SystemLoggerAdapter loggerAdapter;
    StubConfigAdapter configAdapter;

public:
    /**
     * @brief Construct production context with system references
     *
     * @param sysRef Reference to msSystem global
     * @param globalsRef Reference to msGlobals global
     */
    ProductionModeContext(MagicShifterSystem &sysRef, MagicShifterGlobals &globalsRef);

    // Hardware access
    ILEDController &getLEDs() override { return ledAdapter; }
    IButtonInput &getButtons() override { return buttonAdapter; }
    ISensorInput &getSensor() override { return sensorAdapter; }

    // Configuration & state
    IModeConfig &getConfig() override { return configAdapter; }
    uint8_t getBrightness() const override;
    void setBrightness(uint8_t brightness) override;
    uint32_t getCurrentMicros() const override;
    uint32_t getCurrentMillis() const override;
    uint32_t getDeltaMicros() const override;

    // Services
    ILogger &getLogger() override { return loggerAdapter; }
    bool requestModeChange(int modeIndex) override;
};

#endif // PRODUCTION_MODE_CONTEXT_H
