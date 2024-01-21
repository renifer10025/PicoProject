#include "Display.h"
#include <pico/stdlib.h>
#include <hardware/i2c.h>
#include "../Font.h"

#ifndef IIC
#define IIC 1
#endif // !IIC 1


uint Display::DisplayCount = 0;


// Simple constructor
// Sets all fields to 0 and increments DisplayCount by 1
Display::Display()
{
	Screen.Width = 0;
	Screen.Height = 0;
	DisplayBuffer = nullptr;
	//Screen[64][32] = { 0 };
	Address = 0;
	CommunicationProtocol = 0;
	DisplayCount++;
}


// I2C constructor
// Sets Width, Height, Address, SDA and CLK pins
// If no parameters are given, sets all to -1
Display::Display(uint A_Width = -1, uint A_Height = -1, uint A_Address = -1, uint A_SDA_Pin = -1, uint A_CLK_Pin = -1)
{
	DisplayCount++;

	if (A_Width >= 0)
		Screen.Width = A_Width;

	if (A_Height >= 0)
		Screen.Height = A_Height;

	if (A_Height >= 0 && A_Width >= 0)
    {
        DisplayBuffer = new uint[A_Width * A_Height]();
        ReducedDisplayBuffer = new uint[A_Width * A_Height / 8]();
    }
		

	if (A_Address >= 0)
		Address = A_Address;

	if (A_SDA_Pin >= 0)
		SDA_Pin = A_SDA_Pin;

	if (A_CLK_Pin >= 0)
		SCL_Pin = A_CLK_Pin;

	CommunicationProtocol = IIC;

	if (SCL_Pin >= 0 && SDA_Pin >= 0 && SCL_Pin != SDA_Pin)
	{
		gpio_set_function(SCL_Pin, GPIO_FUNC_I2C);
		gpio_set_function(SDA_Pin, GPIO_FUNC_I2C);

		gpio_pull_up(SCL_Pin);
    	gpio_pull_up(SDA_Pin);

		

		i2c_init(i2c1, 400 * 1000); //Use i2c port with baud rate of 4Mhz
		ZeroDisplay();
	}
}

// Complex constructor
// Sets all fields to specified values
Display::Display(uint C_Width = 0, uint C_Height = 0, uint C_Address = 0, uint C_CommunicationProtocol = 0)
{
	DisplayCount++;
	Screen.Width = C_Width;
	Screen.Height = C_Height;
	DisplayBuffer = new uint[C_Width * C_Height];
    ReducedDisplayBuffer = new uint[C_Width * C_Height / 8]();
	Address = C_Address;
	CommunicationProtocol = C_CommunicationProtocol;
}

// Simple destructor
// Sets all fields to 0 and decrements DisplayCount by 1
Display::~Display()
{
	Screen.Width = 0;
	Screen.Height = 0;
	DisplayBuffer = nullptr;
	//Screen[64][32] = { 0 };
	Address = 0;
	DisplayCount--;
}

// Returns the number of active displays
uint Display::GetDisplayCount()
{
	return DisplayCount;
}

// Prints the DisplayBuffer to the console in selected mode - default mode: 0
// 0 - render 0s as ' ' and 1s as '#' | ASCII 32 <-> ' ' / ASCII 35 <-> '#'
// 1 - render 0s as '0' and 1s as '1' | ASCII 48 <-> '0' / ASCII 49 <-> '1'
uint Display::PrintDisplay(bool Binary)
{
	if (Binary == 0)
	{
		for (uint i = 0; i < Screen.Height; i++)
		{
			for (uint j = 0; j < Screen.Width; j++)
			{
				if (DisplayBuffer[i * Screen.Width + j] == 0)
					std::cout << " ";
				else
					std::cout << "#";
				//std::cout << i * Screen.Width + j << " ";
			}
			std::cout << std::endl;
		}
	}
	else
	{
	for (uint i = 0; i < Screen.Height; i++)
		{
			for (uint j = 0; j < Screen.Width; j++)
			{
				std::cout << DisplayBuffer[i * Screen.Width + j];
				//std::cout << i * Screen.Width + j << " ";
			}
			std::cout << std::endl;
		}
	}

	
	return 0;
}

