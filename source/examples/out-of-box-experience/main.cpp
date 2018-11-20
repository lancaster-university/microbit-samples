/*
The MIT License (MIT)

Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Technology Will Save Us and Lancaster University by arrangement with the BBC.

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

#define OOB_SHAKE_OVERSAMPLING                  5
#define OOB_SHAKE_OVERSAMPLING_THRESHOLD        4
#define OOB_SHAKE_THRESHOLD                     1500 

MicroBit uBit;

enum modes {
    WAKE = 0,
    INTRO,
    BUTTONA,
    BUTTONB,
    TURN,
    DOTCHASER,
    NEXT,
    SECRET
};
 
int mode;
int accelX, accelY;
int targetX, targetY;
bool flag = false;

bool button_a_pressed = false;
bool button_b_pressed = false;

MicroBitImage currentFrame;
 
 
//shake game
    int shakeCount;
    int xMin;
    int xMax;
    int yMin;
    int yMax;
    int prevX;
    int prevY;
    
 
// Images and animations -----------------
 
const MicroBitImage dot("0,1,0,1,0\n1,1,1,1,1\n1,1,1,1,1\n0,1,1,1,0\n0,0,1,0,0\n");
 
const char *shake[] = {
    "0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n",
    "0,0,0,0,0\n0,0,0,0,0\n0,0,1,0,0\n0,0,0,0,0\n0,0,0,0,0\n",
    "0,0,0,0,0\n0,1,0,1,0\n0,0,1,0,0\n0,1,0,1,0\n0,0,0,0,0\n",
    "0,0,1,0,0\n0,1,0,1,0\n1,0,1,0,1\n0,1,0,1,0\n0,0,1,0,0\n",
    "1,0,1,0,1\n0,1,0,1,0\n1,0,1,0,1\n0,1,0,1,0\n1,0,1,0,1\n",
    "1,1,1,1,1\n1,1,1,1,1\n1,1,1,1,1\n1,1,1,1,1\n1,1,1,1,1\n"    
};
 
const char *wakeAnim[] = {
    "0,0,0,0,0\n0,0,0,0,0\n0,0,1,0,0\n0,0,0,0,0\n0,0,0,0,0\n",
    "0,0,0,0,0\n0,1,1,1,0\n0,1,1,1,0\n0,1,1,1,0\n0,0,0,0,0\n",
    "1,1,1,1,1\n1,1,1,1,1\n1,1,1,1,1\n1,1,1,1,1\n1,1,1,1,1\n"
};
 
const char *explosionTime[] = {
    "1,1,1,1,1\n1,0,0,0,1\n1,0,0,0,1\n1,0,0,0,1\n1,1,1,1,1\n",
    "1,1,1,1,1\n1,1,1,1,1\n1,1,0,1,1\n1,1,1,1,1\n1,1,1,1,1\n",
    "1,1,1,1,1\n1,1,1,1,1\n1,1,1,1,1\n1,1,1,1,1\n1,1,1,1,1\n",
    "0,0,0,0,0\n0,1,1,1,0\n0,1,1,1,0\n0,1,1,1,0\n0,0,0,0,0\n",
    "0,0,0,0,0\n0,0,0,0,0\n0,0,1,0,0\n0,0,0,0,0\n0,0,0,0,0\n",
    "0,0,0,0,0\n0,1,0,1,0\n0,0,0,0,0\n0,1,0,1,0\n0,0,0,0,0\n",
    "1,0,0,0,1\n0,0,1,0,0\n0,1,1,1,0\n0,0,1,0,0\n1,0,0,0,1\n",
    "0,0,1,0,0\n0,1,0,1,0\n1,0,0,0,1\n0,1,0,1,0\n0,0,1,0,0\n",
    "1,0,0,0,1\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n1,0,0,0,1\n",
    "0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n"    
};
 
const char *twistyTime[] = {
    "0,0,0,0,0\n0,0,0,0,0\n1,0,0,0,1\n0,0,0,0,0\n0,0,0,0,0\n",
    "0,0,0,0,0\n1,0,0,0,1\n1,1,0,1,1\n1,0,0,0,1\n0,0,0,0,0\n",
    "1,0,0,0,1\n1,1,0,1,1\n1,1,1,1,1\n1,1,0,1,1\n1,0,0,0,1\n",
    "0,0,0,1,1\n1,0,0,1,1\n1,1,1,1,1\n1,1,0,0,1\n1,1,0,0,0\n",
    "0,1,1,1,1\n0,0,1,1,1\n1,0,1,0,1\n1,1,1,0,0\n1,1,1,1,0\n",
    "1,1,1,1,1\n0,0,1,1,1\n0,0,1,0,0\n1,1,1,0,0\n1,1,1,1,1\n",
    "1,1,1,1,1\n0,1,1,1,0\n0,0,1,0,0\n0,1,1,1,0\n1,1,1,1,1\n",
    "0,1,1,1,0\n0,0,1,0,0\n0,0,0,0,0\n0,0,1,0,0\n0,1,1,1,0\n",
    "0,0,1,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,1,0,0\n",
    "0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n"
};
 
const char *heart[] = { 
     "0,1,0,1,0\n1,1,1,1,1\n1,1,1,1,1\n0,1,1,1,0\n0,0,1,0,0\n"
};
 
// Arrow images and animations.
const MicroBitImage arrowUpTime("0,0,1,0,0\n0,1,1,1,0\n1,0,1,0,1\n0,0,1,0,0\n0,0,1,0,0\n");
 
const char *arrowDisintegrationTime[] = {
    "0,0,0,0,0\n0,0,1,0,0\n0,0,1,0,0\n1,0,1,0,1\n0,1,1,1,0\n",
    "0,0,0,0,0\n0,0,0,0,0\n0,0,1,0,0\n0,0,1,0,0\n1,0,1,0,1\n",
    "0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,1,0,0\n0,0,1,0,0\n",
    "0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,1,0,0\n",
    "0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n"
};
 
// Bottom arrow from left to right
const char *bottomArrow[] = {
    "0,0,1,0,0\n0,1,0,0,0\n1,1,1,1,1\n0,1,0,0,0\n0,0,1,0,0\n",
    "0,0,0,0,1\n1,0,0,1,0\n1,0,1,0,0\n1,1,0,0,0\n1,1,1,1,0\n",
    "0,0,1,0,0\n0,0,1,0,0\n1,0,1,0,1\n0,1,1,1,0\n0,0,1,0,0\n",
    "1,0,0,0,0\n0,1,0,0,1\n0,0,1,0,1\n0,0,0,1,1\n0,1,1,1,1\n",
    "0,0,1,0,0\n0,0,0,1,0\n1,1,1,1,1\n0,0,0,1,0\n0,0,1,0,0\n"
};
 
const char *topArrow[] = {
    "0,0,1,0,0\n0,1,0,0,0\n1,1,1,1,1\n0,1,0,0,0\n0,0,1,0,0\n",
    "1,1,1,1,0\n1,1,0,0,0\n1,0,1,0,0\n1,0,0,1,0\n0,0,0,0,1\n",
    "0,0,1,0,0\n0,1,1,1,0\n1,0,1,0,1\n0,0,1,0,0\n0,0,1,0,0\n",
    "0,1,1,1,1\n0,0,0,1,1\n0,0,1,0,1\n0,1,0,0,1\n1,0,0,0,0\n",
    "0,0,1,0,0\n0,0,0,1,0\n1,1,1,1,1\n0,0,0,1,0\n0,0,1,0,0\n"
};
 
// ---------------------------
 
// Wake up the device
void wake()
{
    uBit.display.setBrightness(0);
    // Turn on all pixels.
    for(int y=0; y<5; y++) {
        for(int x=0; x<5; x++) {
            uBit.display.image.setPixelValue(x, y, 1);
        }
    }
    
    // Fade in all LEDs.
    for(int i=0; i<255; i++) {
        uBit.display.setBrightness(i);
        uBit.sleep(5);
    }
    // Fade out all LEDs.
    for(int i=255; i>0; i--) {
        uBit.display.setBrightness(i);
        uBit.sleep(5);
    }
    
    // Set brightness back to full and clear screen.
    uBit.display.image.clear();
    uBit.display.setBrightness(255);
    
    // Pulsing animation.
    int animDelay = 50;
    for(int j=0; j<15; j++) {
        for(int i=0; i<3; i++) {
            currentFrame = MicroBitImage(wakeAnim[i]);
            uBit.display.print(currentFrame,0,0,0,animDelay);
        }
        for(int i=2; i>-1; i--) {
            currentFrame = MicroBitImage(wakeAnim[i]);
            uBit.display.print(currentFrame,0,0,0,animDelay);
        }
        animDelay -= 3;
    }
    
    // Fade out last dot.
    for(int i=255; i>=0; i--) {
        uBit.display.setBrightness(i);
        uBit.sleep(1);
    }
    
    // Clear display and set brightnes back to full.
    uBit.display.image.clear();
    uBit.display.setBrightness(255);
    
    uBit.sleep(500);
    
    // Proceed to the next mode.
    mode++;
}
 
void intro()
{
    // Introduce the micro:bit.
    uBit.display.image.clear();
    uBit.display.scroll("HELLO.");
    
    // Proceed to the next mode.
    mode++;
}

void onButtonA(MicroBitEvent)
{
    button_a_pressed = true;
}
 
void pressButtonA() 
{
    // Set up button A event listener
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonA);


    // Wait for a button press.
    while(!button_a_pressed) {
        uBit.display.print("A ",100);
        uBit.sleep(1);
         uBit.display.print("A ",100);
        uBit.sleep(1);
        if(uBit.buttonA.isPressed()) break;
    
        currentFrame = MicroBitImage(topArrow[0]);
        uBit.display.print(currentFrame,0,0,0,100);
       uBit.sleep(100);
        if(uBit.buttonA.isPressed()) break;
    
       currentFrame = MicroBitImage(topArrow[0]);
        uBit.display.print(currentFrame,0,0,0,100);
       uBit.sleep(100);
        if(uBit.buttonA.isPressed()) break;
    }
    
    // SADHBH'S animation goes here.
    for(int i=0; i<10; i++) {
        currentFrame = MicroBitImage(explosionTime[i]);
        uBit.display.print(currentFrame,0,0,0,100);
    }
    
    uBit.display.stopAnimation();
    uBit.sleep(1000);
    
    // Proceed to the next mode.
    
    mode++;
}
 
void onButtonB(MicroBitEvent)
{
    button_b_pressed = true;
}

void pressButtonB()
{
    // Set up button B event listener
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonB);
    
    // Wait for a button press.
    while(!button_b_pressed) {
         uBit.display.print("B ",100);
        uBit.sleep(1);
         uBit.display.print("B ",100);
        uBit.sleep(1);
        if(uBit.buttonB.isPressed()) break;
    
         currentFrame = MicroBitImage(topArrow[4]);
        uBit.display.print(currentFrame,0,0,0,100);
       uBit.sleep(100);
       if(uBit.buttonB.isPressed())break;
    
       currentFrame = MicroBitImage(topArrow[4]);
        uBit.display.print(currentFrame,0,0,0,100);
       uBit.sleep(100);
        if(uBit.buttonB.isPressed())break;
    }
    
    // SADHBH'S animation goes here.
    for(int i=0; i<10; i++) {
        currentFrame = MicroBitImage(twistyTime[i]);
        uBit.display.print(currentFrame,0,0,0,100);
    }
    
    uBit.sleep(2000);
    
    // Proceed to the next mode.
    mode++;
}
 
void updateAccelPosition()
{
    accelX = 0;
    accelY = 0;
 
    if (uBit.accelerometer.getX() > 600)
        accelX++;
    if (uBit.accelerometer.getX() > 250)
        accelX++;
    if (uBit.accelerometer.getX() > -250)
        accelX++;
    if (uBit.accelerometer.getX() > -600)
        accelX++;
 
    if (uBit.accelerometer.getY() > 600)
        accelY++;
    if (uBit.accelerometer.getY() > 250)
        accelY++;
    if (uBit.accelerometer.getY() > -250)
        accelY++;
    if (uBit.accelerometer.getY() > -600)
        accelY++;
}

void turn()
{
    int timeout = 0;
    int samples_high;
    int x, y, z, magnitude;

    uBit.display.scroll("SHAKE!");

    uBit.accelerometer.setRange(8);

    xMax = uBit.accelerometer.getX();
    yMax = uBit.accelerometer.getY();

    while(timeout < 10000) {
        samples_high = 0;
        for (int samples = 0; samples < OOB_SHAKE_OVERSAMPLING; samples++)
        {
            x = uBit.accelerometer.getX();
            y = uBit.accelerometer.getY();
            z = uBit.accelerometer.getZ();
            magnitude = sqrt((x*x)+(y*y)+(z*z));

            if (magnitude > OOB_SHAKE_THRESHOLD)
                samples_high++;
        }

        if (samples_high >= OOB_SHAKE_OVERSAMPLING_THRESHOLD)
            shakeCount++;
        else
            shakeCount--;

        // Clamp shakeCount within range 0..4 
        shakeCount = min(shakeCount, 4);
        shakeCount = max(shakeCount, 0);

        // Display an image matching the shake intensity measured
        currentFrame = MicroBitImage(shake[shakeCount]);
        uBit.display.print(currentFrame);

        // Wait a while.
        uBit.sleep(150);
        timeout += 150;
    }
    
    uBit.accelerometer.setRange(2);
    uBit.display.image.clear();
    uBit.sleep(1000);

    mode++;
}

void insertNewTarget()
{
    targetX = uBit.random(4);
    targetY = uBit.random(4);
    if(targetX == 0 && targetY == 0) targetY++;
    else if(targetX == 0 && targetY == 4) targetY--;
    else if(targetX == 4 && targetY == 0) targetY++;
    else if(targetX == 4 && targetY == 4) targetY--;
}
 
void dotChaser()
{
    uBit.display.scroll("CHASE THE DOT");
    
    int score = 0;
    int toggle = 0;
    int toggleCount = 0;
    
    while(score < 6) {
        if(toggleCount % 5 == 0) toggle = 255-toggle;
        
        updateAccelPosition();
        
        uBit.display.image.clear();
        uBit.display.image.setPixelValue(accelX, accelY, 255);
        uBit.display.image.setPixelValue(targetX, targetY, toggle);
        
        if(targetX == accelX && targetY == accelY) {
            insertNewTarget();
            score++;
        }
        
        uBit.sleep(100);
        
        toggleCount++;
    }
    
    // Fade out last dot.
    for(int i=255; i>=0; i--) {
        uBit.display.setBrightness(i);
        uBit.sleep(1);
    }
    
    // Clear display and set brightnes back to full.
    uBit.display.image.clear();
    uBit.display.setBrightness(255);
    
    uBit.sleep(2000);
    
    mode++;
}
 
#define SNAKE_EMPTY 0
#define SNAKE_UP    1
#define SNAKE_LEFT  2
#define SNAKE_RIGHT 3
#define SNAKE_DOWN  4
 
 
#define SNAKE_FRAME_DELAY 350
 
struct Point
{
    int     x;
    int     y;
};
 
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
    
    // Start in the middle of the screen.
    tail.x = tail.y = 2;    
    head.x = head.y = 2;
    snakeLength = 1;
    growing = 0;
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
            ManagedString s("GAME OVER! SCORE: ");
            ManagedString s2(snakeLength-1);
            
            uBit.display.scroll(s);
            uBit.display.scroll(s2);
            
            return;            
        }
                                          
        // move the head.       
        map.setPixelValue(head.x, head.y, hdirection);
        uBit.display.image.setPixelValue(newHead.x, newHead.y, 255);
 
        if (growing)
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
            growing = 1;
            place_food();
        }
      
        uBit.sleep(SNAKE_FRAME_DELAY);   
    }   
}
 
void next()
{
     if(flag == false){
     flag = true;
    uBit.display.scroll("GREAT! NOW GET CODING!");
     }
    while(!uBit.buttonA.isPressed() && !uBit.buttonB.isPressed()){
    for(int i=0; i<10; i++) {
        currentFrame = MicroBitImage(twistyTime[i]);
        uBit.display.print(currentFrame,0,0,0,100);
         if(uBit.buttonA.isPressed() && uBit.buttonB.isPressed()){
            uBit.display.stopAnimation();
             break;
             }
             }
     for(int i=0; i<10; i++) {
        currentFrame = MicroBitImage(explosionTime[i]);
        uBit.display.print(currentFrame,0,0,0,100);
       if(uBit.buttonA.isPressed() && uBit.buttonB.isPressed()){
            uBit.display.stopAnimation();
             break;
             }
    }
     currentFrame = MicroBitImage(heart[0]);     
        uBit.display.print(currentFrame,0,0,0,400); 
        uBit.sleep(100);
        if(uBit.buttonA.isPressed() && uBit.buttonB.isPressed()){
            uBit.display.stopAnimation();
             break;
             }
        }
        mode++;
     
    
   
    
}

int
main()
{   
    uBit.init();


    new MicroBitAccelerometerService(*uBit.ble, uBit.accelerometer);
    new MicroBitButtonService(*uBit.ble);
    new MicroBitLEDService(*uBit.ble, uBit.display);
    new MicroBitTemperatureService(*uBit.ble, uBit.thermometer);

    if(uBit.buttonA.isPressed()) mode = SECRET;
    while(1)
    {   
        switch(mode) {
            
            case WAKE:
                wake();
                break;
                
            case INTRO:
                intro();
                break;
                
            case BUTTONA:
                pressButtonA();
                break;
                
            case BUTTONB:
                pressButtonB();
                break;
            
            case TURN:
                turn();
                break;
                
            case DOTCHASER:
                dotChaser();
                break;
                
            case NEXT:
                next();
                break;  
            
            case SECRET:
                snake();
                break;
        }
    }      
}

