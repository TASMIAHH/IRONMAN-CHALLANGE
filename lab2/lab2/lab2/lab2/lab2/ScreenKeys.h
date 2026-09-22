//======================================================================
//  ScreenKeys.h
//  "KEYS" - the keyboard reference sheet.
//======================================================================
#ifndef SCREEN_KEYS_H
#define SCREEN_KEYS_H

#define KY_PANEL_X   112
#define KY_PANEL_Y    54
#define KY_PANEL_W   800
#define KY_PANEL_H   414

#define KY_KEY_X     (KY_PANEL_X + 46)     // left column: the key cap
#define KY_ACT_X     (KY_PANEL_X + 330)    // right column: what it does
#define KY_ROW_STEP   24

struct Button gKeysBack;

void keysInit()
{
    uiSetButton(&gKeysBack, 852, 488, 108, 32, "BACK", 1);
}

//----------------------------------------------------------------------
void keysSection(int *y, const char *title)
{
    *y -= 24;

    iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
    uiText(KY_PANEL_X + 26, *y, title, GLUT_BITMAP_HELVETICA_18);

    uiRule(KY_PANEL_X + 26, *y - 7, KY_PANEL_W - 52,
           COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.35);
}

// Draws a key cap on the left and its action on the right.
void keysRow(int *y, const char *keyText, const char *action)
{
    int w;

    *y -= KY_ROW_STEP;

    w = uiTextWidth(keyText, GLUT_BITMAP_HELVETICA_12) + 20;

    iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.16);
    iFilledRectangle(KY_KEY_X, *y - 5, w, 20);

    iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.85);
    uiBorder(KY_KEY_X, *y - 5, w, 20, 1);

    iSetColorA(255, 255, 255, 1.0);
    uiText(KY_KEY_X + 10, *y, keyText, GLUT_BITMAP_HELVETICA_12);

    iSetColorA(220, 228, 238, 1.0);
    uiText(KY_ACT_X, *y, action, GLUT_BITMAP_HELVETICA_12);
}

//----------------------------------------------------------------------
void keysDraw()
{
    int y = KY_PANEL_Y + KY_PANEL_H;

    drawPosterBackground();
    uiDimScreen(0.80);

    iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
    uiTextScaled(KY_PANEL_X, 492, "KEYS", 0.26, 2.5);

    iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.95);
    uiText(KY_PANEL_X + 118, 496, "GAME CONTROLS", GLUT_BITMAP_HELVETICA_18);

    uiDrawButton(&gKeysBack, uiButtonHovered(&gKeysBack));

    uiPanel(KY_PANEL_X, KY_PANEL_Y, KY_PANEL_W, KY_PANEL_H,
            COL_PANEL_R, COL_PANEL_G, COL_PANEL_B, 0.88,
            COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.75);

    y -= 12;

    // ---- menus -------------------------------------------------------
    keysSection(&y, "MENUS AND SCREENS");
    keysRow(&y, "UP / DOWN  or  W / S",     "Move the selection in the main menu");
    keysRow(&y, "LEFT / RIGHT  or  A / D",  "Move between the level cards");
    keysRow(&y, "ENTER",                    "Select the highlighted option");
    keysRow(&y, "M",                        "Toggle Tactical Audio ON / OFF (any screen)");
    keysRow(&y, "ESC  or  BACKSPACE",       "Go back one screen");
    keysRow(&y, "MOUSE LEFT CLICK",         "Click any button directly (including SOUND toggle)");

    // ---- gameplay ----------------------------------------------------
    y -= 14;
    keysSection(&y, "PLAYER CONTROLS");
    keysRow(&y, "W  /  UP ARROW",     "Surface to breathe (L4)  |  Jump / Climb");
    keysRow(&y, "S  /  DOWN ARROW",   "Dive deep (L4)  |  Duck / Climb down");
    keysRow(&y, "D  /  RIGHT ARROW",  "Sprint kick (L4)  |  Push pace / Sprint");
    keysRow(&y, "A  /  LEFT ARROW",   "Glide & recover stamina (L4)  |  Brake / Coast");
    keysRow(&y, "SPACEBAR",           "Dolphin Kick Surge (L4)  |  Jump / Leap");
    keysRow(&y, "P",                  "Pause and resume the stage");
    keysRow(&y, "ESC",                "Leave the stage and return to level select");

    y -= 22;
    iSetColorA(COL_GREY_R, COL_GREY_G, COL_GREY_B, 1.0);
    uiTextClamped(KY_PANEL_X + 26, y,
           "The player controls belong to the training stages, which are still under construction.",
           0.31, KY_PANEL_W - 52);

    // ---- hint strip --------------------------------------------------
    iSetColorA(0, 0, 0, 0.62);
    iFilledRectangle(0, 0, SCREEN_WIDTH, 26);

    iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.95);
    uiTextCentered(SCREEN_WIDTH / 2, 9,
        "ESC or BACKSPACE  -  Return to the main menu",
        GLUT_BITMAP_HELVETICA_12);
}

//----------------------------------------------------------------------
void keysMouseClick(int mx, int my)
{
    if (uiPointInRect(mx, my, gKeysBack.x, gKeysBack.y,
                      gKeysBack.w, gKeysBack.h))
        gCurrentScreen = SCREEN_MENU;
}

#endif
