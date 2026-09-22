//======================================================================
//  ScreenLevelSelect.h
//  Reached from NEW GAME. Shows one card per level.
//  Level 02 stays locked until Level 01 has been completed.
//======================================================================
#ifndef SCREEN_LEVEL_SELECT_H
#define SCREEN_LEVEL_SELECT_H

#define LS_CARD_W     222
#define LS_CARD_H     286
#define LS_CARD_Y     132
#define LS_CARD1_X     34
#define LS_CARD2_X    278
#define LS_CARD3_X    522
#define LS_CARD4_X    766

#define LS_ITEM_COUNT   6          // card 1, card 2, card 3, card 4, back button, records button
#define LS_ITEM_LEVEL1  0
#define LS_ITEM_LEVEL2  1
#define LS_ITEM_LEVEL3  2
#define LS_ITEM_LEVEL4  3
#define LS_ITEM_BACK    4
#define LS_ITEM_RECORDS 5

struct Button gLevelSelectBack;
struct Button gLevelSelectRecords;
int gLevelSelectIndex = 0;

void levelSelectInit()
{
	uiSetButton(&gLevelSelectBack, 48, 46, 170, 44, "BACK TO MENU", 1);
	uiSetButton(&gLevelSelectRecords, 230, 46, 190, 44, "HALL OF FAME", 1);
}

int levelCardX(int level)
{
	if (level == 1) return LS_CARD1_X;
	if (level == 2) return LS_CARD2_X;
	if (level == 3) return LS_CARD3_X;
	return LS_CARD4_X;
}

// Forward declarations for level startups
void level03Start();
void level04Start();

//----------------------------------------------------------------------
// One level card.
//----------------------------------------------------------------------
void drawLevelCard(int level, const char *levelText, const char *disciplineName,
	int unlocked, int completed, int focused)
{
	int x = levelCardX(level);
	int y = LS_CARD_Y;
	int cx = x + LS_CARD_W / 2;

	int accentR, accentG, accentB;

	if (!unlocked)      { accentR = COL_GREY_R; accentG = COL_GREY_G; accentB = COL_GREY_B; }
	else if (completed) { accentR = 90;  accentG = 220; accentB = 130; }
	else                { accentR = COL_GOLD_R; accentG = COL_GOLD_G; accentB = COL_GOLD_B; }

	// ---- plate -------------------------------------------------------
	iSetColorA(COL_PANEL_R, COL_PANEL_G, COL_PANEL_B, unlocked ? 0.82 : 0.68);
	iFilledRectangle(x, y, LS_CARD_W, LS_CARD_H);

	iSetColorA(accentR, accentG, accentB, focused ? 1.0 : 0.65);
	uiBorder(x, y, LS_CARD_W, LS_CARD_H, focused ? 3 : 2);

	// ---- header bar --------------------------------------------------
	iSetColorA(accentR, accentG, accentB, unlocked ? 0.92 : 0.55);
	iFilledRectangle(x + 2, y + LS_CARD_H - 48, LS_CARD_W - 4, 46);

	iSetColorA(COL_PANEL_R, COL_PANEL_G, COL_PANEL_B, 1.0);
	uiTextClampedCentered(cx, y + LS_CARD_H - 30, levelText, 0.44, LS_CARD_W - 50);

	// Medal icon in header if earned
	int medal = profileGetLevelMedal(level);
	if (medal > 0)
	{
		drawMiniMedal(x + LS_CARD_W - 24, y + LS_CARD_H - 24, medal);
	}

	// ---- discipline --------------------------------------------------
	iSetColorA(unlocked ? 255 : COL_GREY_R,
		unlocked ? 255 : COL_GREY_G,
		unlocked ? 255 : COL_GREY_B, 1.0);
	uiTextClampedCentered(cx, y + 200, disciplineName, 0.38, LS_CARD_W - 24);

	// ---- icon --------------------------------------------------------
	if (!unlocked)
	{
		uiDrawPadlock(cx, y + 144, 48, COL_GREY_R, COL_GREY_G, COL_GREY_B);
	}
	else
	{
		if (gTexUiPlay)
		{
			iShowImage(cx - 28, y + 144 - 28, 56, 56, gTexUiPlay);
		}
	}

	// ---- Individual Level Personal Best Score ----
	long best = profileGetLevelHighScore(level);
	if (best > 0)
	{
		char bBuf[32];
		sprintf(bBuf, "BEST: %ld PTS", best);
		iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
		uiTextClampedCentered(cx, y + 96, bBuf, 0.36, LS_CARD_W - 20);
	}
	else
	{
		iSetColorA(COL_GREY_R, COL_GREY_G, COL_GREY_B, 0.70);
		uiTextClampedCentered(cx, y + 96, "BEST: NO RECORD", 0.30, LS_CARD_W - 20);
	}

	// ---- status ------------------------------------------------------
	if (!unlocked)
	{
		iSetColorA(COL_RED_R, COL_RED_G, COL_RED_B, 1.0);
		uiTextClampedCentered(cx, y + 62, "LOCKED", 0.40, LS_CARD_W - 24);

		iSetColorA(COL_GREY_R, COL_GREY_G, COL_GREY_B, 1.0);
		if (level == 2)
			uiTextClampedCentered(cx, y + 36, "Complete LEVEL 01 to unlock", 0.30, LS_CARD_W - 24);
		else
			uiTextClampedCentered(cx, y + 36, "Complete LEVEL 02 to unlock", 0.30, LS_CARD_W - 24);
	}
	else if (completed)
	{
		iSetColorA(90, 220, 130, 1.0);
		uiTextClampedCentered(cx, y + 62, "COMPLETED", 0.40, LS_CARD_W - 24);

		iSetColorA(COL_WHITE_R, COL_WHITE_G, COL_WHITE_B, 0.85);
		uiTextClampedCentered(cx, y + 36, "Click or press ENTER to replay", 0.30, LS_CARD_W - 24);
	}
	else
	{
		iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 1.0);
		uiTextClampedCentered(cx, y + 62, "UNLOCKED", 0.40, LS_CARD_W - 24);

		iSetColorA(COL_WHITE_R, COL_WHITE_G, COL_WHITE_B, 0.85);
		uiTextClampedCentered(cx, y + 36, "Click or press ENTER to start", 0.30, LS_CARD_W - 24);
	}
}

