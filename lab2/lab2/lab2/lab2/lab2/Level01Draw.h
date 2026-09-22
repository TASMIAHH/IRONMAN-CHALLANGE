//======================================================================
//  Level01Draw.h
//  All rendering for LEVEL 01 - RUNNING TRAINING: parallax background,
//  world entities, the runner, the HUD and the result screen.
//
//  Drawing only. Nothing in here changes game state; the simulation
//  lives in Level01.h and runs on the fixedUpdate() tick.
//  Included once, from iMain.cpp, straight after Level01.h.
//======================================================================
#ifndef LEVEL01_DRAW_H
#define LEVEL01_DRAW_H

//----------------------------------------------------------------------
// HUD geometry
//----------------------------------------------------------------------
#define L1_HUD_Y        496
#define L1_HUD_H         80
#define L1_BAR_W        240
#define L1_BAR_H         12

//----------------------------------------------------------------------
// Small local helpers
//----------------------------------------------------------------------
void l1TextRight(int rightX, int y, const char *s, void *font)
{
    uiText(rightX - uiTextWidth(s, font), y, s, font);
}

void l1Bar(int x, int y, int w, int h, double ratio,
           int r, int g, int b)
{
    int fill;

    if (ratio < 0.0) ratio = 0.0;
    if (ratio > 1.0) ratio = 1.0;
    fill = (int)(w * ratio);

    iSetColorA(0, 0, 0, 0.55);
    iFilledRectangle(x, y, w, h);

    if (fill > 0)
    {
        iSetColorA(r, g, b, 0.95);
        iFilledRectangle(x, y, fill, h);
    }

    iSetColorA(255, 255, 255, 0.45);
    uiBorder(x, y, w, h, 1);
}

