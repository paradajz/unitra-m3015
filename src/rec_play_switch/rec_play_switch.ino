/*
    Play/Record control logic for Unitra M3015 cassette deck.

    The switching between the two states is performed with the two sets of relays:
    the first set is responsible for physical switching of the play/record control
    lines in the deck, while the second set is responsible for temporary
    disconnection of those lines from the deck. Switching between the two modes is
    always done during the disconnection to avoid loud thumps which would otherwise occur.
 */

namespace
{
    constexpr bool     ACTIVE_LOW              = false;
    constexpr bool     ACTIVE_HIGH             = true;
    constexpr size_t   TOTAL_SWITCH_PINS       = 6;
    constexpr size_t   TOTAL_DISCONNECT_PINS   = 6;
    constexpr uint8_t  REC_DETECT_PIN          = A0;
    constexpr bool     REC_DETECT_ACTIVE_LOGIC = ACTIVE_LOW;
    constexpr bool     DISCONNECT_ACTIVE_LOGIC = ACTIVE_HIGH;
    constexpr bool     SWITCH_ACTIVE_LOGIC     = ACTIVE_HIGH;
    constexpr uint32_t DISCONNECT_DURATION_MS  = 5;

    constexpr uint8_t SWITCH_PINS[TOTAL_SWITCH_PINS] = {
        8,
        7,
        10,
        4,
        2,
        1,
    };

    constexpr uint8_t DISCONNECT_PINS[TOTAL_DISCONNECT_PINS] = {
        6,
        5,
        9,
        3,
        0,
        A5,
    };

    bool lastRecState = false;

    void setupPins()
    {
        for (size_t i = 0; i < sizeof(SWITCH_PINS); i++)
        {
            pinMode(SWITCH_PINS[i], OUTPUT);
        }

        for (size_t i = 0; i < sizeof(DISCONNECT_PINS); i++)
        {
            pinMode(DISCONNECT_PINS[i], OUTPUT);
        }

        pinMode(REC_DETECT_PIN, INPUT);
    }

    void setDisconnectState(bool state)
    {
        for (size_t i = 0; i < sizeof(DISCONNECT_PINS); i++)
        {
            digitalWrite(DISCONNECT_PINS[i], state ? DISCONNECT_ACTIVE_LOGIC : !DISCONNECT_ACTIVE_LOGIC);
        }
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
        setDisconnectState(true);
        delay(DISCONNECT_DURATION_MS);
        setSwitchState(true);
        delay(DISCONNECT_DURATION_MS);
        setDisconnectState(false);
    }

    void play()
    {
        setDisconnectState(true);
        delay(DISCONNECT_DURATION_MS);
        setSwitchState(false);
        setDisconnectState(false);
    }

    void onStateChange(bool recordState)
    {
        if (recordState)
        {
            record();
        }
        else
        {
            play();
        }
    }

    bool isRecordActive()
    {
        return digitalRead(REC_DETECT_PIN) == REC_DETECT_ACTIVE_LOGIC;
    }
}    // namespace

void setup()
{
    setupPins();
}

void loop()
{
    auto newRecState = isRecordActive();

    if (newRecState != lastRecState)
    {
        onStateChange(newRecState);
        lastRecState = newRecState;
    }
}
