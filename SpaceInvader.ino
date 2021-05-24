/* **********************************************************
/*                      Libraries                           *
/* ******************************************************** */
#include <U8g2lib.h>
#include "sprites.h"
#include "gameplay_variables.h"

#define USE_EEPROM
#ifdef USE_EEPROM
#include <EEPROM.h>
#endif

#define U8G2_CONSTRUCTOR U8G2_SH1106_128X64_NONAME_F_HW_I2C

#define DISP_WIDTH 128
#define DISP_HEIGHT 64

#define FIRE_BUTTON 2
#define LEFT_BUTTON 3
#define RIGHT_BUTTON 4

/* **********************************************************
/*                      Global Classes                      *
/*                     Game structures                      *
/* ******************************************************** */
/* **********************************************************
/*                    Global Variables                      *
/* ******************************************************** */
//                    Aliens & display
//          The array of aliens across the display
U8G2_CONSTRUCTOR display(U8G2_R0, U8X8_PIN_NONE, SCL, SDA);

// widths of aliens
// as aliens are the same type per row we do not need to store their graphic width per alien in the structure above
// that would take a uint8_t per alien rather than just three entries here, 1 per row, saving significant memory
// uint8_t AlienWidth[] = {8, 11, 12}; // top, middle ,bottom widths

// game variables
uint32_t HiScore;
uint8_t FONT_Ascent;
uint8_t FONT_Descent;

/* **********************************************************
/*                       Void Setup                         *
/* ******************************************************** */

void CenterText(const char *Text, uint8_t RowValue)
{
	// center text on display
	display.setCursor(int((DISP_WIDTH - display.getStrWidth(Text)) / 2.0), RowValue);
	display.print(Text);
}

/* **********************************************************
/*                      Void Loop                           *
/* ******************************************************** */

void AttractScreen()
{
	uint8_t RowHeight;
	uint8_t ColPosition;

	ColPosition = display.getStrWidth("8");

	if (HiScore < 10)
	{
		ColPosition = int((DISP_WIDTH - display.getStrWidth("Hi Score ") - ColPosition) / 2.0);
	}
	else if (HiScore < 100)
	{
		ColPosition = int((DISP_WIDTH - display.getStrWidth("Hi Score ") - ColPosition * 2) / 2.0);
	}
	else if (HiScore < 1000)
	{
		ColPosition = int((DISP_WIDTH - display.getStrWidth("Hi Score ") - ColPosition * 3) / 2.0);
	}
	else if (HiScore < 10000)
	{
		ColPosition = int((DISP_WIDTH - display.getStrWidth("Hi Score ") - ColPosition * 4) / 2.0);
	}
	else
	{
		ColPosition = int((DISP_WIDTH - display.getStrWidth("Hi Score ") - ColPosition * 5) / 2.0);
		//65,535 max
	}

	display.clearBuffer();
	RowHeight = FONT_Ascent + (DISP_HEIGHT - 4 * (FONT_Ascent + FONT_Descent + 1)) / 2;
	CenterText("Play", RowHeight);
	//--> Next lines are for debugging <--
	//display.print(" ");display.print(FONT_Ascent); display.print(" ");display.print(FONT_Descent);
	//display.print(" ");display.print(ColPosition);
	RowHeight = RowHeight + FONT_Ascent + FONT_Descent + 1;
	CenterText("Space Invaders", RowHeight);
	RowHeight = RowHeight + FONT_Ascent + FONT_Descent + 1;
	CenterText("Press Fire to start", RowHeight);
	RowHeight = RowHeight + FONT_Ascent + FONT_Descent + 1;
	// Special Center Text ---->
	display.setCursor(ColPosition, RowHeight);
	display.print("Hi Score ");
	display.print(HiScore);
	//display.display();

	if (digitalRead(LEFT_BUTTON) && digitalRead(RIGHT_BUTTON))
	{
		HiScore = 0;
		EEPROM.put(0, HiScore);
	}

	display.sendBuffer();
}
void setup()
{
	//OLED Diplay
	/* U8g2 Project: SSD1306 or SH1106 OLED SPI Board */
	display.begin();

	display.setBitmapMode(1);

	// Buttons init
	pinMode(FIRE_BUTTON, OUTPUT);
	pinMode(LEFT_BUTTON, OUTPUT);
	pinMode(RIGHT_BUTTON, OUTPUT);
	// Buttons in pullup mode
	digitalWrite(FIRE_BUTTON, HIGH);
	digitalWrite(LEFT_BUTTON, HIGH);
	digitalWrite(RIGHT_BUTTON, HIGH);

	// create_button(&fire_button, 2, true);
	// create_button(&left_button, 3, true);
	// create_button(&right_button, 4, true);

	display.setFont(u8g2_font_t0_11b_tf); //font size is ok - it is loaded in PROGMEM
	FONT_Ascent = display.getAscent();	  //getAscent returns the number of pixels above the baseline
	FONT_Descent = -display.getDescent(); //getDescent returns a negative value, a number of pixels below the baseline
	display.setDrawColor(1);			  //set the color

#ifdef USE_EEPROM
	EEPROM.get(0, HiScore);
#else
	HiScore = 0;
#endif
}
void loop()
{
	AttractScreen();
}