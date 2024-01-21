#include <pico/stdlib.h>
#include <iostream>
class Joystick
{
private:
    static uint JoystickCount;
    std::string Name;
    uint16_t Range;
    float VRef;

    bool FlipFlop;

    uint DeadzoneX;
    uint DeadzoneY;

    uint SensitivityX;
    uint SensitivityY;

    uint XAxisPin;      // VRx
    uint YAxisPin;      // VRy
    uint ButtonPin;     // SW
public:
    Joystick();
    Joystick(uint VRx, uint VRy, uint SW);
    ~Joystick();

    static uint GetJoystickCount();
    static uint IncrementJoystickCount();

    uint SetDeadzone(uint Radius);
    uint SetDeadzone(uint RadX, uint RadY);
    uint GetDeadzone();

    uint SetSensitivity(uint Sensitivity);
    uint SetSensitivity(uint SenX, uint SensY);
    uint GetSensitivity();

    uint GetXAxisPin();
    uint SetXAxisPin(uint VRx);

    uint GetYAxisPin();
    uint SetYAxisPin(uint VRy);

    uint GetButtonPin();
    uint SetButtonPin(uint SW);

    uint GetName();
    uint SetName();
    
    uint InitGPIO();
    uint DeinitGPIO();

    float GetVRx();
    float GetVRy();
    bool CheckForButtonPress();

    bool FlipFlopButton(bool* CurrentState, bool* OldState);
    bool GetFlipFlop();

    float Map(float inValue, float minInRange, float maxInRange, float minOutValue, float maxOutValue);
};