//======================================================================
//  GameUI.h
//  Reusable drawing helpers: text, panels, buttons, overlays.
//  Everything is built on top of the iGraphics primitives.
//======================================================================
#ifndef GAME_UI_H
#define GAME_UI_H

//----------------------------------------------------------------------
// Alpha blending.
// iGraphics only switches on the alpha *test*, which gives hard-edged
// cut-outs. Turning on blending as well lets us lay translucent panels
// over the poster so the artwork still shows through behind the menu.
//----------------------------------------------------------------------
void uiEnableBlending()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Same idea as iSetColor(), but with an opacity value (0.0 - 1.0).
void iSetColorA(int r, int g, int b, double alpha)
{
    glColor4f((float)(r / 255.0), (float)(g / 255.0),
              (float)(b / 255.0), (float)alpha);
}

//----------------------------------------------------------------------
// Modern Gaming Font System
//----------------------------------------------------------------------
#include "GameFontData.h"

static unsigned int gTexGameFont = 0;
static int gGameFontLoaded = 0;

static int uiFontFileExists(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (f == NULL) return 0;
    fclose(f);
    return 1;
}

void uiInitGameFont()
{
    if (gGameFontLoaded) return;
    if (uiFontFileExists("Assets/ui_game_font.png"))
    {
        gTexGameFont = iLoadImage("Assets/ui_game_font.png");
        if (gTexGameFont > 0)
            gGameFontLoaded = 1;
    }
    else if (uiFontFileExists("../Assets/ui_game_font.png"))
    {
        gTexGameFont = iLoadImage("../Assets/ui_game_font.png");
        if (gTexGameFont > 0)
            gGameFontLoaded = 1;
    }
}

int uiGameTextWidth(const char *s, double scale)
{
    if (!s) return 0;
    double total = 0.0;
    int i;
    for (i = 0; s[i]; i++)
    {
        unsigned char c = (unsigned char)s[i];
        if (c >= 128) c = 127;
        total += (double)gGameFontAdvance[c] * scale;
    }
    return (int)(total + 0.5);
}

