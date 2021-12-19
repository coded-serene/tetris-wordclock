

#ifndef __Screen_H__
#define __Screen_H__ 

#include "TetrisTypes.h"
#include "FastLED.h"
/*  
 ~~~~~~~~~~~~~~~~
 Wordclock-Project
 ~~~~~~~~~~~~~~~~
© SERENE - Rene Lang 2021 (http://withstupid.net)
 
*/


#define SPRITEDIMENSION 4

class Screen
{
private:
   typedef  pixelColor t_PixelScreen[LINES][ROWS] ; // Ein gesamter Bildschirm
   t_PixelScreen m_screenData;
   
public:
   void SetPixel(int line, int col, pixelColor color);
   pixelColor GetPixel(int line, int col);
   void RemoveLine(int line2remove);
   void CopyLine(int sourceLine, int targetLine);
   void Clear(void);
   bool IsEmpty(void);
   void ScrollDown(int line2remove=LINES-1);
   int GetFirstFilledLine(void);
   int GetFirstColoredLine(pixelColor color2find);
   void FillLineWithColor(int line, pixelColor color);
   void CopyFrom(Screen &sourceScreen);

   void DumpToConsole(void);
   void DumpToLedStripe(CRGB leds[TOTAL_LED_COUNT]);

   static int GetScreenWidth(void);
   static int GetScreenHeight(void);
   Screen();
   ~Screen();
   
};

#endif 
