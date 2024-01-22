#pragma once
#include "Utility.h"

#define IIC 1
#define SPI 2

// commands (see datasheet)
#define OLED_SET_CONTRAST _u(0x81)
#define OLED_SET_ENTIRE_ON _u(0xA4)
#define OLED_SET_NORM_INV _u(0xA6)
#define OLED_SET_DISP _u(0xAE)
#define OLED_SET_MEM_ADDR _u(0x20)
#define OLED_SET_COL_ADDR _u(0x21)
#define OLED_SET_PAGE_ADDR _u(0x22)
#define OLED_SET_DISP_START_LINE _u(0x40)
#define OLED_SET_SEG_REMAP _u(0xA0)
#define OLED_SET_MUX_RATIO _u(0xA8)
#define OLED_SET_COM_OUT_DIR _u(0xC0)
#define OLED_SET_DISP_OFFSET _u(0xD3)
#define OLED_SET_COM_PIN_CFG _u(0xDA)
#define OLED_SET_DISP_CLK_DIV _u(0xD5)
#define OLED_SET_PRECHARGE _u(0xD9)
#define OLED_SET_VCOM_DESEL _u(0xDB)
#define OLED_SET_CHARGE_PUMP _u(0x8D)
#define OLED_SET_HORIZ_SCROLL _u(0x26)
#define OLED_SET_SCROLL _u(0x2E)

#define OLED_ADDR _u(0x3C)
#define OLED_HEIGHT _u(32)
#define OLED_WIDTH _u(128)
#define OLED_PAGE_HEIGHT _u(8)
#define OLED_NUM_PAGES OLED_HEIGHT / OLED_PAGE_HEIGHT
#define OLED_BUF_LEN (OLED_NUM_PAGES * OLED_WIDTH)

#define OLED_WRITE_MODE _u(0xFE)
#define OLED_READ_MODE _u(0xFF)

//  0x00 0
//  0x3C 111100
//  0x80 10000000
//  0xFE 11111110
//  0x40 1000000
//  0xAE 10101110

struct render_area {
    uint start_col;
    uint end_col;
    uint start_page;
    uint end_page;

    int buflen;
};


class Display
{
private:
	DisplaySize Screen;
	uint* DisplayBuffer;
	uint* ReducedDisplayBuffer;
	uint Address;	//SSD1306 -> 0x7B
	uint CommunicationProtocol;
	static uint DisplayCount;

	uint SDA_Pin;
	uint SCL_Pin;

	render_area frame_area;
	uint CursorCol;
	uint CursorRow;
	

public:
	Display();
	Display(uint Width, uint Height, uint Address, uint SDA_Pin, uint SCL_Pin);
	Display(uint Width, uint Height, uint Address, uint CommunicationProtocol);
	~Display();

	static uint GetDisplayCount();
	static uint IncremetDisplayCount();
	static uint DecrementDisplayCount();
	
	uint GetAddress();
	uint SetAddress(uint Addr);

	uint GetDisplaySize();
	uint SetDisplaySize(uint Width, uint Height);

	uint GetDisplayWidth();
	uint GetDisplayHeight();

	uint* GetDisplayBuffer();
	uint* GetReducedDisplayBuffer();

	uint* CalculateReducedDisplayBuffer();

	render_area GetFrameArea();
	uint SetFrameArea(render_area Frame);
	uint SetFrameArea(uint start_col, uint end_col, uint start_page, uint end_page, uint buflen );

	uint PrintDisplay(bool Binary = 0);
	uint PrintReducedDisplay(uint* ReducedDisplayBuffer, bool Binary = 0);

	uint ZeroDisplay();
	uint ClearArea(uint StartX, uint StartY, uint EndX, uint EndY);
	uint RandomPixelTest(uint Times, uint ArraySize, uint SleepDuration, render_area& Area);

	uint SwitchPixelState(uint C_Width, uint C_Height, bool Mode = 0);
	uint SetPixelState(uint C_Width, uint C_Height, int Number);
	uint InverseDisplayBuffer(); // Not working

	//uint ClearAreas(uint TopLeftX, uint TopLeftY, uint BotRightX, uint BotRightY);
	uint ArrayToBitNumberBE(uint* Arr);
	uint ArrayToBitNumberLE(uint* Arr);
	uint* NumberToBitArray(uint Number, uint NumberSize);
	uint* ReorganizeArray(uint* Arr, uint ArrWidth, uint ArrHeight);

	uint OnBootUp();

	uint* AddTwoArrays(uint* Arr, uint StartCollumn, uint StartRow, uint ArrWidth, uint ArrHeight);

	uint* AddLetter(uint* Arr, uint StartCollumn, uint StartRow, uint ArrWidth, uint ArrHeight);

	uint AddTextToBuffer(std::string Text, uint PosX, uint PosY);

	void tests(uint buf[]);

	void fill(uint buf[], uint fill);

	void calc_render_area_buflen(struct render_area *area);

	void oled_send_cmd(uint cmd);

	void oled_send_buf(uint8_t buf[], int buflen);

	void oled_send_buf(uint buf[], int buflen);

	void oled_init();

	void render(uint *buf, struct render_area *area);
	void render(uint *buf);
	void render();
};