#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define FIRST_COLUMN 0
#define TOP_ROW 0
#define BOTTOM_ROW 1

#define INTERRUPT_ONE 0
#define INTERRUPT_TWO 1

#define SPRITE_RUN1 1
#define SPRITE_RUN2 2
#define SPRITE_JUMP 3
#define SPRITE_JUMP_UPPER '.'         // Use the '.' character for the head
#define SPRITE_JUMP_LOWER 4
#define SPRITE_TERRAIN_EMPTY ' '      // User the ' ' character
#define SPRITE_TERRAIN_SOLID 5
#define SPRITE_TERRAIN_SOLID_RIGHT_EDGE 6
#define SPRITE_TERRAIN_SOLID_LEFT_EDGE 7


#define HERO_COLUMN_POSITION 1    // Horizontal position of hero on screen

#define TERRAIN_WIDTH 16
#define TERRAIN_EMPTY 0
#define TERRAIN_LOWER_BLOCK 1
#define TERRAIN_UPPER_BLOCK 2


#define HERO_POSITION_RUN_LOWER_1 1  // Hero is running on lower row (pose 1)
#define HERO_POSITION_RUN_LOWER_2 2  //                              (pose 2)

#define HERO_POSITION_JUMP_1 3       // Starting a jump
#define HERO_POSITION_JUMP_2 4       // Half-way up
#define HERO_POSITION_JUMP_3 5       // Jump is on upper row

#define MIN_HERO_POSITION 2
#define MAX_HERO_POSITION 5

#define OVERLAP_ARRAY_SIZE 4
#define OVERLAP_TOP_ROW_INDEX 0
#define OVERLAP_BOTTOM_ROW_INDEX 1
#define OVERLAP_TERMINATING_CHAR_INDEX 2


extern char terrain_upper[TERRAIN_WIDTH + 1];
extern char terrain_lower[TERRAIN_WIDTH + 1];
extern char player_terrain_overlap[OVERLAP_ARRAY_SIZE]; 
bool left_button_was_pushed = false;
bool right_button_was_pushed = false;
bool playing_game = false;

void initialize_graphics() {
    static byte graphics[] = {
        // Run position 1
        B01100,
        B01100,
        B00000,
        B01110,
        B11100,
        B01100,
        B11010,
        B10011,
        // Run position 2
        B01100,
        B01100,
        B00000,
        B01100,
        B01100,
        B01100,
        B01100,
        B01110,
        // Jump
        B01100,
        B01100,
        B00000,
        B11110,
        B01101,
        B11111,
        B10000,
        B00000,
        // Jump lower
        B11110,
        B01101,
        B11111,
        B10000,
        B00000,
        B00000,
        B00000,
        B00000,
        // Ground
        B11111,
        B11111,
        B11111,
        B11111,
        B11111,
        B11111,
        B11111,
        B11111,
        // Ground right
        B00011,
        B00011,
        B00011,
        B00011,
        B00011,
        B00011,
        B00011,
        B00011,
        // Ground left
        B11000,
        B11000,
        B11000,
        B11000,
        B11000,
        B11000,
        B11000,
        B11000,
    };

    // Skip using character 0, this allows lcd.print() to be used to
    // quickly draw multiple characters
    for (int i = 0; i < 7; i++) {
        lcd.createChar(i + 1, &graphics[i * 8]);
    }
   for (int i = 0; i < TERRAIN_WIDTH; i++) {
        terrain_upper[i] = SPRITE_TERRAIN_EMPTY;
        terrain_lower[i] = SPRITE_TERRAIN_EMPTY;
   }
   for (int i = 0; i < 3; i++) {
      player_terrain_overlap[i] = SPRITE_TERRAIN_EMPTY;
   }

   player_terrain_overlap[OVERLAP_TERMINATING_CHAR_INDEX] = '\0';
}

void draw_home_screen() {
    lcd.clear();
    lcd.setCursor(FIRST_COLUMN, TOP_ROW);
    lcd.print("    WELCOME!    ");
    lcd.setCursor(FIRST_COLUMN, BOTTOM_ROW);
    lcd.print(" Press Any Key. ");
}

