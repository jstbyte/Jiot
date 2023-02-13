#pragma once
#include <Arduino.h>
#include <LittleFS.h>
#define SPIFFS LittleFS
#include <WiFiClientSecure.h>
#include <ESP8266httpUpdate.h>
#include <CertStoreBearSSL.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <RTCMemory.h>
#include <Helper.h>
#include <time.h>
#ifndef RTC_DATA_SIZE
#define RTC_DATA_SIZE 8
#endif
#ifndef MQTT_RETRY_MS
#define MQTT_RETRY_MS 5000
#endif

typedef struct
{
    uint8_t bootCount;
    uint32_t timeout;
    uint8_t data[7];
} RTCData;

typedef struct
{
    String identity;
    String wlanSSID;
    String wlanPASS;
    String hostNAME;
    String mqttHOST;
    u32_t mqttPORT;
} wlan_config_t;

class BootMan
{
protected:
    static RTCMemory<RTCData> rtcData;

public:
    static RTCData *data();
    static bool recover(void *data = nullptr, uint8_t len = 0);
    static void reboot(uint32_t timeout, void *data = nullptr, uint8_t len = 0);
};

class PubSubWiFi : public PubSubClient
{
protected:
    u8_t _maxRetry;
    u8_t _retryCount;
    const char *_pemCert;
    WiFiClient *_wifiClient;
    unsigned long long _timestamp;
    std::function<void(void)> _onRertyExceeds;
    std::function<void(PubSubWiFi *)> _onConnection;

public:
    void eventLoop();
    wlan_config_t init(String path);
    void init(wlan_config_t *config);
    static wlan_config_t loadWlanConfig(String path);
    PubSubWiFi(const char *pemCert) : _pemCert(pemCert){};
    void onConnection(std::function<void(PubSubWiFi *)> cb);
    void onRertyExceeds(std::function<void(void)> cb, u8_t maxRetry);
};

class PubSubX : public PubSubWiFi
{
    /* TOPIC FORMAT: `{SECRAT}/{DEVICE}/{SERVICES}#` */
    /* DEVICE `*` Mean Any Device Under The Same Key */

protected:
    static String _pkey;
    static String _host;

public:
    wlan_config_t init(String path);
    static String parse(char *topic);
    bool pub(String topic, String payload);
    bool sub(String topic, bool parent = false);
    PubSubX(const char *pemCert) : PubSubWiFi(pemCert){};
    static String topic(String topic, bool parent = false);
    static String parse(byte *payload, unsigned int length);
    static HTTPUpdateResult otaUpdate(const char *pemCert, String url);
};
