//======================================================================
//  ScreenProfile.h
//  Iron Man Challenge: The Ultimate Training
//
//  State-of-the-Art Gamer Name Entry & Athlete Profile Interface.
//  Stark Industries / Iron Man HUD themed with glowing arc-reactor,
//  cybernetic terminal styling, live typing cursor, and instant
//  profile switching.
//======================================================================
#ifndef SCREEN_PROFILE_H
#define SCREEN_PROFILE_H

#include <string.h>
#include <ctype.h>

#define PR_PANEL_W       740
#define PR_PANEL_H       450
#define PR_PANEL_X       ((SCREEN_WIDTH  - PR_PANEL_W) / 2)
#define PR_PANEL_Y       ((SCREEN_HEIGHT - PR_PANEL_H) / 2 - 12)

#define PR_INPUT_W       480
#define PR_INPUT_H       48
#define PR_INPUT_X       (PR_PANEL_X + (PR_PANEL_W - PR_INPUT_W) / 2)
#define PR_INPUT_Y       (PR_PANEL_Y + PR_PANEL_H - 180)

#define PR_BTN_CONFIRM   0
#define PR_BTN_CLEAR     1
#define PR_BTN_GUEST     2
#define PR_BTN_BACK      3
#define PR_BTN_COUNT     4

struct Button gProfileButtons[PR_BTN_COUNT];

char   gInputGamerName[MAX_GAMER_NAME] = {0};
int    gInputCursorPos = 0;
double gInputCursorTimer = 0.0;
int    gProfileHoverChip = -1;

//----------------------------------------------------------------------
// Forward declaration
//----------------------------------------------------------------------
void drawPosterBackground();

//----------------------------------------------------------------------
// Initialize Screen & Buttons
//----------------------------------------------------------------------
void profileScreenInit()
{
	int btnY = PR_INPUT_Y - 64;
	int btnW = 180;
	int btnH = 44;
	int gap = 16;
	int totalW = btnW * 3 + gap * 2;
	int startX = PR_PANEL_X + (PR_PANEL_W - totalW) / 2;

	uiSetButton(&gProfileButtons[PR_BTN_CONFIRM], startX, btnY, btnW, btnH, "ENTER TRAINING", 1);
	uiSetButton(&gProfileButtons[PR_BTN_CLEAR], startX + btnW + gap, btnY, btnW, btnH, "CLEAR NAME", 1);
	uiSetButton(&gProfileButtons[PR_BTN_GUEST], startX + (btnW + gap) * 2, btnY, btnW, btnH, "PLAY AS GUEST", 1);
	uiSetButton(&gProfileButtons[PR_BTN_BACK], PR_PANEL_X + 24, PR_PANEL_Y + 18, 140, 34, "MAIN MENU", 1);

	// Pre-fill with active profile if available
	struct GamerProfile *active = profileGetActive();
	if (active && active->name[0])
	{
		strncpy(gInputGamerName, active->name, MAX_GAMER_NAME - 1);
		gInputGamerName[MAX_GAMER_NAME - 1] = '\0';
		gInputCursorPos = (int)strlen(gInputGamerName);
	}
	else if (gProfileCount > 0 && gProfiles[0].name[0])
	{
		strncpy(gInputGamerName, gProfiles[0].name, MAX_GAMER_NAME - 1);
		gInputGamerName[MAX_GAMER_NAME - 1] = '\0';
		gInputCursorPos = (int)strlen(gInputGamerName);
	}
	else
	{
		gInputGamerName[0] = '\0';
		gInputCursorPos = 0;
	}
}

//----------------------------------------------------------------------
// Draw Glowing Stark Arc Reactor Emblem (pure image-based, zero procedural vector idraw)
//----------------------------------------------------------------------
static void drawArcReactorEmblem(int cx, int cy, double pulse)
{
	int size = (int)(54.0 + sin(pulse * 4.0) * 4.0);
	if (gTexUiArcReactor)
	{
		iShowImage(cx - size / 2, cy - size / 2, size, size, gTexUiArcReactor);
	}
}

//----------------------------------------------------------------------
// Draw Sci-Fi Tech Corner Brackets
//----------------------------------------------------------------------
static void drawCornerBrackets(int x, int y, int w, int h, int len, int r, int g, int b, double alpha)
{
	iSetColorA(r, g, b, alpha);
	// Bottom-Left
	iFilledRectangle(x, y, len, 3);
	iFilledRectangle(x, y, 3, len);
	// Bottom-Right
	iFilledRectangle(x + w - len, y, len, 3);
	iFilledRectangle(x + w - 3, y, 3, len);
	// Top-Left
	iFilledRectangle(x, y + h - 3, len, 3);
	iFilledRectangle(x, y + h - len, 3, len);
	// Top-Right
	iFilledRectangle(x + w - len, y + h - 3, len, 3);
	iFilledRectangle(x + w - 3, y + h - len, 3, len);
}

