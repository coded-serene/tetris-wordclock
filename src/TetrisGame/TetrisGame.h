

#ifndef __TetrisGame_H__
#define __TetrisGame_H__ 

#include "TetrisTypes.h"
#include "Screen.h"
#include "Sprite.h"
/*  
 ~~~~~~~~~~~~~~~~
 Wordclock-Project
 ~~~~~~~~~~~~~~~~
© SERENE - Rene Lang 2021 (http://withstupid.net)
 
*/



typedef enum userAction {None, MoveLeft, MoveRight, MoveDown, Fire} userAction;


typedef enum gameState {DropDown, CheckLinesComplete, CreateNewSprite, GameOver} gameState;
class TetrisGame
{
private:
   int nextGameStepMs;
   
   Screen _display;
   Screen _canvas;
   Sprite _sprite;

   gameState m_gameState;
   bool CheckLinesComplete(void);
   bool GameOver(CRGB ledStripe[TOTAL_LED_COUNT]);
   bool CreateNewSprite(void);
   bool DropDown(void);
   
   void StartGame(void);
public:
   void InitTetris(void);
   bool MaintainTetris(CRGB ledStripe[TOTAL_LED_COUNT]);

   bool OnUserAction(CRGB ledStripe[TOTAL_LED_COUNT],userAction action);
   TetrisGame();
   ~TetrisGame();
   
};

#endif 