//======================================================================
//  Ken Perlin's C^2 Smootherstep (zero 1st & 2nd derivative at endpoints)
//  Guarantees ultra-smooth, imperceptible biome cross-fading
//======================================================================
static inline double l1Smootherstep(double t)
{
	if (t <= 0.0) return 0.0;
	if (t >= 1.0) return 1.0;
	return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

//======================================================================
//  Biome weight calculation for ultra-smooth world-space transitions
//  Wide 2400-pixel transition window (transHalf = 1200.0) ensures gradual,
//  natural cross-fading between all 4 running environments.
//======================================================================
double l1GetBiomeWeight(int biome, double worldX)
{
	const double transHalf = 1200.0; // 2400px total transition window (~8 seconds of running)
	if (biome == L1_BIOME_COAST) // 0 to 11000
	{
		if (worldX <= L1_BIOME_0_END - transHalf) return 1.0;
		if (worldX >= L1_BIOME_0_END + transHalf) return 0.0;
		double t = (worldX - (L1_BIOME_0_END - transHalf)) / (2.0 * transHalf);
		return 1.0 - l1Smootherstep(t);
	}
	else if (biome == L1_BIOME_FOREST) // 11000 to 22000
	{
		if (worldX <= L1_BIOME_0_END - transHalf) return 0.0;
		if (worldX < L1_BIOME_0_END + transHalf)
		{
			double t = (worldX - (L1_BIOME_0_END - transHalf)) / (2.0 * transHalf);
			return l1Smootherstep(t);
		}
		if (worldX <= L1_BIOME_1_END - transHalf) return 1.0;
		if (worldX < L1_BIOME_1_END + transHalf)
		{
			double t = (worldX - (L1_BIOME_1_END - transHalf)) / (2.0 * transHalf);
			return 1.0 - l1Smootherstep(t);
		}
		return 0.0;
	}
	else if (biome == L1_BIOME_MOUNTAIN) // 22000 to 33000
	{
		if (worldX <= L1_BIOME_1_END - transHalf) return 0.0;
		if (worldX < L1_BIOME_1_END + transHalf)
		{
			double t = (worldX - (L1_BIOME_1_END - transHalf)) / (2.0 * transHalf);
			return l1Smootherstep(t);
		}
		if (worldX <= L1_BIOME_2_END - transHalf) return 1.0;
		if (worldX < L1_BIOME_2_END + transHalf)
		{
			double t = (worldX - (L1_BIOME_2_END - transHalf)) / (2.0 * transHalf);
			return 1.0 - l1Smootherstep(t);
		}
		return 0.0;
	}
	else // L1_BIOME_SUNSET: 33000 to 44000
	{
		if (worldX <= L1_BIOME_2_END - transHalf) return 0.0;
		if (worldX < L1_BIOME_2_END + transHalf)
		{
			double t = (worldX - (L1_BIOME_2_END - transHalf)) / (2.0 * transHalf);
			return l1Smootherstep(t);
		}
		return 1.0;
	}
}

// Low-level textured quad renderer with independent left/right vertex alpha
void l1DrawBiomeQuad(double x0, double x1, double y0, double y1,
                     float u0, float u1, unsigned int tex,
                     float alphaL, float alphaR)
{
	if (!tex) return;
	if (x1 <= 0.0 || x0 >= (double)SCREEN_WIDTH) return;
	if (alphaL <= 0.001f && alphaR <= 0.001f) return;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
		glColor4f(1.0f, 1.0f, 1.0f, alphaL);
		glTexCoord2f(u0, 0.0f);
		glVertex2f((GLfloat)x0, (GLfloat)y0);

		glColor4f(1.0f, 1.0f, 1.0f, alphaR);
		glTexCoord2f(u1, 0.0f);
		glVertex2f((GLfloat)x1, (GLfloat)y0);

		glColor4f(1.0f, 1.0f, 1.0f, alphaR);
		glTexCoord2f(u1, -1.0f);
		glVertex2f((GLfloat)x1, (GLfloat)y1);

		glColor4f(1.0f, 1.0f, 1.0f, alphaL);
		glTexCoord2f(u0, -1.0f);
		glVertex2f((GLfloat)x0, (GLfloat)y1);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

//======================================================================
//  1. Continuous Multi-Biome Running Background with Seamless Mirror Tiling
//======================================================================
void l1DrawBackground()
{
	double leftWorldX = gL1PlayerX - L1_PLAYER_SCREEN_X;
	double rightWorldX = leftWorldX + (double)SCREEN_WIDTH;

	// High-resolution slicing: 32 slices across 1024px screen (32px each)
	// provides micro-smooth spatial alpha cross-fading during transitions.
	const double sliceW = 32.0;
	int startSlice = (int)floor(leftWorldX / sliceW);
	int endSlice   = (int)ceil(rightWorldX / sliceW);

	int sl, b;
	for (sl = startSlice; sl <= endSlice; sl++)
	{
		double wX0 = sl * sliceW;
		double wX1 = (sl + 1) * sliceW;

		double sx0 = wX0 - gL1PlayerX + L1_PLAYER_SCREEN_X;
		double sx1 = wX1 - gL1PlayerX + L1_PLAYER_SCREEN_X;

		for (b = 0; b < L1_BIOME_COUNT; b++)
		{
			float aL = (float)l1GetBiomeWeight(b, wX0);
			float aR = (float)l1GetBiomeWeight(b, wX1);

			if (aL <= 0.001f && aR <= 0.001f)
				continue;

			// Seamless alternating mirrored segment formula (zero seam edges)
			int seg = (int)floor(wX0 / 1024.0);
			double loc0 = wX0 - seg * 1024.0;
			double loc1 = wX1 - seg * 1024.0;

			float u0, u1;
			if (abs(seg) % 2 != 0)
			{
				u0 = (float)(1.0 - loc0 / 1024.0);
				u1 = (float)(1.0 - loc1 / 1024.0);
			}
			else
			{
				u0 = (float)(loc0 / 1024.0);
				u1 = (float)(loc1 / 1024.0);
			}

			l1DrawBiomeQuad(sx0, sx1, 0.0, (double)SCREEN_HEIGHT, u0, u1, gL1TexBg[b], aL, aR);
		}
	}
}

//======================================================================
//  1b. Dynamic Atmospheric Horizon Glow & Sky Lighting
//======================================================================
void l1DrawAtmosphericHaze()
{
	double viewCenterWorldX = gL1PlayerX - L1_PLAYER_SCREEN_X + SCREEN_WIDTH * 0.5;

	// 1. Sunrise Coastal Horizon Warmth (Biome 0)
	float coastW = (float)l1GetBiomeWeight(L1_BIOME_COAST, viewCenterWorldX);
	if (coastW > 0.01f)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Additive morning glow
		glBegin(GL_QUADS);
			glColor4f(1.0f, 0.72f, 0.38f, 0.14f * coastW);
			glVertex2f(0.0f, (float)L1_GROUND_Y);
			glVertex2f((float)SCREEN_WIDTH, (float)L1_GROUND_Y);
			glColor4f(1.0f, 0.45f, 0.20f, 0.0f);
			glVertex2f((float)SCREEN_WIDTH, 340.0f);
			glVertex2f(0.0f, 340.0f);
		glEnd();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// 2. Redwood Canopy Sunbeams & Forest Light (Biome 1)
	float forestW = (float)l1GetBiomeWeight(L1_BIOME_FOREST, viewCenterWorldX);
	if (forestW > 0.01f)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBegin(GL_QUADS);
			glColor4f(0.55f, 0.90f, 0.40f, 0.08f * forestW);
			glVertex2f(0.0f, 120.0f);
			glVertex2f((float)SCREEN_WIDTH, 120.0f);
			glColor4f(0.95f, 0.90f, 0.45f, 0.12f * forestW);
			glVertex2f((float)SCREEN_WIDTH, 480.0f);
			glVertex2f(0.0f, 480.0f);
		glEnd();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// 3. Alpine Valley Crisp Sky Brilliance (Biome 2)
	float mountainW = (float)l1GetBiomeWeight(L1_BIOME_MOUNTAIN, viewCenterWorldX);
	if (mountainW > 0.01f)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBegin(GL_QUADS);
			glColor4f(0.65f, 0.85f, 1.0f, 0.09f * mountainW);
			glVertex2f(0.0f, 140.0f);
			glVertex2f((float)SCREEN_WIDTH, 140.0f);
			glColor4f(0.85f, 0.92f, 1.0f, 0.0f);
			glVertex2f((float)SCREEN_WIDTH, 420.0f);
			glVertex2f(0.0f, 420.0f);
		glEnd();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// 4. Sunset Marathon Twilight Radiance (Biome 3)
	float sunsetW = (float)l1GetBiomeWeight(L1_BIOME_SUNSET, viewCenterWorldX);
	if (sunsetW > 0.01f)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBegin(GL_QUADS);
			glColor4f(1.0f, 0.50f, 0.15f, 0.18f * sunsetW);
			glVertex2f(0.0f, (float)L1_GROUND_Y);
			glVertex2f((float)SCREEN_WIDTH, (float)L1_GROUND_Y);
			glColor4f(0.95f, 0.25f, 0.40f, 0.0f);
			glVertex2f((float)SCREEN_WIDTH, 380.0f);
			glVertex2f(0.0f, 380.0f);
		glEnd();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

//======================================================================
//  1c. Ambient Environmental Particles & Runner Footstep Dust
//======================================================================
void l1DrawParticles()
{
	int i;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 1. Ambient Weather Particles
	for (i = 0; i < L1_MAX_PARTICLES; i++)
	{
		struct L1Particle *p = &gL1Particles[i];
		int px = (int)p->x;
		int py = (int)p->y;
		if (px < -10 || px > SCREEN_WIDTH + 10 || py < 60 || py > SCREEN_HEIGHT - 60)
			continue;

		double pwX = gL1PlayerX - L1_PLAYER_SCREEN_X + p->x;
		int pBio = 0;
		if (pwX >= L1_BIOME_2_END)      pBio = 3;
		else if (pwX >= L1_BIOME_1_END) pBio = 2;
		else if (pwX >= L1_BIOME_0_END) pBio = 1;

		float alpha = (float)(p->alpha * (0.75 + 0.25 * sin(p->phase)));

		if (pBio == L1_BIOME_COAST)
		{
			// Sunrise sea breeze & light golden coastal motes
			iSetColorA(255, 220, 160, alpha * 0.85f);
			iFilledCircle(px, py, (int)(p->size * 0.7));
		}
		else if (pBio == L1_BIOME_FOREST)
		{
			// Redwood forest leaves & pollen drifting
			if (i % 2 == 0)
				iSetColorA(225, 175, 60, alpha); // Amber leaf
			else
				iSetColorA(140, 210, 80, alpha * 0.8f); // Pine green mote
			iFilledEllipse(px, py, (int)(p->size * 1.3), (int)(p->size * 0.7));
		}
		else if (pBio == L1_BIOME_MOUNTAIN)
		{
			// Alpine wildflower petals & dandelion seeds
			if (i % 2 == 0)
				iSetColorA(250, 250, 255, alpha * 0.9f); // Dandelion fluff
			else
				iSetColorA(255, 185, 205, alpha * 0.85f); // Alpine blossom petal
			iFilledCircle(px, py, (int)(p->size * 0.8));
		}
		else // L1_BIOME_SUNSET
		{
			// Golden sunset dusk particles & celebration confetti
			if (i % 3 == 0)
				iSetColorA(255, 200, 50, alpha); // Gold
			else if (i % 3 == 1)
				iSetColorA(255, 90, 60, alpha);  // Crimson
			else
				iSetColorA(100, 220, 255, alpha); // Cyan
			iFilledRectangle(px, py, (int)(p->size * 1.1), (int)(p->size * 0.9));
		}
	}

	// 2. Runner Footstep Dust Puffs
	for (i = 0; i < L1_MAX_DUST; i++)
	{
		struct L1DustParticle *d = &gL1Dust[i];
		if (d->life > 0.0)
		{
			float ratio = (float)(d->life / d->maxLife);
			float dAlpha = ratio * 0.42f;
			int dSize = (int)d->size;

			// Natural earthy dust color
			iSetColorA(215, 195, 165, dAlpha);
			iFilledCircle((int)d->x, (int)d->y, dSize);
			iSetColorA(245, 230, 205, dAlpha * 0.5f);
			iFilledCircle((int)(d->x - 2), (int)(d->y + 1), (int)(dSize * 0.6));
		}
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

//======================================================================
//  1d. Biome Announcement HUD Floating Banner
//======================================================================
void l1DrawBiomeBanner()
{
	if (gL1BiomeBannerTimer <= 0.0) return;

	double progress = gL1BiomeBannerTimer / 3.6; // 1.0 down to 0.0
	float alpha = 1.0f;
	if (progress > 0.85)
		alpha = (float)((1.0 - progress) / 0.15);
	else if (progress < 0.25)
		alpha = (float)(progress / 0.25);

	if (alpha < 0.0f) alpha = 0.0f;
	if (alpha > 1.0f) alpha = 1.0f;

	int bw = 460;
	int bh = 42;
	int bx = (SCREEN_WIDTH - bw) / 2;
	int by = 430;

	uiPanel(bx, by, bw, bh, 8, 14, 28, 0.82f * alpha,
	        COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, alpha);

	iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, alpha);
	uiTextCentered(SCREEN_WIDTH / 2, by + 23, "NOW ENTERING", GLUT_BITMAP_HELVETICA_10);

	iSetColorA(255, 255, 255, alpha);
	uiTextCentered(SCREEN_WIDTH / 2, by + 8, gL1BiomeBannerText, GLUT_BITMAP_HELVETICA_12);
}

//======================================================================
//  World entities
//======================================================================
void l1DrawObstacle(struct L1Obstacle *o)
{
    int sx = (int)l1ScreenX(o->x);

    if (sx + o->w < -60 || sx > SCREEN_WIDTH + 60)
        return;                                   // off screen

    int drawY = (int)o->y;

    // ------------------------------------------------------------------
    // 1. Natural Soft Volumetric Sunbeam (Atmospheric Light Shaft)
    // Subtle sunlight filtering through the sky/canopy onto the track
    // ------------------------------------------------------------------
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (sx > -100 && sx < SCREEN_WIDTH + 100)
    {
        double beamAngleOffset = 24.0;
        double bx1 = sx - 15.0 - beamAngleOffset;
        double bx2 = sx + o->w + 15.0 - beamAngleOffset;
        double gx1 = sx - 8.0;
        double gx2 = sx + o->w + 8.0;

        // Subtle warm sunlit atmospheric column (natural photographic lighting)
        if (o->type <= L1_OB_COAST_PUDDLE)
            iSetColorA(255, 246, 225, 0.07);
        else if (o->type <= L1_OB_FOREST_MUD)
            iSetColorA(245, 255, 235, 0.06);
        else if (o->type <= L1_OB_MOUNTAIN_CREVASSE)
            iSetColorA(240, 250, 255, 0.06);
        else
            iSetColorA(255, 240, 215, 0.07);

        double beamX[4] = { bx1, bx2, gx2, gx1 };
        double beamY[4] = { (double)(SCREEN_HEIGHT - 60), (double)(SCREEN_HEIGHT - 60), (double)L1_GROUND_Y, (double)L1_GROUND_Y };
        iFilledPolygon(beamX, beamY, 4);

        // Soft center light core
        iSetColorA(255, 255, 245, 0.04);
        double beamMidX[4] = { bx1 + 10.0, bx2 - 10.0, gx2 - 4.0, gx1 + 4.0 };
        iFilledPolygon(beamMidX, beamY, 4);
    }

    // ------------------------------------------------------------------
    // 2. Authentic Athletic Course Markings (Takeoff Zone on Track)
    // Standard white/sand athletic hazard & takeoff stripe on the trail
    // ------------------------------------------------------------------
    {
        int chalkX = sx - 38;
        if (chalkX > -20 && chalkX < SCREEN_WIDTH + 20)
        {
            // Athletic course takeoff stripe on track surface
            iSetColorA(242, 238, 224, 0.65);
            iFilledRectangle(chalkX, L1_GROUND_Y - 2, 5, 4);

            // Subtle athletic dashed approach guides
            iSetColorA(235, 230, 215, 0.45);
            iFilledRectangle(chalkX - 14, L1_GROUND_Y - 1, 6, 2);
            iFilledRectangle(chalkX - 28, L1_GROUND_Y - 1, 6, 2);
        }
    }

    // ------------------------------------------------------------------
    // 3. Realistic Dual-Layer Ground Contact Shadow (Ambient Occlusion)
    // Anchors the obstacle naturally to the soil / pavement
    // ------------------------------------------------------------------
    // Tight dark ambient occlusion contact crevice directly at the base
    iSetColorA(8, 12, 16, 0.65);
    iFilledEllipse(sx + o->w / 2, L1_GROUND_Y - 1, o->w / 2 + 1, 3);

    // Soft directional ground cast shadow (sun casting to the right/forward)
    iSetColorA(16, 20, 26, 0.30);
    iFilledEllipse(sx + o->w / 2 + 8, L1_GROUND_Y - 2, o->w / 2 + 10, 6);

    // ------------------------------------------------------------------
    // 4. Ground Trench Hazards (Realistic Water Puddles, Mud & Crevasses)
    // ------------------------------------------------------------------
    if (o->type == L1_OB_COAST_PUDDLE)
    {
        // Realistic coastal tidal puddle: wet perimeter asphalt/sand
        iSetColorA(25, 38, 48, 0.50);
        iFilledEllipse(sx + o->w / 2, L1_GROUND_Y - 3, o->w / 2 + 5, 10);

        iShowImage(sx, L1_GROUND_Y - 30, o->w, 32, gL1TexPitHazard);

        // Natural sky reflection on water surface
        iSetColorA(75, 150, 195, 0.65);
        iFilledEllipse(sx + o->w / 2, L1_GROUND_Y - 4, o->w / 2 - 6, 7);

        // Sunlight water specular glint
        iSetColorA(255, 255, 255, 0.70);
        iFilledEllipse(sx + o->w / 2 - 12, L1_GROUND_Y - 3, o->w / 6, 3);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        return;
    }
    else if (o->type == L1_OB_FOREST_MUD)
    {
        // Forest muddy marsh trench: dark peat soil rim
        iSetColorA(36, 22, 12, 0.70);
        iFilledEllipse(sx + o->w / 2, L1_GROUND_Y - 3, o->w / 2 + 5, 11);

        iShowImage(sx, L1_GROUND_Y - 30, o->w, 32, gL1TexPitHazard);

        // Wet rich mud clay cavity
        iSetColorA(72, 46, 22, 0.85);
        iFilledEllipse(sx + o->w / 2, L1_GROUND_Y - 4, o->w / 2 - 6, 8);

        // Wet earth sheen highlight
        iSetColorA(130, 95, 50, 0.40);
        iFilledEllipse(sx + o->w / 2 + 8, L1_GROUND_Y - 4, o->w / 5, 3);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        return;
    }
    else if (o->type == L1_OB_MOUNTAIN_CREVASSE)
    {
        // Alpine mountain rock fissure: crisp granite fracture rim
        iSetColorA(180, 190, 205, 0.55);
        iFilledEllipse(sx + o->w / 2, L1_GROUND_Y - 2, o->w / 2 + 4, 8);

        iShowImage(sx, L1_GROUND_Y - 32, o->w, 34, gL1TexPitHazard);

        // Deep dark rocky abyss shadow
        iSetColorA(14, 16, 22, 0.95);
        iFilledRectangle(sx + 6, L1_GROUND_Y - 26, o->w - 12, 24);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        return;
    }

    // ------------------------------------------------------------------
    // 5. Subtle Natural Silhouette Depth Backing
    // Adds soft photographic separation from dense background foliage
    // ------------------------------------------------------------------
    iSetColorA(12, 14, 18, 0.22);
    iFilledRectangle(sx - 1, drawY - 1, o->w + 2, o->h + 2);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    // ------------------------------------------------------------------
    // 6. Render Obstacle Sprite
    // ------------------------------------------------------------------
    iShowImage(sx, drawY, o->w, o->h, gL1TexObstacle[o->type]);

    // ------------------------------------------------------------------
    // 7. Natural Directional Sunlight Rim Lighting (Top Sun Glint)
    // Physically authentic sun specular highlight along the upper crest
    // ------------------------------------------------------------------
    {
        int topY = drawY + o->h - 2;
        int rimInset = 6;
        int rimW = o->w - rimInset * 2;
        if (rimW < 10) rimW = 10;

        if (o->type <= L1_OB_COAST_PUDDLE)
        {
            // Track 0 (Coast - Morning Dawn): Warm golden sunlight rim
            iSetColorA(255, 246, 218, 0.58);
        }
        else if (o->type <= L1_OB_FOREST_MUD)
        {
            // Track 1 (Forest - Sunlit Canopy): Crisp daylight sunbeam glint
            iSetColorA(248, 255, 230, 0.52);
        }
        else if (o->type <= L1_OB_MOUNTAIN_CREVASSE)
        {
            // Track 2 (Mountain - High Alpine): Crisp white-ice specular crest
            iSetColorA(238, 250, 255, 0.60);
        }
        else
        {
            // Track 3 (Marathon - Sunset Boulevard): Warm sunset golden sheen
            iSetColorA(255, 236, 195, 0.58);
        }

        // Primary sunlit top edge
        iFilledRectangle(sx + rimInset, topY, rimW, 2);

        // Softer feathered glint line
        iSetColorA(255, 255, 255, 0.30);
        iFilledRectangle(sx + rimInset + 4, topY + 1, rimW - 8, 1);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    }
}

void l1DrawPickup(struct L1Pickup *p)
{
    int sx;

    if (p->taken) return;

    sx = (int)l1ScreenX(p->x);
    if (sx + p->w < -60 || sx > SCREEN_WIDTH + 60)
        return;

    // Gentle floating bob
    double bob = 4.0 * sin(gL1Elapsed * 4.0 + p->x * 0.05);
    int drawY = (int)(p->y + bob);

    // Subtle pulsating glow aura around power-up
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); // additive glow
    if (p->type == PU_ENERGY)
        iSetColorA(70, 220, 110, 0.28);
    else if (p->type == PU_SHIELD)
        iSetColorA(60, 180, 255, 0.30);
    else if (p->type == PU_BOOTS)
        iSetColorA(255, 170, 50, 0.28);
    else // PU_COIN
        iSetColorA(255, 215, 60, 0.35);

    int glowPad = 12;
    iFilledEllipse(sx + p->w / 2, drawY + p->h / 2, p->w / 2 + glowPad, p->h / 2 + glowPad);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    iShowImage(sx, drawY, p->w, p->h, gL1TexPickup[p->type]);
}

void l1DrawMarkers()
{
    int sx;

    // START gate
    sx = (int)l1ScreenX(600.0);
    if (sx > -200 && sx < SCREEN_WIDTH + 200)
        iShowImage(sx, L1_GROUND_Y, 113, 120, gL1TexStart);

    // FINISH gate
    sx = (int)l1ScreenX((double)L1_LENGTH);
    if (sx > -300 && sx < SCREEN_WIDTH + 300)
        iShowImage(sx - 30, L1_GROUND_Y, 220, 200, gL1TexFinish);
}

//======================================================================
//  The runner
//======================================================================
void l1DrawPlayer()
{
    int frame;
    int px = L1_PLAYER_SCREEN_X;
    int py = (int)(L1_GROUND_Y + gL1PlayerY) - 2;

    // shield bubble sits behind the sprite
    if (gL1ShieldTime > 0.0)
    {
        double cx = px + L1_PLAYER_W / 2.0;
        double cy = py + L1_PLAYER_H * 0.48;

        iShowImage((int)(cx - 64), (int)(cy - 64), 128, 128, gL1TexShieldBubble);
    }

    // after a hit the runner flickers through the mercy window
    if (gL1InvulnTime > 0.0 && ((int)(gL1InvulnTime * 12.0) % 2) == 0)
        return;

    // frame 4 (index 3) of the cycle reads best as an airborne pose
    frame = gL1OnGround ? gL1Frame : 3;

    iShowImage(px, py, L1_PLAYER_W, L1_PLAYER_H, gL1TexRun[frame]);
}

//======================================================================
//  HUD
//======================================================================
void l1DrawHud()
{
    char buf[96];
    int seconds = l1SecondsLeft();
    double progress = gL1PlayerX / (double)L1_LENGTH;

    if (progress < 0.0) progress = 0.0;
    if (progress > 1.0) progress = 1.0;

    // 1. Photorealistic Smoked Frosted Glass HUD Header Banner
    uiDrawPictureBanner(0, L1_HUD_Y, SCREEN_WIDTH, L1_HUD_H, gTexHudBanner);

    // ---- Left: Health & Stamina Bars (Photorealistic Picture Textures) ----
    // HP Bar with Heart Badge & Liquid Ruby Vitality Texture
    uiDrawPictureBar(16, 542, 220, 26, gL1Hp / (double)L1_HP_MAX,
                     gTexHudHpFill, gTexHudBarFrame, gTexHudHpIcon, 28, 28);
    sprintf(buf, "%d", (int)(gL1Hp + 0.5));
    iSetColorA(255, 255, 255, 0.95);
    uiText(242, 548, buf, GLUT_BITMAP_HELVETICA_12);

    // Stamina Bar with Lightning Badge & Liquid Golden Energy Texture
    uiDrawPictureBar(16, 510, 220, 26, gL1Stamina / (double)L1_STAMINA_MAX,
                     gTexHudStaminaFill, gTexHudBarFrame, gTexHudStaminaIcon, 28, 28);
    sprintf(buf, "%d", (int)(gL1Stamina + 0.5));
    iSetColorA(255, 255, 255, 0.95);
    uiText(242, 516, buf, GLUT_BITMAP_HELVETICA_12);

    // ---- level name --------------------------------------------------
    iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
    uiText(310, 554, "LEVEL: 01 - RUNNING TRAINING", GLUT_BITMAP_HELVETICA_12);

    // ---- active power-ups --------------------------------------------
    {
        int x = 300;

        if (gL1ShieldTime > 0.0)
        {
            sprintf(buf, "SHIELD %ds", (int)(gL1ShieldTime + 0.99));
            iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 1.0);
            uiText(x, 528, buf, GLUT_BITMAP_HELVETICA_12);
            x += 100;
        }
        if (gL1BootsTime > 0.0)
        {
            sprintf(buf, "JET BOOTS %ds", (int)(gL1BootsTime + 0.99));
            iSetColorA(120, 230, 130, 1.0);
            uiText(x, 528, buf, GLUT_BITMAP_HELVETICA_12);
        }
        if (gL1Stamina <= 0.0)
        {
            iSetColorA(COL_RED_R, COL_RED_G, COL_RED_B, 1.0);
            uiText(x + 150, 528, "EXHAUSTED", GLUT_BITMAP_HELVETICA_12);
        }
    }

    // ---- course progress with 4 running zones -----------------------
    int pBarX = 348, pBarW = 310;
    iSetColorA(220, 228, 238, 0.85);
    uiText(296, 508, "START", GLUT_BITMAP_HELVETICA_12);
    l1TextRight(716, 508, "FINISH", GLUT_BITMAP_HELVETICA_12);
    l1Bar(pBarX, 509, pBarW, 8, progress, COL_GOLD_R, COL_GOLD_G, COL_GOLD_B);

    // Zone tick markers
    iSetColorA(255, 255, 255, 0.55);
    iFilledRectangle(pBarX + (int)(pBarW * 0.25), 508, 2, 10);
    iFilledRectangle(pBarX + (int)(pBarW * 0.50), 508, 2, 10);
    iFilledRectangle(pBarX + (int)(pBarW * 0.75), 508, 2, 10);

    // Subtle zone labels above progress bar
    iSetColorA(180, 205, 235, 0.75);
    uiTextCentered(pBarX + (int)(pBarW * 0.125), 498, "COAST", GLUT_BITMAP_HELVETICA_10);
    uiTextCentered(pBarX + (int)(pBarW * 0.375), 498, "FOREST", GLUT_BITMAP_HELVETICA_10);
    uiTextCentered(pBarX + (int)(pBarW * 0.625), 498, "VALLEY", GLUT_BITMAP_HELVETICA_10);
    uiTextCentered(pBarX + (int)(pBarW * 0.875), 498, "MARATHON", GLUT_BITMAP_HELVETICA_10);

    // ---- score -------------------------------------------------------
    sprintf(buf, "SCORE: %ld", gL1Score);
    iSetColorA(255, 255, 255, 1.0);
    l1TextRight(1006, 550, buf, GLUT_BITMAP_HELVETICA_18);

    long l1Best = profileGetLevelHighScore(1);
    if (l1Best > 0)
    {
        sprintf(buf, "BEST: %ld", l1Best);
        iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.95);
        l1TextRight(1006, 532, buf, GLUT_BITMAP_HELVETICA_12);
    }

    // ---- time --------------------------------------------------------
    sprintf(buf, "TIME: %02d:%02d", seconds / 60, seconds % 60);
    if (seconds <= 15)
        iSetColorA(COL_RED_R, COL_RED_G, COL_RED_B, 1.0);
    else
        iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 1.0);
    l1TextRight(1006, 516, buf, GLUT_BITMAP_HELVETICA_18);
}

