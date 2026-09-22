//======================================================================
//  ScreenAbout.h
//  "ABOUT THE GAME" - a summary of the submitted project proposal.
//======================================================================
#ifndef SCREEN_ABOUT_H
#define SCREEN_ABOUT_H

#define AB_PANEL_X    64
#define AB_PANEL_Y    54
#define AB_PANEL_W   896
#define AB_PANEL_H   414

#define AB_TEXT_X    (AB_PANEL_X + 30)
#define AB_LINE_STEP  16

struct Button gAboutBack;

void aboutInit()
{
    uiSetButton(&gAboutBack, 852, 488, 108, 32, "BACK", 1);
}

//----------------------------------------------------------------------
// Small helpers that draw one line and move the cursor down.
//----------------------------------------------------------------------
void aboutSection(int *y, const char *title)
{
    *y -= 22;
    iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
    uiTextClamped(AB_TEXT_X, *y, title, 0.40, AB_PANEL_W - 60);
}

void aboutLine(int *y, const char *text)
{
    *y -= AB_LINE_STEP;
    iSetColorA(225, 232, 240, 1.0);
    uiTextClamped(AB_TEXT_X, *y, text, 0.31, AB_PANEL_W - 60);
}

void aboutBullet(int *y, const char *text)
{
    *y -= AB_LINE_STEP;

    iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 1.0);
    iFilledRectangle(AB_TEXT_X, *y + 3, 5, 5);

    iSetColorA(225, 232, 240, 1.0);
    uiTextClamped(AB_TEXT_X + 14, *y, text, 0.30, AB_PANEL_W - 74);
}

//----------------------------------------------------------------------
void aboutDraw()
{
    int y = AB_PANEL_Y + AB_PANEL_H;

    drawPosterBackground();
    uiDimScreen(0.80);

    iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
    uiTextScaled(AB_PANEL_X, 492, "ABOUT THE GAME", 0.26, 2.5);

    uiDrawButton(&gAboutBack, uiButtonHovered(&gAboutBack));

    uiPanel(AB_PANEL_X, AB_PANEL_Y, AB_PANEL_W, AB_PANEL_H,
            COL_PANEL_R, COL_PANEL_G, COL_PANEL_B, 0.88,
            COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.75);

    // ---- title block -------------------------------------------------
    y -= 34;
    iSetColorA(255, 255, 255, 1.0);
    uiTextClamped(AB_TEXT_X, y, "Iron Man Challenge: The Ultimate Training",
           0.48, AB_PANEL_W - 60);

    y -= 19;
    iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 1.0);
    uiTextClamped(AB_TEXT_X, y,
           "Project Proposal Summary   |   Course CSE-1200: Software Development - I   |   "
           "Department of CSE, AUST",
           0.31, AB_PANEL_W - 60);

    y -= 13;
    uiRule(AB_TEXT_X, y, AB_PANEL_W - 60,
           COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.55);

    // ---- the game ----------------------------------------------------
    aboutSection(&y, "THE GAME");
    aboutLine(&y, "A single-player 2D tactical endurance and obstacle-course simulation inspired by the military IRONMAN");
    aboutLine(&y, "triathlon. The cadet takes on grueling training stages: open ocean swimming, high-altitude mountain climbing,");
    aboutLine(&y, "tactical cross-country cycling, and an extreme endurance marathon, clearing obstacles and managing stamina.");
    aboutLine(&y, "Clearing all four sectors earns the prestigious Military Iron Endurance Insignia and the title of IRON TITAN.");

    // ---- objective ---------------------------------------------------
    y -= 8;
    aboutSection(&y, "OBJECTIVE");
    aboutLine(&y, "Test the athlete's reflexes, stamina reserves, and pace management across four elite military endurance tests,");
    aboutLine(&y, "delivering a high-intensity progression culminating in full graduation as an Iron Man Endurance Specialist.");

    // ---- features ----------------------------------------------------
    y -= 8;
    aboutSection(&y, "CORE FEATURES");
    aboutBullet(&y, "Multi-discipline military training - swimming, climbing, cycling, and running with specialized physical mechanics");
    aboutBullet(&y, "Hazard avoidance - navigating underwater currents, rockfalls, terrain barriers, and harsh weather conditions");
    aboutBullet(&y, "Field power-up system - hydration electrolyte packs, adrenaline surges, tactical gear, and oxygen rebreathers");
    aboutBullet(&y, "Stamina & health telemetry - realistic exertion fatigue modeling requiring disciplined pace management");
    aboutBullet(&y, "Military grade qualification scoring with Bronze, Silver, and Gold Honor Medals");
    aboutBullet(&y, "Universal Tactical Audio Toggle - mute or activate tactical sound on every interface page (Button or 'M' key)");

    // ---- team --------------------------------------------------------
    y -= 8;
    aboutSection(&y, "DEVELOPED BY");
    aboutLine(&y, "Md. Mustaeen Bin Saif (00725105101001)      Md. Samiul Islam (00725105101017)      "
                  "Shanjida Imam Tasmia (00725105101024)");

    // ---- hint strip --------------------------------------------------
    iSetColorA(0, 0, 0, 0.62);
    iFilledRectangle(0, 0, SCREEN_WIDTH, 26);

    iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.95);
    uiTextCentered(SCREEN_WIDTH / 2, 9,
        "ESC or BACKSPACE  -  Return to the main menu",
        GLUT_BITMAP_HELVETICA_12);
}

//----------------------------------------------------------------------
void aboutMouseClick(int mx, int my)
{
    if (uiPointInRect(mx, my, gAboutBack.x, gAboutBack.y,
                      gAboutBack.w, gAboutBack.h))
        gCurrentScreen = SCREEN_MENU;
}

#endif
