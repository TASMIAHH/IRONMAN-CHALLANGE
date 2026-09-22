//======================================================================
//  Level02Draw.h
//  All rendering for LEVEL 02 - CLIMBING CHALLENGE.
//
//  Redesigned to match the night-time cliff climbing aesthetic:
//  - Night mountain pixel art background
//  - Right-side rocky mountain cliff with natural stone ledges
//  - Floating stone slab platforms with rugged rock bottoms
//  - Mechanical spinning gear obstacles mounted on platform edges
//  - Adventurer player with backpack, green bedroll, and cyan climbing arc
//  - Multi-faceted falling boulders with motion speed lines
//  - Sleek HUD with climber progress pin and right-side power-up frames
//======================================================================
#ifndef LEVEL02_DRAW_H
#define LEVEL02_DRAW_H

#include <math.h>

//----------------------------------------------------------------------
// HUD layout
//----------------------------------------------------------------------
#define L2_HUD_Y        496
#define L2_HUD_H         80
#define L2_BAR_W        240
#define L2_BAR_H         12

//----------------------------------------------------------------------
// Small helpers
//----------------------------------------------------------------------
void l2TextRight(int rightX, int y, const char *s, void *font)
{
    uiText(rightX - uiTextWidth(s, font), y, s, font);
}

void l2Bar(int x, int y, int w, int h, double ratio,
           int r, int g, int b)
{
    int fill;
    if (ratio < 0.0) ratio = 0.0;
    if (ratio > 1.0) ratio = 1.0;
    fill = (int)(w * ratio);

    iSetColorA(10, 14, 24, 0.75);
    iFilledRectangle(x, y, w, h);

    if (fill > 0)
    {
        int hr = r + 35 > 255 ? 255 : r + 35;
        int hg = g + 35 > 255 ? 255 : g + 35;
        int hb = b + 35 > 255 ? 255 : b + 35;
        iSetColorA(r, g, b, 0.95);
        iFilledRectangle(x, y, fill, h);
        iSetColorA(hr, hg, hb, 0.95);
        iFilledRectangle(x, y + h - 3, fill, 3);
    }

    iSetColorA(180, 200, 230, 0.45);
    uiBorder(x, y, w, h, 1);
}

//======================================================================
// Background - pristine night mountain pixel-art texture
//======================================================================

//======================================================================
// Background - pristine night mountain pixel-art texture
//======================================================================
void l2DrawBackground()
{
	iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, gL2TexBg);
}

//======================================================================
// Ambient Falling Snowflakes
//======================================================================
//======================================================================
// Ambient Falling Snowflakes (Textured particles)
//======================================================================
void l2DrawSnowflakes()
{
	int s;
	for (s = 0; s < 55; s++)
	{
		double speed = 25.0 + (s % 5) * 12.0;
		double fallY = fmod(gL2Elapsed * speed + (double)(s * 73), (double)(SCREEN_HEIGHT + 20));
		int sy = SCREEN_HEIGHT - (int)fallY;
		int drift = (int)(sin(gL2Elapsed * 1.8 + (double)s) * 14.0);
		int sx = (int)fmod((double)(s * 139 + drift + 1024), (double)SCREEN_WIDTH);
		int sz = (s % 3 == 0) ? 8 : 5;

		iShowImage(sx - sz / 2, sy - sz / 2, sz, sz, gL2TexSnowParticle);
	}
}

//======================================================================
// Natural Alpine Mountain Mist & Drifting Cloud Layers (Textured image)
//======================================================================
void l2DrawMountainMist()
{
	double t = gL2Elapsed;
	int m;
	for (m = 0; m < 4; m++)
	{
		double speed = 10.0 + m * 6.0;
		double mistWorldY = 450.0 + m * 750.0;
		double sy = l2ScreenY(mistWorldY);
		if (sy < -120 || sy > SCREEN_HEIGHT + 120) continue;

		double driftX = fmod(t * speed + (double)(m * 280), (double)(SCREEN_WIDTH + 450)) - 220.0;
		iShowImage((int)driftX, (int)sy - 40, 256, 128, gL2TexMist);
		iShowImage((int)(driftX + 220), (int)sy - 20, 256, 128, gL2TexMist);
	}
}