//======================================================================
//  Medal
//======================================================================
void l1DrawMedal(int cx, int cy, int medal)
{
    int r, g, b;
    const char *name;

    if (medal == 3)      { r = 255; g = 196; b = 70;  name = "GOLD MEDAL";   }
    else if (medal == 2) { r = 205; g = 212; b = 224; name = "SILVER MEDAL"; }
    else                 { r = 201; g = 128; b = 62;  name = "BRONZE MEDAL"; }

    if (gL1TexMedal > 0)
    {
        iShowImage(cx - 32, cy - 32, 64, 64, gL1TexMedal);
    }

    iSetColorA(r, g, b, 1.0);
    uiTextCentered(cx, cy - 52, name, GLUT_BITMAP_HELVETICA_18);
}

//======================================================================
//  Overlays: get ready / paused / results
//======================================================================
void l1DrawReadyOverlay()
{
    int go = (gL1ReadyTime > L1_READY_SECONDS * 0.75);

    uiDimScreen(0.55);

    iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
    if (go)
        uiTextScaledCentered(SCREEN_WIDTH / 2, 330, "GO!", 0.70, 4.0);
    else
        uiTextScaledCentered(SCREEN_WIDTH / 2, 330, "GET READY", 0.42, 3.0);

    iSetColorA(255, 255, 255, 1.0);
    uiTextCentered(SCREEN_WIDTH / 2, 286,
                   "LEVEL 01  -  RUNNING TRAINING", GLUT_BITMAP_TIMES_ROMAN_24);

    iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 1.0);
    uiTextCentered(SCREEN_WIDTH / 2, 250,
        "SPACE / W / UP  jump        D or RIGHT  sprint        A or LEFT  ease off",
        GLUT_BITMAP_HELVETICA_12);
    uiTextCentered(SCREEN_WIDTH / 2, 230,
        "Reach the finish line before the clock runs out.",
        GLUT_BITMAP_HELVETICA_12);
}

