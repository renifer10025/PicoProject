#include <pico/stdlib.h>
#include <hardware/i2c.h>
#include <hardware/pwm.h>
#include <hardware/adc.h>
#include <pico/multicore.h>
#include "rotary_encoder.h"
#include "joystick_module.h"
#include "display/Display.h"
#include "Font.h"
#include "display/Utility.h"

#include "display/yellow_blue.h"

#define rtcAddress 0x68
#define displayAddress 0x3C
#define displayAddress2 0x7B

#define MaxPWMWrap 65535    // Max PWM wrap value is 65536

#define ON 1
#define OFF 0

#define Buzzer 1
#define RGBLED 9
#define LED 25
#define VDD_5V 0

#define DisplaySDA 10
#define DisplaySCL 11

void Blink()
{
    while (true)
    {
        gpio_put(LED, ON);
        sleep_ms(50);
        gpio_put(LED, OFF);
        sleep_ms(50);
    }
}

void FlashLED()
{
    gpio_put(LED, ON);
    sleep_ms(50);
    gpio_put(LED, OFF);
    sleep_ms(50);
}

int main()
{
    //multicore_launch_core1(Blink);


    Joystick joystick(26, 27, 22);
    RotaryEncoder RotEnc(15, 14, 13);
    Display display(128, 32, displayAddress, DisplaySDA, DisplaySCL);
    display.ZeroDisplay();

    gpio_init(LED);
    gpio_init(VDD_5V);

    gpio_set_dir(LED, GPIO_OUT);
    gpio_set_dir(VDD_5V, GPIO_OUT);

    gpio_put(VDD_5V, ON);

    gpio_set_function(Buzzer, GPIO_FUNC_PWM);
    uint Slice = pwm_gpio_to_slice_num(Buzzer);
    pwm_set_wrap(Slice, MaxPWMWrap);
    pwm_set_chan_level(Slice, PWM_CHAN_B, 1);
    pwm_set_enabled(Slice, true);
    pwm_set_gpio_level(Buzzer, 0);

    gpio_set_function(RGBLED, GPIO_FUNC_PWM);
    uint Slice2 = pwm_gpio_to_slice_num(RGBLED);
    pwm_set_wrap(Slice2, MaxPWMWrap);
    pwm_set_chan_level(Slice2, PWM_CHAN_A, 1);
    pwm_set_enabled(Slice2, true);
    pwm_set_gpio_level(RGBLED, 0);

    gpio_put(LED, ON);
    sleep_ms(50);
    gpio_put(LED, OFF);
    sleep_ms(50);

    render_area area = {start_col: 0, end_col: OLED_WIDTH - 1, start_page: 0, end_page: OLED_NUM_PAGES - 1, buflen : 0};
    display.SetFrameArea(area);
    display.oled_init();
    display.calc_render_area_buflen(&area);
    uint buf[OLED_BUF_LEN];
    //display.fill(buf, 0x00);
    display.oled_send_cmd(0xA4);
    display.render(buf, &area);

    display.OnBootUp();

    sleep_ms(250);

    display.ZeroDisplay();

    /*for (uint i = 0; i < display.GetDisplayHeight(); i++)
    {
        for (uint j = 0; j < display.GetDisplayWidth(); j++)
        {
            display.SwitchPixelState(j, i);
            display.CalculateReducedDisplayBuffer();
            display.render();
        }
    }*/
    

    uint* ptr = new uint[display.GetDisplaySize() / 8]();

    display.RandomPixelTest(10, 512, 20, area);
    gpio_put(LED, ON);
    sleep_ms(200);
    gpio_put(LED, OFF);
    display.RandomPixelTest(10, 512, 20, area);

    ptr = display.GetReducedDisplayBuffer();
    display.render(ptr, &area);

    display.AddTextToBuffer("Damian Szymecki", 0, 0);

    uint RGB_LED_PWM = 10;
    uint BuzzerPWM = 10;
    bool Flip = false;
    int dir = 0;
    float X = 0;
    float Y = 0;
    bool State = false;
    bool Last = false;
    bool Switches = false;
    int CurrentDir = 0;
    int LastDir = 0;
    while (true)
    {
        dir = RotEnc.CheckStep();
        X = joystick.GetVRx();
        Y = joystick.GetVRy();
        State = joystick.CheckForButtonPress();
        Flip = joystick.FlipFlopButton(&State, &Last);

        if (joystick.GetFlipFlop() == true)
            gpio_put(LED, ON);
        else
            gpio_put(LED, OFF);

        if (dir == 1)
        {
            if (Flip == true)
                RGB_LED_PWM += 50;
            else
                BuzzerPWM += 50;
        }

        if (dir == -1)
        {
            if (Flip == true)
                RGB_LED_PWM -= 50;
            else
                BuzzerPWM -= 50;
        }
        
        pwm_set_gpio_level(Buzzer, BuzzerPWM);
        pwm_set_gpio_level(RGBLED, RGB_LED_PWM);
        

        /*if (X > 0.9)
        {
            display.fill(buf, 0xC0);
            CurrentDir = 1;
        }  
        if (X < -0.9)
        {
            display.fill(buf, 0x30);
            CurrentDir = -1;
        }
        if (Y > 0.9)
        {
            display.fill(buf, 0x0C);
            CurrentDir = 2;
        }
        if (Y < -0.9)
        {
            display.fill(buf, 0x03);
            CurrentDir = -2;
            //display.render(buf, &area);
        }
           
        if (LastDir != CurrentDir)
            display.render(buf, &area);

        LastDir = CurrentDir;*/
        
    }
    

    return 0;
}