// Prints ReducedDisplayBuffer to console using std::cout
uint Display::PrintReducedDisplay(uint* ReducedDisplayBuffer, bool Binary)
{
    // Converting reduced array into printable format
    uint Aux = 0;
    uint* Arr = new uint[Screen.Width * Screen.Height];

    for (uint i = 0; i < Screen.Height; i += 8)
    {
        for (uint j = 0; j < Screen.Width; j++)
        {
            Aux = ReducedDisplayBuffer[j];
            for (uint k = 0; k < 8; k++)
            {
                if (Aux % 2 == 0)
                    Arr[(Screen.Height * k) + (Screen.Width * i) + j] = 0;
                else
                    Arr[(Screen.Height * k) + (Screen.Width * i) + j] = 1;
                Aux /= 10;
            }
        }
    }
	
    // Printing the reduced array
	if (Binary == 0)
	{
        for (int i = 0; i < Screen.Height; i++)
        {
            for (int j = 0; j < Screen.Width; j++)
            {
                if (Arr[i * Screen.Height + j] == 0)
                    std::cout << " ";
                else
                    std::cout << "#";
            }
            std::cout << std::endl;
        }
    }
	
	else
	{
	for (uint i = 0; i < Screen.Height; i++)
		{
			for (uint j = 0; j < Screen.Width; j++)
			{
				std::cout << DisplayBuffer[i * Screen.Width + j];
			}
			std::cout << std::endl;
		}
	}

	return 0;
}

// Returns an address of the display
uint Display::GetAddress()
{
	return Address;
}

// Sets the display's address
uint Display::SetAddress(uint Addr)
{
	Address = Addr;

	return 0;
}

// \returns Screen.Height * Screen.Width
uint Display::GetDisplaySize()
{
	uint Size = Screen.Height * Screen.Width;
	return Size;
}

// Sets the display dimensions and calculates the DisplayBuffer size;
uint Display::SetDisplaySize(uint Width, uint Height)
{
	if (DisplayBuffer != nullptr)
		delete[] DisplayBuffer;

	Display::Screen.Width = Width;
	Display::Screen.Height = Height;
	DisplayBuffer = new uint[Width * Height];

	return 0;
}

// Returns display width
uint Display::GetDisplayWidth()
{
	return Screen.Width;
}

// Returns display width
uint Display::GetDisplayHeight()
{
	return Screen.Height;
}

// Returns pointer to DisplayBuffer
uint* Display::GetDisplayBuffer()
{
	return DisplayBuffer;
}


uint* Display::GetReducedDisplayBuffer()
{
    return ReducedDisplayBuffer;
}


// Instead of returning an array of 1s and 0s, it returns an array of... 
// ...((Screen.Width * Screen.Height) / 8) numbers signifying 8 bits per 1 number
// That are used in void Display::render(uint *buf, struct render_area *area)
// \returns array pointer to a reduced DisplayBuffer 
uint* Display::CalculateReducedDisplayBuffer()
{
    uint Location = 0;
    uint Number = 0;
    uint Iterator = 0;
    uint AuxArr[8] = {0};
    
    //uint* Arr = new uint[Screen.Height * Screen.Width / 8];
    //uint Arr[Screen.Height * Screen.Width / 8];
    
    for (uint i = 0; i < Screen.Height; i += 8)
    {
        for (uint j = 0; j < Screen.Width; j++)
        {
            Number = 0;
            for (int k = 0; k < 8; k++)
            {
                Location = (Screen.Width * i) + (k * Screen.Width) + j;
                AuxArr[k] = DisplayBuffer[Location];
            }
            
            Number = ArrayToBitNumberLE(AuxArr);
            ReducedDisplayBuffer[Iterator] = Number;
            Iterator++;
        }
    }
    return ReducedDisplayBuffer;
}

// Returns FrameArea
render_area Display::GetFrameArea()
{
	return frame_area;
}


uint Display::SetFrameArea(render_area Frame)
{
	frame_area.start_col = Frame.start_col;
	frame_area.end_col = Frame.end_col;
	frame_area.start_page = Frame.start_page;
	frame_area.end_page = Frame.end_page;
	frame_area.buflen = Frame.buflen;
	return 0;
}


