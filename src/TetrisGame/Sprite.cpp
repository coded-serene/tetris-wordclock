#include <Arduino.h>
#include "Sprite.h"



Sprite::Sprite()
{
  this->SetPosition(0,0);
}
int Sprite::GetCurrentLine(void)
{
  return this->m_currentLine;
}
void Sprite::SetPosition(int line, int col)
{
  this->m_currentLine=line;
  this->m_currentCol=col;
}
void Sprite::Draw(Screen &scr)
{
  this->DrawAt(scr,this->m_currentLine, this->m_currentCol);
}

bool Sprite::TrySetNewPosition(Screen &scr, int line2place, int col2Place)
{
  if(this->CheckOverlapAt(scr,line2place,col2Place))
  {
    Serial.println("Sprite::TrySetNewPosition stopped due to Overlap.");
    return false;
  }
  if(this->CheckPixelEscapesScreen(scr,line2place,col2Place))
  {
    Serial.println("Sprite::TrySetNewPosition stopped due to ScreenEscape.");
    return false;
  }
  return true;
}
bool Sprite::MoveDown(Screen &scr)
{
  int newLinePos = this->m_currentLine+1;
  if(TrySetNewPosition(scr,newLinePos,this->m_currentCol))
  {
    m_currentLine = newLinePos;
    Serial.print("Sprite::MoveDown NewPos: "); Serial.println(m_currentLine);
    return true;
  }
  Serial.println("Sprite::MoveDown failed");
  return false;
}
bool Sprite::MoveRight(Screen &scr)
{
  int newColPos = this->m_currentCol+1;
  if(TrySetNewPosition(scr,this->m_currentLine,newColPos))
  {
    m_currentCol = newColPos;
    Serial.print("Sprite::MoveRight NewPos: "); Serial.println(m_currentCol);
    return true;
  }
  Serial.println("Sprite::MoveRight failed");
  return false;
}
bool Sprite::MoveLeft(Screen &scr)
{
  int newColPos = this->m_currentCol-1;
  if(TrySetNewPosition(scr,this->m_currentLine,newColPos))
  {
    m_currentCol = newColPos;
    Serial.print("Sprite::MoveLeft NewPos: "); Serial.println(m_currentCol);
    return true;
  }
  Serial.println("Sprite::MoveLeft failed");
  return false;
}

int Sprite::GetLowestSpritePixelLine(void)
{
  int lowestLine = -1;
  for(int line=0;line<Sprite::GetRectHeight();line++)
  {
    for(int col=0;col<Sprite::GetRectWidth();col++)
    {
      if(this->GetSpritePixel(line,col) != pixelColor::BLACK)
      {
        if(line>lowestLine)
        {
          lowestLine=line;
        }
      }
    }
  }
  return lowestLine;
}


bool Sprite::FlipLeft(Screen &scr)
{
  Sprite dummy;
  
  dummy.LoadFromSpriteLeftFlipped(*this);
  int dummyLowest = dummy.GetLowestSpritePixelLine();
  int thisLowest = this->GetLowestSpritePixelLine();

  int line2Test = this->m_currentLine;
  if(dummyLowest>thisLowest)
  {
    line2Test -= (dummyLowest-thisLowest);
  }
  if(dummy.TrySetNewPosition(scr,line2Test,this->m_currentCol))
  {
    this->LoadFromSprite(dummy);
    this->m_currentLine=line2Test;
    Serial.print("Sprite::FlipLeft Succeeded: "); Serial.println(m_currentCol);
    return true;
  }
  Serial.println("Sprite::FlipLeft failed");
  return false;
}
void Sprite::Clear(void)
{
  Serial.println("Sprite::Clear");
  for(int line=0;line<Sprite::GetRectHeight();line++)
  {
    for(int col=0;col<Sprite::GetRectWidth();col++)
    {
      m_SpriteBitmap[line][col] = pixelColor::BLACK;
    }
  }
}
// Prüft, ob ein sichtbares Pixel des Sprites den Bildschirmrand verlassen an der angegebenen Position verlassen würde
bool Sprite::CheckPixelEscapesScreen(Screen &scr,int line2place, int col2Place)
{
  for(int line=0;line<Sprite::GetRectHeight();line++)
  {
    for(int col=0;col<Sprite::GetRectWidth();col++)
    {
      pixelColor spriteColor = this->GetSpritePixel(line,col);
      if(spriteColor != pixelColor::BLACK)
      {
        int realScreenLine = line2place+line;
        int realScreenCol = col2Place+col;
        if(realScreenCol<0)
        {
          Serial.print("Sprite left min pos reached at spritepos: "); Serial.println(col2Place);
          return true;
        }
        if(realScreenCol>=Screen::GetScreenWidth())
        {
          Serial.print("Sprite right max pos reached at spritepos: "); Serial.println(col2Place);
          return true;
        }
        if(realScreenLine<0)
        {
          Serial.print("Sprite top min pos reached at spritepos: "); Serial.println(line2place);
          return true;
        }
        if(realScreenLine>=Screen::GetScreenHeight())
        {
          Serial.print("Sprite bottom max pos reached at spritepos: "); Serial.println(line2place);
          return true;
        }
      }
    }
  }
  return false; 
}
   
