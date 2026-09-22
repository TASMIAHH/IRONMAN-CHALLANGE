//======================================================================
//  GameConfig.h
//  Iron Man Challenge: The Ultimate Training
//
//  Global constants, screen ids and shared game state.
//  Included once, from iMain.cpp, after iGraphics.h
//======================================================================
#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#pragma warning(disable:4996)   // allow sprintf / fopen without the _s suffix

#include <string.h>

//----------------------------------------------------------------------
// Window
//----------------------------------------------------------------------
#define SCREEN_WIDTH   1024
#define SCREEN_HEIGHT   576
#define GAME_TITLE     "Iron Man Challenge: The Ultimate Training"

//----------------------------------------------------------------------
// Asset paths (relative to the working directory = the project folder)
//----------------------------------------------------------------------
#define PATH_INTRO_POSTER  "Assets/intro_poster.png"
#define PATH_INTRO_HUD_BG  "Assets/intro_hud_bg.png"
#define PATH_INTRO_SOUND   "Audios/intro_sound.mp3"
#define PATH_BG_SOUND      "Audios/background.mp3"
#define PATH_GAMEOVER_SOUND "Audios/gameover.mp3"
#define PATH_LEVEL01_MUSIC "Audios/level01_song.mp3"
#define PATH_COLLISION_SOUND "Audios/collision.mp3"

//----------------------------------------------------------------------
// Screens
//----------------------------------------------------------------------
#define SCREEN_MENU          0
#define SCREEN_LEVEL_SELECT  1
#define SCREEN_ABOUT         2
#define SCREEN_KEYS          3
#define SCREEN_LEVEL_1       4
#define SCREEN_LEVEL_2       5
#define SCREEN_LEVEL_3       6
#define SCREEN_LEVEL_4       7
#define SCREEN_NAME_ENTRY    8
#define SCREEN_RECORDS       9
#define SCREEN_INTRO        10

//----------------------------------------------------------------------
// Standardized Tactical Sound Button (Top-Right on every single page)
//----------------------------------------------------------------------
#define HUD_SOUND_BTN_W     118
#define HUD_SOUND_BTN_H      32
#define HUD_SOUND_BTN_X     (SCREEN_WIDTH - HUD_SOUND_BTN_W - 16)
#define HUD_SOUND_BTN_Y     (SCREEN_HEIGHT - HUD_SOUND_BTN_H - 12)

//----------------------------------------------------------------------
// Levels
// Discipline order follows the real IRONMAN triathlon: swim -> cycle -> run.
// Change these names if you want a different opening discipline.
//----------------------------------------------------------------------
#define LEVEL_1_NAME  "RUNNING TRAINING"
#define LEVEL_2_NAME  "CLIMBING CHALLENGE"
#define LEVEL_3_NAME  "CYCLING EXPEDITION"
#define LEVEL_4_NAME  "SWIMMING EXPEDITION"

#define TOTAL_LEVELS  4

//----------------------------------------------------------------------
// Theme colours (r, g, b) 0-255
//----------------------------------------------------------------------
#define COL_GOLD_R      255
#define COL_GOLD_G      190
#define COL_GOLD_B       60

#define COL_CYAN_R       90
#define COL_CYAN_G      210
#define COL_CYAN_B      255

#define COL_RED_R       225
#define COL_RED_G        60
#define COL_RED_B        50

#define COL_WHITE_R     245
#define COL_WHITE_G     245
#define COL_WHITE_B     245

#define COL_GREY_R      130
#define COL_GREY_G      140
#define COL_GREY_B      150

#define COL_PANEL_R      10
#define COL_PANEL_G      16
#define COL_PANEL_B      34

//----------------------------------------------------------------------
// Shared game state
//----------------------------------------------------------------------
int gCurrentScreen = SCREEN_INTRO;   // which screen is being displayed (boots to intro)

int gMouseX = 0;                    // live cursor position, used for hover
int gMouseY = 0;

int gLevelCompleted[TOTAL_LEVELS + 1];   // 1-based: gLevelCompleted[1], [2]

int gAssetsOk = 1;                  // cleared if an asset failed to load

// UI Texture handles (pure image-based, zero procedural vector idraw)
extern unsigned int gTexUiLock;
extern unsigned int gTexUiPlay;
extern unsigned int gTexUiMedalGold;
extern unsigned int gTexUiMedalSilver;
extern unsigned int gTexUiMedalBronze;
extern unsigned int gTexUiArcReactor;

//----------------------------------------------------------------------
// Marks a level as finished. The gameplay modules will call this when
// the player clears a stage; completing level 1 unlocks level 2.
//----------------------------------------------------------------------
void markLevelCompleted(int level)
{
	if (level >= 1 && level <= TOTAL_LEVELS)
		gLevelCompleted[level] = 1;
}

int isLevelUnlocked(int level)
{
	return 1;                    // all levels are unlocked from the start
}

//----------------------------------------------------------------------
// Defined in iMain.cpp - closes the audio devices and quits the program.
//----------------------------------------------------------------------
void requestExit();

//----------------------------------------------------------------------
// Defined in iMain.cpp - "was this key pressed on THIS tick", so a
// single tap fires once instead of on every 16 ms tick.
//----------------------------------------------------------------------
int keyJustPressed(unsigned char key);
int specialJustPressed(unsigned char key);

void resetProgress()
{
	int i;
	for (i = 0; i <= TOTAL_LEVELS; i++)
		gLevelCompleted[i] = 0;
}

#endif