void uiDrawGameText(double x, double y, const char *s, double scale, int align)
{
    if (!s || !s[0]) return;
    uiInitGameFont();
    if (!gGameFontLoaded || gTexGameFont == 0)
        return;

    double totalW = (double)uiGameTextWidth(s, scale);
    double curX = x;
    if (align == 1)      curX = x - totalW * 0.5;
    else if (align == 2) curX = x - totalW;

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, gTexGameFont);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    double quadW = 64.0 * scale;
    double quadH = 64.0 * scale;
    double quadY0 = y - 16.0 * scale;
    double quadY1 = quadY0 + quadH;

    glBegin(GL_QUADS);
    int i;
    for (i = 0; s[i]; i++)
    {
        unsigned char c = (unsigned char)s[i];
        if (c >= 128) c = 127; // fallback to star or symbol
        double adv = (double)gGameFontAdvance[c] * scale;
        if (c == ' ')
        {
            curX += adv;
            continue;
        }

        int col = c % 16;
        int row = c / 16;
        float u0 = (float)col / 16.0f;
        float u1 = (float)(col + 1) / 16.0f;
        float v0 = (float)row / 8.0f;
        float v1 = (float)(row + 1) / 8.0f;

        double charX = curX - (32.0 * scale - adv * 0.5);

        glTexCoord2f(u0, v0); glVertex2f((GLfloat)charX, (GLfloat)quadY1);
        glTexCoord2f(u0, v1); glVertex2f((GLfloat)charX, (GLfloat)quadY0);
        glTexCoord2f(u1, v1); glVertex2f((GLfloat)(charX + quadW), (GLfloat)quadY0);
        glTexCoord2f(u1, v0); glVertex2f((GLfloat)(charX + quadW), (GLfloat)quadY1);

        curX += adv;
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

static double uiFontToScale(void *font)
{
    if (font == GLUT_BITMAP_HELVETICA_10) return 0.35;
    if (font == GLUT_BITMAP_HELVETICA_12) return 0.40;
    if (font == GLUT_BITMAP_HELVETICA_18) return 0.52;
    if (font == GLUT_BITMAP_TIMES_ROMAN_24) return 0.62;
    return 0.42;
}

//----------------------------------------------------------------------
// Universal Text Functions (automatically use gaming font with GLUT fallback)
//----------------------------------------------------------------------
int uiTextWidth(const char *s, void *font)
{
    uiInitGameFont();
    if (gGameFontLoaded)
        return uiGameTextWidth(s, uiFontToScale(font));
    return glutBitmapLength(font, (const unsigned char *)s);
}

void uiText(int x, int y, const char *s, void *font)
{
    uiInitGameFont();
    if (gGameFontLoaded)
        uiDrawGameText((double)x, (double)y, s, uiFontToScale(font), 0);
    else
        iText((GLdouble)x, (GLdouble)y, (char *)s, font);
}

void uiTextCentered(int centerX, int y, const char *s, void *font)
{
    uiInitGameFont();
    if (gGameFontLoaded)
        uiDrawGameText((double)centerX, (double)y, s, uiFontToScale(font), 1);
    else
        uiText(centerX - uiTextWidth(s, font) / 2, y, s, font);
}

void uiTextScaled(int x, int y, const char *s, double scale, double thickness)
{
    uiInitGameFont();
    if (gGameFontLoaded)
    {
        uiDrawGameText((double)x, (double)y, s, scale * 2.1, 0);
        return;
    }

    int i;
    glPushMatrix();
    glTranslatef((GLfloat)x, (GLfloat)y, 0.0f);
    glScalef((GLfloat)scale, (GLfloat)scale, 1.0f);
    glLineWidth((GLfloat)thickness);
    for (i = 0; s[i]; i++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, s[i]);
    glLineWidth(1.0f);
    glPopMatrix();
}

int uiTextScaledWidth(const char *s, double scale)
{
    uiInitGameFont();
    if (gGameFontLoaded)
        return uiGameTextWidth(s, scale * 2.1);
    return (int)(glutStrokeLength(GLUT_STROKE_ROMAN, (const unsigned char *)s) * scale);
}

void uiTextScaledCentered(int centerX, int y, const char *s, double scale, double thickness)
{
    uiInitGameFont();
    if (gGameFontLoaded)
    {
        uiDrawGameText((double)centerX, (double)y, s, scale * 2.1, 1);
        return;
    }
    uiTextScaled(centerX - uiTextScaledWidth(s, scale) / 2, y, s, scale, thickness);
}

// Auto-fitting centered text: scales down automatically if it exceeds maxWidth
void uiTextClampedCentered(int centerX, int y, const char *s, double scale, double maxW)
{
    uiInitGameFont();
    if (gGameFontLoaded)
    {
        int w = uiGameTextWidth(s, scale);
        if (w > (int)maxW && w > 0)
            scale *= (maxW / (double)w);
        uiDrawGameText((double)centerX, (double)y, s, scale, 1);
        return;
    }
    uiTextScaledCentered(centerX, y, s, scale / 1.8, 2.0);
}

// Auto-fitting left-aligned text: scales down automatically if it exceeds maxWidth
void uiTextClamped(int x, int y, const char *s, double scale, double maxW)
{
    uiInitGameFont();
    if (gGameFontLoaded)
    {
        int w = uiGameTextWidth(s, scale);
        if (w > (int)maxW && w > 0)
            scale *= (maxW / (double)w);
        uiDrawGameText((double)x, (double)y, s, scale, 0);
        return;
    }
    uiText(x, y, s, GLUT_BITMAP_HELVETICA_12);
}

//----------------------------------------------------------------------
// Boxes
//----------------------------------------------------------------------
int uiPointInRect(int px, int py, int x, int y, int w, int h)
{
    return (px >= x && px <= x + w && py >= y && py <= y + h);
}

// A rectangle outline of the given thickness.
void uiBorder(int x, int y, int w, int h, int thickness)
{
    int i;
    for (i = 0; i < thickness; i++)
        iRectangle(x + i, y + i, w - 2 * i, h - 2 * i);
}

// Translucent background plate with a coloured border.
void uiPanel(int x, int y, int w, int h,
             int fillR, int fillG, int fillB, double fillAlpha,
             int borderR, int borderG, int borderB, double borderAlpha)
{
    iSetColorA(fillR, fillG, fillB, fillAlpha);
    iFilledRectangle(x, y, w, h);

    iSetColorA(borderR, borderG, borderB, borderAlpha);
    uiBorder(x, y, w, h, 2);
}

// Darkens whatever has already been drawn, so text stays readable.
void uiDimScreen(double alpha)
{
    iSetColorA(0, 0, 0, alpha);
    iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

// A thin horizontal rule, used to underline headings.
void uiRule(int x, int y, int w, int r, int g, int b, double alpha)
{
    iSetColorA(r, g, b, alpha);
    iFilledRectangle(x, y, w, 2);
}

//----------------------------------------------------------------------
// Buttons
//----------------------------------------------------------------------
struct Button
{
    int x, y, w, h;
    const char *label;
    int enabled;
};

void uiSetButton(struct Button *b, int x, int y, int w, int h,
                 const char *label, int enabled)
{
    b->x = x;
    b->y = y;
    b->w = w;
    b->h = h;
    b->label = label;
    b->enabled = enabled;
}

int uiButtonHovered(const struct Button *b)
{
    return b->enabled && uiPointInRect(gMouseX, gMouseY, b->x, b->y, b->w, b->h);
}

// highlighted = 1 draws the "selected" look (mouse hover or keyboard focus)
void uiDrawButton(const struct Button *b, int highlighted)
{
    int textY = b->y + b->h / 2 - 6;

    if (!b->enabled)
    {
        iSetColorA(20, 24, 34, 0.55);
        iFilledRectangle(b->x, b->y, b->w, b->h);

        iSetColorA(COL_GREY_R, COL_GREY_G, COL_GREY_B, 0.45);
        uiBorder(b->x, b->y, b->w, b->h, 1);

        iSetColorA(COL_GREY_R, COL_GREY_G, COL_GREY_B, 0.75);
        uiTextClampedCentered(b->x + b->w / 2, textY, b->label, 0.46, b->w - 16);
        return;
    }

    if (highlighted)
    {
        iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.30);
        iFilledRectangle(b->x, b->y, b->w, b->h);

        iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
        uiBorder(b->x, b->y, b->w, b->h, 2);

        // accent bar on the left edge
        iFilledRectangle(b->x, b->y, 6, b->h);

        iSetColorA(255, 255, 255, 1.0);
        uiTextClampedCentered(b->x + b->w / 2 + 3, textY, b->label, 0.46, b->w - 20);
    }
    else
    {
        iSetColorA(COL_PANEL_R, COL_PANEL_G, COL_PANEL_B, 0.62);
        iFilledRectangle(b->x, b->y, b->w, b->h);

        iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.75);
        uiBorder(b->x, b->y, b->w, b->h, 1);

        iSetColorA(COL_WHITE_R, COL_WHITE_G, COL_WHITE_B, 1.0);
        uiTextClampedCentered(b->x + b->w / 2, textY, b->label, 0.46, b->w - 16);
    }
}

