//======================================================================
//  GameAssets.h
//  Image loading.
//
//  Images are loaded ONCE into OpenGL textures with iGraphics'
//  iLoadImage(), then drawn every frame with iShowImage(). The other
//  route, iShowBMP(), re-reads the file from disk on every single call,
//  which is far too slow for a game loop.
//
//  IMPORTANT: iLoadImage() needs a live OpenGL context, so it may only
//  be called AFTER iInitialize() has created the window.
//======================================================================
#ifndef GAME_ASSETS_H
#define GAME_ASSETS_H

unsigned int gTexIntroPoster = 0;
int gHasIntroPoster = 0;
unsigned int gTexIntroHudBg = 0;
int gHasIntroHudBg = 0;

// UI Textures
unsigned int gTexUiLock = 0;
unsigned int gTexUiPlay = 0;
unsigned int gTexUiMedalGold = 0;
unsigned int gTexUiMedalSilver = 0;
unsigned int gTexUiMedalBronze = 0;
unsigned int gTexUiArcReactor = 0;

// Photorealistic Picture-Based HUD Textures
unsigned int gTexHudBanner = 0;
unsigned int gTexHudBarFrame = 0;
unsigned int gTexHudHpFill = 0;
unsigned int gTexHudStaminaFill = 0;
unsigned int gTexHudOxygenFill = 0;
unsigned int gTexHudHpIcon = 0;
unsigned int gTexHudStaminaIcon = 0;
unsigned int gTexHudOxygenIcon = 0;
unsigned int gTexHudChipActive = 0;
unsigned int gTexHudChipInactive = 0;

// stb_image fails silently, so check the file is really there first.
int assetFileExists(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (f == NULL) return 0;
    fclose(f);
    return 1;
}

int assetLoadTexture(const char *path, unsigned int *outTexture)
{
    if (!assetFileExists(path))
    {
        printf("[assets] MISSING: %s\n", path);
        gAssetsOk = 0;
        return 0;
    }

    *outTexture = iLoadImage((char *)path);
    return 1;
}

void assetsLoadAll()
{
    gHasIntroPoster = assetLoadTexture(PATH_INTRO_POSTER, &gTexIntroPoster);
    gHasIntroHudBg  = assetLoadTexture(PATH_INTRO_HUD_BG, &gTexIntroHudBg);
    assetLoadTexture("Assets/ui_lock.png", &gTexUiLock);
    assetLoadTexture("Assets/ui_play.png", &gTexUiPlay);
    assetLoadTexture("Assets/ui_medal_gold.png", &gTexUiMedalGold);
    assetLoadTexture("Assets/ui_medal_silver.png", &gTexUiMedalSilver);
    assetLoadTexture("Assets/ui_medal_bronze.png", &gTexUiMedalBronze);
    assetLoadTexture("Assets/ui_arc_reactor.png", &gTexUiArcReactor);

    // Picture-based realistic HUD textures
    assetLoadTexture("Assets/ui_hud_banner.png", &gTexHudBanner);
    assetLoadTexture("Assets/ui_hud_bar_frame.png", &gTexHudBarFrame);
    assetLoadTexture("Assets/ui_hud_hp_fill.png", &gTexHudHpFill);
    assetLoadTexture("Assets/ui_hud_stamina_fill.png", &gTexHudStaminaFill);
    assetLoadTexture("Assets/ui_hud_oxygen_fill.png", &gTexHudOxygenFill);
    assetLoadTexture("Assets/ui_hud_hp_icon.png", &gTexHudHpIcon);
    assetLoadTexture("Assets/ui_hud_stamina_icon.png", &gTexHudStaminaIcon);
    assetLoadTexture("Assets/ui_hud_oxygen_icon.png", &gTexHudOxygenIcon);
    assetLoadTexture("Assets/ui_hud_chip_active.png", &gTexHudChipActive);
    assetLoadTexture("Assets/ui_hud_chip_inactive.png", &gTexHudChipInactive);
}

//----------------------------------------------------------------------
// Draws the poster full-screen. If the file could not be found we fall
// back to a plain dark backdrop so the menu is still usable and the
// problem is visible instead of showing a blank window.
//----------------------------------------------------------------------
void drawPosterBackground()
{
    if (gHasIntroPoster)
    {
        iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, gTexIntroPoster);
    }
    else
    {
        int y;
        for (y = 0; y < SCREEN_HEIGHT; y += 4)
        {
            double t = (double)y / SCREEN_HEIGHT;
            iSetColor((int)(12 + 26 * t), (int)(18 + 40 * t), (int)(40 + 70 * t));
            iFilledRectangle(0, y, SCREEN_WIDTH, 4);
        }

        iSetColor(COL_RED_R, COL_RED_G, COL_RED_B);
        uiTextCentered(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 40,
                       "Asset not found: Assets/intro_poster.png",
                       GLUT_BITMAP_HELVETICA_18);
        iSetColor(COL_GREY_R, COL_GREY_G, COL_GREY_B);
        uiTextCentered(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 62,
                       "Check that the working directory is the project folder.",
                       GLUT_BITMAP_HELVETICA_12);
    }
}

//----------------------------------------------------------------------
// Draws the intro military HUD background
//----------------------------------------------------------------------
void drawIntroHudBackground()
{
    if (gHasIntroHudBg)
    {
        iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, gTexIntroHudBg);
    }
    else
    {
        drawPosterBackground();
    }
}

#endif