//======================================================================
// Authentic Mountain Summit Ridge & Traditional Mountaineer's Cairn
//======================================================================
void l2DrawSummit()
{
    double sy = l2ScreenY((double)L2_SUMMIT_Y);
    int syi;

    if (sy < -140 || sy > SCREEN_HEIGHT + 140) return;
    syi = (int)sy;

    // 1. Natural Mountain Summit Ridge
    iShowImage(L2_PLAY_LEFT, syi - 10, L2_PLAY_RIGHT - L2_PLAY_LEFT, 60, gL2TexPlatTerrace);

    // 2. Traditional Mountaineer's Summit Stone Cairn with Pennant Flag
    {
        int cx = SCREEN_WIDTH / 2;
        iShowImage(cx - 90, syi + 15, 180, 225, gL2TexSummitCairn);

        // Elegant Summit Banner
        iSetColorA(255, 255, 255, 0.95);
        uiTextCentered(cx, syi - 12, "= = =   A L P I N E   S U M M I T   = = =", GLUT_BITMAP_HELVETICA_18);
    }
}

//======================================================================
// Mountain Face & Rugged Rock Couloirs Backdrop
//======================================================================
void l2DrawMountainFace()
{
    // Natural mountain face is rendered by gL2TexBg
}

//======================================================================
// Mountaineering Braided Ropes in Sheer Cliff Zones
//======================================================================
void l2DrawRopes()
{
	double camY = gL2CameraY;
	int sec;

	for (sec = 1; sec <= 2; sec++)
	{
		double secStart = (sec == 1) ? L2_ROPE_SEC1_START : L2_ROPE_SEC2_START;
		double secEnd   = (sec == 1) ? L2_ROPE_SEC1_END   : L2_ROPE_SEC2_END;

		if (camY + SCREEN_HEIGHT < secStart - 40.0 || camY > secEnd + 60.0)
			continue;

		double botSY = l2ScreenY(secStart);
		double topSY = l2ScreenY(secEnd);

		int rBotY = (int)botSY;
		int rTopY = (int)topSY;
		if (rBotY < -10) rBotY = -10;
		if (rTopY > SCREEN_HEIGHT + 10) rTopY = SCREEN_HEIGHT + 10;

		int lane;
		for (lane = 0; lane < L2_ROPE_LANES; lane++)
		{
			int rx = (lane == 0) ? (int)L2_ROPE_X_LEFT :
			         (lane == 1) ? (int)L2_ROPE_X_MID : (int)L2_ROPE_X_RIGHT;

			double sway = 0.0;
			if (gL2RopeActive && gL2RopeLane == lane)
				sway = sin(gL2RopeElapsed * 10.0) * 3.0;
			int drawRX = rx + (int)sway;

			// 1. Hanging Braided Rope Column using gL2TexClimbRope
			int ry;
			for (ry = rBotY; ry < rTopY; ry += 64)
			{
				int segH = 64;
				if (ry + segH > rTopY) segH = rTopY - ry;
				iShowImage(drawRX - 8, ry, 16, segH, gL2TexClimbRope);
			}

			// 2. Top Iron Piton Anchor
			if (topSY >= -20 && topSY <= SCREEN_HEIGHT + 40)
			{
				int ty = (int)topSY;
				iShowImage(drawRX - 12, ty - 6, 24, 24, gL2TexRopeAnchor);
			}

			// 3. Bottom Piton Stake Anchor
			if (botSY >= -20 && botSY <= SCREEN_HEIGHT + 40)
			{
				int by = (int)botSY;
				iShowImage(drawRX - 12, by - 6, 24, 24, gL2TexRopeAnchor);
			}
		}
	}
}

//======================================================================
// Mountain Traps during Rope Climbing (Icicles, Avalanches, Frost Wind)
//======================================================================
void l2DrawRopeTraps()
{
	int i;
	for (i = 0; i < L2_MAX_ROPE_TRAPS; i++)
	{
		struct L2RopeTrap *rt = &gL2RopeTraps[i];
		if (!rt->active) continue;

		double sy = l2ScreenY(rt->y);
		if (sy < -50 || sy > SCREEN_HEIGHT + 50) continue;

		int sx = (int)rt->x;
		int syi = (int)sy;

		// 1. SHARP FALLING ICICLE (Image-based)
		if (rt->type == L2_RTRAP_ICICLE)
		{
			iShowImage(sx - 12, syi - 36, 24, 48, gL2TexIcicleTrap);
		}
		// 2. MOUNTAIN AVALANCHE & ROCKFALL (Image-based)
		else if (rt->type == L2_RTRAP_AVALANCHE)
		{
			iShowImage(sx - 16, syi - 16, 32, 32, gL2TexFallingRock);
		}
		// 3. FREEZING BLIZZARD WIND (Image-based)
		else if (rt->type == L2_RTRAP_FROSTWIND)
		{
			iShowImage(sx - 20, syi - 16, 40, 32, gL2TexMist);
		}
	}
}