uint Display::SetFrameArea(uint StartCol, uint EndCol, uint StartPage, uint EndPage, uint BuferLength )
{
	frame_area.start_col = StartCol;
	frame_area.end_col = EndCol;
	frame_area.start_page = StartPage;
	frame_area.end_page = EndPage;
	frame_area.buflen = BuferLength;
	
	return 0;
}

// Zeroes the display
// Brings the display to it's initial values
uint Display::ZeroDisplay()
{
	for (uint i = 0; i < Screen.Width * Screen.Height; i++)
		DisplayBuffer[i] = 0;

	return 0;
}

uint Display::ClearArea(uint StartX, uint StartY, uint EndX, uint EndY)
{
	for (int i = StartY; i <= EndY; i++)
		for (int j = StartX; j <= EndX; j++)
			SetPixelState(i, j, 1);
	return 0;
}

uint Display::RandomPixelTest(uint Times, uint ArraySize, uint SleepDuration, render_area& Area)
{
    uint Arr[ArraySize] = { 0 };
    for (uint i = 0; i < Times; i++)
    {
        for (uint j = 0; j < ArraySize; j++)
        {
            if (i < 128)
                Arr[j] = rand() % 255;
            /* else if (j >= 128)
                Arr[j] = rand() % 127; */
        }
        render(Arr, &Area);
        sleep_ms(SleepDuration);
    }
    return 0;
}

// Add a single pixel to the display
// C_Width - X | C_Height - Y | Mode - decides which row-column navigation mode to use (explained down below)
// ______For Mode = 0:______
// Values from 0 to n-1, where n = Screen.Height and/or Screen.Width examples:
// For a 5x3 display SwitchPixelState(4, 2, 0) would result in bottom right pixel to flip it's value
// For a 5x3 display SwitchPixelState(4, 0, 0) would result in top right pixel to flip it's value
// 
// ______For Mode = 1:______
// Values from 1 to n, where n = Screen.Height and/or Screen.Width examples:
// For a 5x3 display SwitchPixelState(5, 3, 0) would result in bottom right pixel to flip it's value
// For a 5x3 display SwitchPixelState(5, 1, 0) would result in top right pixel to flip it's value
uint Display::SwitchPixelState(uint C_Width = 0, uint C_Height = 0, bool Mode)
{
	if (Mode == 0)
	{
		uint Index = (C_Height % Screen.Height) * Screen.Width + (C_Width % Screen.Width);

        if (DisplayBuffer[Index] == 1)
            DisplayBuffer[Index] = 0;
        else
            DisplayBuffer[Index] = 1;

        return Index;
	}
	else
	{
		uint Index = ((C_Height - 1) % Screen.Height) * Screen.Width + ((C_Width - 1) % Screen.Width);
        
        if (DisplayBuffer[Index] == 1)
			DisplayBuffer[Index] = 0;
		else
			DisplayBuffer[Index] = 1;

		return Index;
	}
    return 0;
}

uint Display::SetPixelState(uint C_Width, uint C_Height, int Number)
{
    uint Index = (C_Height % Screen.Height) * Screen.Width + (C_Width % Screen.Width);
    DisplayBuffer[Index] = Number;

    return Index;
}

uint Display::InverseDisplayBuffer()
{
	for (uint i = 0; i < Screen.Height; i++)
		for (uint j = 0; j < Screen.Width; j++)
			SwitchPixelState(i, j);

	return 0;
}

// \todo Fix
uint Display::ClearArea(uint StartX, uint StartY, uint EndX, uint EndY)
{
	for (uint i = StartY; i < EndY; i++)
	{
		for (uint j = StartX; j < EndX; j++)
		{
			if (DisplayBuffer[i * Screen.Height + j] == 1)
			{
				SwitchPixelState(i, j);
			}
		}
	}
	return 0;
}


// Big endian
// MSB -> LSB
uint Display::ArrayToBitNumberBE(uint* Arr)
{
    int Number = 0;
    
    for (int i = 0; i < 8; i++)
    {
        Number += Arr[i];
        if (i < 8)
            Number = Number << 1;
    }
    return Number;
}

