#include <pico/stdlib.h>
#include <string>

class Button
{
private:
    uint Pin;
    bool State;
    char Symbol;
    uint Cooldown;
    uint CooldownLength;

    static uint SharedCooldown;
    static uint SharedCooldownLength;

public:
    Button(uint A_Pin);
    Button(uint A_Pin, char A_Symbol);
    Button(uint A_Pin, uint A_CooldownLength);
    Button(uint A_Pin, char A_Symbol, uint A_CooldownLength);
    ~Button();

    static uint SetSharedCooldown(uint A_SharedCooldown);
    static uint GetSharedCooldown();

    static uint SetSharedCooldownLength(uint A_SharedCooldownLength);
    static uint GetSharedCooldownLength();

    static uint IncrementSharedCooldown(uint Amount = 1);
    static uint DecrementSharedCooldown(uint Amount = 1);
    static uint ResetSharedCooldown();
    static bool CanSharedButtonsBePressed();
    
    uint SetButtonPin(uint A_Pin);
    uint GetButtonPin();

    uint SetButtonSymbol(char A_Symbol);
    char GetButtonSymbol();
    std::string GetButtonSymbolString();

    uint GetCooldown();
    uint SetCooldown(uint A_Cooldown);

    uint IncrementCooldown(uint Amount = 1);
    uint DecrementCooldown(uint Amount = 1);

    uint GetCooldownLength();
    uint SetCooldownLenght(uint A_CooldownLength);

    bool CanBePressed();
    uint ResetCooldown();

    bool GetButtonState();
    bool IsPressed();
    bool IsNotPressed();
};