//======================================================================
// Rope Climbing Status HUD & Hazard Warnings
//======================================================================
void l2DrawRopeHud()
{
	char buf[64];
	double timeLeft = L2_ROPE_DURATION - gL2RopeElapsed;
	if (timeLeft < 0.0) timeLeft = 0.0;

	double speedRatio = gL2RopeClimbSpeed / L2_ROPE_SPEED_START;
	double progress = gL2RopeElapsed / L2_ROPE_DURATION;
	if (progress > 1.0) progress = 1.0;

	// 1. Center Top Status Gauge
	int boxW = 340;
	int boxH = 44;
	int boxX = (SCREEN_WIDTH - boxW) / 2;
	int boxY = 444;

	iSetColorA(16, 22, 38, 0.90);
	iFilledRectangle(boxX, boxY, boxW, boxH);
	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.90);
	uiBorder(boxX, boxY, boxW, boxH, 2);

	// Header
	iSetColorA(255, 220, 90, 1.0);
	sprintf(buf, "[ \x7F SHEER CLIFF ROPE ASCENT \x7F ]");
	uiTextCentered(SCREEN_WIDTH / 2, boxY + 28, buf, GLUT_BITMAP_HELVETICA_12);

	// Time & Speed
	sprintf(buf, "TIME: %.1fs   |   SPEED: %.1fx", timeLeft, speedRatio);
	iSetColorA(210, 240, 255, 0.95);
	uiTextCentered(SCREEN_WIDTH / 2, boxY + 12, buf, GLUT_BITMAP_HELVETICA_10);

	// Progress bar on bottom edge
	{
		int barW = boxW - 8;
		int fillW = (int)(barW * progress);
		iSetColorA(30, 40, 60, 0.95);
		iFilledRectangle(boxX + 4, boxY + 2, barW, 4);
		iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 1.0);
		if (fillW > 0)
			iFilledRectangle(boxX + 4, boxY + 2, fillW, 4);
	}

	// 2. Incoming Hazard Warning Indicators above Rope Lanes
	{
		int lane;
		for (lane = 0; lane < L2_ROPE_LANES; lane++)
		{
			int hasHazard = 0;
			int i;
			for (i = 0; i < L2_MAX_ROPE_TRAPS; i++)
			{
				struct L2RopeTrap *rt = &gL2RopeTraps[i];
				if (rt->active && rt->lane == lane && rt->y > gL2PlayerY)
				{
					hasHazard = 1;
					break;
				}
			}

			if (hasHazard)
			{
				int rx = (lane == 0) ? (int)L2_ROPE_X_LEFT :
				         (lane == 1) ? (int)L2_ROPE_X_MID : (int)L2_ROPE_X_RIGHT;
				iShowImage(rx - 12, 450, 24, 24, gL2TexSnowParticle);
			}
		}
	}

	// 3. Announcement Banner when rope is first deployed
	if (gL2RopeBannerTimer > 0.0)
	{
		int banW = 540;
		int banH = 50;
		int banX = (SCREEN_WIDTH - banW) / 2;
		int banY = 260;

		iSetColorA(10, 16, 28, 0.92);
		iFilledRectangle(banX, banY, banW, banH);
		iSetColorA(255, 215, 80, 1.0);
		uiBorder(banX, banY, banW, banH, 2);

		uiTextCentered(SCREEN_WIDTH / 2, banY + 30,
		               "[ \x7F SHEER MOUNTAIN CLIFF - ROPE CLIMB! \x7F ]",
		               GLUT_BITMAP_HELVETICA_18);
		iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 1.0);
		uiTextCentered(SCREEN_WIDTH / 2, banY + 10,
		               "USE  A / D  (OR ARROWS)  TO DODGE HAZARDS  -  W TO BOOST SPEED",
		               GLUT_BITMAP_HELVETICA_12);
	}
}

