#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include "Debouncer.h"
#ifndef MAX_DIO_PIN_COUNT
#define MAX_DIO_PIN_COUNT 8
#endif

class DigiOut /* 255 RETURN VALUE = ERROR */
{
private:
    uint8_t pinCount;
    uint8_t pins[MAX_DIO_PIN_COUNT];

public:
    DigiOut();
    DigiOut(uint8_t pins[], uint8_t pinCount);
    uint8_t count();
    uint8_t *getPins();
    uint8_t getPinByIndex(uint8_t idx);
    uint8_t getIndexByPin(uint8_t pin);
    bool add(uint8_t pin); // Add A Pin
    void load(uint8_t pins[], uint8_t pinCount);
    uint8_t load(String path);              // Returns Loaded Pins Count
    void write(uint8_t idx, uint8_t state); // 0 | 1 | >1
    void write(uint8_t idx);
    void writes();
    void writes(uint8_t state);
    void writes(String states); // Args Eg. [1,0,1,0]
    uint8_t read(uint8_t idx);
    String reads(); // Return Eg. [1,0,1,0]
};

class DebounceDigiOut : public Debouncer, public DigiOut
{
};