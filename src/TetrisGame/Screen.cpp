#include <Arduino.h>
#include "Screen.h"



Screen::Screen()
{

}
void Screen::SetPixel(int line, int col, pixelColor color)
{
  if(line >= LINES || line < 0)
  {
    Serial.print("WARNING! SetPixel exceeded line bounds: "); Serial.println(line);
    return;
  }
  if(col >= ROWS || col < 0)
  {
    Serial.print("WARNING! SetPixel exceeded col bounds: "); Serial.println(col);
    return;
  }

  m_screenData[line][col]=color;
}
pixelColor Screen::GetPixel(int line, int col)
{
  if(line >= LINES || line < 0)
  {
    //Serial.print("WARNING! GetPixel exceeded line bounds: "); Serial.println(line);
    return pixelColor::BLACK;
  }
  if(col >= ROWS || col < 0)
  {
    Serial.print("WARNING! GetPixel exceeded col bounds: "); Serial.println(col);
    return pixelColor::BLACK;
  }

  return m_screenData[line][col];
}

void Screen::RemoveLine(int line2remove)
{
  if(line2remove >= LINES || line2remove < 0)
  {
    Serial.print("WARNING! RemoveLine exceeded bounds: "); Serial.println(line2remove);
    return;
  }

  for(int col=0; col<ROWS;col++)
  {
      this->SetPixel(line2remove,col, pixelColor::BLACK);
  }
}
void Screen::ScrollDown(int line2remove/*=9*/)
{
  Serial.println("--> ScrollDown");
  if(line2remove >= LINES || line2remove < 0)
  {
    Serial.print("WARNING! ScrollDown exceeded bounds: "); Serial.println(line2remove);
    return;
  }

  for(int currLine=line2remove; currLine>=0; currLine--)
  {
    this->CopyLine(currLine-1,currLine);
  }
  this->RemoveLine(0);
  //Serial.println("<-- ScrollDown");
  /*for(int line=line2remove+1; line<LINES ; line++)
  {
      for(int col=0; col<ROWS;col++)
      {
          pixelColor source = this->GetPixel(line+1,col);
          this->SetPixel(line,col,source);
      }
  }
  this->RemoveLine(line2remove);*/
}
void Screen::CopyLine(int sourceLine, int targetLine)
{
  for(int col=0; col<ROWS;col++)
  {
      pixelColor source = this->GetPixel(sourceLine,col);
      this->SetPixel(targetLine,col,source);
  }
}
bool Screen::IsEmpty(void)
{
  for(int line=0;line<(LINES);line++)
  {
      for(int col=0;col<(ROWS);col++)
      {
          pixelColor pc = this->GetPixel(line,col);
          if(pc != pixelColor::BLACK ) return false;
      }
  }
  return true;
}

void Screen::CopyFrom(Screen &sourceScreen)
{
    //Serial.println("-> Screen::CopyFrom");
    for(int i=0;i<LINES;i++)
    {
        for(int j=0;j<ROWS;j++)
        {
           pixelColor pix = sourceScreen.GetPixel(i,j);
           this->SetPixel(i,j,pix);
        }
    }
    //Serial.println("<- Screen::CopyFrom");
}
void Screen::Clear(void)
{
    Serial.println("-> ClearScreen");
    for(int i=0;i<LINES;i++)
    {
        for(int j=0;j<ROWS;j++)
        {
           SetPixel(i,j,pixelColor::BLACK);
        }
    }
    Serial.println("<- ClearScreen");
}

Screen::~Screen()
{
}

void Screen::DumpToConsole(void)
{
    
    Serial.println("-> DumpScreen");
    for(int i=0;i<LINES;i++)
    {
        Serial.print(i); Serial.print(":   ");
        //t_ScreenLine line = screen[i];
        for(int j=0;j<ROWS;j++)
        {
          Serial.print((int)this->GetPixel(i,j)); Serial.print(" ");
           //Serial.print(screen[i][j]); Serial.print(" ");
           // screen[i][j].r=255;
           // screen[i][j].g=255;
           // screen[i][j].b=255;
        }
        Serial.println("");
    }
    Serial.println("<- DumpScreen");
    
}
int Screen::GetFirstFilledLine(void)
{
  for(int line=Screen::GetScreenHeight()-1;line >=0; line--)
  {
      bool allPixelSet=true;
      for(int col=0; col<Screen::GetScreenWidth();col++)
      {
          if(this->GetPixel(line,col) == pixelColor::BLACK)
          {
              allPixelSet=false;
              break;
          }
      }
      if(allPixelSet)
      {
          return line;
      }
  }  
  return -1; // keine Zeile gefunden, wo alles gesetzt ist.
}
int Screen::GetFirstColoredLine(pixelColor color2find)
{
  for(int line=Screen::GetScreenHeight()-1;line >=0; line--)
  {
      bool allPixelSet=true;
      for(int col=0; col<Screen::GetScreenWidth();col++)
      {
          if(this->GetPixel(line,col) != color2find)
          {
              allPixelSet=false;
              break;
          }
      }
      if(allPixelSet)
      {
          return line;
      }
  }  
  return -1; // keine Zeile gefunden, wo alles gesetzt ist.
}
void Screen::FillLineWithColor(int line, pixelColor color)
{
  for(int col=0; col<Screen::GetScreenWidth();col++)
  {
    this->SetPixel(line,col,color);
  }
}
void Screen::DumpToLedStripe(CRGB leds[TOTAL_LED_COUNT])
{
    //Serial.println("-> ScreenToLedStripe");
    int ledid=0;
    bool evenLine=true;
    for(int i=0;i<LINES;i++)
    {
        for(int j=0;j<ROWS;j++)
        {
            int ledid2Adress = i*ROWS;
            if(evenLine==false)
            {
                ledid2Adress+=ROWS-j-1;
            }
            else
            {
                ledid2Adress+=j;
            }

            pixelColor col = this->GetPixel(i,j); // screen[i][j];
            int r=255;
            int g=0;
            int b=0;
            switch(col)
            {
              case pixelColor::BLACK: r=0;g=0;b=0; 
                                      break;
              case pixelColor::WHITE: r=255;g=255;b=255; 
                                      break;
              case pixelColor::RED:   r=255;g=0;b=0; 
                                      break;
              case pixelColor::GREEN: r=0;g=255;b=0; 
                                      break;
              case pixelColor::ORANGE:r=255;g=204;b=51; 
                                      break;
              case pixelColor::BLUE:  r=0;g=0;b=255; 
                                      break; 
              case pixelColor::TURKY: r=120;g=51;b=205; 
                                      break; 
              default: r=255;g=255;b=255; 
                      break;                        
            }
           
            
            leds[ledid2Adress].r=r;
            leds[ledid2Adress].g=g;
            leds[ledid2Adress].b=b;
            ledid++;
        }
        evenLine = !evenLine;
    }
    //Serial.println("<- ScreenToLedStripe");
}

/*static*/ int Screen::GetScreenWidth(void)
{
  return ROWS;
}
/*static*/ int Screen::GetScreenHeight(void)
{
  return LINES;
}


