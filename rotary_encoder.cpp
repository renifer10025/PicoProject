#include <pico/stdlib.h>
#include "rotary_encoder.h"

// Default constructor
// Sets all member variables to 0;
RotaryEncoder::RotaryEncoder()
{
    PinA = -1;
    PinB = -1;
    ButtonPin = -1;
    LastStepDirection = 0;
    StateOfCLK = 0;
    Counter = 0;
}

// Default destructor
// Sets all member variables to 0;
RotaryEncoder::~RotaryEncoder()
{
    if (PinA != -1)
        gpio_deinit(PinA);

    if (PinB != -1)
        gpio_deinit(PinB);

    if (ButtonPin != -1)
        gpio_deinit(ButtonPin);

    PinA = 0;
    PinB = 0;
    ButtonPin = 0;
    LastStepDirection = 0;
    StateOfCLK = 0;
    Counter = 0;
}

// Constructor to use with all pins of the rotary encoder
// Sets all member variables to CLK(PinA), DT(PinB) and SW(ButtonPin)
RotaryEncoder::RotaryEncoder(uint CLK, uint DT, uint SW)
{
    if (CLK != DT && CLK != SW && DT != SW)
    {
        if (CLK >= 0)
            PinA = CLK;

        if (DT >= 0)
            PinB = DT;

        if (ButtonPin >= 0)
            ButtonPin = SW;

        LastStepDirection = 0;

        gpio_init(PinA);
        gpio_init(PinB);
        gpio_init(ButtonPin);

        gpio_set_dir(PinA, GPIO_IN);
        gpio_set_dir(PinB, GPIO_IN);
        gpio_set_dir(ButtonPin, GPIO_IN);

        StateOfCLK = 1;
        OldCLK = 1;
        Counter = 0;
    }
    
    
}

// Constructor to use without the rotary encoder's button
// Sets all member variables to CLK(PinA), DT(PinB)
RotaryEncoder::RotaryEncoder(uint CLK, uint DT)
{
    PinA = CLK;
    PinB = DT;
    LastStepDirection = 0;
    StateOfCLK = gpio_get(PinA);
    Counter = 0;
}

// Checks which in direction the rotary encoder was rotated
// Returns -1 if left and 1 if right and 0 if no movement
uint RotaryEncoder::CheckStep()
{
    StateOfCLK = gpio_get(PinA);
    

    if ((StateOfCLK != OldCLK))
    {
        if (gpio_get(PinB) != StateOfCLK)
        {
            // inccrement
            LastStepDirection = 1;
            Counter++;
        }
        else
        {
            // decrement
            LastStepDirection = 0;
            Counter--;
        }
    }
    else
        return 0;
    
    OldCLK = StateOfCLK;

    if (LastStepDirection)
        return 1;
    else
        return -1;
}

// Checks the state of button pin
// Returns true if button is pressed and false if not pressed
bool RotaryEncoder::CheckButtonState()
{
    return !gpio_get(ButtonPin);
}

// returns the state of selected pin
// 0 - PinA(CLK)
// 1 - PinB(DT)
// 2 - ButtonPin(SW)
uint RotaryEncoder::GetPinState(uint EncoderPin)
{
    uint ret = 0;
    switch (EncoderPin)
    {
    case 0:     // PinA(CLK)
        ret = gpio_get(PinA);
        break;

    case 1:     // PinB(DT)
        ret = gpio_get(PinB);
        break;

    case 2:     // ButtonPin(SW)
        ret = gpio_get(ButtonPin);
        break;
    
    default:    // Error, not selected a valid pin
        ret = -1;
        break;
    }

    return ret;
}

// Returns the value of PinA variable
uint RotaryEncoder::GetPinA()
{
    return PinA;
}

// Sets the value of PinA
uint RotaryEncoder::SetPinA(uint CLK)
{
    PinA = CLK;
    return PinA;
}

// Returns the value of PinB variable
uint RotaryEncoder::GetPinB()
{
    return PinB;
}

// Sets the value of PinB
uint RotaryEncoder::SetPinB(uint DT)
{
    PinB = DT;
    return PinB;
}

// Returns the value of ButtonPin variable
uint RotaryEncoder::GetButtonPin()
{
    return ButtonPin;
}

// Sets the value of ButtonPin
uint RotaryEncoder::SetButtonPin(uint SW)
{
    ButtonPin = SW;
    return ButtonPin;
}

// Returns the value of ErrorState variable
uint RotaryEncoder::GetErrorState()
{
    return ErrorState;
}

// TODO
uint RotaryEncoder::CheckForErrors(uint CLK, uint DT, uint SW)
{
    // Check if any pin number is the same twice
    if ((CLK != DT && CLK != SW && DT != SW) == false)
        ErrorState | 1;

    if ((CLK >= 0 || DT >= 0 || SW >= 0) == false)
        ErrorState | 2;
    

    return ErrorState;
}

// TODO
uint RotaryEncoder::init_rotary_encoder_gpio()
{
    return 0;
}

// TODO
uint RotaryEncoder::deinit_rotary_encoder_gpio()
{
    return 0;
}