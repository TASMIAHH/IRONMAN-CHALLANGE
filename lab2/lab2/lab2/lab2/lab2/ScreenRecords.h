//======================================================================
//  ScreenRecords.h
//  Iron Man Challenge: The Ultimate Training
//
//  Hall of Fame & Athlete Dossier Screen.
//  Displays individual high scores for each level, medals earned,
//  career statistics, and a ranked global leaderboard of all athletes.
//======================================================================
#ifndef SCREEN_RECORDS_H
#define SCREEN_RECORDS_H

#define REC_PANEL_X      44
#define REC_PANEL_Y      42
#define REC_PANEL_W     936
#define REC_PANEL_H     488

#define REC_LEFT_W      430
#define REC_RIGHT_W     464
#define REC_RIGHT_X     (REC_PANEL_X + REC_LEFT_W + 18)

struct Button gRecordsBack;
struct Button gRecordsSwitch;

void recordsInit()
{
	uiSetButton(&gRecordsBack, REC_PANEL_X + 24, REC_PANEL_Y + 16, 170, 38, "BACK TO MENU", 1);
	uiSetButton(&gRecordsSwitch, REC_PANEL_X + 210, REC_PANEL_Y + 16, 210, 38, "CHANGE ATHLETE", 1);
}

//----------------------------------------------------------------------
// Draw Mini Medal Icon (pure image-based, zero procedural vector idraw)
//----------------------------------------------------------------------
static void drawMiniMedal(int cx, int cy, int medal)
{
	if (medal <= 0) return;

	unsigned int tex = gTexUiMedalBronze;
	if (medal == 3) tex = gTexUiMedalGold;
	else if (medal == 2) tex = gTexUiMedalSilver;

	if (tex)
	{
		iShowImage(cx - 14, cy - 14, 28, 28, tex);
	}
}

