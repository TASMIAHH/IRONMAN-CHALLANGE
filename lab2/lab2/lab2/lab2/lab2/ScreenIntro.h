//======================================================================
//  ScreenIntro.h
//  Iron Man Challenge: The Ultimate Training
//
//  State-of-the-Art Military Ironman Challenge Opening Interface.
//  Represents an extreme military triathlon & tactical endurance
//  simulation academy across the 4 core disciplines:
//  - STAGE I:   2.4 MILES (3.8 KM) OPEN WATER SWIM
//  - STAGE II:  ALPINE MOUNTAIN & ROPE CLIMB
//  - STAGE III: 112 MILES (180 KM) CYCLING EXPEDITION
//  - STAGE IV:  26.2 MILES (42.2 KM) MARATHON RUN
//
//  Features:
//  - 60 FPS animated tactical radar sweep with bearing markers
//  - Real-time biometric ECG heartbeat oscilloscope
//  - Dynamic laser scanner beam traversing training disciplines
//  - Ambient atmospheric tactical particles
//  - Interactive discipline inspection & tactical combat buttons
//  - Universal sound toggle support
//======================================================================
#ifndef SCREEN_INTRO_H
#define SCREEN_INTRO_H

#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//----------------------------------------------------------------------
// Buttons on the Intro Screen
//----------------------------------------------------------------------
#define INTRO_BTN_START       0
#define INTRO_BTN_DOSSIER     1
#define INTRO_BTN_DIRECTIVES  2
#define INTRO_BTN_BRIEFING    3
#define INTRO_BTN_COUNT       4

static struct Button gIntroButtons[INTRO_BTN_COUNT];

//----------------------------------------------------------------------
// Animation & Particle State
//----------------------------------------------------------------------
static double gIntroAnimTimer = 0.0;
static int    gIntroHoverSector = -1;

#define INTRO_PARTICLE_COUNT 48
struct IntroParticle
{
	double x, y;
	double vx, vy;
	double size;
	double alpha;
};
static struct IntroParticle gIntroParticles[INTRO_PARTICLE_COUNT];
static int gIntroParticlesInit = 0;

static void introInitParticles()
{
	int i;
	for (i = 0; i < INTRO_PARTICLE_COUNT; i++)
	{
		gIntroParticles[i].x = (double)(rand() % SCREEN_WIDTH);
		gIntroParticles[i].y = (double)(rand() % SCREEN_HEIGHT);
		gIntroParticles[i].vx = ((double)(rand() % 100) / 100.0 - 0.5) * 18.0;
		gIntroParticles[i].vy = 12.0 + (double)(rand() % 100) / 100.0 * 24.0;
		gIntroParticles[i].size = 1.5 + (double)(rand() % 100) / 100.0 * 2.5;
		gIntroParticles[i].alpha = 0.2 + (double)(rand() % 100) / 100.0 * 0.6;
	}
	gIntroParticlesInit = 1;
}

static void introUpdateParticles(double dt)
{
	int i;
	if (!gIntroParticlesInit) introInitParticles();

	for (i = 0; i < INTRO_PARTICLE_COUNT; i++)
	{
		gIntroParticles[i].x += gIntroParticles[i].vx * dt;
		gIntroParticles[i].y += gIntroParticles[i].vy * dt;

		if (gIntroParticles[i].y > SCREEN_HEIGHT + 10)
		{
			gIntroParticles[i].y = -10;
			gIntroParticles[i].x = (double)(rand() % SCREEN_WIDTH);
		}
		if (gIntroParticles[i].x < -10) gIntroParticles[i].x = SCREEN_WIDTH + 10;
		if (gIntroParticles[i].x > SCREEN_WIDTH + 10) gIntroParticles[i].x = -10;
	}
}