//----------------------------------------------------------------------
// Padlock icon (pure image-based, zero procedural vector idraw)
//----------------------------------------------------------------------
void uiDrawPadlock(int cx, int cy, int size, int r, int g, int b)
{
    if (gTexUiLock)
    {
        iShowImage(cx - size / 2, cy - size / 2, size, size, gTexUiLock);
    }
}

//----------------------------------------------------------------------
// Universal Tactical Sound Button & Toast System (Available on ALL pages)
//----------------------------------------------------------------------
static double gAudioToastTimer = 0.0;
static double gSoundAnimTimer = 0.0;

void uiTriggerAudioNotification()
{
    gAudioToastTimer = 2.0;
}

void uiUpdateAudioToast(double dt)
{
    gSoundAnimTimer += dt;
    if (gAudioToastTimer > 0.0)
    {
        gAudioToastTimer -= dt;
        if (gAudioToastTimer < 0.0) gAudioToastTimer = 0.0;
    }
}

int uiGetSoundButtonX()
{
    if (gCurrentScreen >= SCREEN_LEVEL_1 && gCurrentScreen <= SCREEN_LEVEL_4)
        return 736;
    return HUD_SOUND_BTN_X;
}

int uiCheckSoundButtonClick(int mx, int my)
{
    int btnX = uiGetSoundButtonX();
    if (uiPointInRect(mx, my, btnX, HUD_SOUND_BTN_Y, HUD_SOUND_BTN_W, HUD_SOUND_BTN_H))
    {
        audioToggleMute();
        uiTriggerAudioNotification();
        return 1;
    }
    return 0;
}