void l1DrawPauseOverlay()
{
    uiDimScreen(0.68);

    iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
    uiTextScaledCentered(SCREEN_WIDTH / 2, 320, "PAUSED", 0.46, 3.0);

    iSetColorA(255, 255, 255, 1.0);
    uiTextCentered(SCREEN_WIDTH / 2, 268, "P  -  Resume", GLUT_BITMAP_HELVETICA_18);
    uiTextCentered(SCREEN_WIDTH / 2, 240, "ESC  -  Quit to level select",
                   GLUT_BITMAP_HELVETICA_18);
}

void l1DrawResultOverlay()
{
    char buf[96];
    int won = (gL1State == L1_STATE_WON);
    int cx = SCREEN_WIDTH / 2;
    int elapsed = (int)gL1Elapsed;
    int y;

    uiDimScreen(0.78);

    uiPanel(cx - 250, 118, 500, 340,
            COL_PANEL_R, COL_PANEL_G, COL_PANEL_B, 0.94,
            won ? 90 : COL_RED_R, won ? 220 : COL_RED_G, won ? 130 : COL_RED_B, 1.0);

    // ---- headline ----------------------------------------------------
    if (won)
    {
        iSetColorA(90, 220, 130, 1.0);
        uiTextScaledCentered(cx, 400, "COURSE COMPLETE", 0.26, 2.5);
    }
    else
    {
        iSetColorA(COL_RED_R, COL_RED_G, COL_RED_B, 1.0);
        uiTextScaledCentered(cx, 400, "TRAINING FAILED", 0.26, 2.5);
    }

    iSetColorA(220, 228, 238, 1.0);
    if (won)
        uiTextCentered(cx, 374, "You reached the finish line in time.",
                       GLUT_BITMAP_HELVETICA_12);
    else if (gL1LoseReason == L1_LOSE_HEALTH)
        uiTextCentered(cx, 374, "Your health reached zero.",
                       GLUT_BITMAP_HELVETICA_12);
    else
        uiTextCentered(cx, 374, "The timer expired before the finish line.",
                       GLUT_BITMAP_HELVETICA_12);

    uiRule(cx - 200, 360, 400, COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.5);

    // ---- figures -----------------------------------------------------
    y = 330;

    iSetColorA(200, 210, 225, 1.0);
    uiText(cx - 190, y, "FINAL SCORE", GLUT_BITMAP_HELVETICA_12);
    sprintf(buf, "%ld", gL1Score);
    iSetColorA(255, 255, 255, 1.0);
    l1TextRight(cx + 190, y - 3, buf, GLUT_BITMAP_HELVETICA_18);

    long l1Best = profileGetLevelHighScore(1);
    if (gNewHighScoreAchieved)
    {
        iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
        uiTextCentered(cx, y - 16, "★ NEW PERSONAL BEST RECORD! ★", GLUT_BITMAP_HELVETICA_12);
    }
    else if (l1Best > 0)
    {
        char pbBuf[32];
        sprintf(pbBuf, "PERSONAL BEST: %ld", l1Best);
        iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.85);
        uiTextCentered(cx, y - 16, pbBuf, GLUT_BITMAP_HELVETICA_10);
    }

    y -= 30;
    iSetColorA(200, 210, 225, 1.0);
    uiText(cx - 190, y, "TIME TAKEN", GLUT_BITMAP_HELVETICA_12);
    sprintf(buf, "%02d:%02d", elapsed / 60, elapsed % 60);
    iSetColorA(255, 255, 255, 1.0);
    l1TextRight(cx + 190, y - 3, buf, GLUT_BITMAP_HELVETICA_18);

    y -= 30;
    iSetColorA(200, 210, 225, 1.0);
    uiText(cx - 190, y, "DISTANCE", GLUT_BITMAP_HELVETICA_12);
    sprintf(buf, "%d %%", (int)(100.0 * gL1PlayerX / L1_LENGTH));
    iSetColorA(255, 255, 255, 1.0);
    l1TextRight(cx + 190, y - 3, buf, GLUT_BITMAP_HELVETICA_18);

    y -= 30;
    iSetColorA(200, 210, 225, 1.0);
    uiText(cx - 190, y, "COMPLETION", GLUT_BITMAP_HELVETICA_12);
    iSetColorA(won ? 90 : COL_RED_R, won ? 220 : COL_RED_G, won ? 130 : COL_RED_B, 1.0);
    l1TextRight(cx + 190, y - 3, won ? "COMPLETED" : "NOT COMPLETED",
                GLUT_BITMAP_HELVETICA_18);

    // ---- medal -------------------------------------------------------
    if (won)
    {
        l1DrawMedal(cx, 196, gL1Medal);
    }
    else
    {
        iSetColorA(COL_GREY_R, COL_GREY_G, COL_GREY_B, 1.0);
        uiTextCentered(cx, 176, "NO MEDAL AWARDED", GLUT_BITMAP_HELVETICA_18);
    }

    // ---- hint --------------------------------------------------------
    iSetColorA(0, 0, 0, 0.62);
    iFilledRectangle(0, 0, SCREEN_WIDTH, 26);

    iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.95);
    uiTextCentered(cx, 9, "ENTER  -  Run it again        ESC  -  Back to level select",
                   GLUT_BITMAP_HELVETICA_12);
}

