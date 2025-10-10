/*
    Play/Record control logic for Unitra M3015 cassette deck.
 */

namespace
{
    constexpr bool    ACTIVE_LOW                     = false;
    constexpr bool    ACTIVE_HIGH                    = true;
    constexpr size_t  TOTAL_SWITCH_PINS              = 6;
    constexpr uint8_t REC_DETECT_PIN                 = A0;
    constexpr bool    REC_DETECT_ACTIVE_LOGIC        = ACTIVE_LOW;
    constexpr bool    SWITCH_ACTIVE_LOGIC            = ACTIVE_HIGH;
    constexpr uint8_t SWITCH_PINS[TOTAL_SWITCH_PINS] = {
        8,
        7,
        10,
        4,
        2,
        1,
    };

    bool lastRecState = false;

    void setupPins()
    {
        for (size_t i = 0; i < sizeof(SWITCH_PINS); i++)
        {
            pinMode(SWITCH_PINS[i], OUTPUT);
        }

        pinMode(REC_DETECT_PIN, INPUT);
    }

    void setSwitchState(bool state)
    {
        for (size_t i = 0; i < sizeof(SWITCH_PINS); i++)
        {
            digitalWrite(SWITCH_PINS[i], state ? SWITCH_ACTIVE_LOGIC : !SWITCH_ACTIVE_LOGIC);
        }
    }

    void record()
    {
        setSwitchState(true);
    }

    void play()
    {
        setSwitchState(false);
    }

    void onStateChange(bool recState)
    {
        if (recState)
        {
            record();
        }
        else
        {
            play();
        }
    }

    bool isRecModeActive()
    {
        return digitalRead(REC_DETECT_PIN) == REC_DETECT_ACTIVE_LOGIC;
    }

    void checkState()
    {
        auto newState = isRecModeActive();

        if (newState != lastRecState)
        {
            lastRecState = newState;
            onStateChange(newState);
        }
    }
}    // namespace

void setup()
{
    setupPins();
}

void loop()
{
    checkState();
}