void uiDrawSoundToggleButton()
{
    int x = uiGetSoundButtonX();
    int y = HUD_SOUND_BTN_Y;
    int w = HUD_SOUND_BTN_W;
    int h = HUD_SOUND_BTN_H;

    int hovered = uiPointInRect(gMouseX, gMouseY, x, y, w, h);

    if (!gAudioMuted)
    {
        // Sound is ONLINE: Tactical military cyan & gold styling
        iSetColorA(COL_PANEL_R, COL_PANEL_G, COL_PANEL_B, hovered ? 0.95 : 0.85);
        iFilledRectangle(x, y, w, h);

        iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, hovered ? 1.0 : 0.75);
        uiBorder(x, y, w, h, hovered ? 2 : 1);

        // Status LED (Green)
        iSetColorA(50, 240, 130, 1.0);
        iFilledCircle(x + 12, y + h / 2, 4);

        // 3 Animated Audio Equalizer Bars
        int barX = x + 24;
        int barBaseY = y + 8;
        int b;
        for (b = 0; b < 3; b++)
        {
            double phase = gSoundAnimTimer * 6.0 + b * 1.8;
            int barH = (int)(4.0 + fabs(sin(phase)) * 13.0);
            iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.90);
            iFilledRectangle(barX + b * 5, barBaseY, 3, barH);
        }

        // Text
        iSetColorA(255, 255, 255, 1.0);
        uiTextClamped(x + 44, y + 10, "SOUND: ON", 0.34, w - 48);
    }
    else
    {
        // Sound is MUTED: Tactical alert crimson & titanium styling
        iSetColorA(26, 12, 16, hovered ? 0.95 : 0.85);
        iFilledRectangle(x, y, w, h);

        iSetColorA(COL_RED_R, COL_RED_G, COL_RED_B, hovered ? 1.0 : 0.75);
        uiBorder(x, y, w, h, hovered ? 2 : 1);

        // Status LED (Red)
        iSetColorA(250, 60, 60, 1.0);
        iFilledCircle(x + 12, y + h / 2, 4);

        // Flatline Muted Audio Bar
        iSetColorA(COL_RED_R, COL_RED_G, COL_RED_B, 0.75);
        iFilledRectangle(x + 24, y + h / 2 - 1, 14, 2);

        // Text
        iSetColorA(COL_GREY_R, COL_GREY_G, COL_GREY_B, 1.0);
        uiTextClamped(x + 44, y + 10, "SOUND: OFF", 0.34, w - 48);
    }
}