// Little endian
// LSB -> MSB
uint Display::ArrayToBitNumberLE(uint* Arr)
{
    int Number = 0;
    
    for (int i = 7; i >= 0; i--)
    {
        Number += Arr[i];
        if (i > 0)
            Number = Number << 1;
    }
    return Number;
}

// \warning Remember to use delete[] after done with use of the returned pointer
// \returns Pointer to an array
uint* Display::NumberToBitArray(uint Number, uint NumberSize = 8)
{
    uint* Arr = new uint[NumberSize]();
    uint Aux = 0;
    for (int i = 0; i < NumberSize - 1; i++)
    {
        Aux = Number % 2;
        if (i < NumberSize - 2)
            Number = Number >> 1;
        Arr[i] = Aux;
    }
    return Arr;
}


// \ ss
uint Display::OnBootUp()
{
    // TODO: 
}


uint* Display::AddTwoArrays(uint* Arr, uint StartCollumn, uint StartRow, uint ArrWidth, uint ArrHeight)
{
    for (int i = 0; i < Screen.Height; i++)
        for (int j = 0; j < Screen.Width; j++)
            if (j >= StartRow && j < StartRow + ArrWidth)
                if (i >= StartCollumn && i < StartRow + ArrHeight)
                    DisplayBuffer[i * Screen.Width + j] = Arr[(j - StartCollumn) * ArrWidth + (StartCollumn - j)];

    return DisplayBuffer;
}