//======================================================================
// Natural Mountain Rock Ledges with Pillowed Snowdrifts (Image-based)
//======================================================================
void l2DrawPlatform(struct L2Platform *p)
{
    double sy = l2ScreenY(p->y);
    int sx, syi;

    if (!p->active) return;
    if (sy > SCREEN_HEIGHT + 45 || sy + p->h < -45) return;

    sx  = (int)p->x;
    syi = (int)sy;

    double shakeX = 0.0, shakeY = 0.0;
    if (p->type == L2_PLAT_CRUMBLING && p->crumbleTimer >= 0.0)
    {
        double urgency = 1.0 - (p->crumbleTimer / L2_CRUMBLE_TIME);
        shakeX = sin(urgency * 45.0) * 3.5 * urgency;
        shakeY = cos(urgency * 40.0) * 2.5 * urgency;
    }

    int px = sx + (int)shakeX;
    int py = syi + (int)shakeY;
    int pw = p->w;
    int ph = p->h;
    int isTerrace = (pw >= 450);

    if (isTerrace)
    {
        iShowImage(px, py - 12, pw, ph + 32, gL2TexPlatTerrace);
    }
    else if (p->type == L2_PLAT_MOVING)
    {
        iShowImage(px, py - 12, pw, ph + 32, gL2TexPlatIce);
    }
    else if (p->type == L2_PLAT_CRUMBLING)
    {
        iShowImage(px, py - 12, pw, ph + 32, gL2TexPlatCrumble);
    }
    else
    {
        iShowImage(px, py - 12, pw, ph + 32, gL2TexPlatStone);
    }
}

//======================================================================
// Natural Alpine Frost Spikes & Jagged Glacial Ice Crystals (Image-based)
//======================================================================
void l2DrawGear(struct L2Gear *g)
{
    double sy = l2ScreenY(g->y);
    int cx, cy;

    if (!g->active) return;
    if (sy < -50 || sy > SCREEN_HEIGHT + 50) return;

    cx = (int)g->x;
    cy = (int)sy;

    iShowImage(cx - 16, cy - 8, 32, 32, gL2TexFrostSpike);
}

//======================================================================
// Multi-Faceted Falling Boulders (Image-based)
//======================================================================
void l2DrawRock(struct L2FallingRock *r)
{
    double sy;
    int sx, syi;

    if (!r->active) return;

    sy = l2ScreenY(r->y);
    if (sy > SCREEN_HEIGHT + 80 || sy + r->h < -80) return;

    sx  = (int)r->x;
    syi = (int)sy;

    iShowImage(sx, syi, r->w, r->h, gL2TexFallingRock);
}

//======================================================================
// Giant Falling Snow Trap / Avalanche Snow Mass (Image-based)
//======================================================================
void l2DrawSnowChunk(struct L2SnowChunk *sc)
{
    double sy;
    int cx, cy, r;

    if (!sc->active) return;

    sy = l2ScreenY(sc->y);
    if (sy > SCREEN_HEIGHT + 90 || sy < -90) return;

    cx = (int)sc->x;
    cy = (int)sy;
    r  = (int)sc->size;

    iShowImage(cx - r, cy - r, r * 2, r * 2, gL2TexSnowBoulder);
}

//======================================================================
// Authentic Mountaineering Equipment & Supplies (Image-based)
//======================================================================
void l2DrawPickup(struct L2Pickup *pu)
{
    double sy;
    int sx, syi, cx, cy;

    if (pu->taken) return;

    sy = l2ScreenY(pu->y);
    if (sy > SCREEN_HEIGHT + 40 || sy + pu->h < -40) return;

    sx  = (int)pu->x;
    syi = (int)sy;
    cx  = sx + pu->w / 2;
    cy  = syi + pu->h / 2;

    if (pu->type == PU_WINTER_HEAL)
        iShowImage(cx - 14, cy - 14, 28, 28, gL2TexPuHeal);
    else if (pu->type == PU_WINTER_STAMINA || pu->type == PU_ENERGY)
        iShowImage(cx - 14, cy - 14, 28, 28, gL2TexPuStamina);
    else if (pu->type == PU_SHIELD)
        iShowImage(cx - 14, cy - 14, 28, 28, gL2TexPuShield);
    else
        iShowImage(cx - 14, cy - 14, 28, 28, gL2TexPuBoots);
}

