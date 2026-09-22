//======================================================================
//  ScreenLevel.h
//  Placeholder stage screen for LEVEL 02.
//
//  Level 01 is implemented in Level01.h / Level01Draw.h. This screen is
//  what the later stages still look like until they are built.
//======================================================================
#ifndef SCREEN_LEVEL_H
#define SCREEN_LEVEL_H

struct Button gLevelBack;

void levelScreenInit()
{
    uiSetButton(&gLevelBack, 48, 46, 210, 44, "BACK TO LEVEL SELECT", 1);
}

//----------------------------------------------------------------------
void levelScreenDraw(int level)
{
    const char *levelText   = (level == 1) ? "LEVEL 01" : "LEVEL 02";
    const char *disciplineName = (level == 1) ? LEVEL_1_NAME : LEVEL_2_NAME;
    int cx = SCREEN_WIDTH / 2;

    drawPosterBackground();
    uiDimScreen(0.90);

    // ---- stage title -------------------------------------------------
    iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
    uiTextScaledCentered(cx, 468, levelText, 0.42, 3.0);

    iSetColorA(255, 255, 255, 1.0);
    uiTextCentered(cx, 428, disciplineName, GLUT_BITMAP_TIMES_ROMAN_24);

    uiRule(cx - 200, 412, 400, COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.55);

    // ---- construction notice ----------------------------------------
    uiPanel(cx - 330, 168, 660, 208,
            COL_PANEL_R, COL_PANEL_G, COL_PANEL_B, 0.88,
            COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.70);

    iSetColorA(COL_CYAN_R, COL_CYAN_B, COL_CYAN_B, 1.0);
    uiTextClampedCentered(cx, 336, "STAGE MODULE UNDER CONSTRUCTION", 0.44, 600);

    iSetColorA(220, 228, 238, 1.0);
    uiTextCentered(cx, 302,
        "The gameplay for this discipline has not been built yet.",
        GLUT_BITMAP_HELVETICA_12);
    uiTextCentered(cx, 284,
        "Player movement, obstacles, power-ups, health and stamina, the stage",
        GLUT_BITMAP_HELVETICA_12);
    uiTextCentered(cx, 266,
        "timer and the scoring system all belong here.",
        GLUT_BITMAP_HELVETICA_12);

    // ---- status ------------------------------------------------------
    if (gLevelCompleted[level])
    {
        iSetColorA(90, 220, 130, 1.0);
        uiTextCentered(cx, 226, "STATUS:  COMPLETED", GLUT_BITMAP_HELVETICA_18);
    }
    else
    {
        iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
        uiTextCentered(cx, 226, "STATUS:  NOT COMPLETED", GLUT_BITMAP_HELVETICA_18);
    }

    iSetColorA(COL_GREY_R, COL_GREY_G, COL_GREY_B, 1.0);
    uiTextClampedCentered(cx, 196,
        "Clear LEVEL 01 to keep progressing through the training course.",
        0.33, 600);

    uiDrawButton(&gLevelBack, uiButtonHovered(&gLevelBack));

    // ---- hint strip --------------------------------------------------
    iSetColorA(0, 0, 0, 0.62);
    iFilledRectangle(0, 0, SCREEN_WIDTH, 26);

    iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.95);
    uiTextCentered(SCREEN_WIDTH / 2, 9,
        "ESC  -  Return to level select",
        GLUT_BITMAP_HELVETICA_12);
}

//----------------------------------------------------------------------
void levelScreenMouseClick(int mx, int my)
{
    if (uiPointInRect(mx, my, gLevelBack.x, gLevelBack.y,
                      gLevelBack.w, gLevelBack.h))
        gCurrentScreen = SCREEN_LEVEL_SELECT;
}

#endif
