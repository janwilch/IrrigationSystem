#include "src/State.hpp"
#include "src/OnDelayTimer.hpp"

#pragma region global vars

const int COUNT = 5;

State state = State::NONE;
State lastState = State::NONE;
OnDelayTimer ton;

const int q_measureEnable = 13;
const int q_pump = 12;

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

int irrigationMask[COUNT] = {LOW};

#pragma endregion

#pragma region Arduino functions

void setup()
{
    pinMode(q_measureEnable, OUTPUT);
    pinMode(q_pump, OUTPUT);

    for (int pin : q_vlvs)
        pinMode(pin, OUTPUT);

    for (int pin : i_measures)
        pinMode(pin, INPUT);

    // close valves; turn off pump (note: relay closes contact on LOW)
    for (int pin : q_vlvs)
        digitalWrite(pin, HIGH);

    digitalWrite(q_pump, HIGH);

    state = State::INIT;
    Serial.begin(9600);
}

void loop()
{
    // output debug info
    if (state != lastState)
    {
        Serial.println(state);
        lastState = state;
    }

    // state machine
    switch (state)
    {
    // SENSORS ON
    case State::INIT:
    { // measuring corrodes the contacts -> only activate when needed
        digitalWrite(q_measureEnable, HIGH);
        state = State::AWAIT_MEASUREMENT;
        break;
    }

    // DELAY BEFORE TAKING MEASUREMENT
    case State::AWAIT_MEASUREMENT:
    {
        if (ton.IsElapsed(1000))
        {
            state = State::MEASURING;
            ton.Reset();
        }
        break;
    }

    // STORE MEASUREMENTS, STOP SENSORS
    case State::MEASURING:
    {
        bool anyDry = false;
        for (int i = 0; i < COUNT; i++)
        {
            // HIGH = too dry; LOW = moisture OK
            int sensVal = digitalRead(i_measures[i]);
            irrigationMask[i] = sensVal;
            if (sensVal == HIGH)
                anyDry = true;
        }

        if (anyDry)
            state = State::START_IRRIGATION;
        else
            state = State::WAITING;

        digitalWrite(q_measureEnable, LOW);
        break;
    }

    // PUMP ON, NECESSARY VALVES OPEN
    case State::START_IRRIGATION:
    {
        digitalWrite(q_pump, LOW);
        for (int i = 0; i < COUNT; i++)
            // open any valve (LOW) whose sensor reports "too dry" (HIGH)
            digitalWrite(q_vlvs[i], invert(irrigationMask[i]));
        state = State::IRRIGATING;
        break;
    }

    // STOP AFTER PUMPING FOR 5 s
    case State::IRRIGATING:
    {
        if (ton.IsElapsed(5000))
        {
            digitalWrite(q_pump, HIGH);
            for (int pin : q_vlvs)
                digitalWrite(pin, HIGH);

            state = State::WAITING;
            ton.Reset();
        }
        break;
    }

    // MEASURE AGAIN AFTER 10 MINUTES (600.000 ms)
    case State::WAITING:
    {
        if (ton.IsElapsed(600000))
        {
            state = State::INIT;
            ton.Reset();
        }
        break;
    }

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
