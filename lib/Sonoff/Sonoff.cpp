#include "Sonoff.h"

Sonoff::Sonoff() : pinCount(0){};
Sonoff::Sonoff(uint8_t pins[], uint8_t pinCount)
{
    this->load(pins, pinCount);
};

uint8_t Sonoff::count()
{
    return pinCount;
}

uint8_t *Sonoff::getPins()
{
    return pins;
}

uint8_t Sonoff::getPinByIndex(uint8_t idx)
{
    if (idx < pinCount)
    {
        return pins[idx];
    }

    return 255;
}

uint8_t Sonoff::getIndexByPin(uint8_t pin)
{
    /* Return 255 If Not Found */
    for (uint8_t i = 0; i < pinCount; i++)
    {
        if (pins[i] == pin)
        {
            return i;
        }
    }
    return 255;
}

bool Sonoff::add(uint8_t pin) /* Add A Pin */
{
    if (pinCount < MAX_SONOFF_PIN_COUNT)
    {
        pinMode(pin, OUTPUT);
        pins[pinCount] = pin;
        pinCount++;
        return true;
    }

    return false;
}

void Sonoff::load(uint8_t *pns, uint8_t pCount)
{
    memcpy(this->pins, pns, pCount);
    pinCount = pCount;
    for (uint8_t i = 0; i < pinCount; i++)
    {
        pinMode(pins[i], OUTPUT);
    }
}

uint8_t Sonoff::load(String path, bool states) /* Returns Loaded Pins Count */
{
    File file = LittleFS.open(path, "r");
    StaticJsonDocument<128> jdoc;

    if (deserializeJson(jdoc, file) == DeserializationError::Ok)
    {
        if (states) /* Load DigiOut Pins Default State */
        {
            uint8_t index = 0;
            for (JsonVariant v : jdoc.as<JsonArray>())
            {
                if (index <= pinCount)
                {
                    digitalWrite(pins[index], v.as<u8>());
                    index++;
                    continue;
                }
                break;
            }
            file.close();
            return index;
        }

        pinCount = 0; /* Load DigiOut Pins */
        for (JsonVariant v : jdoc.as<JsonArray>())
        {
            if (pinCount < MAX_SONOFF_PIN_COUNT)
            {
                pins[pinCount] = v.as<u8>();
                pinMode(pins[pinCount], OUTPUT);
                pinCount++;
                continue;
            }
            break;
        }
    }

    file.close();
    return pinCount;
}

void Sonoff::write(uint8_t idx, uint8_t state) /* 0 | 1 | >1 */
{
    if (idx < pinCount)
    {
        if (state > 1)
        {
            digitalWrite(pins[idx], !digitalRead(pins[idx]));
        }
        else
        {
            digitalWrite(pins[idx], state);
        }
    }
}

void Sonoff::write(uint8_t idx)
{
    if (idx < pinCount)
    {
        digitalWrite(pins[idx], !digitalRead(pins[idx]));
    }
}

void Sonoff::writes(uint8_t state)
{
    for (uint8_t i = 0; i < pinCount; i++)
    {
        digitalWrite(pins[i], state);
    }
}

void Sonoff::writes()
{
    for (uint8_t i = 0; i < pinCount; i++)
    {
        digitalWrite(pins[i], !digitalRead(pins[i]));
    }
}

void Sonoff::write_(uint8_t *states)
{
    for (uint8_t i = 0; i < pinCount; i++)
    {
        digitalWrite(pins[i], states[i]);
    }
}

void Sonoff::writes(String states)
{
    StaticJsonDocument<128> jdoc;
    if (deserializeJson(jdoc, states) == DeserializationError::Ok)
    {
        uint8_t idx = 0;
        for (JsonVariant v : jdoc.as<JsonArray>())
        {
            if (idx < MAX_SONOFF_PIN_COUNT)
            {
                digitalWrite(pins[idx], v.as<u8>());
                idx++;
                continue;
            }
            break;
        }
    }
}

uint8_t Sonoff::read(uint8_t idx)
{
    if (idx < pinCount)
    {
        return digitalRead(pins[idx]);
    }

    return 255;
}

uint8_t Sonoff::read()
{
    uint8_t result = digitalRead(pins[0]);
    for (uint8_t i = 1; i < pinCount; i++)
    {
        if (digitalRead(pins[i]) != result)
        {
            return 3;
        }
    }

    return result;
}

void Sonoff::reads(uint8_t *states)
{
    for (uint8_t i = 0; i < pinCount; i++)
    {
        states[i] = digitalRead(pins[i]);
    }
}

String Sonoff::reads()
{

    if (pinCount)
    {
        String res = "[";
        for (uint8_t i = 0; i < pinCount; i++)
        {
            res += String(digitalRead(pins[i]) ? 1 : 0) + ",";
        }
        res[res.length() - 1] = ']';
        return res;
    }

    return String("[]");
}

/* Sonoffe Functions */
void Sonoffe::writer(uint8_t index, uint8_t state)
{
    if (state == 255)
    { /* Event Fire Only */
        start(true);
        return;
    }

    if (index < pinCount)
    {
        if (state < 128)
        {
            if (digitalRead(pins[index]) != state)
            {
                write(index, state);
                start();
            }
            return;
        }

        state -= 128;
        write(index, state);
    }
}