//----------------------------------------------------------------------
// Initialize Intro Screen Buttons
//----------------------------------------------------------------------
void introInit()
{
	introInitParticles();

	int btnW = 210;
	int btnH = 42;
	int gap = 16;
	int totalW = btnW * 4 + gap * 3;
	int startX = (SCREEN_WIDTH - totalW) / 2;
	int btnY = 32;

	uiSetButton(&gIntroButtons[INTRO_BTN_START],
		startX, btnY, btnW, btnH, "COMMENCE TRAINING", 1);

	uiSetButton(&gIntroButtons[INTRO_BTN_DOSSIER],
		startX + (btnW + gap) * 1, btnY, btnW, btnH, "ATHLETE DOSSIER", 1);

	uiSetButton(&gIntroButtons[INTRO_BTN_DIRECTIVES],
		startX + (btnW + gap) * 2, btnY, btnW, btnH, "TACTICAL DIRECTIVES", 1);

	uiSetButton(&gIntroButtons[INTRO_BTN_BRIEFING],
		startX + (btnW + gap) * 3, btnY, btnW, btnH, "MISSION BRIEFING", 1);
}

//----------------------------------------------------------------------
// Draw Military Tactical Corner Brackets
//----------------------------------------------------------------------
static void introDrawCornerBrackets(int x, int y, int w, int h, int len, int r, int g, int b, double alpha)
{
	iSetColorA(r, g, b, alpha);
	// Bottom-Left
	iFilledRectangle(x, y, len, 2);
	iFilledRectangle(x, y, 2, len);
	// Bottom-Right
	iFilledRectangle(x + w - len, y, len, 2);
	iFilledRectangle(x + w - 2, y, 2, len);
	// Top-Left
	iFilledRectangle(x, y + h - 2, len, 2);
	iFilledRectangle(x, y + h - len, 2, len);
	// Top-Right
	iFilledRectangle(x + w - len, y + h - 2, len, 2);
	iFilledRectangle(x + w - 2, y + h - len, 2, len);
}

//----------------------------------------------------------------------
// Draw Animated 360-Degree Tactical Military Radar
//----------------------------------------------------------------------
static void introDrawTacticalRadar(int cx, int cy, int radius)
{
	// Outer panel plate
	iSetColorA(COL_PANEL_R, COL_PANEL_G, COL_PANEL_B, 0.78);
	iFilledCircle(cx, cy, radius + 4);

	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.70);
	iCircle(cx, cy, radius + 4);

	// Concentric Distance Range Rings (represents 2.4M, 112M, 26.2M)
	iSetColorA(40, 160, 180, 0.40);
	iCircle(cx, cy, radius * 0.33);
	iCircle(cx, cy, radius * 0.66);
	iCircle(cx, cy, radius);

	// Crosshairs
	iSetColorA(40, 160, 180, 0.35);
	iLine(cx - radius, cy, cx + radius, cy);
	iLine(cx, cy - radius, cx, cy + radius);

	// Rotating radar sweep line
	double sweepAngle = fmod(gIntroAnimTimer * 2.2, 2.0 * M_PI);
	double sweepX = cx + cos(sweepAngle) * radius;
	double sweepY = cy + sin(sweepAngle) * radius;

	// Fading phosphor trail (6 trailing lines)
	int t;
	for (t = 0; t < 6; t++)
	{
		double trailAngle = sweepAngle - (double)t * 0.08;
		double tx = cx + cos(trailAngle) * radius;
		double ty = cy + sin(trailAngle) * radius;
		double alpha = 0.60 * (1.0 - (double)t / 6.0);
		iSetColorA(60, 240, 150, alpha);
		iLine(cx, cy, tx, ty);
	}

	// Active sweep line
	iSetColorA(180, 255, 200, 0.95);
	iLine(cx, cy, sweepX, sweepY);

	// Central command dot
	iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
	iFilledCircle(cx, cy, 3);

	// Waypoint blips (4 disciplines)
	double bAngles[4] = { 0.8, 2.3, 3.9, 5.2 };
	double bDists[4]  = { 0.72, 0.85, 0.45, 0.60 };
	const char *bTags[4] = { "SWIM", "CLIMB", "BIKE", "RUN" };

	for (t = 0; t < 4; t++)
	{
		double bx = cx + cos(bAngles[t]) * (radius * bDists[t]);
		double by = cy + sin(bAngles[t]) * (radius * bDists[t]);

		double diff = fmod(fabs(sweepAngle - bAngles[t]), 2.0 * M_PI);
		double blipAlpha = 0.35;
		if (diff < 0.6) blipAlpha = 1.0 - diff * 0.8;

		iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, blipAlpha);
		iFilledCircle(bx, by, 3);

		if (blipAlpha > 0.6)
		{
			iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, blipAlpha);
			uiTextClamped((int)bx + 6, (int)by - 4, bTags[t], 0.24, 40);
		}
	}

	// Bearing labels
	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.75);
	uiTextClampedCentered(cx, cy + radius + 7, "N // 000", 0.22, 50);
	uiTextClampedCentered(cx, cy - radius - 14, "S // 180", 0.22, 50);
}

