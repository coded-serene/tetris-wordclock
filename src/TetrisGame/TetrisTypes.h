

#ifndef __TetrisTypes_H__
#define __TetrisTypes_H__ 
/*  
 ~~~~~~~~~~~~~~~~
 Wordclock-Project
 ~~~~~~~~~~~~~~~~
© SERENE - Rene Lang 2021 (http://withstupid.net)
 
*/

#define ROWS  12 /*myWC12x12 width*/
#define LINES 12 /*myWC12x12 height*/    // unten: line=0
#define TOTAL_LED_COUNT (ROWS*LINES)


typedef enum _pixelColor {BLACK,WHITE,RED,GREEN,ORANGE,BLUE,TURKY} pixelColor;
typedef  pixelColor t_Screen[LINES][ROWS] ; // Ein gesamter Bildschirm



typedef struct t_piece {
        char bitmap[4*4+1];
        pixelColor pieceColor;
        } t_piece;



#endif 
