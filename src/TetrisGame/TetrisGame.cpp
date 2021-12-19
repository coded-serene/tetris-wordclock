
#include "FastLED.h"
#include "Arduino.h"
#include "TetrisTypes.h"

#include "TetrisGame.h"

t_piece p0 = 
    { ".x.."
      "xxx."
      "...."
      "....",pixelColor::ORANGE };

t_piece p1 = 
    { "xxx."
      "x..."
      "...."
      "....",pixelColor::BLUE };

t_piece p2 = 
    { "x..."
      "xxx."
      "...."
      "....",pixelColor::GREEN };

t_piece p3 = 
    { ".xx."
      ".xx."
      "...."
      "....",pixelColor::RED };

t_piece p4 = 
    { "xxx."
      "...."
      "...."
      "....",pixelColor::TURKY };
#define ALLPIECES   5
t_piece availablePieces[ALLPIECES];

void TetrisGame::InitTetris(void)
{
    nextGameStepMs=0;

    randomSeed(millis());
    _canvas.Clear();
    _display.Clear();
    availablePieces[0] = p0;
    availablePieces[1] = p1;
    availablePieces[2] = p2;
    availablePieces[3] = p3;
    availablePieces[4] = p4;

    StartGame();
}

bool TetrisGame::CheckLinesComplete(void)
{
    Serial.println("** CheckLinesComplete");

    // haben wir bereits vollkommen aufgeblitze Zeilen? --> löschen
    int highlightedLine = _canvas.GetFirstColoredLine(pixelColor::WHITE);
    if(highlightedLine != -1)
    {
        _canvas.ScrollDown(highlightedLine);
        _display.CopyFrom(_canvas);
    }
    
    nextGameStepMs=millis()+200;

    int filledLine = _canvas.GetFirstFilledLine();
    if(filledLine == -1)
    {
        // es gibt keine weitere gefüllte Zeile -> Fertig.
        m_gameState=gameState::CreateNewSprite;
    }
    else
    {
        // Zeile erleuchten
        _canvas.FillLineWithColor(filledLine,pixelColor::WHITE);
        nextGameStepMs=millis()+400;
    }
    _display.CopyFrom(_canvas);
    
    return true;
}
void TetrisGame::StartGame(void)
{
    _canvas.Clear();
    int lastLine = Screen::GetScreenHeight()-1;
    int lastCol = Screen::GetScreenHeight();
    _canvas.SetPixel(lastLine,0,pixelColor::BLUE);
    _canvas.SetPixel(lastLine,lastCol,pixelColor::BLUE);
    _display.CopyFrom(_canvas);
    //_display.DumpToLedStripe(ledStripe);

    nextGameStepMs=millis()+1000;
    m_gameState=gameState::CreateNewSprite;
    FastLED.show();
}
int gameOverAnimation=0;
bool TetrisGame::GameOver(CRGB ledStripe[TOTAL_LED_COUNT])
{
    Serial.println("** GameOver");
    if(gameOverAnimation==0)
    {
        if(!_display.IsEmpty() )
        {
            _display.ScrollDown();
            _display.DumpToLedStripe(ledStripe);
            FastLED.show();
            //_display.DumpToConsole();
            nextGameStepMs=millis()+50;
            return true;
        }
        gameOverAnimation=1;
    }
    
    if(gameOverAnimation==1)
    {
        for(int i=0;i<Screen::GetScreenHeight()-1;i++)
        {
            if(_display.GetPixel(i,0) == pixelColor::BLACK)
            {
                _display.FillLineWithColor(i,pixelColor::RED);
                _display.DumpToLedStripe(ledStripe);
                FastLED.show();
                nextGameStepMs=millis()+50;
                return true;
            }
        }
        gameOverAnimation=2;
    }
    if(gameOverAnimation==2)
    {
        if(!_display.IsEmpty() )
        {
            
            _display.ScrollDown();
            _display.DumpToLedStripe(ledStripe);
            FastLED.show();
            //_display.DumpToConsole();
            nextGameStepMs=millis()+50;
            return true;
        }
        else
        {
            gameOverAnimation=3;
        }
    }

    Serial.println("--- RESTART GAME ----");
    StartGame();
    return true;
}
bool TetrisGame::CreateNewSprite(void)
{
    Serial.println("** CreateNewSprite");
    _display.CopyFrom(_canvas);
    _sprite.Draw(_display);
    int piece2LoadId=random(0,ALLPIECES);
    t_piece piece2Load = availablePieces[piece2LoadId];

    Serial.println("PlaceSprite");
    _sprite.LoadFromPiece(piece2Load);
    int col = random(0, ROWS-1-Sprite::GetRectWidth());
    _sprite.SetPosition(0,col);

    m_gameState=gameState::DropDown;
    return true;
}
bool TetrisGame::DropDown(void)
{
    Serial.println("** DropDown");
    _display.CopyFrom(_canvas);
    _sprite.Draw(_display);
    bool moved = _sprite.MoveDown(_canvas);
    nextGameStepMs=millis()+1000;
    if(! moved)
    {
        _sprite.Draw(_canvas);

        if(_sprite.GetCurrentLine() == 0)
        {
            gameOverAnimation=0;
            m_gameState=gameState::GameOver;
        }
        else
        {
            _sprite.Clear();
            Serial.println("---------- CheckScroll ------------");
            nextGameStepMs=millis()+10;
            m_gameState=gameState::CheckLinesComplete;
        }
    }
    return true;
}
bool TetrisGame::MaintainTetris(CRGB ledStripe[TOTAL_LED_COUNT])
{
    bool ret = false;
    
    if(millis() < nextGameStepMs)
    {
        return false;
    }

    switch(this->m_gameState)
    {
        case gameState::CheckLinesComplete: 
                                ret = CheckLinesComplete();
                                break;
        case gameState::GameOver: 
                                ret = GameOver(ledStripe);
                                break;
        case gameState::CreateNewSprite:
                                ret = CreateNewSprite();
                                break;
        case gameState::DropDown:
                                ret = DropDown();
                                break;
    }

    if(ret)    
    {
        //_canvas.DumpToConsole();
        _display.DumpToLedStripe(ledStripe);        
        FastLED.show();
    }
    return ret;
}

TetrisGame::TetrisGame()
{

}
TetrisGame::~TetrisGame()
{

}

bool TetrisGame::OnUserAction(CRGB ledStripe[TOTAL_LED_COUNT],userAction action)
{
    bool moved = false;
    switch(action)
    {
        case userAction::MoveLeft:  moved = _sprite.MoveLeft(_canvas);
                                    break;
        case userAction::MoveRight: moved = _sprite.MoveRight(_canvas);
                                    break;
        case userAction::Fire:      moved = _sprite.FlipLeft(_canvas);
                                    break;
        case userAction::MoveDown:  moved = _sprite.MoveDown(_canvas);
                                    break;
        default:
                                    break;
    }
    if(moved)
    {
        // force update
        _display.CopyFrom(_canvas);
        _sprite.Draw(_display);
        _display.DumpToLedStripe(ledStripe);
        FastLED.show();
    }
    return moved;
}