//----------------------------------------------------------------------
// Draw Real-Time Biometric ECG Heartbeat Waveform
//----------------------------------------------------------------------
static void introDrawBiometrics(int x, int y, int w, int h)
{
	// Panel background
	iSetColorA(COL_PANEL_R, COL_PANEL_G, COL_PANEL_B, 0.75);
	iFilledRectangle(x, y, w, h);

	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.65);
	uiBorder(x, y, w, h, 1);

	// Title
	iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.95);
	uiTextClamped(x + 10, y + h - 16, "CADET BIOMETRIC TELEMETRY // ZONE 4", 0.26, w - 20);

	// Oscilloscope line
	int midY = y + h / 2 - 4;
	int i;
	int prevX = x + 8;
	int prevY = midY;

	glLineWidth(1.8f);
	for (i = 0; i < w - 16; i += 2)
	{
		int curX = x + 8 + i;
		double t = (double)i * 0.05 - gIntroAnimTimer * 5.0;
		double wave = sin(t) * 3.0;

		// Simulated periodic QRS heartbeat spikes
		double beatT = fmod(fabs(t), 4.0);
		if (beatT < 0.2)
			wave -= 6.0;
		else if (beatT < 0.45)
			wave += 16.0;
		else if (beatT < 0.7)
			wave -= 8.0;

		int curY = midY + (int)wave;
		if (curY < y + 4) curY = y + 4;
		if (curY > y + h - 22) curY = y + h - 22;

		if (i > 0)
		{
			iSetColorA(50, 240, 130, 0.85);
			iLine(prevX, prevY, curX, curY);
		}
		prevX = curX;
		prevY = curY;
	}
	glLineWidth(1.0f);

	// Readouts
	char vitalsBuf[128];
	sprintf(vitalsBuf, "HR: 168 BPM   |   VO2 MAX: 84.5 ML/KG   |   CADENCE: 180 SPM   |   HYDRATION: 100%%");
	iSetColorA(220, 235, 250, 0.90);
	uiTextClamped(x + 10, y + 6, vitalsBuf, 0.24, w - 20);
}

//----------------------------------------------------------------------
// Draw Dynamic Laser Scanner Beam
//----------------------------------------------------------------------
static void introDrawLaserScanner()
{
	double scanPhase = sin(gIntroAnimTimer * 1.4) * 0.5 + 0.5;
	int scanY = 90 + (int)(scanPhase * (SCREEN_HEIGHT - 210));

	// Gradient beam
	int b;
	for (b = -4; b <= 4; b++)
	{
		double alpha = 0.55 * (1.0 - fabs((double)b) / 5.0);
		iSetColorA(60, 220, 255, alpha);
		iFilledRectangle(0, scanY + b, SCREEN_WIDTH, 1);
	}
	// Core bright line
	iSetColorA(255, 255, 255, 0.90);
	iFilledRectangle(0, scanY, SCREEN_WIDTH, 1);
}