//======================================================================
// Character Sprite Drawing Helper with Alpha Blending & Flip
//======================================================================
void l2DrawCharSprite(double x, double y, double w, double h, unsigned int tex, int flipH, float alpha)
{
	if (tex == 0) return;

	if (!flipH && alpha >= 0.98f)
	{
		iShowImage((int)x, (int)y, (int)w, (int)h, tex);
		return;
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (alpha < 0.98f)
	{
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glColor4f(1.0f, 1.0f, 1.0f, alpha);
	}
	else
	{
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}

	float u0 = flipH ? 1.0f : 0.0f;
	float u1 = flipH ? 0.0f : 1.0f;

	glBegin(GL_QUADS);
		glTexCoord2f(u0, 0.0f);  glVertex2f((GLfloat)x, (GLfloat)y);
		glTexCoord2f(u1, 0.0f);  glVertex2f((GLfloat)(x + w), (GLfloat)y);
		glTexCoord2f(u1, -1.0f); glVertex2f((GLfloat)(x + w), (GLfloat)(y + h));
		glTexCoord2f(u0, -1.0f); glVertex2f((GLfloat)x, (GLfloat)(y + h));
	glEnd();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glDisable(GL_TEXTURE_2D);
}

//======================================================================
// Mountaineer Player - Hooded Blue Suit, Tactical Pack, Ice Axes & Crampons
//======================================================================
void l2DrawPlayer()
{
    int px = (int)gL2PlayerX;
    int py = (int)l2ScreenY(gL2PlayerY);
    int cx = px + L2_PLAYER_W / 2;

    float charAlpha = 1.0f;
    if (gL2InvulnTime > 0.0)
    {
        // Smooth mercy flash: never vanishes
        charAlpha = (((int)(gL2InvulnTime * 14.0)) % 2 == 0) ? 0.38f : 0.88f;
    }

    if (gL2ShieldTime > 0.0)
    {
        iShowImage(cx - 42, py + (int)(L2_PLAYER_H * 0.45) - 42, 84, 84, gL2TexShieldBubble);
    }

    if (gL2BootsTime > 0.0)
    {
        iShowImage(cx - 14, py - 6, 28, 28, gL2TexPuBoots);
    }

    // DRAW CLIMBER CHARACTER (Uploaded Mountaineer Character)
    if (gL2TexChar > 0)
    {
        double spriteW = 42.0;
        double spriteH = 82.0;
        double drawX = (double)cx - spriteW * 0.5;
        double drawY = (double)py;
        int flipH = !gL2FacingRight;

        if (gL2PlayerState == L2_PS_ROPE)
        {
            // Facing and climb step motion on the rope
            flipH = 0;
            double climbBob = sin(gL2RopeElapsed * 10.0) * 2.5;
            drawY += climbBob;
        }
        else if (gL2PlayerState == L2_PS_CLIMB)
        {
            // Face into the climbed wall
            flipH = (gL2ClimbWall == 1) ? 1 : 0;
            if (fabs(gL2PlayerVY) > 10.0)
            {
                drawY += sin(gL2Elapsed * 12.0) * 2.0;
            }
        }
        else if (gL2PlayerState == L2_PS_AIR)
        {
            // Airborne dynamics
            if (gL2PlayerVY > 0)
                drawY += 2.0;
            else
                drawY -= 1.0;
        }
        else
        {
            // Running / walking footstep bob
            if (fabs(gL2PlayerVX) > 20.0)
            {
                drawY += fabs(sin(gL2Elapsed * 15.0)) * 2.5;
            }
            else
            {
                // Subtle idle breathing
                drawY += sin(gL2Elapsed * 2.5) * 1.0;
            }
        }

        l2DrawCharSprite(drawX, drawY, spriteW, spriteH, gL2TexChar, flipH, charAlpha);
    }
}

//======================================================================
void l2DrawGround()
{
    double gy = l2ScreenY(0.0);
    int gyi = (int)gy;

    if (gyi > SCREEN_HEIGHT) return;
    if (gyi < -100) return;

    // Draw textured rock terrace ground
    int x;
    for (x = 0; x < SCREEN_WIDTH; x += 180)
    {
        iShowImage(x, gyi - 28, 180, 48, gL2TexPlatTerrace);
    }
    // Below the terrace edge, fill with solid dark stone texture if ground is raised
    if (gyi > 28)
    {
        for (x = 0; x < SCREEN_WIDTH; x += 160)
        {
            int y;
            for (y = 0; y < gyi - 28; y += 80)
            {
                iShowImage(x, y, 160, 80, gL2TexPlatStone);
            }
        }
    }
}

//======================================================================
// Helper: Draw Power-Up Status Frame on Right Side
//======================================================================
void l2DrawPowerUpFrame(int x, int y, int size, int type, int isActive, int secondsLeft)
{
    iSetColorA(14, 18, 32, 0.88);
    iFilledRectangle(x, y, size, size);

    if (isActive)
    {
        iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 1.0);
        uiBorder(x, y, size, size, 2);
    }
    else
    {
        iSetColorA(70, 80, 105, 0.7);
        uiBorder(x, y, size, size, 1);
    }

    int cx = x + size / 2;
    int cy = y + size / 2;

    if (type == PU_BOOTS)
    {
        iShowImage(cx - 14, cy - 14, 28, 28, gL2TexPuBoots);
    }
    else if (type == PU_ENERGY || type == PU_WINTER_STAMINA)
    {
        iShowImage(cx - 14, cy - 14, 28, 28, gL2TexPuStamina);
    }
    else if (type == PU_SHIELD)
    {
        iShowImage(cx - 14, cy - 14, 28, 28, gL2TexPuShield);
    }
    else if (type == PU_WINTER_HEAL)
    {
        iShowImage(cx - 14, cy - 14, 28, 28, gL2TexPuHeal);
    }

    if (isActive && secondsLeft > 0)
    {
        char sbuf[16];
        sprintf(sbuf, "%ds", secondsLeft);
        iSetColorA(255, 255, 255, 1.0);
        uiTextCentered(cx, y + 4, sbuf, GLUT_BITMAP_HELVETICA_10);
    }
}