// 
uint Display::AddTextToBuffer(std::string Str, uint PosX = 0, uint PosY = 0)
{
    for (int iter = 0; iter < Str.length(); iter++)
    {
        switch (Str[iter])
        {
        case 'A':
            AddTwoArrays(a, PosX, PosY, 8, 8);
            break;

        case 'Ą':
            AddTwoArrays(a_, PosX, PosY, 8, 8);
            break;

        case 'B':
            AddTwoArrays(b, PosX, PosY, 8, 8);
            break;

        case 'C':
            AddTwoArrays(c, PosX, PosY, 8, 8);
            break;

        case 'Ć':
            AddTwoArrays(c_, PosX, PosY, 8, 8);
            break;

        case 'D':
            AddTwoArrays(d, PosX, PosY, 8, 8);
            break;

        case 'E':
            AddTwoArrays(e, PosX, PosY, 8, 8);
            break;

        case 'Ę':
            AddTwoArrays(e_, PosX, PosY, 8, 8);
            break;

        case 'F':
            AddTwoArrays(f, PosX, PosY, 8, 8);
            break;

        case 'G':
            AddTwoArrays(g, PosX, PosY, 8, 8);
            break;

        case 'H':
            AddTwoArrays(h, PosX, PosY, 8, 8);
            break;

        case 'I':
            AddTwoArrays(i, PosX, PosY, 8, 8);
            break;

        case 'J':
            AddTwoArrays(j, PosX, PosY, 8, 8);
            break;

        case 'K':
            AddTwoArrays(k, PosX, PosY, 8, 8);
            break;

        case 'L':
            AddTwoArrays(l, PosX, PosY, 8, 8);
            break;

        case 'Ł':
            AddTwoArrays(l_, PosX, PosY, 8, 8);
            break;

        case 'M':
            AddTwoArrays(m, PosX, PosY, 8, 8);
            break;

        case 'N':
            AddTwoArrays(n, PosX, PosY, 8, 8);
            break;

        case 'Ń':
            AddTwoArrays(n_, PosX, PosY, 8, 8);
            break;

        case 'O':
            AddTwoArrays(o, PosX, PosY, 8, 8);
            break;

        case 'Ó':
            AddTwoArrays(o_, PosX, PosY, 8, 8);
            break;

        case 'P':
            AddTwoArrays(p, PosX, PosY, 8, 8);
            break;

        case 'Q':
            AddTwoArrays(q, PosX, PosY, 8, 8);
            break;

        case 'R':
            AddTwoArrays(r, PosX, PosY, 8, 8);
            break;

        case 'S':
            AddTwoArrays(s, PosX, PosY, 8, 8);
            break;

        case 'Ś':
            AddTwoArrays(s_, PosX, PosY, 8, 8);
            break;

        case 'T':
            AddTwoArrays(t, PosX, PosY, 8, 8);
            break;

        case 'U':
            AddTwoArrays(u, PosX, PosY, 8, 8);
            break;

        case 'W':
            AddTwoArrays(w, PosX, PosY, 8, 8);
            break;

        case 'V':
            AddTwoArrays(v, PosX, PosY, 8, 8);
            break;

        case 'X':
            AddTwoArrays(x, PosX, PosY, 8, 8);
            break;

        case 'Y':
            AddTwoArrays(y, PosX, PosY, 8, 8);
            break;

        case 'Z':
            AddTwoArrays(z, PosX, PosY, 8, 8);
            break;

        case 'Ź':
            AddTwoArrays(z_, PosX, PosY, 8, 8);
            break;

        case 'Ż':
            AddTwoArrays(z__, PosX, PosY, 8, 8);
            break;

//  Numbers

        case '1':
            AddTwoArrays(one, PosX, PosY, 8, 8);
            break;

        case '2':
            AddTwoArrays(two, PosX, PosY, 8, 8);
            break;

        case '3':
            AddTwoArrays(three, PosX, PosY, 8, 8);
            break;

        case '4':
            AddTwoArrays(four, PosX, PosY, 8, 8);
            break;

        case '5':
            AddTwoArrays(five, PosX, PosY, 8, 8);
            break;

        case '6':
            AddTwoArrays(six, PosX, PosY, 8, 8);
            break;

        case '7':
            AddTwoArrays(seven, PosX, PosY, 8, 8);
            break;
            
        case '8':
            AddTwoArrays(eight, PosX, PosY, 8, 8);
            break;
        
        case '9':
            AddTwoArrays(nine, PosX, PosY, 8, 8);
            break;

        case '0':
            AddTwoArrays(zero, PosX, PosY, 8, 8);
            break;

//  Symbols

        case '<':
            AddTwoArrays(less, PosX, PosY, 8, 8);
            break;
            
        case '>':
            AddTwoArrays(more, PosX, PosY, 8, 8);
            break;
        
        case '/':
            AddTwoArrays(back_slash, PosX, PosY, 8, 8);
            break;

        case '\\':
            AddTwoArrays(slash, PosX, PosY, 8, 8);
            break;

        case '?':
            AddTwoArrays(question_mark, PosX, PosY, 8, 8);
            break;
            
        case '.':
            AddTwoArrays(period, PosX, PosY, 8, 8);
            break;
        
        case ',':
            AddTwoArrays(comma, PosX, PosY, 8, 8);
            break;

        case '|':
            AddTwoArrays(bar, PosX, PosY, 8, 8);
            break;

        case '[':
            AddTwoArrays(square_bracket_open, PosX, PosY, 8, 8);
            break;
            
        case ']':
            AddTwoArrays(square_bracket_close, PosX, PosY, 8, 8);
            break;
        
        case '{':
            AddTwoArrays(curly_bracket_open, PosX, PosY, 8, 8);
            break;

        case '}':
            AddTwoArrays(curly_bracket_close, PosX, PosY, 8, 8);
            break;

        case '\'':
            AddTwoArrays(back_slash, PosX, PosY, 8, 8);
            break;
            
        case '"':
            AddTwoArrays(quote, PosX, PosY, 8, 8);
            break;
        
        case ':':
            AddTwoArrays(colon, PosX, PosY, 8, 8);
            break;

        case ';':
            AddTwoArrays(semicolon, PosX, PosY, 8, 8);
            break;

        case '`':
            AddTwoArrays(backquote, PosX, PosY, 8, 8);
            break;
            
        case '~':
            AddTwoArrays(tilde, PosX, PosY, 8, 8);
            break;
        
        case '!':
            AddTwoArrays(exclanation_mark, PosX, PosY, 8, 8);
            break;

        case '@':
            AddTwoArrays(at, PosX, PosY, 8, 8);
            break;

        case '#':
            AddTwoArrays(hash, PosX, PosY, 8, 8);
            break;
            
        case '$':
            AddTwoArrays(dollar, PosX, PosY, 8, 8);
            break;
        
        case '%':
            AddTwoArrays(percent, PosX, PosY, 8, 8);
            break;

        case '^':
            AddTwoArrays(cater, PosX, PosY, 8, 8);
            break;

        case '&':
            AddTwoArrays(ampersand, PosX, PosY, 8, 8);
            break;
            
        case '*':
            AddTwoArrays(asterisk, PosX, PosY, 8, 8);
            break;
        
        case '(':
            AddTwoArrays(parenthesis_open, PosX, PosY, 8, 8);
            break;

        case ')':
            AddTwoArrays(parenthesis_close, PosX, PosY, 8, 8);
            break;

        case '_':
            AddTwoArrays(underscore, PosX, PosY, 8, 8);
            break;
            
        case '-':
            AddTwoArrays(dash, PosX, PosY, 8, 8);
            break;

        case '+':
           AddTwoArrays(plus, PosX, PosY, 8, 8);
            break;
            
        case '=':
            AddTwoArrays(equals, PosX, PosY, 8, 8);
            break;

// Arrows

        case 'Arrow_up':
            AddTwoArrays(arrow_up, PosX, PosY, 8, 8);
            break;
        
        case 'Arrow_down':
            AddTwoArrays(arrow_down, PosX, PosY, 8, 8);
            break;
        
        case 'Arrow_left':
            AddTwoArrays(arrow_left, PosX, PosY, 8, 8);
            break;
        
        case 'Arrow_right':
            AddTwoArrays(arrow_right, PosX, PosY, 8, 8);
            break;
        
// Hearts

        case 'Heart_full':
            AddTwoArrays(heart_full, PosX, PosY, 8, 8);
            break;

        case 'Heart_half':
            AddTwoArrays(heart_half, PosX, PosY, 8, 8);
            break;

        case 'Heart_empty':
            AddTwoArrays(heart_empty, PosX, PosY, 8, 8);
            break;

// Animated

        case 'Loading_animation_1':
            AddTwoArrays(loading_animation_1, PosX, PosY, 8, 8);
            break;

        case 'Loading_animation_2':
            AddTwoArrays(loading_animation_2, PosX, PosY, 8, 8);
            break;

        case 'Loading_animation_3':
            AddTwoArrays(loading_animation_3, PosX, PosY, 8, 8);
            break;

        case 'Loading_animation_4':
            AddTwoArrays(loading_animation_4, PosX, PosY, 8, 8);
            break;

        case 'Loading_animation_5':
            AddTwoArrays(loading_animation_5, PosX, PosY, 8, 8);
            break;

        default:    //  UNKNOWN CHARACTER
            AddTwoArrays(unknown_character, PosX, PosY, 8, 8);
            break;
        }
    }
}