//----------------------------------------------------------------------
void levelSelectDraw()
{
	drawPosterBackground();
	uiDimScreen(0.84);

	// ---- Active Athlete Banner (Top Left) ----
	uiPanel(34, SCREEN_HEIGHT - 54, 380, 36,
		COL_PANEL_R, COL_PANEL_G, COL_PANEL_B, 0.88,
		COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.85);

	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 1.0);
	uiTextClamped(46, SCREEN_HEIGHT - 43, "ATHLETE:", 0.30, 65);

	iSetColorA(255, 255, 255, 1.0);
	uiTextClamped(112, SCREEN_HEIGHT - 43, profileGetActiveName(), 0.36, 150);

	char scBuf[32];
	sprintf(scBuf, "%ld PTS", profileGetActive() ? profileGetActive()->totalScore : 0L);
	iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
	uiTextClamped(275, SCREEN_HEIGHT - 43, scBuf, 0.34, 110);

	iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
	uiTextScaledCentered(SCREEN_WIDTH / 2, 494, "SELECT YOUR CHALLENGE", 0.30, 2.5);

	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.95);
	uiTextCentered(SCREEN_WIDTH / 2, 456,
		"All stages are unlocked - select any challenge to play",
		GLUT_BITMAP_HELVETICA_12);

	uiRule(SCREEN_WIDTH / 2 - 220, 442, 440,
		COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.55);

	drawLevelCard(1, "LEVEL 01", LEVEL_1_NAME,
		isLevelUnlocked(1), gLevelCompleted[1],
		gLevelSelectIndex == LS_ITEM_LEVEL1);

	drawLevelCard(2, "LEVEL 02", LEVEL_2_NAME,
		isLevelUnlocked(2), gLevelCompleted[2],
		gLevelSelectIndex == LS_ITEM_LEVEL2);

	drawLevelCard(3, "LEVEL 03", LEVEL_3_NAME,
		isLevelUnlocked(3), gLevelCompleted[3],
		gLevelSelectIndex == LS_ITEM_LEVEL3);

	drawLevelCard(4, "LEVEL 04", LEVEL_4_NAME,
		isLevelUnlocked(4), gLevelCompleted[4],
		gLevelSelectIndex == LS_ITEM_LEVEL4);

	gLevelSelectBack.enabled = 1;
	uiDrawButton(&gLevelSelectBack, gLevelSelectIndex == LS_ITEM_BACK);

	gLevelSelectRecords.enabled = 1;
	uiDrawButton(&gLevelSelectRecords, gLevelSelectIndex == LS_ITEM_RECORDS);

	// bottom hint strip
	iSetColorA(0, 0, 0, 0.62);
	iFilledRectangle(0, 0, SCREEN_WIDTH, 26);

	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.95);
	uiTextCentered(SCREEN_WIDTH / 2, 9,
		"LEFT / RIGHT  -  Choose        ENTER  -  Start        ESC  -  Back to menu",
		GLUT_BITMAP_HELVETICA_12);
}