void uiDrawAudioNotification()
{
    if (gAudioToastTimer <= 0.0) return;

    double alpha = (gAudioToastTimer > 0.4) ? 1.0 : (gAudioToastTimer / 0.4);
    int w = 270;
    int h = 32;
    int x = (SCREEN_WIDTH - w) / 2;
    int y = SCREEN_HEIGHT - 50;

    iSetColorA(COL_PANEL_R, COL_PANEL_G, COL_PANEL_B, 0.92 * alpha);
    iFilledRectangle(x, y, w, h);

    if (gAudioMuted)
    {
        iSetColorA(COL_RED_R, COL_RED_G, COL_RED_B, 0.95 * alpha);
        uiBorder(x, y, w, h, 2);
        uiTextClampedCentered(x + w / 2, y + 9, "[ TACTICAL COMMS: MUTED ]", 0.38, w - 16);
    }
    else
    {
        iSetColorA(50, 240, 130, 0.95 * alpha);
        uiBorder(x, y, w, h, 2);
        uiTextClampedCentered(x + w / 2, y + 9, "[ TACTICAL COMMS: ONLINE ]", 0.38, w - 16);
    }
}

//======================================================================
//  Photorealistic Picture-Based HUD Rendering System
//  Replaces flat coder-drawn rectangles with smooth, natural pictures
//======================================================================

// 1. Photorealistic Frosted Glass HUD Header Banner
void uiDrawPictureBanner(int x, int y, int w, int h, unsigned int texBanner)
{
    if (texBanner)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 1.0f, 0.95f);
        iShowImage(x, y, w, h, texBanner);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    }
}

// 2. Photorealistic Picture-Based Status Bar (HP, Stamina, Oxygen)
// Sub-UV texture mapping smoothly reveals the liquid gradient without stretching!
void uiDrawPictureBar(int x, int y, int barW, int barH, double ratio,
                      unsigned int texFill, unsigned int texFrame, unsigned int texIcon,
                      int iconW, int iconH)
{
    if (ratio < 0.0) ratio = 0.0;
    if (ratio > 1.0) ratio = 1.0;

    int curX = x;

    // A. Draw Icon Badge on Left
    if (texIcon && iconW > 0 && iconH > 0)
    {
        int iconY = y + (barH - iconH) / 2;
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        iShowImage(curX, iconY, iconW, iconH, texIcon);
        curX += iconW + 4;
    }

    // B. Draw Textured Bar Slot & Fill
    // Inset geometry matching the metallic frame inner slot
    int innerPadX = 14;
    int innerPadY = 5;
    int innerX = curX + innerPadX;
    int innerY = y + innerPadY;
    int innerW = barW - innerPadX * 2;
    int innerH = barH - innerPadY * 2;

    int fillW = (int)(innerW * ratio);
    if (fillW > innerW) fillW = innerW;

    // Render textured fill with sub-UV mapping
    if (fillW > 0 && texFill)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texFill);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f((GLfloat)innerX, (GLfloat)innerY);

            glTexCoord2f((GLfloat)ratio, 0.0f);
            glVertex2f((GLfloat)(innerX + fillW), (GLfloat)innerY);

            glTexCoord2f((GLfloat)ratio, -1.0f);
            glVertex2f((GLfloat)(innerX + fillW), (GLfloat)(innerY + innerH));

            glTexCoord2f(0.0f, -1.0f);
            glVertex2f((GLfloat)innerX, (GLfloat)(innerY + innerH));
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }

    // C. Draw Metallic Beveled Bar Frame Casing over the slot
    if (texFrame)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        iShowImage(curX, y, barW, barH, texFrame);
    }
}

// 3. Photorealistic Biome Waypoint Chip
void uiDrawPictureChip(int x, int y, int w, int h, int active, const char *label,
                       unsigned int texActive, unsigned int texInactive)
{
    unsigned int tex = active ? texActive : texInactive;
    if (tex)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 1.0f, active ? 1.0f : 0.82f);
        iShowImage(x, y, w, h, tex);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    }

    if (label && label[0])
    {
        if (active)
            iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
        else
            iSetColorA(180, 195, 215, 0.75);
        uiTextCentered(x + w / 2, y + 6, (char *)label, GLUT_BITMAP_HELVETICA_10);
    }
}

#endif