//----------------------------------------------------------------------
// Main Draw Routine for the Intro Screen
//----------------------------------------------------------------------
void introDraw()
{
	int i;
	int cx = SCREEN_WIDTH / 2;

	// 1. Draw Panoramic 4-Discipline Background
	drawIntroHudBackground();

	// 2. Subtle Darkening to keep HUD crystal clear & high-contrast
	uiDimScreen(0.42);

	// 3. Dynamic Laser Scanner Beam traversing disciplines
	introDrawLaserScanner();

	// 4. Ambient Drifting Particles
	for (i = 0; i < INTRO_PARTICLE_COUNT; i++)
	{
		iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, gIntroParticles[i].alpha);
		iFilledCircle((int)gIntroParticles[i].x, (int)gIntroParticles[i].y, (int)gIntroParticles[i].size);
	}

	// 5. Tactical Header Bar (Top of Screen)
	uiPanel(16, SCREEN_HEIGHT - 44, SCREEN_WIDTH - 160, 32,
		COL_PANEL_R, COL_PANEL_G, COL_PANEL_B, 0.88,
		COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.70);

	iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
	uiTextClamped(28, SCREEN_HEIGHT - 33,
		"[ MILITARY ENDURANCE SIMULATOR // COMBAT READINESS SUITE ]", 0.30, 420);

	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.90);
	uiTextClamped(460, SCREEN_HEIGHT - 33,
		"PROTOCOL: 2.4M SWIM  |  ALPINE CLIMB  |  112M BIKE  |  26.2M RUN", 0.28, 400);

	// 6. Tactical Radar (Top-Left overlay)
	introDrawTacticalRadar(72, SCREEN_HEIGHT - 126, 44);

	// 7. Biometrics Telemetry Box (Top-Right under sound toggle)
	introDrawBiometrics(SCREEN_WIDTH - 420, SCREEN_HEIGHT - 134, 404, 76);

	// 8. Main Cinematic Title Block
	int titleY = SCREEN_HEIGHT - 180;

	// Title Card Background Plate
	int titleW = 760;
	int titleH = 88;
	int titleX = (SCREEN_WIDTH - titleW) / 2;

	uiPanel(titleX, titleY - 14, titleW, titleH,
		COL_PANEL_R, COL_PANEL_G, COL_PANEL_B, 0.86,
		COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.90);

	introDrawCornerBrackets(titleX - 4, titleY - 18, titleW + 8, titleH + 8, 20,
		COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.95);

	// "IRON MAN CHALLENGE" - Large Forged Gold Gaming Title
	iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
	uiTextScaledCentered(cx, titleY + 38, "IRON MAN CHALLENGE", 0.44, 3.2);

	// "THE ULTIMATE TRAINING" - High-Contrast Cyan Banner
	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 1.0);
	uiTextClampedCentered(cx, titleY + 14, "THE ULTIMATE TRAINING", 0.42, titleW - 40);

	iSetColorA(220, 235, 245, 0.90);
	uiTextClampedCentered(cx, titleY - 4,
		"EXTREME MILITARY TRIATHLON & ENDURANCE OBSTACLE COURSE", 0.28, titleW - 60);

	// 9. Discipline Inspection Highlight (Hover detection on 4 sectors)
	if (gIntroHoverSector >= 0 && gIntroHoverSector < 4)
	{
		int secX = gIntroHoverSector * 256;
		iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.16);
		iFilledRectangle(secX, 90, 256, 170);

		iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.85);
		introDrawCornerBrackets(secX + 6, 96, 244, 158, 16,
			COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.95);
	}

	// 10. Pulsing Call-To-Action Prompt
	double pulse = sin(gIntroAnimTimer * 4.0) * 0.5 + 0.5;
	int promptAlpha = (int)(160 + pulse * 95);
	iSetColorA(255, 255, 255, (double)promptAlpha / 255.0);
	uiTextClampedCentered(cx, 88,
		"[ CLICK COMMENCE OR PRESS ENTER / SPACE TO BEGIN MISSION ]", 0.40, 680);

	// 11. Bottom Tactical Command Buttons
	for (i = 0; i < INTRO_BTN_COUNT; i++)
	{
		uiDrawButton(&gIntroButtons[i], uiButtonHovered(&gIntroButtons[i]));
	}

	// 12. Bottom Status Strip
	iSetColorA(0, 0, 0, 0.70);
	iFilledRectangle(0, 0, SCREEN_WIDTH, 22);

	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.85);
	uiTextCentered(cx, 6,
		"ENTER / SPACE - Commence        M - Sound Toggle        "
		"1-4 - Select Disciplines        Mouse Navigation Active",
		GLUT_BITMAP_HELVETICA_10);
}