void draw_end_game(unsigned int score) {
    // will be displayed on screen
    char score_result[TERRAIN_WIDTH + 1] = "   SCORE = ";

    // how many bytes are needed to represent the score
    byte digits = (score > 9999) ? 5 : (score > 999) ? 4 : (score > 99) ? 3 : (score > 9) ? 2 : 1;

    // take the original string and insert and EOS character 
    // needed for when we print it to the LCD
    score_result[16-digits] = '\0';

    // draw top row
    lcd.clear();
    lcd.setCursor(FIRST_COLUMN, TOP_ROW);
    lcd.print("   GAME OVER    ");

    // draw bottom row
    lcd.setCursor(FIRST_COLUMN, BOTTOM_ROW);
    // first draw the score display
    lcd.print(score_result);
    // set the cursor to point to the starting index that contains the score val
    lcd.setCursor(16 - digits, BOTTOM_ROW);
    // print the score
    lcd.print(score);
}

bool draw_player(byte position, char *terrain_upper, char *terrain_lower, unsigned int score) {
    byte player_sprite_lower = SPRITE_TERRAIN_EMPTY;
    byte player_sprite_upper = SPRITE_TERRAIN_EMPTY;
    char old_upper_sprite    = terrain_upper[HERO_COLUMN_POSITION];
    char old_lower_sprite    = terrain_lower[HERO_COLUMN_POSITION];
    bool collision_happpened = false;

    switch (position) {
    
        case HERO_POSITION_RUN_LOWER_1:
            player_sprite_lower = SPRITE_RUN1;
            break;

        case HERO_POSITION_RUN_LOWER_2:
            player_sprite_lower = SPRITE_RUN2;
            break;

        case HERO_POSITION_JUMP_1:
            player_sprite_lower = SPRITE_JUMP;
            break;
        
        case HERO_POSITION_JUMP_2:
            player_sprite_upper = SPRITE_JUMP_UPPER;
            player_sprite_lower = SPRITE_JUMP_LOWER;
            break;

        case HERO_POSITION_JUMP_3:
            player_sprite_upper = SPRITE_JUMP;
            break;
        
        default:
            break;
    }

    if (player_sprite_upper != ' ') {
        // attach player
        terrain_upper[HERO_COLUMN_POSITION] = player_sprite_upper;

        if (old_upper_sprite != SPRITE_TERRAIN_EMPTY) {
            collision_happpened = true;
        }
    }

    if (player_sprite_lower != ' ') {
        // attach player
        terrain_lower[HERO_COLUMN_POSITION] = player_sprite_lower;

        if (old_lower_sprite != SPRITE_TERRAIN_EMPTY) {
            collision_happpened = true;
        }
    }

    byte digits = (score > 9999) ? 5 : (score > 999) ? 4 : (score > 99) ? 3 : (score > 9) ? 2 : 1;

    terrain_upper[TERRAIN_WIDTH] = '\0';
    terrain_lower[TERRAIN_WIDTH] = '\0';

    // print top row; i.e. player, terrain, score
    char temp = terrain_upper[TERRAIN_WIDTH - digits]; // temp save the sprite before the start of score
    terrain_upper[TERRAIN_WIDTH - digits] = '\0'; // overwrite sprite we just saved in temp 
    lcd.setCursor(FIRST_COLUMN, TOP_ROW);
    lcd.print(terrain_upper);
    terrain_upper[TERRAIN_WIDTH - digits] = temp; // unco the changes made by temp
    lcd.setCursor(TERRAIN_WIDTH - digits, TOP_ROW);
    lcd.print(score);

    // print bottom row
    lcd.setCursor(FIRST_COLUMN, BOTTOM_ROW);
    lcd.print(terrain_lower);

    // now that hero is drawn, reattached old terrain sprite so that the map function still works
    terrain_upper[HERO_COLUMN_POSITION] = old_upper_sprite;
    terrain_lower[HERO_COLUMN_POSITION] = old_lower_sprite;

    return collision_happpened;
}

