/*
The MIT License (MIT)

Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Lancaster University by arrangement with the BBC.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "MicroBit.h"

#define SNAKE_EMPTY 0
#define SNAKE_UP    1
#define SNAKE_LEFT  2
#define SNAKE_RIGHT 3
#define SNAKE_DOWN  4

#define SNAKE_FRAME_DELAY   350
#define GROWTH_SPEED        3

struct Point
{
    int     x;
    int     y;
};

MicroBit        uBit;
Point           head;                 // Location of the head of our snake.
Point           tail;                 // Location of the tail of our snake.
Point           food;                 // Location of food.
MicroBitImage   map(5,5);  

void place_food()
{
    int r = uBit.random(24);
    int x = 0; int y = 0;
    
    while (r > 0)
    {
        x = (x+1) % 5;
        if (x == 0)
            y = (y+1) % 5;
            
        if(map.getPixelValue(x,y) == SNAKE_EMPTY)
            r--;
    }
    
    food.x = x;
    food.y = y;
}

void snake()
{   
    Point newHead;              // Calculated placement of new head position based on user input.    
    int hdirection;             // Head's direction of travel
    int tdirection;             // Tail's direction of travel
    int snakeLength;            // number of segments in the snake.
    int growing;                // boolean state indicating if we've just eaten some food.
    int score;
    
    // Start in the middle of the screen.
    tail.x = tail.y = 2;    
    head.x = head.y = 2;
    snakeLength = 1;
    growing = 0;
    score = 0;
    map.clear();
        
    uBit.display.image.setPixelValue(head.x, head.y, 255);
        
    // Add some random food.    
    place_food();
        
    while (1)
    {    
        // Flash the food is necessary;       
        uBit.display.image.setPixelValue(food.x, food.y, uBit.systemTime() % 1000 < 500 ? 0 : 255);
          
        int dx = uBit.accelerometer.getX();
        int dy = uBit.accelerometer.getY();
        
        newHead.x = head.x;
        newHead.y = head.y;
        
        if (abs(dx) > abs(dy))
        {
            if(dx < 0)
            {
                hdirection = SNAKE_LEFT;
                newHead.x = newHead.x == 0 ? 4 : newHead.x-1;
            }
            else
            {
                hdirection = SNAKE_RIGHT;
                newHead.x = newHead.x == 4 ? 0 : newHead.x+1;
            }            
        }
        else    
        {
            if(dy < 0)
            {
                hdirection = SNAKE_UP;
                newHead.y = newHead.y == 0 ? 4 : newHead.y-1;
            }
            else
            {
                hdirection = SNAKE_DOWN;
                newHead.y = newHead.y == 4 ? 0 : newHead.y+1;
            }
        }           
        
        int status = map.getPixelValue(newHead.x, newHead.y);
        if (status == SNAKE_UP || status == SNAKE_DOWN || status == SNAKE_LEFT || status == SNAKE_RIGHT)
        {
            uBit.display.scroll("GAME OVER! SCORE: ");
            uBit.display.scroll(score);
            
            return;            
        }
                                          
        // move the head.       
        map.setPixelValue(head.x, head.y, hdirection);
        uBit.display.image.setPixelValue(newHead.x, newHead.y, 255);

        if (growing == GROWTH_SPEED)
        {
            growing = 0;
            snakeLength++;
        }
        else
        {        
            // move the tail.
            tdirection = map.getPixelValue(tail.x,tail.y);     
            map.setPixelValue(tail.x, tail.y, SNAKE_EMPTY);         
            uBit.display.image.setPixelValue(tail.x, tail.y, 0);
    
            // Move our record of the tail's location.        
            if (snakeLength == 1)
            {
                tail.x = newHead.x;
                tail.y = newHead.y;
            }
            else
            {
                if (tdirection == SNAKE_UP)
                    tail.y = tail.y == 0 ? 4 : tail.y-1;
                
                if (tdirection == SNAKE_DOWN)
                    tail.y = tail.y == 4 ? 0 : tail.y+1;
            
                if (tdirection == SNAKE_LEFT)
                    tail.x = tail.x == 0 ? 4 : tail.x-1;
                
                if (tdirection == SNAKE_RIGHT)
                    tail.x = tail.x == 4 ? 0 : tail.x+1;
            }
        }

        // Update our record of the head location and away we go!
        head.x = newHead.x;
        head.y = newHead.y;
      
        // if we've eaten some food, replace the food and grow ourselves!
        if (head.x == food.x && head.y == food.y)
        {
            growing++;
            score++;
            place_food();
        }
      
        uBit.sleep(SNAKE_FRAME_DELAY);   
    }   
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    // Insert your code here!
    uBit.display.scroll("SNAKE v1.0");

    while(1)
        snake();
}

