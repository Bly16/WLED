#pragma once
#include "wled.h"
#include "SHT4x.h" // Nowa biblioteka dla SHT45

#ifdef WLED_DISABLE_MQTT
#error "This user mod requires MQTT to be enabled."
#endif

class ShtUsermod : public Usermod
{
  private:
    bool enabled = false;
    bool firstRunDone = false;
    bool initDone = false;
    bool haMqttDiscovery = false;
    bool haMqttDiscoveryDone = false;

    // Zmienne SHT45
    SHT4x *shtTempHumidSensor = nullptr;
    byte unitOfTemp = 0; // 0 = Celsius, 1 = Fahrenheit
    bool shtInitDone = false;
    bool shtReadDataSuccess = false;
    const byte shtI2cAddress = 0x44; // Standardowy adres I2C dla SHT45
    unsigned long shtLastTimeUpdated = 0;
    bool shtDataRequested = false;
    float shtCurrentTempC = 0.0f;
    float shtCurrentHumidity = 0.0f;

    void initShtTempHumiditySensor();
    void cleanupShtTempHumiditySensor();
    void cleanup();
    inline bool isShtReady() { return shtInitDone; }

    void publishTemperatureAndHumidityViaMqtt();
    void publishHomeAssistantAutodiscovery();
    void appendDeviceToMqttDiscoveryMessage(JsonDocument& root);

  public:
    static const char _name[];
    static const char _enabled[];
    static const char _unitOfTemp[];
    static const char _haMqttDiscovery[];

    void setup();
    void loop();
    void onMqttConnect(bool sessionPresent);
    void appendConfigData();
    void addToConfig(JsonObject &root);
    bool readFromConfig(JsonObject &root);
    void addToJsonInfo(JsonObject& root);

    bool isEnabled() { return enabled; }

    float getTemperature();
    float getTemperatureC() { return roundf(shtCurrentTempC * 10.0f) / 10.0f; }
    float getTemperatureF() { return (getTemperatureC() * 1.8f) + 32.0f; }
    float getHumidity() { return roundf(shtCurrentHumidity * 10.0f) / 10.0f; }
    const char* getUnitString();

    uint16_t getId() { return USERMOD_ID_SHT; }
};
