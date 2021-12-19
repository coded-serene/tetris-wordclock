

#ifndef __Sprite_H__
#define __Sprite_H__ 

#include "TetrisTypes.h"
#include "Screen.h"
/*  
 ~~~~~~~~~~~~~~~~
 Wordclock-Project
 ~~~~~~~~~~~~~~~~
© SERENE - Rene Lang 2021 (http://withstupid.net)
 
*/


#define SPRITEDIMENSION 4

class Sprite
{
private:

   typedef pixelColor t_Sprite[SPRITEDIMENSION][SPRITEDIMENSION];
   t_Sprite m_SpriteBitmap;
   int m_currentLine;
   int m_currentCol;

   // Prüft, ob ein sichtbares Pixel des Sprites den Bildschirmrand verlassen an der angegebenen Position verlassen würde
   bool CheckPixelEscapesScreen(Screen &scr,int line2place, int col2Place); 
   bool TrySetNewPosition(Screen &scr, int line2place, int col2Place);
public:
   void SetPosition(int line, int col);
   int GetCurrentLine(void);

   void LoadFromPiece(t_piece piece2CreateFrom );
   void LoadFromSprite(Sprite &sourceSprite );
   void LoadFromSpriteLeftFlipped(Sprite &sourceSprite );

   void DrawAt(Screen &scr,int line2place, int col2Place);
   void Draw(Screen &scr);
   void Clear(void);
   bool CheckOverlapAt(Screen &scr,int line2place, int col2Place);

   bool MoveDown(Screen &scr);
   bool MoveRight(Screen &scr);
   bool MoveLeft(Screen &scr);
   bool FlipLeft(Screen &scr);

   int GetLowestSpritePixelLine(void);

   
   static int GetRectWidth(void);
   static int GetRectHeight(void);

   void SetSpritePixel(int line, int col, pixelColor color);
   pixelColor GetSpritePixel(int line, int col);
   Sprite();
   ~Sprite();
   
};

#endif 