bool Sprite::CheckOverlapAt(Screen &scr,int line2place, int col2Place)
{
  for(int line=0;line<Sprite::GetRectHeight();line++)
  {
    for(int col=0;col<Sprite::GetRectWidth();col++)
    {
      pixelColor spriteColor = this->GetSpritePixel(line,col);
      if(spriteColor != pixelColor::BLACK)
      {
        // im Sprite in an dieser Stelle ein Pixel
        pixelColor screenColor = scr.GetPixel(line2place+line,col2Place+col);
        if(screenColor != pixelColor::BLACK)
        {
          // und im Canvas-Screen auch. -> Kollision!
          return true;
        }
      }
    }
  }
  return false;  
}
pixelColor Sprite::GetSpritePixel(int line, int col)
{
  if(line >= Sprite::GetRectHeight() || line < 0)
  {
    Serial.print("WARNING! GetSpritePixel exceeded line bounds: "); Serial.println(line);
    return pixelColor::BLACK;
  }
  if(col >= Sprite::GetRectWidth() || col < 0)
  {
    Serial.print("WARNING! GetSpritePixel exceeded col bounds: "); Serial.println(col);
    return pixelColor::BLACK;
  }

  return m_SpriteBitmap[line][col];
}
void Sprite::SetSpritePixel(int line, int col, pixelColor color)
{
  if(line >= Sprite::GetRectHeight() || line < 0)
  {
    Serial.print("WARNING! SetSpritePixel exceeded line bounds: "); Serial.println(line);
    return;
  }
  if(col >= Sprite::GetRectWidth() || col < 0)
  {
    Serial.print("WARNING! SetSpritePixel exceeded col bounds: "); Serial.println(col);
    return;
  }

  m_SpriteBitmap[line][col] = color;
}
void Sprite::DrawAt(Screen &scr,int line2place, int col2Place)
{
    //Serial.println("Placing piece.");
    for(int line=0; line<SPRITEDIMENSION ; line++)
    {
        for(int col=0; col<SPRITEDIMENSION;col++)
        {
            pixelColor pxc = GetSpritePixel(line,col);
            if(pxc != pixelColor::BLACK)
            {
              scr.SetPixel(line+line2place,col+col2Place,pxc);
            }
        }
    }
}
void  Sprite::LoadFromSpriteLeftFlipped(Sprite &sourceSprite )
{
  for(int line=0; line<SPRITEDIMENSION ; line++)
    {
        for(int col=0; col<SPRITEDIMENSION;col++)
        {
            pixelColor pxc = sourceSprite.GetSpritePixel(line,col);
            this->SetSpritePixel(SPRITEDIMENSION-1-col,line,pxc);
        }
    }
}
void  Sprite::LoadFromSprite(Sprite &sourceSprite )
{
  for(int line=0; line<SPRITEDIMENSION ; line++)
    {
        for(int col=0; col<SPRITEDIMENSION;col++)
        {
            pixelColor pxc = sourceSprite.GetSpritePixel(line,col);
            this->SetSpritePixel(line,col,pxc);
        }
    }
}
void Sprite::LoadFromPiece(t_piece piece2CreateFrom )
{
  Serial.println("Sprite::LoadFromPiece");
    for(int piecePixel=0; piecePixel<SPRITEDIMENSION*SPRITEDIMENSION ; piecePixel++)
    {
        pixelColor color = pixelColor::BLACK;
        if(piece2CreateFrom.bitmap[piecePixel]=='x')
        {
            color=piece2CreateFrom.pieceColor;
        }
        int line=piecePixel/SPRITEDIMENSION;
        int col=piecePixel%SPRITEDIMENSION;
        m_SpriteBitmap[line][col] = color;
    }
}
/*static*/ int Sprite::GetRectWidth(void)
{
  return SPRITEDIMENSION;
}
/*static*/ int Sprite::GetRectHeight(void)
{
  return SPRITEDIMENSION;
}
Sprite::~Sprite()
{
}