//======================================================================
// Master HUD - Matching Reference Image
//======================================================================
void l2DrawHud()
{
    char buf[96];
    int seconds = l2SecondsLeft();
    double progress = gL2PlayerY / (double)L2_SUMMIT_Y;
    if (progress < 0.0) progress = 0.0;
    if (progress > 1.0) progress = 1.0;

    // 1. Photorealistic Smoked Frosted Glass HUD Header Banner
    uiDrawPictureBanner(0, L2_HUD_Y, SCREEN_WIDTH, L2_HUD_H, gTexHudBanner);

    // ---- 1. LEFT SIDE: HP & STAMINA (Photorealistic Picture Textures) ----
    // HP Bar with Heart Badge & Liquid Ruby Vitality Texture
    uiDrawPictureBar(16, 542, 220, 26, gL2Hp / (double)L2_HP_MAX,
                     gTexHudHpFill, gTexHudBarFrame, gTexHudHpIcon, 28, 28);
    sprintf(buf, "%d", (int)(gL2Hp + 0.5));
    iSetColorA(255, 255, 255, 0.95);
    uiText(242, 548, buf, GLUT_BITMAP_HELVETICA_12);

    // Stamina Bar with Lightning Badge & Liquid Golden Energy Texture
    uiDrawPictureBar(16, 510, 220, 26, gL2Stamina / (double)L2_STAMINA_MAX,
                     gTexHudStaminaFill, gTexHudBarFrame, gTexHudStaminaIcon, 28, 28);
    sprintf(buf, "%d", (int)(gL2Stamina + 0.5));
    iSetColorA(255, 255, 255, 0.95);
    uiText(242, 516, buf, GLUT_BITMAP_HELVETICA_12);

    // ---- 2. CENTER: LEVEL TITLE & SUMMIT SLIDER -----------------------
    iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
    uiText(310, 554, "LEVEL: 02 - CLIMBING CHALLENGE", GLUT_BITMAP_HELVETICA_12);

    iSetColorA(200, 212, 230, 0.85);
    uiText(300, 508, "BASE", GLUT_BITMAP_HELVETICA_10);
    l2TextRight(665, 508, "SUMMIT", GLUT_BITMAP_HELVETICA_10);

    int trackX = 345;
    int trackW = 270;
    int trackY = 512;

    iSetColorA(25, 30, 48, 0.95);
    iFilledRectangle(trackX, trackY, trackW, 4);
    iSetColorA(65, 75, 100, 0.8);
    uiBorder(trackX, trackY, trackW, 4, 1);

    int pFill = (int)(trackW * progress);
    if (pFill > 0)
    {
        iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.95);
        iFilledRectangle(trackX, trackY, pFill, 4);
    }

    // Climber Pin Avatar Marker on Track
    {
        int pinX = trackX + pFill;
        int pinY = 512;
        iShowImage(pinX - 8, pinY, 16, 26, gL2TexChar);
    }

    // ---- 3. RIGHT SIDE: SCORE & TIME ----------------------------------
    sprintf(buf, "SCORE: %ld", gL2Score);
    iSetColorA(255, 255, 255, 1.0);
    l2TextRight(1006, 550, buf, GLUT_BITMAP_HELVETICA_18);

    long l2Best = profileGetLevelHighScore(2);
    if (l2Best > 0)
    {
        sprintf(buf, "BEST: %ld", l2Best);
        iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.95);
        l2TextRight(1006, 532, buf, GLUT_BITMAP_HELVETICA_12);
    }

    sprintf(buf, "TIME: %02d:%02d", seconds / 60, seconds % 60);
    if (seconds <= 20)
        iSetColorA(COL_RED_R, COL_RED_G, COL_RED_B, 1.0);
    else
        iSetColorA(55, 190, 245, 1.0);
    l2TextRight(1006, 516, buf, GLUT_BITMAP_HELVETICA_18);

    // ---- 4. RIGHT-SIDE POWER-UP STATUS FRAMES ------------------------
    int bootsActive  = (gL2BootsTime > 0.0);
    int energyActive = (gL2Stamina >= 80.0);

    l2DrawPowerUpFrame(896, 468, 48, PU_BOOTS,  bootsActive,  (int)(gL2BootsTime + 0.99));
    l2DrawPowerUpFrame(956, 468, 48, PU_ENERGY, energyActive, 0);

    l2DrawPowerUpFrame(896, 16, 48, PU_BOOTS,  bootsActive,  0);
    l2DrawPowerUpFrame(956, 16, 48, PU_ENERGY, energyActive, 0);
}

