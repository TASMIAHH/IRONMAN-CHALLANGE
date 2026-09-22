//======================================================================
//  ScreenMenu.h
//  The main menu: the intro poster with NEW GAME / KEYS /
//  ABOUT THE GAME / EXIT laid over it. The intro theme loops here.
//======================================================================
#ifndef SCREEN_MENU_H
#define SCREEN_MENU_H

#define MENU_ITEM_COUNT   5

#define MENU_NEW_GAME     0
#define MENU_RECORDS      1
#define MENU_KEYS         2
#define MENU_ABOUT        3
#define MENU_EXIT         4

// panel geometry
#define MENU_PANEL_W    330
#define MENU_PANEL_H    364

// shifts the whole menu box further down the screen
#define MENU_PANEL_Y_OFFSET   25

#define MENU_PANEL_X    ((SCREEN_WIDTH  - MENU_PANEL_W) / 2)
#define MENU_PANEL_Y    (((SCREEN_HEIGHT - MENU_PANEL_H) / 2) - MENU_PANEL_Y_OFFSET)

#define MENU_BTN_X       (MENU_PANEL_X + 20)
#define MENU_BTN_W      290
#define MENU_BTN_H       48
#define MENU_BTN_STEP    58
#define MENU_BTN_TOP_Y  (MENU_PANEL_Y + 252)   // y of the first (top) button

// game title text
#define GAME_TITLE_TEXT "IRON MAN CHALLENGE: THE ULTIMATE TRAINING"

// title font made a bit smaller
#define TITLE_FONT_SCALE      0.24f
#define TITLE_FONT_THICKNESS  3.0f

// CHANGED: geometry for the sound mute button (bottom-right corner)
#define SOUND_BTN_W   110
#define SOUND_BTN_H    36
#define SOUND_BTN_X   (SCREEN_WIDTH - SOUND_BTN_W - 20)
#define SOUND_BTN_Y   (SCREEN_HEIGHT - SOUND_BTN_H - 20)

// Active athlete banner & switch button
#define BANNER_X       20
#define BANNER_Y       (SCREEN_HEIGHT - 56)
#define BANNER_W       370
#define BANNER_H       40

struct Button gMenuButtons[MENU_ITEM_COUNT];
struct Button gMenuSwitchGamer;
int gMenuIndex = 0;                // keyboard / hover selection

//----------------------------------------------------------------------
// Helper to draw a big, thick, centered title using a stroke
// (vector) font, since bitmap fonts cannot be scaled or thickened.
// Now using modern gaming text font with outline & shadow
//----------------------------------------------------------------------
void uiStrokeTextCentered(int cx, int cy, const char* text, float scale, float lineWidth)
{
	uiTextClampedCentered(cx, cy, text, 0.44, SCREEN_WIDTH - 60);
}

//----------------------------------------------------------------------
// Sound button is now handled universally across all pages in GameUI.h
//----------------------------------------------------------------------
void drawSoundButton()
{
	// Handled globally in iDraw() via uiDrawSoundToggleButton()
}

//----------------------------------------------------------------------
void menuInit()
{
	uiSetButton(&gMenuButtons[MENU_NEW_GAME], MENU_BTN_X,
		MENU_BTN_TOP_Y - 0 * MENU_BTN_STEP,
		MENU_BTN_W, MENU_BTN_H, "NEW GAME", 1);

	uiSetButton(&gMenuButtons[MENU_RECORDS], MENU_BTN_X,
		MENU_BTN_TOP_Y - 1 * MENU_BTN_STEP,
		MENU_BTN_W, MENU_BTN_H, "HALL OF FAME / RECORDS", 1);

	uiSetButton(&gMenuButtons[MENU_KEYS], MENU_BTN_X,
		MENU_BTN_TOP_Y - 2 * MENU_BTN_STEP,
		MENU_BTN_W, MENU_BTN_H, "KEYS", 1);

	uiSetButton(&gMenuButtons[MENU_ABOUT], MENU_BTN_X,
		MENU_BTN_TOP_Y - 3 * MENU_BTN_STEP,
		MENU_BTN_W, MENU_BTN_H, "ABOUT THE GAME", 1);

	uiSetButton(&gMenuButtons[MENU_EXIT], MENU_BTN_X,
		MENU_BTN_TOP_Y - 4 * MENU_BTN_STEP,
		MENU_BTN_W, MENU_BTN_H, "EXIT", 1);

	uiSetButton(&gMenuSwitchGamer, BANNER_X + BANNER_W + 8, BANNER_Y, 94, BANNER_H, "SWITCH", 1);
}

