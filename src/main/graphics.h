#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define FIRST_COLUMN 0
#define TOP_ROW 0
#define BOTTOM_ROW 1

#define PIN_BUTTON 2
#define PIN_AUTOPLAY 1
#define PIN_READWRITE 10
#define PIN_CONTRAST 12

#define SPRITE_RUN1 1
#define SPRITE_RUN2 2
#define SPRITE_JUMP 3
#define SPRITE_JUMP_UPPER '.'         // Use the '.' character for the head
#define SPRITE_JUMP_LOWER 4
#define SPRITE_TERRAIN_EMPTY ' '      // User the ' ' character
#define SPRITE_TERRAIN_SOLID 5
#define SPRITE_TERRAIN_SOLID_RIGHT 6
#define SPRITE_TERRAIN_SOLID_LEFT 7

#define HERO_HORIZONTAL_POSITION 1    // Horizontal position of hero on screen

#define TERRAIN_WIDTH 16
#define TERRAIN_EMPTY 0
#define TERRAIN_LOWER_BLOCK 1
#define TERRAIN_UPPER_BLOCK 2

#define HERO_POSITION_OFF 0          // Hero is invisible
#define HERO_POSITION_RUN_LOWER_1 1  // Hero is running on lower row (pose 1)
#define HERO_POSITION_RUN_LOWER_2 2  //                              (pose 2)

#define HERO_POSITION_JUMP_1 3       // Starting a jump
#define HERO_POSITION_JUMP_2 4       // Half-way up
#define HERO_POSITION_JUMP_3 5       // Jump is on upper row
#define HERO_POSITION_JUMP_4 6       // Jump is on upper row
#define HERO_POSITION_JUMP_5 7       // Jump is on upper row
#define HERO_POSITION_JUMP_6 8       // Jump is on upper row
#define HERO_POSITION_JUMP_7 9       // Half-way down
#define HERO_POSITION_JUMP_8 10      // About to land

#define HERO_POSITION_RUN_UPPER_1 11 // Hero is running on upper row (pose 1)
#define HERO_POSITION_RUN_UPPER_2 12 //                              (pose 2)

static char terrain_upper[TERRAIN_WIDTH + 1];
static char terrain_lower[TERRAIN_WIDTH + 1];
static bool left_button_pushed = false;
static bool right_button_pushed = false;
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
    int i;
    // Skip using character 0, this allows lcd.print() to be used to
    // quickly draw multiple characters
    for (i = 0; i < 7; ++i) {
        lcd.createChar(i + 1, &graphics[i * 8]);
    }
   for (i = 0; i < TERRAIN_WIDTH; ++i) {
       terrain_upper[i] = SPRITE_TERRAIN_EMPTY;
       terrain_lower[i] = SPRITE_TERRAIN_EMPTY;
   }
}

void draw_home_screen() {
    lcd.setCursor(FIRST_COLUMN, TOP_ROW);
    lcd.print("    WELCOME!    ");
    lcd.setCursor(FIRST_COLUMN, BOTTOM_ROW);
    lcd.print(" Press Any Key. ");
}

void draw_player(byte position, char *terrain_upper, char *terrain_lower) {
    byte player_sprite_lower = SPRITE_TERRAIN_EMPTY;
    byte player_sprite_upper = SPRITE_TERRAIN_EMPTY;

    switch (position) {
        case HERO_POSITION_OFF:
            // no player - already set
            break;
        
        case HERO_POSITION_RUN_LOWER_1:
            player_sprite_lower = SPRITE_RUN1;
            break;

        case HERO_POSITION_RUN_LOWER_2:
            player_sprite_lower = SPRITE_RUN2;
            break;

        case HERO_POSITION_RUN_UPPER_1:
            player_sprite_upper = SPRITE_RUN1;
            break;

        case HERO_POSITION_RUN_UPPER_2:
            player_sprite_upper = SPRITE_RUN2;
            break;

        case HERO_POSITION_JUMP_1:
        case HERO_POSITION_JUMP_8:
            player_sprite_lower = SPRITE_JUMP;
            break;
        
        case HERO_POSITION_JUMP_2:
        case HERO_POSITION_JUMP_7:
            player_sprite_upper = SPRITE_JUMP_UPPER;
            player_sprite_lower = SPRITE_JUMP_LOWER;
            break;

        case HERO_POSITION_JUMP_3:
        case HERO_POSITION_JUMP_4:
        case HERO_POSITION_JUMP_5:
        case HERO_POSITION_JUMP_6:
            player_sprite_upper = SPRITE_JUMP;
            break;
        
        default:
            break;
    }
    terrain_upper[TERRAIN_WIDTH] = '\0';
    terrain_lower[TERRAIN_WIDTH] = '\0';


    terrain_upper[HERO_HORIZONTAL_POSITION] = player_sprite_upper;
    lcd.setCursor(FIRST_COLUMN, TOP_ROW);
    lcd.print(terrain_upper);


    terrain_lower[HERO_HORIZONTAL_POSITION] = player_sprite_lower;
    lcd.setCursor(FIRST_COLUMN, BOTTOM_ROW);
    lcd.print(terrain_lower);
}

#endif
