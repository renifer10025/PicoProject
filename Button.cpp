#include "Button.h"

uint Button::SharedCooldown = 0;
uint Button::SharedCooldownLength = 0;

Button::Button(uint A_Pin)
{
    gpio_init(A_Pin);
    gpio_set_dir(A_Pin, GPIO_IN);
    Pin = A_Pin;
    Symbol = '\0';
    Cooldown = 0;
    CooldownLength = 50;
    SharedCooldown = 0;
    SharedCooldownLength = 0;
}

Button::Button(uint A_Pin, char A_Symbol)
{
    gpio_init(A_Pin);
    gpio_set_dir(A_Pin, GPIO_IN);
    Pin = A_Pin;
    Symbol = A_Symbol;
    Cooldown = 0;
    CooldownLength = 50;
    SharedCooldown = 0;
    SharedCooldownLength = 0;
}

Button::Button(uint A_Pin, uint A_CooldownLength)
{
    gpio_init(A_Pin);
    gpio_set_dir(A_Pin, GPIO_IN);
    Pin = A_Pin;
    Symbol = '\0';
    Cooldown = 0;
    CooldownLength = A_CooldownLength;
    SharedCooldown = 0;
    SharedCooldownLength = 0;
}

Button::Button(uint A_Pin, char A_Symbol, uint A_CooldownLength)
{
    gpio_init(A_Pin);
    gpio_set_dir(A_Pin, GPIO_IN);
    Pin = A_Pin;
    Symbol = A_Symbol;
    Cooldown = 0;
    CooldownLength = A_CooldownLength;
    SharedCooldown = 0;
    SharedCooldownLength = 0;
}

Button::~Button()
{
    gpio_deinit(Pin);
    State = 0;
    Pin = 0;
    Symbol = '\0';
    Cooldown = 0;
    CooldownLength = 0;
    SharedCooldown = 0;
}

uint Button::SetSharedCooldown(uint A_SharedCooldown)
{
    SharedCooldown = A_SharedCooldown;
    return 0;
}

uint Button::GetSharedCooldown()
{
    return SharedCooldown;
}

uint Button::SetSharedCooldownLength(uint A_SharedCooldownLength)
{
    SharedCooldownLength - A_SharedCooldownLength;
    return 0;
}

uint Button::GetSharedCooldownLength()
{
    return SharedCooldownLength;
}

uint Button::IncrementSharedCooldown(uint Amount)
{
    SharedCooldown += Amount;
    return SharedCooldown;
}

uint Button::DecrementSharedCooldown(uint Amount)
{
    if (SharedCooldown <= Amount)
    {
        SharedCooldown = 0;
        return 0;
    }
    
    SharedCooldown -= Amount;
    return SharedCooldown;
}

uint Button::ResetSharedCooldown()
{
    SharedCooldown = SharedCooldownLength;
    return 0;
}

bool Button::CanSharedButtonsBePressed()
{
    return SharedCooldown <= 0 ? true : false;
}

uint Button::SetButtonPin(uint A_Pin)
{
    gpio_init(A_Pin);
    gpio_set_dir(A_Pin, GPIO_IN);
    Pin = A_Pin;
    return 0;
}

uint Button::GetButtonPin()
{
    return Pin;
}


uint Button::SetButtonSymbol(char A_Symbol)
{
    Symbol = A_Symbol;
    return 0;
}

char Button::GetButtonSymbol()
{
    return Symbol;
}

std::string Button::GetButtonSymbolString()
{
    std::string str(1, Symbol);
    return str;
}

uint Button::GetCooldown()
{
    return Cooldown;
}

uint Button::SetCooldown(uint A_Cooldown)
{
    Cooldown = A_Cooldown;
    return 0;
}

uint Button::IncrementCooldown(uint Amount)
{
    Cooldown += Amount;
    return Cooldown;
}

uint Button::DecrementCooldown(uint Amount)
{
    if (Cooldown <= Amount)
    {
        Cooldown = 0;
        return 0;
    }

    Cooldown -= Amount;
    return Cooldown;
}

uint Button::GetCooldownLength()
{
    return CooldownLength;
}

uint Button::SetCooldownLenght(uint A_CooldownLength)
{
    CooldownLength = A_CooldownLength;
    return 0;
}

bool Button::CanBePressed()
{
    if (CanSharedButtonsBePressed() == true)
    {
        if (Cooldown <= 0)
            return true;

        else
            return false;
    }
    else
        return false;
}   

uint Button::ResetCooldown()
{
    Cooldown = CooldownLength;
    return 0;
}

bool Button::GetButtonState()
{
    return gpio_get(Pin);
}

bool Button::IsPressed()
{
    if (gpio_get(Pin) != 0)
    {
        ResetCooldown();
        if (GetSharedCooldown() != 0)
            ResetSharedCooldown();

        return true;
    }
    else
        return false;
}

bool Button::IsNotPressed()
{
    if (gpio_get(Pin) == 0)
        return false;
    
    else
    {
        ResetCooldown();
        if (GetSharedCooldown() != 0)
            ResetSharedCooldown();
            
        return true;
    }
}