//----------------------------------------------------------------------
// Confirm & Submit Current Gamer Name
//----------------------------------------------------------------------
void profileScreenConfirm()
{
	char nameToUse[MAX_GAMER_NAME];
	if (gInputGamerName[0] == '\0')
	{
		strcpy(nameToUse, "GUEST ATHLETE");
	}
	else
	{
		strncpy(nameToUse, gInputGamerName, MAX_GAMER_NAME - 1);
		nameToUse[MAX_GAMER_NAME - 1] = '\0';
	}

	profileSelectOrAdd(nameToUse);
	audioPlayBackgroundTheme();
	gCurrentScreen = SCREEN_MENU;
}

//----------------------------------------------------------------------
// Main Draw Routine
//----------------------------------------------------------------------
void profileScreenDraw()
{
	drawPosterBackground();
	uiDimScreen(0.85);

	gInputCursorTimer += 0.024;

	int cx = SCREEN_WIDTH / 2;

	// ---- Outer Glassmorphic Terminal Panel ----
	uiPanel(PR_PANEL_X, PR_PANEL_Y, PR_PANEL_W, PR_PANEL_H,
		COL_PANEL_R, COL_PANEL_G, COL_PANEL_B, 0.88,
		COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.75);

	// Corner tech brackets
	drawCornerBrackets(PR_PANEL_X - 4, PR_PANEL_Y - 4, PR_PANEL_W + 8, PR_PANEL_H + 8, 24,
		COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.95);

	// ---- Header with Arc Reactor Emblem ----
	int arcX = PR_PANEL_X + 54;
	int arcY = PR_PANEL_Y + PR_PANEL_H - 46;
	drawArcReactorEmblem(arcX, arcY, gInputCursorTimer);

	iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
	uiTextScaled(PR_PANEL_X + 90, arcY + 4, "ATHLETE IDENTIFICATION", 0.22, 2.2);

	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.95);
	uiTextClamped(PR_PANEL_X + 90, arcY - 18, "IRON MAN CHALLENGE: THE ULTIMATE TRAINING - PILOT DOSSIER", 0.32, PR_PANEL_W - 120);

	uiRule(PR_PANEL_X + 24, PR_PANEL_Y + PR_PANEL_H - 84, PR_PANEL_W - 48,
		COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.60);

	// ---- Instruction / Prompt ----
	iSetColorA(255, 255, 255, 1.0);
	uiTextClampedCentered(cx, PR_INPUT_Y + PR_INPUT_H + 20,
		"ENTER YOUR CALLSIGN / GAMER NAME TO ACCESS TRAINING", 0.44, PR_PANEL_W - 60);

	// ---- Cybernetic Text Input Box ----
	// Ambient glow under input
	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.18);
	iFilledRectangle(PR_INPUT_X - 3, PR_INPUT_Y - 3, PR_INPUT_W + 6, PR_INPUT_H + 6);

	// Input background
	iSetColorA(8, 14, 28, 0.95);
	iFilledRectangle(PR_INPUT_X, PR_INPUT_Y, PR_INPUT_W, PR_INPUT_H);

	// Dual borders with Gold focus
	iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.95);
	uiBorder(PR_INPUT_X, PR_INPUT_Y, PR_INPUT_W, PR_INPUT_H, 2);

	// Input box left accent tag
	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 1.0);
	iFilledRectangle(PR_INPUT_X, PR_INPUT_Y, 6, PR_INPUT_H);

	// Display entered name with blinking cursor
	char displayText[MAX_GAMER_NAME + 4] = {0};
	strncpy(displayText, gInputGamerName, MAX_GAMER_NAME - 1);

	int showCursor = ((int)(gInputCursorTimer * 2.2) % 2 == 0);
	if (showCursor && strlen(displayText) < MAX_GAMER_NAME - 1)
	{
		strcat(displayText, "_");
	}

	iSetColorA(255, 255, 255, 1.0);
	if (gInputGamerName[0] == '\0' && !showCursor)
	{
		iSetColorA(COL_GREY_R, COL_GREY_G, COL_GREY_B, 0.65);
		uiTextClamped(PR_INPUT_X + 20, PR_INPUT_Y + 14, "TYPE YOUR NAME HERE...", 0.44, PR_INPUT_W - 80);
	}
	else
	{
		uiTextClamped(PR_INPUT_X + 20, PR_INPUT_Y + 14, displayText, 0.50, PR_INPUT_W - 80);
	}

	// Character counter on the right edge of input box
	char countBuf[16];
	sprintf(countBuf, "%d/%d", (int)strlen(gInputGamerName), MAX_GAMER_NAME - 1);
	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.75);
	uiTextClamped(PR_INPUT_X + PR_INPUT_W - 54, PR_INPUT_Y + 15, countBuf, 0.32, 50);

	// Status feedback line below input box
	int existingIdx = profileFindIndex(gInputGamerName);
	if (gInputGamerName[0] == '\0')
	{
		iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.85);
		uiTextClampedCentered(cx, PR_INPUT_Y - 18, "Type using your keyboard or click an athlete below", 0.34, PR_INPUT_W);
	}
	else if (existingIdx >= 0)
	{
		char statBuf[128];
		sprintf(statBuf, "[ ACTIVE ATHLETE FOUND: %s | CAREER: %ld PTS | %s ]",
			gProfiles[existingIdx].name,
			gProfiles[existingIdx].totalScore,
			profileGetRankTitle(gProfiles[existingIdx].totalScore));
		iSetColorA(90, 230, 140, 1.0);
		uiTextClampedCentered(cx, PR_INPUT_Y - 18, statBuf, 0.34, PR_PANEL_W - 60);
	}
	else
	{
		iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.95);
		uiTextClampedCentered(cx, PR_INPUT_Y - 18, "[ NEW ATHLETE REGISTRATION - FRESH RECORD WILL BE CREATED ]", 0.34, PR_PANEL_W - 60);
	}

	// ---- Action Buttons ----
	int b;
	for (b = 0; b < PR_BTN_COUNT - 1; b++)
	{
		uiDrawButton(&gProfileButtons[b], uiButtonHovered(&gProfileButtons[b]));
	}

	// Show "MAIN MENU" back button only if we already have an active profile
	if (profileGetActive() != NULL)
	{
		uiDrawButton(&gProfileButtons[PR_BTN_BACK], uiButtonHovered(&gProfileButtons[PR_BTN_BACK]));
	}

	// ---- Saved Athlete Quick-Select Shelf ----
	int shelfY = PR_PANEL_Y + 48;
	iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.75);
	uiTextClamped(PR_PANEL_X + 24, shelfY + 54, "PREVIOUS ATHLETES / SAVED PROFILES:", 0.33, 300);

	uiRule(PR_PANEL_X + 24, shelfY + 46, PR_PANEL_W - 48, COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.35);

	if (gProfileCount == 0)
	{
		iSetColorA(COL_GREY_R, COL_GREY_G, COL_GREY_B, 0.70);
		uiTextClamped(PR_PANEL_X + 24, shelfY + 16, "No saved athlete profiles yet. Enter your callsign above to begin!", 0.32, PR_PANEL_W - 48);
	}
	else
	{
		// Render up to 4 quick profile chips
		int showCount = (gProfileCount > 4) ? 4 : gProfileCount;
		int chipW = (PR_PANEL_W - 48 - (showCount - 1) * 12) / showCount;
		int chipH = 38;
		int c;

		for (c = 0; c < showCount; c++)
		{
			int chipX = PR_PANEL_X + 24 + c * (chipW + 12);
			int hovered = (gProfileHoverChip == c);
			int isCurrent = (stricmp(gProfiles[c].name, gInputGamerName) == 0);

			if (isCurrent)
			{
				iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.28);
				iFilledRectangle(chipX, shelfY, chipW, chipH);
				iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
				uiBorder(chipX, shelfY, chipW, chipH, 2);
			}
			else if (hovered)
			{
				iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.25);
				iFilledRectangle(chipX, shelfY, chipW, chipH);
				iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 1.0);
				uiBorder(chipX, shelfY, chipW, chipH, 1);
			}
			else
			{
				iSetColorA(14, 20, 36, 0.70);
				iFilledRectangle(chipX, shelfY, chipW, chipH);
				iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.45);
				uiBorder(chipX, shelfY, chipW, chipH, 1);
			}

			// Name & Score on chip
			iSetColorA(255, 255, 255, 1.0);
			uiTextClamped(chipX + 8, shelfY + 20, gProfiles[c].name, 0.34, chipW - 16);

			char ptsBuf[32];
			sprintf(ptsBuf, "%ld PTS", gProfiles[c].totalScore);
			iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.90);
			uiTextClamped(chipX + 8, shelfY + 6, ptsBuf, 0.28, chipW - 16);
		}
	}

	// ---- Bottom Hint Strip ----
	iSetColorA(0, 0, 0, 0.65);
	iFilledRectangle(0, 0, SCREEN_WIDTH, 26);

	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.95);
	uiTextCentered(SCREEN_WIDTH / 2, 9,
		"TYPE NAME  -  Use Keyboard        ENTER  -  Confirm & Enter        "
		"BACKSPACE  -  Delete        ESC  -  Main Menu (if profile active)",
		GLUT_BITMAP_HELVETICA_12);
}