//======================================================================
// Overlays

//======================================================================
// Overlays
//======================================================================
void l2DrawReadyOverlay()
{
	int go = (gL2ReadyTime > L2_READY_SECONDS * 0.75);

	uiDimScreen(0.55);

	iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
	if (go)
		uiTextScaledCentered(SCREEN_WIDTH / 2, 330, "CLIMB!", 0.60, 4.0);
	else
		uiTextScaledCentered(SCREEN_WIDTH / 2, 330, "GET READY", 0.42, 3.0);

	iSetColorA(255, 255, 255, 1.0);
	uiTextCentered(SCREEN_WIDTH / 2, 286, "LEVEL 02  -  CLIMBING CHALLENGE", GLUT_BITMAP_TIMES_ROMAN_24);

	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 1.0);
	uiTextCentered(SCREEN_WIDTH / 2, 250, "SPACE: JUMP BETWEEN BLOCKS  |  A/D: MOVE  |  W: SCALE ROPES  |  P: PAUSE", GLUT_BITMAP_HELVETICA_12);
	uiTextCentered(SCREEN_WIDTH / 2, 230, "Reach the summit before time runs out. Manage your stamina!", GLUT_BITMAP_HELVETICA_12);
	uiTextCentered(SCREEN_WIDTH / 2, 210, "Scale the mountain platforms, survive rope climbing sections & traps!", GLUT_BITMAP_HELVETICA_12);
}

void l2DrawPauseOverlay()
{
	uiDimScreen(0.68);

	iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
	uiTextScaledCentered(SCREEN_WIDTH / 2, 320, "PAUSED", 0.46, 3.0);

	iSetColorA(255, 255, 255, 1.0);
	uiTextCentered(SCREEN_WIDTH / 2, 268, "P  -  Resume", GLUT_BITMAP_HELVETICA_18);
	uiTextCentered(SCREEN_WIDTH / 2, 240, "ESC  -  Quit to level select", GLUT_BITMAP_HELVETICA_18);
}

