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

#include "graphics.h"
#define TERRAIN_WIDTH 16

char terrain_upper[TERRAIN_WIDTH + 1];
char terrain_lower[TERRAIN_WIDTH + 1];
char player_terrain_overlap[OVERLAP_ARRAY_SIZE];

const int left_button_pin = 9;
const int right_button_pin = 8;
unsigned int game_speed = 500;
unsigned long long total_frames_drawn = 0;


void setup() {
    // GPIO 2
    attachInterrupt(INTERRUPT_ONE, right_button_is_pushed, RISING);
    // GPIO 3
    attachInterrupt(INTERRUPT_TWO, left_button_is_pushed, RISING);
  
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);

    // set up button
    pinMode(left_button_pin, INPUT);
    pinMode(right_button_pin, INPUT);

    // set up graphics
    initialize_graphics();

}

void loop(){
    playing_game = false;
    unsigned int player_position = HERO_POSITION_RUN_LOWER_2;
    static byte new_terrain_type = TERRAIN_EMPTY;
    byte duration_between_terrain = 0;
    unsigned int score = 0;
    bool collision_happened = false;

    // set up graphics
    initialize_graphics();

    while (!playing_game) {
        draw_home_screen();
        if (left_button_was_pushed || right_button_was_pushed) {
            playing_game = true;
            right_button_was_pushed = false;
            left_button_was_pushed = false;
            lcd.clear();
            break;
        }
        delay(100);
    }
    
    while (playing_game) {

        if (duration_between_terrain == 0) {
            if (new_terrain_type == TERRAIN_EMPTY) {
                new_terrain_type = (random(3) == 0) ? TERRAIN_UPPER_BLOCK : TERRAIN_LOWER_BLOCK;
                duration_between_terrain = 5 + random(10);
            }
            else {
                new_terrain_type = TERRAIN_EMPTY;
                duration_between_terrain = 10 + random(10);
            }
        }

        if (player_position < MIN_HERO_POSITION) {
            player_position = MIN_HERO_POSITION;
        }

        draw_map(terrain_upper, terrain_lower, new_terrain_type, player_terrain_overlap);
        collision_happened = draw_player(player_position, terrain_upper, terrain_lower, score);


        if (collision_happened) {
            break; // game over
        }

        if (total_frames_drawn % 3 == 0) {
          player_position--;
        }

        if (total_frames_drawn % 16 == 0) {
            score++;
            game_speed -= 10;

            if (game_speed < 15) {
              game_speed = 15;
            }
        }

        if (right_button_was_pushed) {
            right_button_was_pushed = false;
            player_position++;

            if (player_position > MAX_HERO_POSITION) {
                player_position = MAX_HERO_POSITION;
            }
        }

        
        delay(game_speed);
        total_frames_drawn++;
        duration_between_terrain--;
    }

    draw_end_game(score);
    delay(2000);
}


void left_button_is_pushed() {
    left_button_was_pushed = true;
}

void right_button_is_pushed() {
    right_button_was_pushed = true;
}