//----------------------------------------------------------------------
// Mouse Interactions
//----------------------------------------------------------------------
void profileScreenMouseMove()
{
	int shelfY = PR_PANEL_Y + 48;
	int showCount = (gProfileCount > 4) ? 4 : gProfileCount;
	if (showCount > 0)
	{
		int chipW = (PR_PANEL_W - 48 - (showCount - 1) * 12) / showCount;
		int chipH = 38;
		int c;
		gProfileHoverChip = -1;
		for (c = 0; c < showCount; c++)
		{
			int chipX = PR_PANEL_X + 24 + c * (chipW + 12);
			if (uiPointInRect(gMouseX, gMouseY, chipX, shelfY, chipW, chipH))
			{
				gProfileHoverChip = c;
				return;
			}
		}
	}
	else
	{
		gProfileHoverChip = -1;
	}
}

void profileScreenMouseClick(int mx, int my)
{
	// Check Buttons
	if (uiPointInRect(mx, my, gProfileButtons[PR_BTN_CONFIRM].x, gProfileButtons[PR_BTN_CONFIRM].y,
		gProfileButtons[PR_BTN_CONFIRM].w, gProfileButtons[PR_BTN_CONFIRM].h))
	{
		profileScreenConfirm();
		return;
	}

	if (uiPointInRect(mx, my, gProfileButtons[PR_BTN_CLEAR].x, gProfileButtons[PR_BTN_CLEAR].y,
		gProfileButtons[PR_BTN_CLEAR].w, gProfileButtons[PR_BTN_CLEAR].h))
	{
		gInputGamerName[0] = '\0';
		gInputCursorPos = 0;
		return;
	}

	if (uiPointInRect(mx, my, gProfileButtons[PR_BTN_GUEST].x, gProfileButtons[PR_BTN_GUEST].y,
		gProfileButtons[PR_BTN_GUEST].w, gProfileButtons[PR_BTN_GUEST].h))
	{
		strcpy(gInputGamerName, "GUEST");
		gInputCursorPos = (int)strlen(gInputGamerName);
		profileScreenConfirm();
		return;
	}

	if (profileGetActive() != NULL &&
		uiPointInRect(mx, my, gProfileButtons[PR_BTN_BACK].x, gProfileButtons[PR_BTN_BACK].y,
		gProfileButtons[PR_BTN_BACK].w, gProfileButtons[PR_BTN_BACK].h))
	{
		gCurrentScreen = SCREEN_MENU;
		return;
	}

	// Check Profile Chips
	int shelfY = PR_PANEL_Y + 48;
	int showCount = (gProfileCount > 4) ? 4 : gProfileCount;
	if (showCount > 0)
	{
		int chipW = (PR_PANEL_W - 48 - (showCount - 1) * 12) / showCount;
		int chipH = 38;
		int c;
		for (c = 0; c < showCount; c++)
		{
			int chipX = PR_PANEL_X + 24 + c * (chipW + 12);
			if (uiPointInRect(mx, my, chipX, shelfY, chipW, chipH))
			{
				strncpy(gInputGamerName, gProfiles[c].name, MAX_GAMER_NAME - 1);
				gInputGamerName[MAX_GAMER_NAME - 1] = '\0';
				gInputCursorPos = (int)strlen(gInputGamerName);
				return;
			}
		}
	}
}

//----------------------------------------------------------------------
// Character & Key Input Handler
//----------------------------------------------------------------------
void profileScreenCharInput(unsigned char c)
{
	if (c == 8) // Backspace
	{
		int len = (int)strlen(gInputGamerName);
		if (len > 0)
		{
			gInputGamerName[len - 1] = '\0';
			gInputCursorPos = len - 1;
		}
		return;
	}

	if (c == 13) // Enter
	{
		profileScreenConfirm();
		return;
	}

	if (c == 27) // Escape
	{
		if (profileGetActive() != NULL)
		{
			gCurrentScreen = SCREEN_MENU;
		}
		return;
	}

	// Alphanumeric, spaces, hyphens, underscores
	if ((isalnum(c) || c == ' ' || c == '-' || c == '_') && strlen(gInputGamerName) < MAX_GAMER_NAME - 2)
	{
		int len = (int)strlen(gInputGamerName);
		// Avoid leading space or double space
		if (c == ' ' && (len == 0 || gInputGamerName[len - 1] == ' '))
			return;

		gInputGamerName[len] = (char)c;
		gInputGamerName[len + 1] = '\0';
		gInputCursorPos = len + 1;
	}
}

#endif
