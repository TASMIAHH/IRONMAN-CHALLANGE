//======================================================================
//  Iron Man Challenge: The Ultimate Training
//  CSE-1200 Software Development - I   |   Department of CSE, AUST
//
//  Built with iGraphics 4.0 / OpenGL / GLUT, Visual Studio 2013, Win32.
//
//  This file owns the iGraphics callbacks and routes them to whichever
//  screen is currently active. The screens themselves live in the
//  Screen*.h headers.
//
//  NOTE ON THE FILE LAYOUT
//  iGraphics.h defines global variables and #defines
//  STB_IMAGE_IMPLEMENTATION, so it can only be included by ONE source
//  file. The whole game is therefore a single translation unit: every
//  module is a header included below, in dependency order, and iMain.cpp
//  stays the only .cpp in the project.
//======================================================================

#include "iGraphics.h"

#include "GameConfig.h"          // constants, screen ids, shared state
#include "GameAudio.h"           // mciSendString wrappers
#include "GameProfile.h"         // persistent gamer profiles & high scores
#include "GameUI.h"              // text, panels, buttons
#include "GameAssets.h"          // texture loading
#include "Level01.h"             // LEVEL 01 simulation
#include "Level01Draw.h"         // LEVEL 01 rendering
#include "Level02.h"             // LEVEL 02 simulation
#include "Level02Draw.h"         // LEVEL 02 rendering
#include "Level03.h"             // LEVEL 03 simulation
#include "Level03Draw.h"         // LEVEL 03 rendering
#include "Level04.h"             // LEVEL 04 simulation
#include "Level04Draw.h"         // LEVEL 04 rendering
#include "ScreenProfile.h"       // Athlete Name Entry & Profile interface
#include "ScreenRecords.h"       // Hall of Fame & Leaderboards
#include "ScreenIntro.h"         // Military Triathlon & Extreme Endurance Opening Interface
#include "ScreenMenu.h"
#include "ScreenLevelSelect.h"
#include "ScreenAbout.h"
#include "ScreenKeys.h"
#include "ScreenLevel.h"

//----------------------------------------------------------------------
// Key codes
//----------------------------------------------------------------------
#define KEY_BACKSPACE   8
#define KEY_ENTER      13
#define KEY_ESCAPE     27

//----------------------------------------------------------------------
// Edge detection.
//
// iGraphics gives us the *held* state of every key. That is right for
// player movement, but a menu must react once per press, otherwise a
// single tap of ENTER fires on every 16 ms tick. We keep last tick's
// state and compare.
//----------------------------------------------------------------------
unsigned char gPrevKey[256];
unsigned char gPrevSpecial[256];

int keyJustPressed(unsigned char key)
{
	return isKeyPressed(key) && !gPrevKey[key];
}

int specialJustPressed(unsigned char key)
{
	return isSpecialKeyPressed(key) && !gPrevSpecial[key];
}

void snapshotKeyState()
{
	int i;
	for (i = 0; i < 256; i++)
	{
		gPrevKey[i] = (unsigned char)(isKeyPressed((unsigned char)i) ? 1 : 0);
		gPrevSpecial[i] = (unsigned char)(isSpecialKeyPressed((unsigned char)i) ? 1 : 0);
	}
}

//----------------------------------------------------------------------
// Shuts the audio devices down cleanly, then quits. Called by EXIT.
//----------------------------------------------------------------------
void requestExit()
{
	profileSaveAll();
	audioShutdown();
	exit(0);
}

//----------------------------------------------------------------------
// Rendering
//----------------------------------------------------------------------
void iDraw()
{
	iClear();
	uiEnableBlending();

	switch (gCurrentScreen)
	{
	case SCREEN_INTRO:         introDraw();         break;
	case SCREEN_NAME_ENTRY:    profileScreenDraw(); break;
	case SCREEN_RECORDS:       recordsDraw();       break;
	case SCREEN_MENU:          menuDraw();          break;
	case SCREEN_LEVEL_SELECT:  levelSelectDraw();   break;
	case SCREEN_ABOUT:         aboutDraw();         break;
	case SCREEN_KEYS:          keysDraw();          break;
	case SCREEN_LEVEL_1:       level01Draw();       break;
	case SCREEN_LEVEL_2:       level02Draw();       break;
	case SCREEN_LEVEL_3:       level03Draw();       break;
	case SCREEN_LEVEL_4:       level04Draw();       break;
	default:                   introDraw();         break;
	}

	// Universal Tactical Sound Toggle & Audio Notification across EVERY screen
	uiDrawSoundToggleButton();
	uiDrawAudioNotification();
}

