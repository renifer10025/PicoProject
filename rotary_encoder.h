class RotaryEncoder
{
private:
    uint PinA;          // CLK
    uint PinB;          // DT
    uint ButtonPin;     // SW

    uint VCC_5VPin;

    bool StateOfCLK;
    bool OldCLK;
    uint Counter;

    bool LastStepDirection; // 1(true) - clockwise | 0(false) - counter clockwise

    uint ErrorState;    // no bits "high" - no errors detected
                        // bit 1 - any 2 pins have the same pin number
                        // bit 2 - any value is below 0

    enum EncoderPins
    {
        CLK,
        DT,
        SW,
    };   

public:
    RotaryEncoder();
    RotaryEncoder(uint CLK, uint DT, uint SW);
    RotaryEncoder(uint CLK, uint DT);
    ~RotaryEncoder();

    uint CheckStep();
    bool CheckButtonState();
    bool FlipFlopButton();
    uint GetPinState(uint EncoderPin);

    uint GetPinA();
    uint SetPinA(uint CLK);

    uint GetPinB();
    uint SetPinB(uint DT);

    uint GetButtonPin();
    uint SetButtonPin(uint SW);

    uint GetErrorState();
    uint CheckForErrors(uint CLK = 0, uint DT = 0, uint SW = 0);

    uint init_rotary_encoder_gpio();
    uint deinit_rotary_encoder_gpio();
};