//----------------------------------------------------------------------
// Runs the currently selected menu entry.
//----------------------------------------------------------------------
void menuActivate(int index)
{
	switch (index)
	{
	case MENU_NEW_GAME:
		// The intro theme stops here and the in-game music takes over.
		audioPlayBackgroundTheme();
		gCurrentScreen = SCREEN_LEVEL_SELECT;
		break;

	case MENU_RECORDS:
		recordsInit();
		gCurrentScreen = SCREEN_RECORDS;
		break;

	case MENU_KEYS:
		gCurrentScreen = SCREEN_KEYS;
		break;

	case MENU_ABOUT:
		gCurrentScreen = SCREEN_ABOUT;
		break;

	case MENU_EXIT:
		requestExit();
		break;
	}
}

//----------------------------------------------------------------------
void menuDraw()
{
	int i;

	drawPosterBackground();

	// game name shown centered directly above the menu box
	iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
	uiStrokeTextCentered(SCREEN_WIDTH / 2, MENU_PANEL_Y + MENU_PANEL_H + 40,
		GAME_TITLE_TEXT, TITLE_FONT_SCALE, TITLE_FONT_THICKNESS);

	// ---- Active Athlete Banner (Top Left) ----
	uiPanel(BANNER_X, BANNER_Y, BANNER_W, BANNER_H,
		COL_PANEL_R, COL_PANEL_G, COL_PANEL_B, 0.88,
		COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.85);

	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 1.0);
	uiTextClamped(BANNER_X + 12, BANNER_Y + 12, "ATHLETE:", 0.30, 65);

	iSetColorA(255, 255, 255, 1.0);
	uiTextClamped(BANNER_X + 80, BANNER_Y + 12, profileGetActiveName(), 0.38, 160);

	char scBuf[32];
	sprintf(scBuf, "%ld PTS", profileGetActive() ? profileGetActive()->totalScore : 0L);
	iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
	uiTextClamped(BANNER_X + 250, BANNER_Y + 12, scBuf, 0.34, 110);

	uiDrawButton(&gMenuSwitchGamer, uiButtonHovered(&gMenuSwitchGamer));

	// ---- Main Menu Panel ----
	uiPanel(MENU_PANEL_X, MENU_PANEL_Y, MENU_PANEL_W, MENU_PANEL_H,
		COL_PANEL_R, COL_PANEL_G, COL_PANEL_B, 0.72,
		COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.90);

	// "MAIN MENU" centered in the box
	iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
	uiTextCentered(MENU_PANEL_X + MENU_PANEL_W / 2, MENU_PANEL_Y + MENU_PANEL_H - 26,
		"MAIN MENU", GLUT_BITMAP_TIMES_ROMAN_24);

	uiRule(MENU_PANEL_X + 22, MENU_PANEL_Y + MENU_PANEL_H - 36,
		MENU_PANEL_W - 44, COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.65);

	for (i = 0; i < MENU_ITEM_COUNT; i++)
		uiDrawButton(&gMenuButtons[i], (i == gMenuIndex));

	// bottom hint strip
	iSetColorA(0, 0, 0, 0.62);
	iFilledRectangle(0, 0, SCREEN_WIDTH, 26);

	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.95);
	uiTextCentered(SCREEN_WIDTH / 2, 9,
		"ARROW KEYS or W / S  -  Navigate        ENTER  -  Select        "
		"ESC  -  Opening Screen        Mouse click also works",
		GLUT_BITMAP_HELVETICA_12);
}

//----------------------------------------------------------------------
void menuMouseMove()
{
	int i;
	for (i = 0; i < MENU_ITEM_COUNT; i++)
	{
		if (uiButtonHovered(&gMenuButtons[i]))
		{
			gMenuIndex = i;          // hovering also moves the selection
			return;
		}
	}
}

void menuMouseClick(int mx, int my)
{
	// Check Switch Gamer button
	if (uiPointInRect(mx, my, gMenuSwitchGamer.x, gMenuSwitchGamer.y,
		gMenuSwitchGamer.w, gMenuSwitchGamer.h))
	{
		profileScreenInit();
		gCurrentScreen = SCREEN_NAME_ENTRY;
		return;
	}

	int i;
	for (i = 0; i < MENU_ITEM_COUNT; i++)
	{
		if (gMenuButtons[i].enabled &&
			uiPointInRect(mx, my, gMenuButtons[i].x, gMenuButtons[i].y,
			gMenuButtons[i].w, gMenuButtons[i].h))
		{
			gMenuIndex = i;
			menuActivate(i);
			return;
		}
	}
}

//----------------------------------------------------------------------
// Called from fixedUpdate(); only "just pressed" events get here.
//----------------------------------------------------------------------
void menuKeyDown(int movedUp, int movedDown, int confirmed)
{
	if (movedUp)
		gMenuIndex = (gMenuIndex - 1 + MENU_ITEM_COUNT) % MENU_ITEM_COUNT;

	if (movedDown)
		gMenuIndex = (gMenuIndex + 1) % MENU_ITEM_COUNT;

	if (confirmed)
		menuActivate(gMenuIndex);
}

#endif