//======================================================================
//  Frame entry point
//======================================================================
void level01Draw()
{
    int i;

    // 1. Continuous multi-zone running background with smooth cross-fading
    l1DrawBackground();

    // 2. Dynamic atmospheric horizon glow & natural sunbeams
    l1DrawAtmosphericHaze();

    // 3. Start & Finish line arches
    l1DrawMarkers();

    // 4. World obstacles
    for (i = 0; i < gL1ObstacleCount; i++)
        l1DrawObstacle(&gL1Obstacles[i]);

    // 5. Power-up pickups
    for (i = 0; i < gL1PickupCount; i++)
        l1DrawPickup(&gL1Pickups[i]);

    // 6. Ambient weather particles & runner footstep dust
    l1DrawParticles();

    // 7. Runner athlete sprite
    l1DrawPlayer();

    // 8. Zone transition announcement banner
    l1DrawBiomeBanner();

    // 9. Sleek HUD with 4-zone progress bar
    l1DrawHud();

    // 10. Overlays
    if (gL1State == L1_STATE_READY)       l1DrawReadyOverlay();
    else if (gL1State == L1_STATE_PAUSED) l1DrawPauseOverlay();
    else if (gL1State == L1_STATE_WON ||
             gL1State == L1_STATE_LOST)   l1DrawResultOverlay();
}

#endif