//----------------------------------------------------------------------
// Mouse
//----------------------------------------------------------------------
void updateHover(int mx, int my)
{
	gMouseX = mx;
	gMouseY = my;

	if (gCurrentScreen == SCREEN_INTRO)             introMouseMove();
	else if (gCurrentScreen == SCREEN_NAME_ENTRY)   profileScreenMouseMove();
	else if (gCurrentScreen == SCREEN_MENU)         menuMouseMove();
	else if (gCurrentScreen == SCREEN_LEVEL_SELECT) levelSelectMouseMove();
}

void iMouseMove(int mx, int my)
{
	updateHover(mx, my);
}

void iPassiveMouseMove(int mx, int my)
{
	updateHover(mx, my);
}

void iMouse(int button, int state, int mx, int my)
{
	gMouseX = mx;
	gMouseY = my;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// 1. Universal Sound Toggle Check (Available on EVERY single screen)
		if (uiCheckSoundButtonClick(mx, my))
			return;

		// 2. Screen-specific mouse click routing
		switch (gCurrentScreen)
		{
		case SCREEN_INTRO:         introMouseClick(mx, my);         break;
		case SCREEN_NAME_ENTRY:    profileScreenMouseClick(mx, my); break;
		case SCREEN_RECORDS:       recordsMouseClick(mx, my);       break;
		case SCREEN_MENU:          menuMouseClick(mx, my);          break;
		case SCREEN_LEVEL_SELECT:  levelSelectMouseClick(mx, my);   break;
		case SCREEN_ABOUT:         aboutMouseClick(mx, my);         break;
		case SCREEN_KEYS:          keysMouseClick(mx, my);          break;
		}
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		// reserved
	}
}