//----------------------------------------------------------------------
// Main Draw Routine
//----------------------------------------------------------------------
void recordsDraw()
{
	drawPosterBackground();
	uiDimScreen(0.86);

	// Outer glassmorphic frame
	uiPanel(REC_PANEL_X, REC_PANEL_Y, REC_PANEL_W, REC_PANEL_H,
		COL_PANEL_R, COL_PANEL_G, COL_PANEL_B, 0.88,
		COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.70);

	// Header
	iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
	uiTextScaled(REC_PANEL_X + 24, REC_PANEL_Y + REC_PANEL_H - 32, "IRON MAN HALL OF FAME", 0.25, 2.4);

	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.90);
	uiTextClamped(REC_PANEL_X + 370, REC_PANEL_Y + REC_PANEL_H - 32,
		"OFFICIAL ATHLETE DOSSIER & LEADERBOARDS", 0.32, 540);

	uiRule(REC_PANEL_X + 24, REC_PANEL_Y + REC_PANEL_H - 46, REC_PANEL_W - 48,
		COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.55);

	// ==================================================================
	// LEFT COLUMN: ACTIVE ATHLETE DOSSIER
	// ==================================================================
	struct GamerProfile *active = profileGetActive();
	int cardY = REC_PANEL_Y + REC_PANEL_H - 60;

	// Active athlete banner card
	uiPanel(REC_PANEL_X + 24, cardY - 76, REC_LEFT_W, 72,
		14, 22, 42, 0.85,
		COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.85);

	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.80);
	uiTextClamped(REC_PANEL_X + 38, cardY - 24, "CURRENT ATHLETE PROFILE", 0.28, 200);

	iSetColorA(255, 255, 255, 1.0);
	uiTextClamped(REC_PANEL_X + 38, cardY - 48, active ? active->name : "NO ATHLETE SELECTED", 0.52, 240);

	iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
	uiTextClamped(REC_PANEL_X + 38, cardY - 68, active ? profileGetRankTitle(active->totalScore) : "UNRANKED", 0.32, 240);

	// Total score & runs on the right of athlete card
	char scoreBuf[64];
	sprintf(scoreBuf, "%ld PTS", active ? active->totalScore : 0L);
	iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
	uiTextClamped(REC_PANEL_X + 290, cardY - 38, scoreBuf, 0.48, 150);

	char runsBuf[64];
	sprintf(runsBuf, "RUNS: %d", active ? active->gamesPlayed : 0);
	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.90);
	uiTextClamped(REC_PANEL_X + 290, cardY - 64, runsBuf, 0.30, 150);

	// Level-by-level cards
	int lvlY = cardY - 92;
	int lvlH = 64;
	int gap = 8;
	const char *lvlTitles[5] = { "", "LEVEL 01: RUNNING TRAINING", "LEVEL 02: CLIMBING CHALLENGE", "LEVEL 03: CYCLING EXPEDITION", "LEVEL 04: SWIMMING EXPEDITION" };

	int l;
	for (l = 1; l <= TOTAL_LEVELS; l++)
	{
		int py = lvlY - (l - 1) * (lvlH + gap) - lvlH;
		long lScore = active ? active->levelHighScore[l] : 0L;
		int lMedal = active ? active->levelMedal[l] : 0;
		int lComp  = active ? active->levelCompleted[l] : 0;

		uiPanel(REC_PANEL_X + 24, py, REC_LEFT_W, lvlH,
			10, 16, 32, 0.80,
			COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.45);

		// Left accent bar
		iSetColorA(lComp ? 90 : COL_GOLD_R, lComp ? 220 : COL_GOLD_G, lComp ? 130 : COL_GOLD_B, 0.90);
		iFilledRectangle(REC_PANEL_X + 24, py, 4, lvlH);

		// Level Title
		iSetColorA(240, 245, 255, 1.0);
		uiTextClamped(REC_PANEL_X + 38, py + 42, lvlTitles[l], 0.33, 260);

		// High Score
		char hsBuf[64];
		if (lScore > 0)
			sprintf(hsBuf, "BEST SCORE: %ld PTS", lScore);
		else
			sprintf(hsBuf, "BEST SCORE: NO RECORD");

		iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.95);
		uiTextClamped(REC_PANEL_X + 38, py + 22, hsBuf, 0.34, 250);

		// Status & Medal
		drawMiniMedal(REC_PANEL_X + REC_LEFT_W - 30, py + 34, lMedal);

		char medalBuf[32];
		sprintf(medalBuf, "MEDAL: %s", profileGetMedalName(lMedal));
		iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.85);
		uiTextClamped(REC_PANEL_X + 38, py + 6, medalBuf, 0.28, 160);

		iSetColorA(lComp ? 90 : COL_GREY_R, lComp ? 220 : COL_GREY_G, lComp ? 130 : COL_GREY_B, 1.0);
		uiTextClamped(REC_PANEL_X + 200, py + 6, lComp ? "[ COMPLETED ]" : "[ UNCOMPLETED ]", 0.28, 140);
	}

	// ==================================================================
	// RIGHT COLUMN: GLOBAL HALL OF FAME / LEADERBOARD
	// ==================================================================
	uiPanel(REC_RIGHT_X, REC_PANEL_Y + 64, REC_RIGHT_W, REC_PANEL_H - 124,
		8, 14, 28, 0.88,
		COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.65);

	// Table Header
	int thY = REC_PANEL_Y + REC_PANEL_H - 96;
	iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
	uiTextClamped(REC_RIGHT_X + 16, thY, "GLOBAL LEADERBOARD - TOP ATHLETES", 0.36, 400);

	uiRule(REC_RIGHT_X + 16, thY - 8, REC_RIGHT_W - 32, COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.40);

	// Column titles
	int colY = thY - 26;
	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.85);
	uiTextClamped(REC_RIGHT_X + 16, colY, "RANK", 0.28, 50);
	uiTextClamped(REC_RIGHT_X + 66, colY, "CALLSIGN", 0.28, 120);
	uiTextClamped(REC_RIGHT_X + 200, colY, "L1", 0.28, 40);
	uiTextClamped(REC_RIGHT_X + 248, colY, "L2", 0.28, 40);
	uiTextClamped(REC_RIGHT_X + 296, colY, "L3", 0.28, 40);
	uiTextClamped(REC_RIGHT_X + 344, colY, "L4", 0.28, 40);
	uiTextClamped(REC_RIGHT_X + 394, colY, "TOTAL", 0.28, 60);

	uiRule(REC_RIGHT_X + 16, colY - 6, REC_RIGHT_W - 32, COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.25);

	// Sort profiles by total score descending for display
	int sortedIndices[MAX_SAVED_PROFILES];
	int i, j;
	for (i = 0; i < gProfileCount; i++) sortedIndices[i] = i;

	for (i = 0; i < gProfileCount - 1; i++)
	{
		for (j = i + 1; j < gProfileCount; j++)
		{
			if (gProfiles[sortedIndices[j]].totalScore > gProfiles[sortedIndices[i]].totalScore)
			{
				int tmp = sortedIndices[i];
				sortedIndices[i] = sortedIndices[j];
				sortedIndices[j] = tmp;
			}
		}
	}

	// Render rows
	int rowY = colY - 24;
	int rowH = 28;
	int maxRows = 9;
	int displayCount = (gProfileCount > maxRows) ? maxRows : gProfileCount;

	if (displayCount == 0)
	{
		iSetColorA(COL_GREY_R, COL_GREY_G, COL_GREY_B, 0.70);
		uiTextClamped(REC_RIGHT_X + 30, rowY - 20, "No records recorded yet. Complete challenges to rank!", 0.32, REC_RIGHT_W - 60);
	}

	for (i = 0; i < displayCount; i++)
	{
		int pIdx = sortedIndices[i];
		struct GamerProfile *p = &gProfiles[pIdx];
		int isCurrent = (pIdx == gActiveProfileIndex);

		if (isCurrent)
		{
			iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.20);
			iFilledRectangle(REC_RIGHT_X + 10, rowY - 4, REC_RIGHT_W - 20, rowH);
			iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.80);
			uiBorder(REC_RIGHT_X + 10, rowY - 4, REC_RIGHT_W - 20, rowH, 1);
		}
		else if (i % 2 == 1)
		{
			iSetColorA(255, 255, 255, 0.04);
			iFilledRectangle(REC_RIGHT_X + 10, rowY - 4, REC_RIGHT_W - 20, rowH);
		}

		// Rank indicator
		char rkBuf[16];
		sprintf(rkBuf, "#%d", i + 1);
		if (i == 0)      iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
		else if (i == 1) iSetColorA(215, 225, 235, 1.0);
		else if (i == 2) iSetColorA(205, 135, 75, 1.0);
		else             iSetColorA(COL_GREY_R, COL_GREY_G, COL_GREY_B, 1.0);
		uiTextClamped(REC_RIGHT_X + 16, rowY + 6, rkBuf, 0.30, 40);

		// Name
		iSetColorA(isCurrent ? 255 : 230, isCurrent ? 255 : 235, isCurrent ? 255 : 245, 1.0);
		uiTextClamped(REC_RIGHT_X + 66, rowY + 6, p->name, 0.32, 120);

		// Level scores
		char sBuf[32];
		sprintf(sBuf, "%ld", p->levelHighScore[1]);
		iSetColorA(COL_WHITE_R, COL_WHITE_G, COL_WHITE_B, 0.85);
		uiTextClamped(REC_RIGHT_X + 200, rowY + 6, sBuf, 0.28, 40);

		sprintf(sBuf, "%ld", p->levelHighScore[2]);
		uiTextClamped(REC_RIGHT_X + 248, rowY + 6, sBuf, 0.28, 40);

		sprintf(sBuf, "%ld", p->levelHighScore[3]);
		uiTextClamped(REC_RIGHT_X + 296, rowY + 6, sBuf, 0.28, 40);

		sprintf(sBuf, "%ld", p->levelHighScore[4]);
		uiTextClamped(REC_RIGHT_X + 344, rowY + 6, sBuf, 0.28, 40);

		// Total
		sprintf(sBuf, "%ld", p->totalScore);
		iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
		uiTextClamped(REC_RIGHT_X + 394, rowY + 6, sBuf, 0.32, 60);

		rowY -= (rowH + 2);
	}

	// ---- Bottom Action Buttons ----
	uiDrawButton(&gRecordsBack, uiButtonHovered(&gRecordsBack));
	uiDrawButton(&gRecordsSwitch, uiButtonHovered(&gRecordsSwitch));

	// Bottom Hint Strip
	iSetColorA(0, 0, 0, 0.65);
	iFilledRectangle(0, 0, SCREEN_WIDTH, 26);

	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.95);
	uiTextCentered(SCREEN_WIDTH / 2, 9,
		"ESC  -  Back to Menu        Mouse Click  -  Switch Athlete or Navigate",
		GLUT_BITMAP_HELVETICA_12);
}

//----------------------------------------------------------------------
// Mouse Interactions
//----------------------------------------------------------------------
void recordsMouseClick(int mx, int my)
{
	if (uiPointInRect(mx, my, gRecordsBack.x, gRecordsBack.y, gRecordsBack.w, gRecordsBack.h))
	{
		gCurrentScreen = SCREEN_MENU;
		return;
	}

	if (uiPointInRect(mx, my, gRecordsSwitch.x, gRecordsSwitch.y, gRecordsSwitch.w, gRecordsSwitch.h))
	{
		profileScreenInit();
		gCurrentScreen = SCREEN_NAME_ENTRY;
		return;
	}
}

#endif
