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
    CursorCol = 0;
    CursorRow = 0;
    IncremetDisplayCount();
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
    CursorCol = 0;
    CursorRow = 0;

	if (SCL_Pin >= 0 && SDA_Pin >= 0 && SCL_Pin != SDA_Pin)
	{
		gpio_set_function(SCL_Pin, GPIO_FUNC_I2C);
		gpio_set_function(SDA_Pin, GPIO_FUNC_I2C);

		gpio_pull_up(SCL_Pin);
    	gpio_pull_up(SDA_Pin);

		

		i2c_init(i2c1, 400 * 1000); //Use i2c port with baud rate of 4Mhz
		ZeroDisplay();
	}
    IncremetDisplayCount();
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
    CursorCol = 0;
    CursorRow = 0;
    IncremetDisplayCount();
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
    CursorCol = 0;
    CursorRow = 0;
    DecrementDisplayCount();
}

// Returns the number of active displays
uint Display::GetDisplayCount()
{
	return DisplayCount;
}

uint Display::IncremetDisplayCount()
{
    DisplayCount++;
    return DisplayCount;
}

uint Display::DecrementDisplayCount()
{
    DisplayCount--;
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
			SetPixelState(i, j, 0);
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
uint* Display::NumberToBitArray(uint Number = 0, uint NumberSize = 8)
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


uint* Display::ReorganizeArray(uint* Arr, uint ArrWidth, uint ArrHeight)
{
    uint* ptr = new uint[ArrWidth * ArrHeight];
    for (uint i = 0; i < ArrHeight; i++)
        for (uint j = 0; j < ArrWidth; j++)
            ptr[j * ArrWidth + i] = Arr[i * ArrWidth + j];

    for (uint i = 0; i < ArrHeight * ArrWidth; i++)
        Arr[i] = ptr[i];
        
    delete[] ptr;
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

    CursorCol++;
    return DisplayBuffer;
}

uint* Display::AddLetter(uint* Arr, uint StartCollumn, uint StartRow, uint ArrWidth = 8, uint ArrHeight = 8)
{
    
    if (CursorRow >= Screen.Width)
    {
        CursorRow = 0;
        CursorCol += 8;
    }
    
    uint* LetterArr = new uint[ArrWidth * ArrHeight]();
    uint* ptr = nullptr;
    
    for (int i = 0; i < ArrHeight; i++)
    {
        ptr = NumberToBitArray(Arr[i]);
        for (int j = 0; j < ArrWidth; j++)
            LetterArr[i * ArrWidth + j] = ptr[j];
    }
    delete[] ptr;
    //ReorganizeArray(LetterArr, ArrWidth, ArrHeight);
                
    for (uint i = 0; i < Screen.Height; i++)// i = Collumn
        for (uint j = 0; j < Screen.Width; j++)// j = Row
            if (j >= CursorRow && j < CursorRow + ArrHeight)
                if (i >= CursorCol && i < CursorCol + ArrWidth)
                {
                    uint LetterArrIndex = (j - CursorRow) * ArrWidth + (i - CursorCol);
                    uint DisplayBufferIndex = i * Screen.Width + j;
                    DisplayBuffer[DisplayBufferIndex] = LetterArr[LetterArrIndex];
                }

    delete[] LetterArr;
    CursorRow += 8;
    return DisplayBuffer;
}


// Adds text to buffer
// For now works with lower case text
uint Display::AddTextToBuffer(std::string Str, uint PosX = 0, uint PosY = 0)
{   
    if (PosX <= (Screen.Width - 1))
        CursorCol = PosX;
    if (PosY <= (Screen.Height - 1))
        CursorRow = PosY;

    for (int iter = 0; iter < Str.length(); iter++)
    {
        switch (Str[iter])
        {
        case 'a':
            AddLetter(a, CursorCol, CursorRow, 8, 8);
            break;

        case 'ą':
            AddLetter(a_, CursorCol, CursorRow, 8, 8);
            break;

        case 'b':
            AddLetter(b, CursorCol, CursorRow, 8, 8);
            break;

        case 'c':
            AddLetter(c, CursorCol, CursorRow, 8, 8);
            break;

        case 'ć':
            AddLetter(c_, CursorCol, CursorRow, 8, 8);
            break;

        case 'd':
            AddLetter(d, CursorCol, CursorRow);
            break;

        case 'e':
            AddLetter(e, CursorCol, CursorRow, 8, 8);
            break;

        case 'ę':
            AddLetter(e_, CursorCol, CursorRow, 8, 8);
            break;

        case 'f':
            AddLetter(f, CursorCol, CursorRow, 8, 8);
            break;

        case 'g':
            AddLetter(g, CursorCol, CursorRow, 8, 8);
            break;

        case 'h':
            AddLetter(h, CursorCol, CursorRow, 8, 8);
            break;

        case 'i':
            AddLetter(i, CursorCol, CursorRow, 8, 8);
            break;

        case 'j':
            AddLetter(j, CursorCol, CursorRow, 8, 8);
            break;

        case 'k':
            AddLetter(k, CursorCol, CursorRow, 8, 8);
            break;

        case 'l':
            AddLetter(l, CursorCol, CursorRow, 8, 8);
            break;

        case 'ł':
            AddLetter(l_, CursorCol, CursorRow, 8, 8);
            break;

        case 'm':
            AddLetter(m, CursorCol, CursorRow, 8, 8);
            break;

        case 'n':
            AddLetter(n, CursorCol, CursorRow, 8, 8);
            break;

        case 'ń':
            AddLetter(n_, CursorCol, CursorRow, 8, 8);
            break;

        case 'o':
            AddLetter(o, CursorCol, CursorRow, 8, 8);
            break;

        case 'ó':
            AddLetter(o_, CursorCol, CursorRow, 8, 8);
            break;

        case 'p':
            AddLetter(p, CursorCol, CursorRow, 8, 8);
            break;

        case 'q':
            AddLetter(q, CursorCol, CursorRow, 8, 8);
            break;

        case 'r':
            AddLetter(r, CursorCol, CursorRow, 8, 8);
            break;

        case 's':
            AddLetter(s, CursorCol, CursorRow, 8, 8);
            break;

        case 'ś':
            AddLetter(s_, CursorCol, CursorRow, 8, 8);
            break;

        case 't':
            AddLetter(t, CursorCol, CursorRow, 8, 8);
            break;

        case 'u':
            AddLetter(u, CursorCol, CursorRow, 8, 8);
            break;

        case 'w':
            AddLetter(w, CursorCol, CursorRow, 8, 8);
            break;

        case 'v':
            AddLetter(v, CursorCol, CursorRow, 8, 8);
            break;

        case 'x':
            AddLetter(x, CursorCol, CursorRow, 8, 8);
            break;

        case 'y':
            AddLetter(y, CursorCol, CursorRow, 8, 8);
            break;

        case 'z':
            AddLetter(z, CursorCol, CursorRow, 8, 8);
            break;

        case 'ź':
            AddLetter(z_, CursorCol, CursorRow, 8, 8);
            break;

        case 'ż':
            AddLetter(z__, CursorCol, CursorRow, 8, 8);
            break;

//  Numbers

        case '1':
            AddLetter(one, CursorCol, CursorRow, 8, 8);
            break;

        case '2':
            AddLetter(two, CursorCol, CursorRow, 8, 8);
            break;

        case '3':
            AddLetter(three, CursorCol, CursorRow, 8, 8);
            break;

        case '4':
            AddLetter(four, CursorCol, CursorRow, 8, 8);
            break;

        case '5':
            AddLetter(five, CursorCol, CursorRow, 8, 8);
            break;

        case '6':
            AddLetter(six, CursorCol, CursorRow, 8, 8);
            break;

        case '7':
            AddLetter(seven, CursorCol, CursorRow, 8, 8);
            break;
            
        case '8':
            AddLetter(eight, CursorCol, CursorRow, 8, 8);
            break;
        
        case '9':
            AddLetter(nine, CursorCol, CursorRow, 8, 8);
            break;

        case '0':
            AddLetter(zero, CursorCol, CursorRow, 8, 8);
            break;

//  Symbols

        case '<':
            AddLetter(less, CursorCol, CursorRow, 8, 8);
            break;
            
        case '>':
            AddLetter(more, CursorCol, CursorRow, 8, 8);
            break;
        
        case '/':
            AddLetter(back_slash, CursorCol, CursorRow, 8, 8);
            break;

        case '\\':
            AddLetter(slash, CursorCol, CursorRow, 8, 8);
            break;

        case '?':
            AddLetter(question_mark, CursorCol, CursorRow, 8, 8);
            break;
            
        case '.':
            AddLetter(period, CursorCol, CursorRow, 8, 8);
            break;
        
        case ',':
            AddLetter(comma, CursorCol, CursorRow, 8, 8);
            break;

        case '|':
            AddLetter(bar, CursorCol, CursorRow, 8, 8);
            break;

        case '[':
            AddLetter(square_bracket_open, CursorCol, CursorRow, 8, 8);
            break;
            
        case ']':
            AddLetter(square_bracket_close, CursorCol, CursorRow, 8, 8);
            break;
        
        case '{':
            AddLetter(curly_bracket_open, CursorCol, CursorRow, 8, 8);
            break;

        case '}':
            AddLetter(curly_bracket_close, CursorCol, CursorRow, 8, 8);
            break;

        case '\'':
            AddLetter(back_slash, CursorCol, CursorRow, 8, 8);
            break;
            
        case '"':
            AddLetter(quote, CursorCol, CursorRow, 8, 8);
            break;
        
        case ':':
            AddLetter(colon, CursorCol, CursorRow, 8, 8);
            break;

        case ';':
            AddLetter(semicolon, CursorCol, CursorRow, 8, 8);
            break;

        case '`':
            AddLetter(backquote, CursorCol, CursorRow, 8, 8);
            break;
            
        case '~':
            AddLetter(tilde, CursorCol, CursorRow, 8, 8);
            break;
        
        case '!':
            AddLetter(exclanation_mark, CursorCol, CursorRow, 8, 8);
            break;

        case '@':
            AddLetter(at, CursorCol, CursorRow, 8, 8);
            break;

        case '#':
            AddLetter(hash, CursorCol, CursorRow, 8, 8);
            break;
            
        case '$':
            AddLetter(dollar, CursorCol, CursorRow, 8, 8);
            break;
        
        case '%':
            AddLetter(percent, CursorCol, CursorRow, 8, 8);
            break;

        case '^':
            AddLetter(cater, CursorCol, CursorRow, 8, 8);
            break;

        case '&':
            AddLetter(ampersand, CursorCol, CursorRow, 8, 8);
            break;
            
        case '*':
            AddLetter(asterisk, CursorCol, CursorRow, 8, 8);
            break;
        
        case '(':
            AddLetter(parenthesis_open, CursorCol, CursorRow, 8, 8);
            break;

        case ')':
            AddLetter(parenthesis_close, CursorCol, CursorRow, 8, 8);
            break;

        case '_':
            AddLetter(underscore, CursorCol, CursorRow, 8, 8);
            break;
            
        case '-':
            AddLetter(dash, CursorCol, CursorRow, 8, 8);
            break;

        case '+':
           AddLetter(plus, CursorCol, CursorRow, 8, 8);
            break;
            
        case '=':
            AddLetter(equals, CursorCol, CursorRow, 8, 8);
            break;

        case ' ':
            AddLetter(space, CursorCol, CursorRow, 8, 8);
            break;

// Arrows

        case 'Arrow_up':
            AddLetter(arrow_up, CursorCol, CursorRow, 8, 8);
            break;
        
        case 'Arrow_down':
            AddLetter(arrow_down, CursorCol, CursorRow, 8, 8);
            break;
        
        case 'Arrow_left':
            AddLetter(arrow_left, CursorCol, CursorRow, 8, 8);
            break;
        
        case 'Arrow_right':
            AddLetter(arrow_right, CursorCol, CursorRow, 8, 8);
            break;
        
// Hearts

        case 'Heart_full':
            AddLetter(heart_full, CursorCol, CursorRow, 8, 8);
            break;

        case 'Heart_half':
            AddLetter(heart_half, CursorCol, CursorRow, 8, 8);
            break;

        case 'Heart_empty':
            AddLetter(heart_empty, CursorCol, CursorRow, 8, 8);
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
            AddTwoArrays(unknown_character, CursorCol, CursorRow, 8, 8);
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
    uint8_t buf[2] = {0x80, cmd};

	i2c_write_blocking(i2c1,(OLED_ADDR & OLED_WRITE_MODE), buf, 2, false);
}


void Display::oled_send_buf(uint8_t buf[], int buflen) {
    // in horizontal addressing mode, the column address pointer auto-increments
    // and then wraps around to the next page, so we can send the entire frame
    // buffer in one gooooooo!

    // copy our frame buffer into a new buffer because we need to add the control byte
    // to the beginning

    // TODO find a more memory-efficient way to do this..
    // maybe break the data transfer into pages?
    uint8_t* tmp = new uint8_t;

    for (int i = 1; i < buflen + 1; i++) {
        tmp[i] = buf[i - 1];
    }
    // Co = 0, D/C = 1 => the driver expects data to be written to RAM
    tmp[0] = 0x40;
    i2c_write_blocking(i2c1, (OLED_ADDR & OLED_WRITE_MODE), tmp, buflen + 1, false);

    delete tmp;
}


void Display::oled_send_buf(uint buf[], int buflen) {
    // in horizontal addressing mode, the column address pointer auto-increments
    // and then wraps around to the next page, so we can send the entire frame
    // buffer in one gooooooo!

    // copy our frame buffer into a new buffer because we need to add the control byte
    // to the beginning

    // TODO find a more memory-efficient way to do this..
    // maybe break the data transfer into pages?
    uint8_t* tmp = new uint8_t;

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


void Display::render(uint *buf) {
    // update a portion of the display with a render area
    oled_send_cmd(OLED_SET_COL_ADDR);
    oled_send_cmd(frame_area.start_col);
    oled_send_cmd(frame_area.end_col);

    oled_send_cmd(OLED_SET_PAGE_ADDR);
    oled_send_cmd(frame_area.start_page);
    oled_send_cmd(frame_area.end_page);

    oled_send_buf(buf, frame_area.buflen);
}


void Display::render()
{
    CalculateReducedDisplayBuffer();
    // update a portion of the display with a render area
    oled_send_cmd(OLED_SET_COL_ADDR);
    oled_send_cmd(frame_area.start_col);
    oled_send_cmd(frame_area.end_col);

    oled_send_cmd(OLED_SET_PAGE_ADDR);
    oled_send_cmd(frame_area.start_page);
    oled_send_cmd(frame_area.end_page);

    oled_send_buf(ReducedDisplayBuffer, frame_area.buflen);
}