//----------------------------------------------------------------------
// Mouse Interactions
//----------------------------------------------------------------------
void introMouseMove()
{
	// Detect which of the 4 discipline sectors is hovered (0..3)
	if (gMouseY >= 90 && gMouseY <= 260)
	{
		gIntroHoverSector = gMouseX / 256;
	}
	else
	{
		gIntroHoverSector = -1;
	}
}

void introMouseClick(int mx, int my)
{
	// 1. COMMENCE TRAINING
	if (uiPointInRect(mx, my, gIntroButtons[INTRO_BTN_START].x, gIntroButtons[INTRO_BTN_START].y,
		gIntroButtons[INTRO_BTN_START].w, gIntroButtons[INTRO_BTN_START].h))
	{
		// If profile exists, go straight to main menu; otherwise name entry
		if (profileGetActive() != NULL)
			gCurrentScreen = SCREEN_MENU;
		else
			gCurrentScreen = SCREEN_NAME_ENTRY;
		return;
	}

	// 2. ATHLETE DOSSIER
	if (uiPointInRect(mx, my, gIntroButtons[INTRO_BTN_DOSSIER].x, gIntroButtons[INTRO_BTN_DOSSIER].y,
		gIntroButtons[INTRO_BTN_DOSSIER].w, gIntroButtons[INTRO_BTN_DOSSIER].h))
	{
		profileScreenInit();
		gCurrentScreen = SCREEN_NAME_ENTRY;
		return;
	}

	// 3. TACTICAL DIRECTIVES
	if (uiPointInRect(mx, my, gIntroButtons[INTRO_BTN_DIRECTIVES].x, gIntroButtons[INTRO_BTN_DIRECTIVES].y,
		gIntroButtons[INTRO_BTN_DIRECTIVES].w, gIntroButtons[INTRO_BTN_DIRECTIVES].h))
	{
		gCurrentScreen = SCREEN_KEYS;
		return;
	}

	// 4. MISSION BRIEFING
	if (uiPointInRect(mx, my, gIntroButtons[INTRO_BTN_BRIEFING].x, gIntroButtons[INTRO_BTN_BRIEFING].y,
		gIntroButtons[INTRO_BTN_BRIEFING].w, gIntroButtons[INTRO_BTN_BRIEFING].h))
	{
		gCurrentScreen = SCREEN_ABOUT;
		return;
	}

	// Clicking anywhere in the central action area also starts the game
	if (my >= 80 && my <= 320)
	{
		if (profileGetActive() != NULL)
			gCurrentScreen = SCREEN_MENU;
		else
			gCurrentScreen = SCREEN_NAME_ENTRY;
		return;
	}
}

//----------------------------------------------------------------------
// Tick update (called every 16 ms from fixedUpdate())
//----------------------------------------------------------------------
void introUpdate(int confirmed, int goBack)
{
	gIntroAnimTimer += 0.016;
	uiUpdateAudioToast(0.016);
	introUpdateParticles(0.016);

	if (confirmed)
	{
		if (profileGetActive() != NULL)
			gCurrentScreen = SCREEN_MENU;
		else
			gCurrentScreen = SCREEN_NAME_ENTRY;
	}
}

#endif
