#include "pico/stdlib.h"            //standardowa biblioteka
#include "pico/multicore.h"         //wątki
#include <hardware/pwm.h>
#include <hardware/gpio.h>
#include "Font.h"
#include "Display.h"
#include "Button.h"
//#include <string>


#define GPIO_ON 1
#define GPIO_OFF 0

#define LED0 25 // On board LED

#define Buzzer 22 // Buzzer pin

#define Button0 0 //
#define Button1 1 //
#define Button2 2 //
#define Button3 3 //
#define Button4 4 //
#define Button5 5 //
#define Button6 6 //
#define Button7 7 // Temporairly tap sensor
#define Button8 8 //
#define Button9 9 //
#define ButtonAux 10 // Auxiliary button pin

#define DisplaySDA 18 // Display Data
#define DisplaySCL 19 // Display Clock

//#define MPU6050SDA 18 // Accelerometer Data
//#define MPU6050SCL 19 // Accelerometer Clock
#define MPU6050INT 20 // Accelerometer interrupt


void flash(int Duration_ms = 50, int FlashCount = 1);
void PWMSetup();


int main()
{

    gpio_init(LED0);
    gpio_set_dir(LED0, GPIO_OUT);

    Display display(128, 32, 0x3C, DisplaySDA, DisplaySCL);
    Button btn7(Button7, 'a', 50);
    btn7.SetSharedCooldown(0);
    btn7.SetSharedCooldownLength(0);
 
    display.SetFrameArea(0, 127, 0, 3, 512);
    display.oled_init();
    display.calc_render_area_buflen(&display.GetRenderArea());
    display.ZeroDisplay();
    display.render();
    display.oled_send_cmd(0xA4);
    
    
    
    display.ZeroDisplay();
    display.render();
      

    
    

    
    //sleep_ms(500);

    display.AddTextToBuffer("hello!@#", 10, 10);
    display.InverseDisplayBuffer();
    display.render();
    sleep_ms(50);


    display.ZeroDisplay();
    display.render();


    while (true)
    {
        if (btn7.GetCooldown() > 0)
            gpio_put(LED0, GPIO_ON);
        else
            gpio_put(LED0, GPIO_OFF);

        if (btn7.CanBePressed() && btn7.IsPressed())
        {
            display.AddTextToBuffer(btn7.GetButtonSymbolString(), display.GetCursorRow(), display.GetCursorColumn());
            display.render();
            display.IncrementCursorColumn(16);
            display.IncrementCursorRow(8);

            display.AddTextToBuffer(std::to_string(display.GetCursorRow()), 8, 8);
            display.render();

            
        }
        

        btn7.DecrementCooldown();
        sleep_ms(10);
    }
    

    

   /*display.SwitchPixelState(10, 10);
    display.oled_init();
    display.calc_render_area_buflen(&area);
    uint buf[OLED_BUF_LEN];
    display.fill(buf, 0x01);
    
    display.render(buf, &area);*/

    /*flash();
    sleep_ms(500);

    int i = 0;
    while (true)
    {
        display.AddTextToBuffer("a", i, i);
        display.render(display.CalculateReducedDisplayBuffer(), &area);
        sleep_ms(100);
        i++;
        flash();
    }*/
    

    
    


    /*display.SetFrameArea(0, display.GetDisplayWidth() - 1, 0, display.GetDisplayHeight() / 8, 0);

    display.AddTextToBuffer("bruhs", 0, 0);

    display.CalculateReducedDisplayBuffer();

    display.render(display.GetDisplayBuffer());*/


    /*display.ZeroDisplay();
    //display.AddTextToBuffer("hello!", 0, 0);
    display.AddLetter(a, 5, 5, 8, 8);
    display.CalculateReducedDisplayBuffer();
    display.render();
    flash();
    flash();
    sleep_ms(500);
    flash();
    flash();

    //multicore_launch_core1(flash);


    gpio_init(MPU6050INT);

    
    gpio_set_dir(Buzzer, GPIO_OUT);


    gpio_set_dir(MPU6050INT, GPIO_IN);

    int i = 0;
    int j = 0;
    uint* ptr = new uint[display.GetDisplaySize() / 8]();

    display.ZeroDisplay();

    while (true)
    {

        display.SwitchPixelState(i % display.GetDisplayWidth(), j % display.GetDisplayHeight());

        display.CalculateReducedDisplayBuffer();
        display.render();
        i++;
        j++;
        sleep_ms(500);
        flash();
    }
    */
}

void flash(int Duration_ms, int FlashCount)
{
    for (int i = 0; i < FlashCount; i++)
    {
        gpio_put(LED0, GPIO_ON);
        sleep_ms(Duration_ms);

        gpio_put(LED0, GPIO_OFF);
        sleep_ms(Duration_ms);
    }
}
