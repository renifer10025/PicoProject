#include "Joystick_module.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"

Joystick::Joystick()
{
    XAxisPin = -1;
    YAxisPin = -1;
    ButtonPin = -1;
    Range = 4096;
    VRef = 3.3;
    FlipFlop = false;
}


Joystick::Joystick(uint VRx, uint VRy, uint SW)
{
    XAxisPin = VRx;
    YAxisPin = VRy;
    ButtonPin = SW;
    FlipFlop = false;

    gpio_init(ButtonPin);
    gpio_pull_up(ButtonPin);
    gpio_set_dir(ButtonPin, GPIO_IN);
    
    adc_init();
    adc_gpio_init(XAxisPin);
    adc_gpio_init(YAxisPin);

    Range = 4096;
    VRef = 3.3;
}


Joystick::~Joystick()
{
    //gpio_deinit()

    XAxisPin = 0;
    YAxisPin = 0;
    ButtonPin = 0;
    Range = 0;
    VRef = 0;
}


uint Joystick::GetJoystickCount()
{
    return JoystickCount;
}


uint Joystick::IncrementJoystickCount()
{
    JoystickCount++;
    return JoystickCount;
}


uint Joystick::SetDeadzone(uint Radius)
{
    DeadzoneX = Radius;
    DeadzoneY = Radius;
    return DeadzoneX;
}


uint Joystick::GetDeadzone()
{
    return DeadzoneX;
}


uint Joystick::GetXAxisPin()
{
    return XAxisPin;
}


uint Joystick::SetXAxisPin(uint VRx)
{
    XAxisPin = VRx;
    return XAxisPin;
}


uint Joystick::GetYAxisPin()
{
    return YAxisPin;
}


uint Joystick::SetYAxisPin(uint VRy)
{
    YAxisPin = VRy;
    return YAxisPin;
}


uint Joystick::GetButtonPin()
{
    return ButtonPin;
}


uint Joystick::SetButtonPin(uint SW)
{
    ButtonPin = SW;
    return ButtonPin;
}


uint Joystick::GetName()
{
    return 0;
}


uint Joystick::SetName()
{
    return 0;
}


uint Joystick::InitGPIO()
{
    return 0;
}


uint Joystick::DeinitGPIO()
{
    return 0;
}

// \return Value between 1 and -1
float Joystick::GetVRx()
{
    float ret = 0;
    
    adc_select_input(1); // adc_select_input(x) where x = (0, 1, 2 and 3) is equivalent to pins 26, 27, 28 and 29
    ret = adc_read();
    ret *= (VRef / (Range - 1));
    
    return Map(ret, 0, 3.3, -1, 1);
}

// Returns a value between 1 and -1
float Joystick::GetVRy()
{
    float ret = 0;
    adc_select_input(0);
    ret = adc_read();
    ret *= (VRef / (Range - 1));

    return Map(ret, 0, 3.3, -1, 1);
}

// Returns true(1) if button is pressed and false(0) if not
bool Joystick::CheckForButtonPress()
{
    return !gpio_get(ButtonPin);
}

// Returns the state of FlipFlop if it was changed
// Returns 1 if state equals to true and -1 if equals to false
bool Joystick::FlipFlopButton(bool* CurrentState, bool* OldState)
{
    if (*CurrentState != *OldState)
    {
        sleep_ms(1);    // Wait for input to stabilize (needs about 700us to stabilize)
        if (*CurrentState == true)
        {
            FlipFlop = !FlipFlop;
        }

        /* if (FlipFlop == true)
            Dir = 1;
        else
            Dir = -1; */
    }
    *OldState = *CurrentState;
    
    return FlipFlop;
}

bool Joystick::GetFlipFlop()
{
    return FlipFlop;
}

// Maps a given value from a given value range, between a given range
// Map(2400, 0, 4096, 0, 1) returns 0.585938
// Map(2400, 0, 4096, -1, 1) returns 0.171875
float Joystick::Map(float inValue, float minInRange, float maxInRange, float minOutValue, float maxOutValue)
{
    return (inValue - minInRange) * (maxOutValue - minOutValue) / (maxInRange - minInRange) + minOutValue;;
}
