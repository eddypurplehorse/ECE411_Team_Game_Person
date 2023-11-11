/**
 * @file    main.ino 
 * @author  Team 10; Alex, Eddy, Paul, Same 
 * @brief   A flappy bird clone game
 * @version 0.1
 * @date    2023-11-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <Arduino_FreeRTOS.h>
#include "graphics.h"

const int left_button_pin = 9;
const int right_button_pin = 8;

// concurent function
void TaskBlink( void *pvParameters );
void LCD_Game( void *pvParameters );


void setup() {

    // GPIO 2
    attachInterrupt(0, right_button_is_pushed, FALLING);
    // GPIO 3
    attachInterrupt(1, left_button_is_pushed, FALLING);
  
    // Now set up two tasks to run independently.
    xTaskCreate(
        TaskBlink,  
        "Blink",   // A name just for humans
        128,       // This stack size can be checked & adjusted by reading the Stack Highwater
        NULL,
        0,         // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        NULL 
    );

    xTaskCreate(
        LCD_Game,
        "game",
        128,
        NULL,
        3,
        NULL
    );

}

void loop(){

}


void TaskBlink(void *pvParameters) {
    (void) pvParameters;

    // initialize digital LED_BUILTIN on pin 13 as an output.
    pinMode(LED_BUILTIN, OUTPUT);

    // A Task shall never return or exit.
    while(1) {
        digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
        vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
        digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
        vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
    }
}


void LCD_Game(void *pvParameters) {
    (void) pvParameters;
    
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);

    // set up button
    pinMode(left_button_pin, INPUT);
    pinMode(right_button_pin, INPUT);

    // set up graphics
    initialize_graphics();
    playing_game = false;
    
    while (true) {

        while (!playing_game) {
            draw_home_screen();
            if (left_button_pushed || right_button_pushed) {
                playing_game = true;
                right_button_pushed = false;
                left_button_pushed = false;
                lcd.clear();
                break;
            }
        }
    
        while (playing_game) {
            lcd.setCursor(FIRST_COLUMN, TOP_ROW);
            lcd.print("playing...");
            lcd.clear();
            int i = 1;

            while (true) {
           
                draw_player(i, terrain_upper, terrain_lower);
                if (right_button_pushed) {
                    i++;

                    if (i > 12)
                        i = 12;

                    right_button_pushed = false;
                }

                if (left_button_pushed) {
                    i--;

                    if (i < 1)
                        i = 1;

                    left_button_pushed = false;
                }
                lcd.setCursor(10, TOP_ROW);
                lcd.print(i);
                vTaskDelay( 100 / portTICK_PERIOD_MS );
                lcd.clear();
            }
        }
        
    }
}


bool left_button_is_pushed() {
    left_button_pushed = true;
}

bool right_button_is_pushed() {
    right_button_pushed = true;
}