void Display::tests(uint buf[])
{
    for (int i = 0; i < OLED_BUF_LEN; i++)
        buf[i] = (0xAA);
}

void Display::fill(uint buf[], uint fill) {
    // fill entire buffer with the same byte
    for (int i = 0; i < OLED_BUF_LEN; i++) {
        buf[i] = fill;
    }
}

void Display::calc_render_area_buflen(struct render_area *area) {
    // calculate how long the flattened buffer will be for a render area
    area->buflen = (area->end_col - area->start_col + 1) * (area->end_page - area->start_page + 1);
}

void Display::oled_send_cmd(uint cmd) {
    // I2C write process expects a control byte followed by data
    // this "data" can be a command or data to follow up a command

    // Co = 1, D/C = 0 => the driver expects a command
    uint buf[2] = {0x80, cmd};

	i2c_write_blocking(i2c1,(OLED_ADDR & OLED_WRITE_MODE), buf, 2, false);
}

void Display::oled_send_buf(uint buf[], int buflen) {
    // in horizontal addressing mode, the column address pointer auto-increments
    // and then wraps around to the next page, so we can send the entire frame
    // buffer in one gooooooo!

    // copy our frame buffer into a new buffer because we need to add the control byte
    // to the beginning

    // TODO find a more memory-efficient way to do this..
    // maybe break the data transfer into pages?
    uint* tmp = new uint;

    for (int i = 1; i < buflen + 1; i++) {
        tmp[i] = buf[i - 1];
    }
    // Co = 0, D/C = 1 => the driver expects data to be written to RAM
    tmp[0] = 0x40;
    i2c_write_blocking(i2c1, (OLED_ADDR & OLED_WRITE_MODE), tmp, buflen + 1, false);

    delete tmp;
}