void generate_upper(char *terrain, byte new_terrain, char *overlap) {
    int index_of_right_most_terrain = TERRAIN_WIDTH - 1;

    if (new_terrain == TERRAIN_UPPER_BLOCK) {
        new_terrain = SPRITE_TERRAIN_SOLID;
    }
    else {
        new_terrain = SPRITE_TERRAIN_EMPTY;
    }

    for (int current_index = 0; current_index < TERRAIN_WIDTH; current_index++) {
        char current_terrain = terrain[current_index];
        char next_terrain;
        
        if (current_index == index_of_right_most_terrain) {
            terrain[current_index] = new_terrain;
            break;
        }
        else {
            next_terrain = terrain[current_index + 1];
        }

        if (current_index == HERO_COLUMN_POSITION) {
            overlap[OVERLAP_TOP_ROW_INDEX] = terrain[current_index];
        }

        switch (current_terrain) {
            case SPRITE_TERRAIN_EMPTY:
                ;
                if (next_terrain == SPRITE_TERRAIN_SOLID) {
                    terrain[current_index] = SPRITE_TERRAIN_SOLID_RIGHT_EDGE;
                }
                else {
                    terrain[current_index] = SPRITE_TERRAIN_EMPTY;
                }
                break;
            
            case SPRITE_TERRAIN_SOLID_RIGHT_EDGE:
                terrain[current_index] = SPRITE_TERRAIN_SOLID;
                break;

            case SPRITE_TERRAIN_SOLID:
                ;
                if (
                    next_terrain == SPRITE_TERRAIN_SOLID ||
                    next_terrain == SPRITE_TERRAIN_SOLID_LEFT_EDGE
                ) {
                    terrain[current_index] = SPRITE_TERRAIN_SOLID;
                }
                else {
                    terrain[current_index] = SPRITE_TERRAIN_SOLID_LEFT_EDGE;
                }
                break;

            case SPRITE_TERRAIN_SOLID_LEFT_EDGE:
                ;
                if (next_terrain == SPRITE_TERRAIN_SOLID) {
                    terrain[current_index] = SPRITE_TERRAIN_SOLID_RIGHT_EDGE;
                }
                else {
                    terrain[current_index] = SPRITE_TERRAIN_EMPTY;
                }
                
                break;

        }
    }
}

void generate_lower(char *terrain, byte new_terrain, char *overlap) {
    int index_of_right_most_terrain = TERRAIN_WIDTH - 1;

    if (new_terrain == TERRAIN_LOWER_BLOCK) {
        new_terrain = SPRITE_TERRAIN_SOLID;
    }
    else {
        new_terrain = SPRITE_TERRAIN_EMPTY;
    }

    for (int current_index = 0; current_index < TERRAIN_WIDTH; current_index++) {
        char current_terrain = terrain[current_index];
        char next_terrain;
        
        if (current_index == index_of_right_most_terrain) {
            terrain[current_index] = new_terrain;
            break;
        }
        else {
            next_terrain = terrain[current_index + 1];
        }

        if (current_index == HERO_COLUMN_POSITION) {
            overlap[OVERLAP_BOTTOM_ROW_INDEX] = terrain[current_index];
        }

        switch (current_terrain) {
            case SPRITE_TERRAIN_EMPTY:
                ;
                if (next_terrain == SPRITE_TERRAIN_SOLID) {
                    terrain[current_index] = SPRITE_TERRAIN_SOLID_RIGHT_EDGE;
                }
                else {
                    terrain[current_index] = SPRITE_TERRAIN_EMPTY;
                }
                break;
            
            case SPRITE_TERRAIN_SOLID_RIGHT_EDGE:
                terrain[current_index] = SPRITE_TERRAIN_SOLID;
                break;

            case SPRITE_TERRAIN_SOLID:
                ;
                if (
                    next_terrain == SPRITE_TERRAIN_SOLID ||
                    next_terrain == SPRITE_TERRAIN_SOLID_LEFT_EDGE
                ) {
                    terrain[current_index] = SPRITE_TERRAIN_SOLID;
                }
                else {
                    terrain[current_index] = SPRITE_TERRAIN_SOLID_LEFT_EDGE;
                }
                break;

            case SPRITE_TERRAIN_SOLID_LEFT_EDGE:
                ;
                if (next_terrain == SPRITE_TERRAIN_SOLID) {
                    terrain[current_index] = SPRITE_TERRAIN_SOLID_RIGHT_EDGE;
                }
                else {
                    terrain[current_index] = SPRITE_TERRAIN_EMPTY;
                }
                
                break;

        }
    }
}

void draw_map(char *terrain_upper, char *terrain_lower, byte new_terrain, char *overlap) {
    generate_upper(terrain_upper, new_terrain, overlap);
    generate_lower(terrain_lower, new_terrain, overlap);
}

void HW_firmware_test() {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 16; j++) {
            lcd.setCursor(j, i);
            lcd.print("\xff");
            delay(200);
            lcd.clear();
        }
        lcd.clear();
    }
    lcd.clear();
}

#endif