//----------------------------------------------------------------------
// Fixed logic tick.
// iGraphics calls this every 16 ms (~62 times a second) from a Windows
// timer set up inside iInitialize(). All game logic belongs here, never
// in iDraw().
//----------------------------------------------------------------------
void fixedUpdate()
{
	uiUpdateAudioToast(0.016);

	// Global Tactical Audio / Comms Toggle (Press 'M' anywhere in the game)
	if (keyJustPressed('m') || keyJustPressed('M'))
	{
		audioToggleMute();
		uiTriggerAudioNotification();
	}

	int moveUp = specialJustPressed(GLUT_KEY_UP) || keyJustPressed('w') || keyJustPressed('W');
	int moveDown = specialJustPressed(GLUT_KEY_DOWN) || keyJustPressed('s') || keyJustPressed('S');
	int moveLeft = specialJustPressed(GLUT_KEY_LEFT) || keyJustPressed('a') || keyJustPressed('A');
	int moveRight = specialJustPressed(GLUT_KEY_RIGHT) || keyJustPressed('d') || keyJustPressed('D');

	int confirm = keyJustPressed(KEY_ENTER);
	int goBack = keyJustPressed(KEY_ESCAPE) || keyJustPressed(KEY_BACKSPACE);

	switch (gCurrentScreen)
	{
	case SCREEN_INTRO:
		introUpdate(confirm || keyJustPressed(' '), goBack);
		if (keyJustPressed('1')) levelSelectStart(1);
		if (keyJustPressed('2')) levelSelectStart(2);
		if (keyJustPressed('3')) { markLevelCompleted(1); markLevelCompleted(2); levelSelectStart(3); }
		if (keyJustPressed('4')) { markLevelCompleted(1); markLevelCompleted(2); markLevelCompleted(3); levelSelectStart(4); }
		break;

	case SCREEN_NAME_ENTRY:
	{
		if (keyJustPressed(KEY_BACKSPACE)) profileScreenCharInput(8);
		if (keyJustPressed(KEY_ENTER))     profileScreenCharInput(13);
		if (keyJustPressed(KEY_ESCAPE))    profileScreenCharInput(27);

		int ch;
		for (ch = 32; ch <= 126; ch++)
		{
			if (keyJustPressed((unsigned char)ch))
			{
				profileScreenCharInput((unsigned char)ch);
			}
		}
		break;
	}

	case SCREEN_RECORDS:
		if (goBack || confirm)
		{
			gCurrentScreen = SCREEN_MENU;
		}
		break;

	case SCREEN_MENU:
		// ESC is deliberately ignored here so the game is never closed
		// by accident during the demo. Use the EXIT button.
		menuKeyDown(moveUp, moveDown, confirm);
		break;

	case SCREEN_LEVEL_SELECT:
		levelSelectKeyDown(moveLeft || moveUp, moveRight || moveDown, confirm);
		if (keyJustPressed('u') || keyJustPressed('U'))
		{
			markLevelCompleted(1);
			markLevelCompleted(2);
		}
		if (keyJustPressed('1')) levelSelectStart(1);
		if (keyJustPressed('2')) levelSelectStart(2);
		if (keyJustPressed('3')) { markLevelCompleted(1); markLevelCompleted(2); levelSelectStart(3); }
		if (keyJustPressed('4')) { markLevelCompleted(1); markLevelCompleted(2); markLevelCompleted(3); levelSelectStart(4); }
		if (goBack)
		{
			audioPlayIntroTheme();          // back on the menu, intro loops again
			gCurrentScreen = SCREEN_MENU;
		}
		break;

	case SCREEN_ABOUT:
	case SCREEN_KEYS:
		if (goBack)
			gCurrentScreen = SCREEN_MENU;
		break;

	case SCREEN_LEVEL_1:
		// Jump is edge-triggered so holding the key cannot re-launch the
		// runner the instant he lands; sprint/brake are polled as held
		// keys inside level01Update().
		level01Update(keyJustPressed(' ') ||
			keyJustPressed('w') || keyJustPressed('W') ||
			specialJustPressed(GLUT_KEY_UP),
			keyJustPressed('p') || keyJustPressed('P'),
			goBack,
			confirm);
		break;

	case SCREEN_LEVEL_2:
		// SPACE is the dedicated jump key (edge-triggered).
		// WASD / arrows for movement are polled as held keys inside
		// level02Update(), just like sprint/brake are in Level 01.
		level02Update(keyJustPressed(' '),
			keyJustPressed('p') || keyJustPressed('P'),
			goBack,
			confirm);
		break;

	case SCREEN_LEVEL_3:
		// SPACE, W, or UP for bunny hop leap (edge-triggered).
		// D / A (sprint/brake) are polled as held keys inside level03Update().
		level03Update(keyJustPressed(' ') ||
			keyJustPressed('w') || keyJustPressed('W') ||
			specialJustPressed(GLUT_KEY_UP),
			keyJustPressed('p') || keyJustPressed('P'),
			goBack,
			confirm);
		break;

	case SCREEN_LEVEL_4:
		// SPACE for Dolphin Kick surge (edge-triggered).
		// W/S (dive/surface) and D/A (sprint/glide) are polled as held
		// keys inside level04Update().
		level04Update(keyJustPressed(' '),
			keyJustPressed('p') || keyJustPressed('P'),
			goBack,
			confirm);
		break;
	}

	snapshotKeyState();
}

//----------------------------------------------------------------------
char gWindowTitle[] = GAME_TITLE;

int main()
{
	resetProgress();
	profileInit();
	profileLoadAll();

	introInit();
	profileScreenInit();
	recordsInit();
	menuInit();
	levelSelectInit();
	aboutInit();
	keysInit();
	levelScreenInit();

	// Creates the window and the OpenGL context.
	iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, gWindowTitle);

	// Textures need a live GL context, so this must come after iInitialize.
	assetsLoadAll();
	level01LoadAssets();
	level02LoadAssets();     // loads gL2TexBg (Assets/level02_background.png)
	level03LoadAssets();     // loads Level 03 backgrounds, sprites, obstacles, pickups
	level04LoadAssets();     // loads Level 04 backgrounds, swimmer sprites, obstacles, pickups
	uiEnableBlending();

	// The intro theme loops on the menu until NEW GAME is chosen.
	audioInit();
	audioPlayIntroTheme();
	iSetTimer(1000, audioLoopWatchdog);

	iStart();
	return 0;
}