void l2DrawResultOverlay()
{
    char buf[96];
    int won = (gL2State == L2_STATE_WON);
    int cx  = SCREEN_WIDTH / 2;
    int elapsed = (int)gL2Elapsed;
    int heightPct = (int)(100.0 * gL2PlayerY / L2_SUMMIT_Y);
    int y;

    if (heightPct > 100) heightPct = 100;

    uiDimScreen(0.78);

    uiPanel(cx - 250, 108, 500, 350,
            COL_PANEL_R, COL_PANEL_G, COL_PANEL_B, 0.94,
            won ? 90 : COL_RED_R, won ? 220 : COL_RED_G, won ? 130 : COL_RED_B, 1.0);

    if (won)
    {
        iSetColorA(90, 220, 130, 1.0);
        uiTextScaledCentered(cx, 408, "SUMMIT REACHED", 0.26, 2.5);
    }
    else
    {
        iSetColorA(COL_RED_R, COL_RED_G, COL_RED_B, 1.0);
        uiTextScaledCentered(cx, 408, "CLIMB FAILED", 0.30, 2.5);
    }

    iSetColorA(220, 228, 238, 1.0);
    if (won)
        uiTextCentered(cx, 382, "You conquered the mountain!",
                       GLUT_BITMAP_HELVETICA_12);
    else if (gL2LoseReason == L2_LOSE_HEALTH)
        uiTextCentered(cx, 382, "Your health reached zero.",
                       GLUT_BITMAP_HELVETICA_12);
    else
        uiTextCentered(cx, 382, "The timer expired before you reached the summit.",
                       GLUT_BITMAP_HELVETICA_12);

    uiRule(cx - 200, 368, 400, COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.5);

    y = 340;

    iSetColorA(200, 210, 225, 1.0);
    uiText(cx - 190, y, "FINAL SCORE", GLUT_BITMAP_HELVETICA_12);
    sprintf(buf, "%ld", gL2Score);
    iSetColorA(255, 255, 255, 1.0);
    l2TextRight(cx + 190, y - 3, buf, GLUT_BITMAP_HELVETICA_18);

    long l2Best = profileGetLevelHighScore(2);
    if (gNewHighScoreAchieved)
    {
        iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
        uiTextCentered(cx, y - 16, "★ NEW PERSONAL BEST RECORD! ★", GLUT_BITMAP_HELVETICA_12);
    }
    else if (l2Best > 0)
    {
        char pbBuf[32];
        sprintf(pbBuf, "PERSONAL BEST: %ld", l2Best);
        iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.85);
        uiTextCentered(cx, y - 16, pbBuf, GLUT_BITMAP_HELVETICA_10);
    }

    y -= 30;
    iSetColorA(200, 210, 225, 1.0);
    uiText(cx - 190, y, "TIME TAKEN", GLUT_BITMAP_HELVETICA_12);
    sprintf(buf, "%02d:%02d", elapsed / 60, elapsed % 60);
    iSetColorA(255, 255, 255, 1.0);
    l2TextRight(cx + 190, y - 3, buf, GLUT_BITMAP_HELVETICA_18);

    y -= 30;
    iSetColorA(200, 210, 225, 1.0);
    uiText(cx - 190, y, "HEIGHT REACHED", GLUT_BITMAP_HELVETICA_12);
    sprintf(buf, "%d %%", heightPct);
    iSetColorA(255, 255, 255, 1.0);
    l2TextRight(cx + 190, y - 3, buf, GLUT_BITMAP_HELVETICA_18);

    y -= 30;
    iSetColorA(200, 210, 225, 1.0);
    uiText(cx - 190, y, "COMPLETION", GLUT_BITMAP_HELVETICA_12);
    iSetColorA(won ? 90 : COL_RED_R, won ? 220 : COL_RED_G, won ? 130 : COL_RED_B, 1.0);
    l2TextRight(cx + 190, y - 3, won ? "COMPLETED" : "NOT COMPLETED",
                GLUT_BITMAP_HELVETICA_18);

    if (won)
    {
        l1DrawMedal(cx, 196, gL2Medal);
    }
    else
    {
        iSetColorA(COL_GREY_R, COL_GREY_G, COL_GREY_B, 1.0);
        uiTextCentered(cx, 176, "NO MEDAL AWARDED", GLUT_BITMAP_HELVETICA_18);
    }

    iSetColorA(0, 0, 0, 0.62);
    iFilledRectangle(0, 0, SCREEN_WIDTH, 26);

    iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.95);
    uiTextCentered(cx, 9, "ENTER  -  Try again        ESC  -  Back to level select",
                   GLUT_BITMAP_HELVETICA_12);
}

//======================================================================
// Frame entry point

//======================================================================
// Master Draw Routine
//======================================================================
void level02Draw()
{
	int i;

	// 1. Mountain Backdrop, Snow & Alpine Mist
	l2DrawBackground();
	l2DrawGround();
	l2DrawSnowflakes();
	l2DrawMountainFace();
	l2DrawMountainMist();
	l2DrawRopes();
	l2DrawSummit();

	// 2. Platforms & Blocks
	for (i = 0; i < gL2PlatformCount; i++)
		l2DrawPlatform(&gL2Platforms[i]);

	// 3. Obstacles & Hazards
	for (i = 0; i < gL2GearCount; i++)
		l2DrawGear(&gL2Gears[i]);

	for (i = 0; i < L2_MAX_ROCKS; i++)
		l2DrawRock(&gL2Rocks[i]);

	for (i = 0; i < L2_MAX_SNOW_CHUNKS; i++)
		l2DrawSnowChunk(&gL2SnowChunks[i]);

	// 4. Rope Sections & Traps
	if (gL2RopeActive)
		l2DrawRopeTraps();

	// 5. In-World Pickups
	for (i = 0; i < gL2PickupCount; i++)
		l2DrawPickup(&gL2Pickups[i]);

	// 6. Mountaineer Player
	l2DrawPlayer();

	// 7. HUD & Overlays
	l2DrawHud();

	if (gL2RopeActive)
		l2DrawRopeHud();

	if (gL2State == L2_STATE_READY)       l2DrawReadyOverlay();
	else if (gL2State == L2_STATE_PAUSED) l2DrawPauseOverlay();
	else if (gL2State == L2_STATE_WON || gL2State == L2_STATE_LOST) l2DrawResultOverlay();
}

#endif