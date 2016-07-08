/*
The MIT License (MIT)

Copyright (c) 2016 Lancaster University.

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


//
//
// A simple game of Space Invaders for the BBC micro:bit, using the
// accelerometer and buttons to control the player's ship.
//
// As well as illustrating the use of sensors, the display and events,
// this demonstration also provides a highly elegant example of
// how fibers can be used to create safe and elegant programs... Note the
// use of event handlers when the player fires, and the indepenent fibers
// used to control the players ship, aliens, bullets and screen refresh...
//
//

#include "MicroBit.h"

#define GAME_ON         0
#define GAME_OVER       1

struct Point
{
    int     x;
    int     y;
};

MicroBit        uBit;
MicroBitImage   invaders(5,5);
int             score;
int             game_over;
int             level;
int             INVADER_SPEED = 750;
int             PLAYER_SPEED = 150;
int             BULLET_SPEED = 50;
Point           player;
Point           bullet;

/**
 * Add a new row of space invaders to the game.
 */
int
addRow()
{
    // If we're adding a row of invaders, but we're out of space, it's game over!!
    for (int x=0; x<5; x++)
        if (invaders.getPixelValue(x,4))
            return GAME_OVER;

    // Otherwise, move down the invaders, and add a new role at the top.
    invaders.shiftDown(1);

    for (int x=1; x<4; x++)
        invaders.setPixelValue(x,0,255);

    return GAME_ON;
}

/*
 * Display Game Over and show the player's score.
 */
void
gameOver()
{
    uBit.display.clear();

    uBit.display.scroll("GAME OVER! SCORE:");
    uBit.display.scroll(score);
}

/*
 * Calculate the speed of an invaders movement, based on the game level
 */
int
invaderSpeed()
{
    return max(INVADER_SPEED - level*50, 50);
}

/*
 * Determine if the are any space invaders in the given column
 */
bool
invadersInColumn(int x)
{
    for (int y = 0; y < 5; y++)
        if (invaders.getPixelValue(x,y))
            return true;

    return false;
}

/*
 * Determine the number of space invaders currently on screen
 */
bool
invaderCount()
{
    int count = 0;

    for (int x=0; x<5; x++)
        for (int y=0; y<5; y++)
            if (invaders.getPixelValue(x,y))
                count++;

    return count;
}

/*
 * Move space invaders on the screen.
 */
void
invaderUpdate()
{
    bool movingRight = true;

    while(!game_over)
    {
        // Wait for next update;    
        uBit.sleep(invaderSpeed());

        if (movingRight)
        {
            if(invadersInColumn(4))
            {
                movingRight = false;
                if (addRow() == GAME_OVER)
                {
                    game_over = true;
                    return;
                }
            }
            else
            {
                invaders.shiftRight(1);
            }
        }
        else
        {
            if(invadersInColumn(0))
            {
                movingRight = true;
                if (addRow() == GAME_OVER)
                {
                    game_over = true;
                    return;
                }
            }
            else
            {
                invaders.shiftLeft(1);
            }
        }

        if (invaderCount() == 0)
        {
            level++;
            addRow();
        }
    }
}

/* 
 * Move the bullet up the screen
 */
void
bulletUpdate()
{
    while (!game_over)
    {
        uBit.sleep(BULLET_SPEED);
        if (bullet.y != -1)
            bullet.y--;

        if (invaders.getPixelValue(bullet.x, bullet.y) > 0)
        {
            score++;
            invaders.setPixelValue(bullet.x, bullet.y, 0);
            bullet.x = -1;
            bullet.y = -1;
        }
    }
}

/*
 * Move the player across the screen.
 */
void
playerUpdate()
{
    while (!game_over)
    {
        uBit.sleep(PLAYER_SPEED);

        if(uBit.accelerometer.getX() < -300 && player.x > 0)
            player.x--;

        if(uBit.accelerometer.getX() > 300 && player.x < 4)
            player.x++;
    }
}

/*
 * Fire a new missile from the player
 */
void
fire(MicroBitEvent)
{
    if (bullet.y == -1)
    {
        bullet.y = 4;
        bullet.x = player.x;
    }
}

/*
 * A simple game of space invaders
 */
void 
spaceInvaders()
{   
    // Reset all game state.
    game_over = 0;
    level = 0;
    score = 0;
    player.x = 2;
    player.y = 4;

    bullet.x = -1;
    bullet.y = -1;

    // Add a single row of invaders at the start.. cannon fodder!
    invaders.clear();
    addRow();

    // Spawn independent fibers to handle the movement of each player
    create_fiber(invaderUpdate);
    create_fiber(bulletUpdate);
    create_fiber(playerUpdate);

    // Register event handlers for button presses (either button fires!)
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, fire);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, fire);

    // Now just keep the screen refreshed.
    while (!game_over)
    {    
        uBit.sleep(10);
        uBit.display.image.paste(invaders);
        uBit.display.image.setPixelValue(player.x, player.y, 255);
        uBit.display.image.setPixelValue(bullet.x, bullet.y, 255);
    }

    // Display GAME OVER and score
    gameOver();
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    // Welcome message
    uBit.display.scroll("INVADERS!");

    // Keep playing forever
    while(1)
        spaceInvaders();
}

