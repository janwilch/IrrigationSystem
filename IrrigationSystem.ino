#include "src/State.hpp"
#include "src/OnDelayTimer.hpp"

#pragma region global vars

const int COUNT = 5;

State state;
OnDelayTimer ton;

const int q_measureEnable = 13;

const int q_vlv1 = 2;
const int q_vlv2 = 3;
const int q_vlv3 = 4;
const int q_vlv4 = 5;
const int q_vlv5 = 6;
const int q_vlvs[COUNT] = {q_vlv1, q_vlv2, q_vlv3, q_vlv4, q_vlv5};

const int i_measure1 = 11;
const int i_measure2 = 10;
const int i_measure3 = 9;
const int i_measure4 = 8;
const int i_measure5 = 7;
const int i_measures[COUNT] = {i_measure1, i_measure2, i_measure3, i_measure4, i_measure5};

int irrigationMask[5] = {LOW};

#pragma endregion

#pragma region Arduino functions

void setup()
{
    pinMode(q_measureEnable, OUTPUT);
    for (int pin : q_vlvs)
        pinMode(pin, OUTPUT);

    for (int pin : i_measures)
        pinMode(pin, INPUT);

    state = State::INIT;
}

void loop()
{
    switch (state)
    {
    case State::INIT:
        for (int pin : q_vlvs)
            digitalWrite(pin, HIGH);

        if (ton.IsElapsed(5000)) // TODO - sensible idle time
        {
            state = State::MEASURE_ENABLE;
            ton.Reset();
        }
        break;

    case State::MEASURE_ENABLE:
        digitalWrite(q_measureEnable, HIGH);
        if (ton.IsElapsed(2000))
        {
            state = State::MEASURING;
            ton.Reset();
        }
        break;

    case State::MEASURING:
        for (int i = 0; i < COUNT; i++)
            irrigationMask[i] = digitalRead(i_measures[i]);

        digitalWrite(q_measureEnable, LOW);
        state = State::IRRIGATING;
        break;

    case State::IRRIGATING:
        for (int i = 0; i < COUNT; i++)
            digitalWrite(q_vlvs[i], invert(irrigationMask[i]));

        if (ton.IsElapsed(5000))
        {
            state = State::INIT;
            ton.Reset();
        }
        break;

    default:
        break;
    }
}

#pragma endregion

#pragma region local functions

int invert(int signal)
{
    if (signal == HIGH)
        return LOW;
    else if (signal == LOW)
        return HIGH;
    else
        return -1;
}

#pragma endregion