void Display::oled_init() {
    // some of these commands are not strictly necessary as the reset
    // process defaults to some of these but they are shown here
    // to demonstrate what the initialization sequence looks like

    // some configuration values are recommended by the board manufacturer

    oled_send_cmd(OLED_SET_DISP | 0x00); // set display off

    /* memory mapping */
    oled_send_cmd(OLED_SET_MEM_ADDR); // set memory address mode
    oled_send_cmd(0x00); // horizontal addressing mode

    /* resolution and layout */
    oled_send_cmd(OLED_SET_DISP_START_LINE); // set display start line to 0

    oled_send_cmd(OLED_SET_SEG_REMAP | 0x01); // set segment re-map
    // column address 127 is mapped to SEG0

    oled_send_cmd(OLED_SET_MUX_RATIO); // set multiplex ratio
    oled_send_cmd(OLED_HEIGHT - 1); // our display is only 32 pixels high

    oled_send_cmd(OLED_SET_COM_OUT_DIR | 0x08); // set COM (common) output scan direction
    // scan from bottom up, COM[N-1] to COM0

    oled_send_cmd(OLED_SET_DISP_OFFSET); // set display offset
    oled_send_cmd(0x00); // no offset

    oled_send_cmd(OLED_SET_COM_PIN_CFG); // set COM (common) pins hardware configuration
    oled_send_cmd(0x02); // manufacturer magic number

    /* timing and driving scheme */
    oled_send_cmd(OLED_SET_DISP_CLK_DIV); // set display clock divide ratio
    oled_send_cmd(0x80); // div ratio of 1, standard freq

    oled_send_cmd(OLED_SET_PRECHARGE); // set pre-charge period
    oled_send_cmd(0xF1); // Vcc internally generated on our board

    oled_send_cmd(OLED_SET_VCOM_DESEL); // set VCOMH deselect level
    oled_send_cmd(0x30); // 0.83xVcc

    /* display */
    oled_send_cmd(OLED_SET_CONTRAST); // set contrast control
    oled_send_cmd(0xFF);

    oled_send_cmd(OLED_SET_ENTIRE_ON); // set entire display on to follow RAM content

    oled_send_cmd(OLED_SET_NORM_INV); // set normal (not inverted) display

    oled_send_cmd(OLED_SET_CHARGE_PUMP); // set charge pump
    oled_send_cmd(0x14); // Vcc internally generated on our board

    oled_send_cmd(OLED_SET_SCROLL | 0x00); // deactivate horizontal scrolling if set
    // this is necessary as memory writes will corrupt if scrolling was enabled

    oled_send_cmd(OLED_SET_DISP | 0x01); // turn display on
}

void Display::render(uint *buf, struct render_area *area) {
    // update a portion of the display with a render area
    oled_send_cmd(OLED_SET_COL_ADDR);
    oled_send_cmd(area->start_col);
    oled_send_cmd(area->end_col);

    oled_send_cmd(OLED_SET_PAGE_ADDR);
    oled_send_cmd(area->start_page);
    oled_send_cmd(area->end_page);

    oled_send_buf(buf, area->buflen);
}


void Display::render(uint *buf, struct render_area *area) {
    // update a portion of the display with a render area
    oled_send_cmd(OLED_SET_COL_ADDR);
    oled_send_cmd(area->start_col);
    oled_send_cmd(area->end_col);

    oled_send_cmd(OLED_SET_PAGE_ADDR);
    oled_send_cmd(area->start_page);
    oled_send_cmd(area->end_page);

    oled_send_buf(buf, area->buflen);
}


void Display::render()
{
    // update a portion of the display with a render area
    oled_send_cmd(OLED_SET_COL_ADDR);
    oled_send_cmd(frame_area.start_col);
    oled_send_cmd(frame_area.end_col);

    oled_send_cmd(OLED_SET_PAGE_ADDR);
    oled_send_cmd(frame_area.start_page);
    oled_send_cmd(frame_area.end_page);

    oled_send_buf(ReducedDisplayBuffer, frame_area.buflen);
}