//----------------------------------------------------------------------
void levelSelectStart(int level)
{
	if (!isLevelUnlocked(level))
		return;                       // locked: ignore the request

	if (level == 1)
	{
		level01Start();               // builds the course and starts the theme
		gCurrentScreen = SCREEN_LEVEL_1;
	}
	else if (level == 2)
	{
		level02Start();               // builds the course and starts the theme
		gCurrentScreen = SCREEN_LEVEL_2;
	}
	else if (level == 3)
	{
		level03Start();               // builds the course and starts the theme
		gCurrentScreen = SCREEN_LEVEL_3;
	}
	else if (level == 4)
	{
		level04Start();               // builds the course and starts the theme
		gCurrentScreen = SCREEN_LEVEL_4;
	}
}

void levelSelectActivate(int index)
{
	if (index == LS_ITEM_LEVEL1) levelSelectStart(1);
	else if (index == LS_ITEM_LEVEL2) levelSelectStart(2);
	else if (index == LS_ITEM_LEVEL3) levelSelectStart(3);
	else if (index == LS_ITEM_LEVEL4) levelSelectStart(4);
	else if (index == LS_ITEM_BACK)
	{
		audioPlayIntroTheme();
		gCurrentScreen = SCREEN_MENU;
	}
	else if (index == LS_ITEM_RECORDS)
	{
		recordsInit();
		gCurrentScreen = SCREEN_RECORDS;
	}
}

//----------------------------------------------------------------------
void levelSelectMouseMove()
{
	if (uiPointInRect(gMouseX, gMouseY, LS_CARD1_X, LS_CARD_Y, LS_CARD_W, LS_CARD_H))
		gLevelSelectIndex = LS_ITEM_LEVEL1;
	else if (uiPointInRect(gMouseX, gMouseY, LS_CARD2_X, LS_CARD_Y, LS_CARD_W, LS_CARD_H))
		gLevelSelectIndex = LS_ITEM_LEVEL2;
	else if (uiPointInRect(gMouseX, gMouseY, LS_CARD3_X, LS_CARD_Y, LS_CARD_W, LS_CARD_H))
		gLevelSelectIndex = LS_ITEM_LEVEL3;
	else if (uiPointInRect(gMouseX, gMouseY, LS_CARD4_X, LS_CARD_Y, LS_CARD_W, LS_CARD_H))
		gLevelSelectIndex = LS_ITEM_LEVEL4;
	else if (uiButtonHovered(&gLevelSelectBack))
		gLevelSelectIndex = LS_ITEM_BACK;
	else if (uiButtonHovered(&gLevelSelectRecords))
		gLevelSelectIndex = LS_ITEM_RECORDS;
}

void levelSelectMouseClick(int mx, int my)
{
	if (uiPointInRect(mx, my, LS_CARD1_X, LS_CARD_Y, LS_CARD_W, LS_CARD_H))
		levelSelectActivate(LS_ITEM_LEVEL1);
	else if (uiPointInRect(mx, my, LS_CARD2_X, LS_CARD_Y, LS_CARD_W, LS_CARD_H))
		levelSelectActivate(LS_ITEM_LEVEL2);
	else if (uiPointInRect(mx, my, LS_CARD3_X, LS_CARD_Y, LS_CARD_W, LS_CARD_H))
		levelSelectActivate(LS_ITEM_LEVEL3);
	else if (uiPointInRect(mx, my, LS_CARD4_X, LS_CARD_Y, LS_CARD_W, LS_CARD_H))
		levelSelectActivate(LS_ITEM_LEVEL4);
	else if (uiPointInRect(mx, my, gLevelSelectBack.x, gLevelSelectBack.y,
		gLevelSelectBack.w, gLevelSelectBack.h))
		levelSelectActivate(LS_ITEM_BACK);
	else if (uiPointInRect(mx, my, gLevelSelectRecords.x, gLevelSelectRecords.y,
		gLevelSelectRecords.w, gLevelSelectRecords.h))
		levelSelectActivate(LS_ITEM_RECORDS);
}

void levelSelectKeyDown(int movedLeft, int movedRight, int confirmed)
{
	if (movedLeft)
		gLevelSelectIndex = (gLevelSelectIndex - 1 + LS_ITEM_COUNT) % LS_ITEM_COUNT;

	if (movedRight)
		gLevelSelectIndex = (gLevelSelectIndex + 1) % LS_ITEM_COUNT;

	if (confirmed)
		levelSelectActivate(gLevelSelectIndex);
}

#endif
