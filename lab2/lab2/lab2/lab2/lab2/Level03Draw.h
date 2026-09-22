//======================================================================
//  Level03Draw.h
//  All rendering for LEVEL 03 - CYCLING EXPEDITION
//
//  STRICTLY IMAGE-BASED RENDERING:
//  No procedural vector drawings or iDraw polygon/shape primitives.
//  - Seamless scrolling biome backgrounds matching terrain elevation
//    (Forest Hills -> Mountain Rail -> Lava Hills -> Snow Hills)
//  - Animated 7-frame realistic cycling sprite sequence
//  - Realistic jump and crash sprites
//  - Full image-based obstacle sprites (logs, boulders, rail barriers, carts,
//    volcanic rocks, magma vents, ice rocks, snowdrifts)
//  - Full image-based power-ups (energy drinks, shields, nitro, golden cogs)
//  - Textured finish arch and sleek glassmorphism HUD
//  - Dynamic weather particles (leaves, mist, embers, snowflakes)
//  - Dynamic tire spray/skid puffs
//  - Dynamic Stunt Hop floating pop-up banners
//  - Tachometer Gear indicator & Combo Multiplier
//======================================================================
#ifndef LEVEL03_DRAW_H
#define LEVEL03_DRAW_H

#include <math.h>
#include <stdlib.h>

//----------------------------------------------------------------------
// Small HUD helper
//----------------------------------------------------------------------
void l3Bar(int x, int y, int w, int h, double ratio, int r, int g, int b)
{
	int fill;
	if (ratio < 0.0) ratio = 0.0;
	if (ratio > 1.0) ratio = 1.0;
	fill = (int)(w * ratio);

	iSetColorA(12, 16, 28, 0.78);
	iFilledRectangle(x, y, w, h);

	if (fill > 0)
	{
		iSetColorA(r, g, b, 0.95);
		iFilledRectangle(x + 1, y + 1, fill - 2, h - 2);
	}

	iSetColorA(220, 230, 255, 0.55);
	uiBorder(x, y, w, h, 1);
}

//======================================================================
//======================================================================
//  Biome weight calculation for smooth world-space transitions
//======================================================================
double l3GetBiomeWeight(int biome, double worldX)
{
	const double transHalf = 500.0; // 1000px total transition window
	if (biome == L3_BIOME_HILL) // 0 to 5000
	{
		if (worldX <= L3_BIOME_0_END - transHalf) return 1.0;
		if (worldX >= L3_BIOME_0_END + transHalf) return 0.0;
		double t = (worldX - (L3_BIOME_0_END - transHalf)) / (2.0 * transHalf);
		return 1.0 - (t * t * (3.0 - 2.0 * t));
	}
	else if (biome == L3_BIOME_LAVA) // 5000 to 10000
	{
		if (worldX <= L3_BIOME_0_END - transHalf) return 0.0;
		if (worldX < L3_BIOME_0_END + transHalf)
		{
			double t = (worldX - (L3_BIOME_0_END - transHalf)) / (2.0 * transHalf);
			return t * t * (3.0 - 2.0 * t);
		}
		if (worldX <= L3_BIOME_1_END - transHalf) return 1.0;
		if (worldX < L3_BIOME_1_END + transHalf)
		{
			double t = (worldX - (L3_BIOME_1_END - transHalf)) / (2.0 * transHalf);
			return 1.0 - (t * t * (3.0 - 2.0 * t));
		}
		return 0.0;
	}
	else if (biome == L3_BIOME_SNOW) // 10000 to 15000
	{
		if (worldX <= L3_BIOME_1_END - transHalf) return 0.0;
		if (worldX < L3_BIOME_1_END + transHalf)
		{
			double t = (worldX - (L3_BIOME_1_END - transHalf)) / (2.0 * transHalf);
			return t * t * (3.0 - 2.0 * t);
		}
		if (worldX <= L3_BIOME_2_END - transHalf) return 1.0;
		if (worldX < L3_BIOME_2_END + transHalf)
		{
			double t = (worldX - (L3_BIOME_2_END - transHalf)) / (2.0 * transHalf);
			return 1.0 - (t * t * (3.0 - 2.0 * t));
		}
		return 0.0;
	}
	else // L3_BIOME_RAIL: 15000 to 20000
	{
		if (worldX <= L3_BIOME_2_END - transHalf) return 0.0;
		if (worldX < L3_BIOME_2_END + transHalf)
		{
			double t = (worldX - (L3_BIOME_2_END - transHalf)) / (2.0 * transHalf);
			return t * t * (3.0 - 2.0 * t);
		}
		return 1.0;
	}
}

// Low-level textured quad renderer with independent left/right vertex alpha
void l3DrawBiomeQuad(double x0, double x1, double y0, double y1,
                     float u0, float u1, unsigned int tex,
                     float alphaL, float alphaR)
{
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
//  1. Continuous World-Space Scrolling Biome Backgrounds
//======================================================================
void l3DrawBackground()
{
	double leftWorldX = gL3PlayerX - L3_PLAYER_SCREEN_X;
	double rightWorldX = leftWorldX + (double)SCREEN_WIDTH;

	// Divide viewport into 128px slices (8 slices across 1024px screen)
	// 128 divides 1024 segment width evenly (8 slices per segment).
	const double sliceW = 128.0;
	int startSlice = (int)floor(leftWorldX / sliceW);
	int endSlice   = (int)ceil(rightWorldX / sliceW);

	int sl, b;
	for (sl = startSlice; sl <= endSlice; sl++)
	{
		double wX0 = sl * sliceW;
		double wX1 = (sl + 1) * sliceW;

		double sx0 = wX0 - gL3PlayerX + L3_PLAYER_SCREEN_X;
		double sx1 = wX1 - gL3PlayerX + L3_PLAYER_SCREEN_X;

		for (b = 0; b < L3_BIOME_COUNT; b++)
		{
			float aL = (float)l3GetBiomeWeight(b, wX0);
			float aR = (float)l3GetBiomeWeight(b, wX1);

			if (aL <= 0.001f && aR <= 0.001f)
				continue;

			float u0 = 0.0f, u1 = 1.0f;

			if (b == L3_BIOME_HILL)
			{
				int seg = (int)floor(wX0 / 1024.0);
				double loc0 = wX0 - seg * 1024.0;
				double loc1 = wX1 - seg * 1024.0;
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
			}
			else if (b == L3_BIOME_LAVA)
			{
				// Lava highway tiles continuously forward
				int seg = (int)floor((wX0 - L3_BIOME_0_END) / 1024.0);
				double loc0 = (wX0 - L3_BIOME_0_END) - seg * 1024.0;
				double loc1 = (wX1 - L3_BIOME_0_END) - seg * 1024.0;
				u0 = (float)(loc0 / 1024.0);
				u1 = (float)(loc1 / 1024.0);
			}
			else if (b == L3_BIOME_SNOW)
			{
				// Snow hills alternate elevation wave
				int seg = (int)floor((wX0 - L3_BIOME_1_END) / 1024.0);
				double loc0 = (wX0 - L3_BIOME_1_END) - seg * 1024.0;
				double loc1 = (wX1 - L3_BIOME_1_END) - seg * 1024.0;
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
			}
			else // L3_BIOME_RAIL
			{
				// Rail trestle tiles continuously forward
				int seg = (int)floor((wX0 - L3_BIOME_2_END) / 1024.0);
				double loc0 = (wX0 - L3_BIOME_2_END) - seg * 1024.0;
				double loc1 = (wX1 - L3_BIOME_2_END) - seg * 1024.0;
				u0 = (float)(loc0 / 1024.0);
				u1 = (float)(loc1 / 1024.0);
			}

			l3DrawBiomeQuad(sx0, sx1, 0.0, (double)SCREEN_HEIGHT, u0, u1, gL3TexBg[b], aL, aR);
		}
	}
}

//======================================================================
//  1b. Dynamic Atmospheric Horizon Glow & Gradient
//======================================================================
void l3DrawAtmosphericHaze()
{
	// Volcanic Heat Glow approaching & traversing Lava Track
	float lavaW = (float)l3GetBiomeWeight(L3_BIOME_LAVA, gL3PlayerX + 300.0);
	if (lavaW > 0.02f)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Additive luminous atmospheric glow
		glBegin(GL_QUADS);
			glColor4f(1.0f, 0.38f, 0.08f, 0.16f * lavaW);
			glVertex2f(0.0f, 130.0f);
			glVertex2f((float)SCREEN_WIDTH, 130.0f);
			glColor4f(0.85f, 0.18f, 0.02f, 0.0f);
			glVertex2f((float)SCREEN_WIDTH, 360.0f);
			glVertex2f(0.0f, 360.0f);
		glEnd();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// Alpine Frost Mist approaching & traversing Snow Track
	float snowW = (float)l3GetBiomeWeight(L3_BIOME_SNOW, gL3PlayerX + 300.0);
	if (snowW > 0.02f)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBegin(GL_QUADS);
			glColor4f(0.82f, 0.90f, 1.0f, 0.12f * snowW);
			glVertex2f(0.0f, 160.0f);
			glVertex2f((float)SCREEN_WIDTH, 160.0f);
			glColor4f(0.88f, 0.94f, 1.0f, 0.0f);
			glVertex2f((float)SCREEN_WIDTH, 440.0f);
			glVertex2f(0.0f, 440.0f);
		glEnd();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// Golden Sunset Trestle Glow approaching & traversing Rail Track
	float railW = (float)l3GetBiomeWeight(L3_BIOME_RAIL, gL3PlayerX + 300.0);
	if (railW > 0.02f)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBegin(GL_QUADS);
			glColor4f(1.0f, 0.85f, 0.40f, 0.10f * railW);
			glVertex2f(0.0f, 170.0f);
			glVertex2f((float)SCREEN_WIDTH, 170.0f);
			glColor4f(1.0f, 0.65f, 0.20f, 0.0f);
			glVertex2f((float)SCREEN_WIDTH, 380.0f);
			glVertex2f(0.0f, 380.0f);
		glEnd();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

//======================================================================
//  1c. Lava Chasm Gaps (Organic fractured basalt ledges & boiling magma)
//======================================================================
void l3DrawLavaGaps()
{
	int i, p;
	// Only render if in or approaching the Lava biome
	if (gL3PlayerX < (L3_BIOME_0_END - 800.0) || gL3PlayerX > (L3_BIOME_1_END + 800.0))
		return;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	for (i = 0; i < gL3LavaGapCount; i++)
	{
		double startSX = l3ScreenX(gL3LavaGaps[i].startX);
		double endSX   = l3ScreenX(gL3LavaGaps[i].endX);
		double gapW    = endSX - startSX;

		// Skip if completely outside screen view
		if (endSX < -160.0 || startSX > (double)SCREEN_WIDTH + 160.0)
			continue;

		// Full basalt causeway height from Y = 0 to the background lava lake (Y = 196)
		int chasmH = 196;

		// 1. Animated Boiling Magma Pool filling the breach completely
		// Replaces the causeway from Y = 0 to 196, merging seamlessly with the background lava lake
		int animFrame = ((int)(gL3Elapsed * 7.5)) % 4;
		iShowImage((int)(startSX - 12.0), 0, (int)(gapW + 24.0), chasmH, gL3TexLavaPool[animFrame]);

		// 2. Photorealistic Fractured Basalt Road Ledges
		// Left takeoff ledge: spans Y = 0 to 196, seamlessly feathers into left road, jagged cliff drops into magma
		int ledgeW = 135;
		iShowImage((int)(startSX - 105.0), 0, ledgeW, chasmH, gL3TexLavaCliffL);

		// Right landing ledge: spans Y = 0 to 196, jagged cliff faces left into chasm, feathers into right road
		iShowImage((int)(endSX - 30.0), 0, ledgeW, chasmH, gL3TexLavaCliffR);

		// 3. Radiant Upward Volcanic Heat Glow
		// Additive glowing amber/orange haze rising above the boiling breach into the sky
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBegin(GL_QUADS);
			glColor4f(1.0f, 0.45f, 0.05f, 0.28f);
			glVertex2f((GLfloat)(startSX - 20.0), 120.0f);
			glVertex2f((GLfloat)(endSX + 20.0), 120.0f);
			glColor4f(0.9f, 0.2f, 0.02f, 0.0f);
			glVertex2f((GLfloat)(endSX + 20.0), 320.0f);
			glVertex2f((GLfloat)(startSX - 20.0), 320.0f);
		glEnd();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		// 4. Rising Fiery Embers & Sparks ascending from the molten pool
		for (p = 0; p < 10; p++)
		{
			double cycle = fmod(gL3Elapsed * 55.0 + p * 24.0 + i * 35.0, 180.0);
			double ey = 30.0 + cycle;
			double ex = startSX + 15.0 + ((p * 37 + i * 19) % (int)(gapW - 30))
			            + sin(cycle * 0.05 + p) * 12.0;
			double alpha = 1.0 - (cycle / 180.0);
			if (alpha < 0.0) alpha = 0.0;
			if (alpha > 1.0) alpha = 1.0;

			glColor4f(1.0f, 0.95f, 0.7f, (float)(alpha * 0.95));
			iShowImage((int)ex, (int)ey, 14, 14, gL3TexLavaEmber);
		}
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

//======================================================================
//  2. Ambient Weather Particles
//======================================================================
void l3DrawParticles()
{
	int i;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (i = 0; i < L3_MAX_PARTICLES; i++)
	{
		struct L3Particle *p = &gL3Particles[i];
		int px = (int)p->x;
		int py = (int)p->y;
		if (px < -20 || px > SCREEN_WIDTH + 20 || py < -20 || py > SCREEN_HEIGHT + 20)
			continue;

		// Determine particle biome by its actual horizontal world position
		double pwX = gL3PlayerX - L3_PLAYER_SCREEN_X + p->x;
		int pBio = l3GetCurrentBiome(pwX);

		if (pBio == L3_BIOME_HILL)
		{
			// Golden autumn leaves drifting
			float leafAlpha = (float)(0.60 + sin(p->phase + gL3Elapsed * 3.0) * 0.25);
			if (i % 3 == 0)
				iSetColorA(225, 140, 45, leafAlpha); // Amber orange
			else if (i % 3 == 1)
				iSetColorA(210, 175, 50, leafAlpha); // Golden yellow
			else
				iSetColorA(185, 85, 35, leafAlpha);  // Russet red

			int sz = (int)p->size;
			if (sz < 6) sz = 6;
			iShowImage(px, py, sz, sz, gL3TexLeaf);
		}
		else if (pBio == L3_BIOME_LAVA)
		{
			// Photorealistic floating volcanic embers & fiery sparks
			float sparkAlpha = (float)(0.75 + sin(p->phase + gL3Elapsed * 5.0) * 0.25);
			int sz = (int)(p->size * 1.8);
			if (sz < 8) sz = 8;
			if (sz > 16) sz = 16;
			glColor4f(1.0f, (i % 2 == 0) ? 0.95f : 0.75f, 0.4f, sparkAlpha);
			iShowImage(px, py, sz, sz, gL3TexLavaEmber);
		}
		else if (pBio == L3_BIOME_SNOW)
		{
			// Snow Hills: Soft crisp snowflakes
			int sz = (int)p->size;
			if (sz < 6) sz = 6;
			iShowImage(px, py, sz, sz, gL3TexSnow);
		}
		else // L3_BIOME_RAIL
		{
			// Mountain mist & wind streaks
			int len = (int)(p->size * 5.0 + 8.0);
			iShowImage(px, py, len, 12, gL3TexMist);
		}
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

//======================================================================
//  3. World Entities & Markers
//======================================================================
void l3DrawStartFinish()
{
	// Start line
	double startSx = l3ScreenX(60.0);
	if (startSx > -150 && startSx < SCREEN_WIDTH + 150)
	{
		iShowImage((int)startSx, (int)l3GetTrackY(60.0), 90, 180, gL3TexStart);
	}

	// Finish line arch
	double finishSx = l3ScreenX(L3_LENGTH);
	if (finishSx > -200 && finishSx < SCREEN_WIDTH + 200)
	{
		iShowImage((int)finishSx, (int)l3GetTrackY(L3_LENGTH), 140, 240, gL3TexFinish);
	}
}

void l3DrawObstacle(struct L3Obstacle *o)
{
	double sx = l3ScreenX(o->x);
	if (sx + o->w < -80 || sx > SCREEN_WIDTH + 80) return;

	int drawH = o->h;
	int drawY = (int)o->y;

	// Dynamic volcanic geyser plume pulsation
	if (o->type == L3_OB_LAVA_VENT)
	{
		int pulse = (int)(sin(o->phase) * 10.0);
		drawH += pulse;
	}

	// Dynamic rolling cart wheel motion or gentle bob
	if (o->type == L3_OB_CART)
	{
		drawY += (int)(sin(o->phase * 2.0) * 1.5);
	}

	iShowImage((int)sx, drawY, o->w, drawH, gL3TexObstacle[o->type]);
}

void l3DrawPickup(struct L3Pickup *p)
{
	if (p->taken) return;

	double sx = l3ScreenX(p->x);
	if (sx + p->w < -60 || sx > SCREEN_WIDTH + 60) return;

	// Floating levitation bobbing above the road
	int floatY = (int)(p->y + sin(gL3Elapsed * 4.2 + p->x * 0.04) * 4.0);

	iShowImage((int)sx, floatY, p->w, p->h, gL3TexPickup[p->type]);
}

//======================================================================
//  4. Dynamic Tire Spray & Skid Effects
//======================================================================
void l3DrawTireEffects()
{
	if (!gL3OnGround) return;

	int sprintKey = isKeyPressed('d') || isKeyPressed('D') || isSpecialKeyPressed(GLUT_KEY_RIGHT);
	int brakeKey  = isKeyPressed('a') || isKeyPressed('A') || isSpecialKeyPressed(GLUT_KEY_LEFT);

	int curBio = l3GetCurrentBiome(gL3PlayerX);
	int px = L3_PLAYER_SCREEN_X;
	int py = (int)gL3PlayerY;

	// Rear wheel contact point
	int rx = px + 26;
	int ry = py + 3;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (sprintKey || gL3NitroTime > 0.0)
	{
		// Dynamic spray kick behind rear wheel
		int s;
		for (s = 0; s < 4; s++)
		{
			double offX = -8.0 - (s * 8.0) - fmod(gL3Elapsed * 80.0 + s * 7.0, 16.0);
			double offY = 2.0 + (s * 3.0) + sin(gL3Elapsed * 15.0 + s) * 3.0;

			if (curBio == L3_BIOME_SNOW)
			{
				iShowImage((int)(rx + offX), (int)(ry + offY), 6, 6, gL3TexSnow);
			}
			else if (curBio == L3_BIOME_LAVA)
			{
				iShowImage((int)(rx + offX), (int)(ry + offY), 6, 6, gL3TexLavaEmber);
			}
			else
			{
				iShowImage((int)(rx + offX), (int)(ry + offY), 8, 8, gL3TexMist);
			}
		}
	}
	else if (brakeKey)
	{
		// Tire skid smoke puffs
		int s;
		for (s = 0; s < 3; s++)
		{
			double offX = -6.0 - (s * 9.0);
			double offY = 1.0 + (s * 4.0);
			iShowImage((int)(rx + offX), (int)(ry + offY), 14, 10, gL3TexMist);
		}
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

//======================================================================
//  5. Animated Cyclist Character Rendering
//======================================================================
void l3DrawCyclist()
{
	int px = L3_PLAYER_SCREEN_X;
	int py = (int)gL3PlayerY;

	// Hit mercy invulnerability flashing
	if (gL3InvulnTime > 0.0)
	{
		if (fmod(gL3InvulnTime * 16.0, 2.0) < 1.0)
			glColor4f(1.0f, 0.4f, 0.4f, 0.65f);
		else
			glColor4f(1.0f, 1.0f, 1.0f, 0.85f);
	}

	// Smooth bike tilt matrix transform around the bottom bracket
	glPushMatrix();
	double pivotX = px + L3_PLAYER_W * 0.5;
	double pivotY = py + L3_PLAYER_H * 0.25;
	glTranslated(pivotX, pivotY, 0.0);
	glRotated(gL3Tilt, 0.0, 0.0, 1.0);
	glTranslated(-pivotX, -pivotY, 0.0);

	// Select realistic sprite
	if (gL3State == L3_STATE_LOST && gL3LoseReason == L3_LOSE_HEALTH)
	{
		// Crash state
		iShowImage(px, py, L3_PLAYER_W, L3_PLAYER_H, gL3TexCycleCrash);
	}
	else if (!gL3OnGround)
	{
		// Bunny Hop Airborne Jump
		iShowImage(px, py, L3_PLAYER_W, L3_PLAYER_H, gL3TexCycleJump);
	}
	else if (gL3InvulnTime > 0.8)
	{
		// Impact reaction frame
		iShowImage(px, py, L3_PLAYER_W, L3_PLAYER_H, gL3TexCycleCrash);
	}
	else
	{
		// Smooth pedaling cycle
		int frameIdx = gL3Frame % 7;
		iShowImage(px, py, L3_PLAYER_W, L3_PLAYER_H, gL3TexCycle[frameIdx]);
	}

	glPopMatrix();

	// Reset GL color
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	// Tire effects (spray / skid)
	l3DrawTireEffects();

	// Active Buff Indicators - floating badges ABOVE helmet, never covering character
	if (gL3ShieldTime > 0.0)
	{
		int badgeY = py + L3_PLAYER_H + 10 + (int)(sin(gL3Elapsed * 6.0) * 3.0);
		iShowImage(px + 58, badgeY, 28, 28, gL3TexPickup[L3_PU_SHIELD]);
	}

	if (gL3NitroTime > 0.0)
	{
		int badgeY = py + L3_PLAYER_H + 10 + (int)(cos(gL3Elapsed * 6.0) * 3.0);
		int badgeX = (gL3ShieldTime > 0.0) ? (px + 90) : (px + 58);
		iShowImage(badgeX, badgeY, 28, 28, gL3TexPickup[L3_PU_NITRO]);
	}
}

//======================================================================
//  6. Stunt Hop Popup Banner
//======================================================================
void l3DrawStuntText()
{
	if (gL3StuntTimer <= 0.0) return;

	int px = L3_PLAYER_SCREEN_X;
	int py = (int)gL3PlayerY;

	// Floating dynamic bounce above helmet
	double t = gL3StuntTimer / 1.4;
	float alpha = (float)(t > 0.3 ? 1.0 : t / 0.3);
	int textY = py + L3_PLAYER_H + 24 + (int)((1.0 - t) * 20.0);
	int textX = px + L3_PLAYER_W / 2;

	// Background badge
	iSetColorA(20, 24, 38, 0.88 * alpha);
	iFilledRectangle(textX - 95, textY - 6, 190, 26);
	iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, alpha);
	uiBorder(textX - 95, textY - 6, 190, 26, 2);

	iSetColorA(255, 225, 75, alpha);
	uiTextCentered(textX, textY, gL3StuntText, GLUT_BITMAP_HELVETICA_12);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

//======================================================================
//  6b. Dynamic Biome Waypoint Milestone Banner
//======================================================================
void l3DrawBiomeWaypointBanner()
{
	const char *bannerTitle = NULL;
	const char *bannerSub = NULL;
	int bR = 255, bG = 200, bB = 50;
	double progress = 0.0;

	if (gL3PlayerX >= 4600.0 && gL3PlayerX <= 5600.0)
	{
		bannerTitle = "NOW ENTERING: VOLCANIC LAVA BASIN";
		bannerSub   = "Warning: Extreme Magma Heat & Boiling Lava Chasms Ahead!";
		bR = 255; bG = 110; bB = 30;
		double d = gL3PlayerX - 4600.0;
		progress = (d < 500.0) ? (d / 500.0) : ((5600.0 - gL3PlayerX) / 500.0);
	}
	else if (gL3PlayerX >= 9600.0 && gL3PlayerX <= 10600.0)
	{
		bannerTitle = "NOW ENTERING: ALPINE SNOW HILLS";
		bannerSub   = "Caution: Sub-Zero Glacial Slopes & Frozen Ice Hazards!";
		bR = 90; bG = 210; bB = 255;
		double d = gL3PlayerX - 9600.0;
		progress = (d < 500.0) ? (d / 500.0) : ((10600.0 - gL3PlayerX) / 500.0);
	}
	else if (gL3PlayerX >= 14600.0 && gL3PlayerX <= 15600.0)
	{
		bannerTitle = "NOW ENTERING: MOUNTAIN RAILWAY";
		bannerSub   = "Final Sprint: High-Altitude Steel Trestle Bridge to Finish Line!";
		bR = 255; bG = 215; bB = 60;
		double d = gL3PlayerX - 14600.0;
		progress = (d < 500.0) ? (d / 500.0) : ((15600.0 - gL3PlayerX) / 500.0);
	}

	if (!bannerTitle || progress <= 0.01) return;

	float alpha = (float)(progress > 1.0 ? 1.0 : progress);
	int bw = 480, bh = 42;
	int bx = (SCREEN_WIDTH - bw) / 2;
	int by = SCREEN_HEIGHT - 138;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	iSetColorA(14, 18, 30, 0.88 * alpha);
	iFilledRectangle(bx, by, bw, bh);

	iSetColorA(bR, bG, bB, alpha);
	uiBorder(bx, by, bw, bh, 2);

	iSetColorA(bR, bG, bB, alpha);
	uiTextCentered(SCREEN_WIDTH / 2, by + 24, (char*)bannerTitle, GLUT_BITMAP_HELVETICA_12);

	iSetColorA(240, 245, 255, 0.90 * alpha);
	uiTextCentered(SCREEN_WIDTH / 2, by + 8, (char*)bannerSub, GLUT_BITMAP_HELVETICA_10);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

//======================================================================
//  7. Sleek Heads-Up Display (HUD)
//======================================================================
void l3DrawHUD()
{
	char buf[64];

	// 1. Photorealistic Smoked Frosted Glass HUD Header Banner
	uiDrawPictureBanner(0, SCREEN_HEIGHT - 84, SCREEN_WIDTH, 84, gTexHudBanner);

	// ---- Left: Health & Stamina Bars (Photorealistic Picture Textures) ----
	// HP Bar with Heart Badge & Liquid Ruby Vitality Texture
	uiDrawPictureBar(16, SCREEN_HEIGHT - 44, 210, 26, gL3Hp / (double)L3_HP_MAX,
	                 gTexHudHpFill, gTexHudBarFrame, gTexHudHpIcon, 28, 28);
	sprintf(buf, "%d", (int)(gL3Hp + 0.5));
	iSetColorA(255, 255, 255, 0.95);
	uiText(232, SCREEN_HEIGHT - 37, buf, GLUT_BITMAP_HELVETICA_12);

	// Stamina Bar with Lightning Badge & Liquid Golden Energy Texture
	uiDrawPictureBar(16, SCREEN_HEIGHT - 74, 210, 26, gL3Stamina / (double)L3_STAMINA_MAX,
	                 gTexHudStaminaFill, gTexHudBarFrame, gTexHudStaminaIcon, 28, 28);
	sprintf(buf, "%d", (int)(gL3Stamina + 0.5));
	iSetColorA(255, 255, 255, 0.95);
	uiText(232, SCREEN_HEIGHT - 67, buf, GLUT_BITMAP_HELVETICA_12);

	// ---- Center: Biome Navigation Tracker (Beveled Titanium Chips) ----
	int curBiome = l3GetCurrentBiome(gL3PlayerX);
	const char *biomeNames[4] = {
		"FOREST HILLS", "LAVA BASIN", "SNOW HILLS", "RAIL TRACKS"
	};

	int bx;
	for (bx = 0; bx < 4; bx++)
	{
		int chipX = 295 + bx * 105;
		int chipY = SCREEN_HEIGHT - 38;
		int active = (bx == curBiome);

		uiDrawPictureChip(chipX, chipY, 100, 24, active, biomeNames[bx],
		                  gTexHudChipActive, gTexHudChipInactive);
	}

	// Overall Track Distance Progress Bar (Sleek Textured Picture Bar)
	int progX = 295;
	int progY = SCREEN_HEIGHT - 70;
	int progW = 415;
	uiDrawPictureBar(progX, progY, progW, 18, gL3PlayerX / (double)L3_LENGTH,
	                 gTexHudOxygenFill, gTexHudBarFrame, 0, 0, 0);

	sprintf(buf, "PROGRESS: %d M / %d M", (int)gL3PlayerX, L3_LENGTH);
	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.95);
	uiTextCentered(progX + progW / 2, progY - 12, buf, GLUT_BITMAP_HELVETICA_10);

	// ---- Right: Gear Badge, Speedometer, Timer, Score ----
	// Gear Indicator Badge (Beveled Titanium Chip)
	const char *gearLabels[5] = { "GEAR 1", "GEAR 2", "GEAR 3", "GEAR 4", "TURBO 5" };
	int gIdx = gL3CurrentGear - 1;
	if (gIdx < 0) gIdx = 0;
	if (gIdx > 4) gIdx = 4;

	int gearX = 724;
	int gearY = SCREEN_HEIGHT - 38;
	uiDrawPictureChip(gearX, gearY, 76, 24, (gIdx == 4), gearLabels[gIdx],
	                  gTexHudChipActive, gTexHudChipInactive);

	// Speed KM/H
	int kmh = (int)(gL3Speed * 0.12);
	sprintf(buf, "%d KM/H", kmh);
	iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
	uiText(808, SCREEN_HEIGHT - 32, buf, GLUT_BITMAP_HELVETICA_12);

	// Combo Multiplier Badge
	if (gL3Combo > 1)
	{
		char comboBuf[32];
		sprintf(comboBuf, "x%d COMBO!", gL3Combo);
		iSetColorA(255, 215, 0, 1.0);
		uiText(905, SCREEN_HEIGHT - 32, comboBuf, GLUT_BITMAP_HELVETICA_12);
	}

	// Time remaining
	int timeLeft = (int)(L3_TIME_LIMIT - gL3Elapsed);
	if (timeLeft < 0) timeLeft = 0;
	sprintf(buf, "TIME: %02d:%02d", timeLeft / 60, timeLeft % 60);
	if (timeLeft <= 15)
		iSetColorA(COL_RED_R, COL_RED_G, COL_RED_B, 1.0);
	else
		iSetColorA(COL_WHITE_R, COL_WHITE_G, COL_WHITE_B, 0.95);
	uiText(724, SCREEN_HEIGHT - 58, buf, GLUT_BITMAP_HELVETICA_12);

	// Score
	sprintf(buf, "SCORE: %ld", gL3Score);
	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 1.0);
	uiText(835, SCREEN_HEIGHT - 58, buf, GLUT_BITMAP_HELVETICA_12);

	long l3Best = profileGetLevelHighScore(3);
	if (l3Best > 0)
	{
		sprintf(buf, "BEST: %ld", l3Best);
		iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.95);
		uiText(920, SCREEN_HEIGHT - 58, buf, GLUT_BITMAP_HELVETICA_12);
	}

	// Active Buff Badges
	if (gL3ShieldTime > 0.0)
	{
		iShowImage(724, SCREEN_HEIGHT - 76, 14, 14, gL3TexPickup[L3_PU_SHIELD]);
		sprintf(buf, "SHIELD: %.1fs", gL3ShieldTime);
		iSetColorA(90, 220, 255, 1.0);
		uiText(742, SCREEN_HEIGHT - 74, buf, GLUT_BITMAP_HELVETICA_10);
	}
	if (gL3NitroTime > 0.0)
	{
		iShowImage(835, SCREEN_HEIGHT - 76, 14, 14, gL3TexPickup[L3_PU_NITRO]);
		sprintf(buf, "NITRO: %.1fs", gL3NitroTime);
		iSetColorA(255, 160, 40, 1.0);
		uiText(853, SCREEN_HEIGHT - 74, buf, GLUT_BITMAP_HELVETICA_10);
	}

	// Bottom Controls Guide strip
	iSetColorA(0, 0, 0, 0.60);
	iFilledRectangle(0, 0, SCREEN_WIDTH, 24);
	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.90);
	uiTextCentered(SCREEN_WIDTH / 2, 7,
		"D / RIGHT - Sprint (Stamina)    A / LEFT - Brake / Recover    SPACE / UP - Bunny Hop    P - Pause    ESC - Exit",
		GLUT_BITMAP_HELVETICA_12);
}

//======================================================================
//  8. Modal Overlays (READY / PAUSED / WON / LOST)
//======================================================================
void l3DrawOverlays()
{
	char buf[64];

	// Overlays for State: READY / PAUSED / WON / LOST
	if (gL3State == L3_STATE_READY)
	{
		uiDimScreen(0.72);
		int cw = 640, ch = 250;
		int cx = (SCREEN_WIDTH - cw) / 2;
		int cy = (SCREEN_HEIGHT - ch) / 2;

		iSetColorA(COL_PANEL_R, COL_PANEL_G, COL_PANEL_B, 0.94);
		iFilledRectangle(cx, cy, cw, ch);
		iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
		uiBorder(cx, cy, cw, ch, 3);

		iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
		uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 195, "STAGE 03: CYCLING EXPEDITION", 0.44, cw - 60);

		iSetColorA(COL_WHITE_R, COL_WHITE_G, COL_WHITE_B, 0.95);
		uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 155, "Cross 4 Extreme Terrains: Forest Hill, Lava Basin, Snow Hills & Mountain Rail!", 0.33, cw - 60);

		iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.95);
		uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 115, "D / RIGHT - Pedal & Sprint     A / LEFT - Brake & Recover", 0.32, cw - 60);
		uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 90, "SPACE / UP - Bunny Hop Leap over obstacles and pits", 0.32, cw - 60);

		int count = (int)ceil(gL3ReadyTime);
		sprintf(buf, "STARTING IN %d...", count);
		iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
		uiTextCentered(SCREEN_WIDTH / 2, cy + 40, buf, GLUT_BITMAP_HELVETICA_18);
	}
	else if (gL3State == L3_STATE_PAUSED)
	{
		uiDimScreen(0.75);
		int cw = 560, ch = 210;
		int cx = (SCREEN_WIDTH - cw) / 2;
		int cy = (SCREEN_HEIGHT - ch) / 2;

		iSetColorA(COL_PANEL_R, COL_PANEL_G, COL_PANEL_B, 0.95);
		iFilledRectangle(cx, cy, cw, ch);
		iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
		uiBorder(cx, cy, cw, ch, 3);

		iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
		uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 145, "GAME PAUSED", 0.48, cw - 60);
		uiRule(cx + 40, cy + 105, cw - 80, COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.5);

		iSetColorA(COL_WHITE_R, COL_WHITE_G, COL_WHITE_B, 0.95);
		uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 50, "Press  P  to Resume    |    ESC  to Return to Menu", 0.34, cw - 60);
	}
	else if (gL3State == L3_STATE_WON)
	{
		uiDimScreen(0.80);
		int cw = 640, ch = 340;
		int cx = (SCREEN_WIDTH - cw) / 2;
		int cy = (SCREEN_HEIGHT - ch) / 2;

		iSetColorA(COL_PANEL_R, COL_PANEL_G, COL_PANEL_B, 0.95);
		iFilledRectangle(cx, cy, cw, ch);
		iSetColorA(90, 230, 130, 1.0);
		uiBorder(cx, cy, cw, ch, 3);

		iSetColorA(90, 230, 130, 1.0);
		uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 280, "VICTORY! CHALLENGE COMPLETED!", 0.46, cw - 60);

		iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
		if (gL3Medal == 3)      uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 236, "[ \x7F\x7F\x7F GOLD MEDAL CHAMPION \x7F\x7F\x7F ]", 0.44, cw - 60);
		else if (gL3Medal == 2) uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 236, "[ \x7F\x7F SILVER MEDALIST \x7F\x7F ]", 0.44, cw - 60);
		else                    uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 236, "[ \x7F BRONZE MEDALIST \x7F ]", 0.44, cw - 60);

		sprintf(buf, "FINAL SCORE: %ld", gL3Score);
		iSetColorA(COL_WHITE_R, COL_WHITE_G, COL_WHITE_B, 1.0);
		uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 185, buf, 0.44, cw - 60);

		long l3Best = profileGetLevelHighScore(3);
		if (gNewHighScoreAchieved)
		{
			iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
			uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 164, "★ NEW PERSONAL BEST RECORD! ★", 0.38, cw - 60);
		}
		else if (l3Best > 0)
		{
			char pbBuf[32];
			sprintf(pbBuf, "PERSONAL BEST: %ld PTS", l3Best);
			iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.85);
			uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 164, pbBuf, 0.30, cw - 60);
		}

		sprintf(buf, "TIME ELAPSED: %02d:%02d", (int)gL3Elapsed / 60, (int)gL3Elapsed % 60);
		iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.95);
		uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 144, buf, 0.36, cw - 60);

		uiRule(cx + 50, cy + 115, cw - 100, COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.65);

		iSetColorA(COL_WHITE_R, COL_WHITE_G, COL_WHITE_B, 0.90);
		uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 65, "Press ENTER to Replay    |    ESC for Level Select", 0.32, cw - 60);
	}
	else if (gL3State == L3_STATE_LOST)
	{
		uiDimScreen(0.82);
		int cw = 640, ch = 340;
		int cx = (SCREEN_WIDTH - cw) / 2;
		int cy = (SCREEN_HEIGHT - ch) / 2;

		iSetColorA(COL_PANEL_R, COL_PANEL_G, COL_PANEL_B, 0.95);
		iFilledRectangle(cx, cy, cw, ch);
		iSetColorA(COL_RED_R, COL_RED_G, COL_RED_B, 1.0);
		uiBorder(cx, cy, cw, ch, 3);

		iSetColorA(COL_RED_R, COL_RED_G, COL_RED_B, 1.0);
		uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 280, "EXPEDITION FAILED", 0.52, cw - 60);

		iSetColorA(COL_WHITE_R, COL_WHITE_G, COL_WHITE_B, 0.95);
		if (gL3LoseReason == L3_LOSE_HEALTH)
			uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 232, "Bike was wrecked from too many collisions!", 0.36, cw - 60);
		else
			uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 232, "Time Expired! The stage timer ran out!", 0.36, cw - 60);

		sprintf(buf, "DISTANCE REACHED: %d M / %d M", (int)gL3PlayerX, L3_LENGTH);
		iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.95);
		uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 182, buf, 0.38, cw - 60);

		sprintf(buf, "FINAL SCORE: %ld", gL3Score);
		iSetColorA(COL_WHITE_R, COL_WHITE_G, COL_WHITE_B, 1.0);
		uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 152, buf, 0.38, cw - 60);

		long l3Best = profileGetLevelHighScore(3);
		if (gNewHighScoreAchieved)
		{
			iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
			uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 132, "★ NEW PERSONAL BEST RECORD! ★", 0.34, cw - 60);
		}
		else if (l3Best > 0)
		{
			char pbBuf[32];
			sprintf(pbBuf, "PERSONAL BEST: %ld PTS", l3Best);
			iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.85);
			uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 132, pbBuf, 0.28, cw - 60);
		}

		uiRule(cx + 50, cy + 112, cw - 100, COL_RED_R, COL_RED_G, COL_RED_B, 0.65);

		iSetColorA(COL_WHITE_R, COL_WHITE_G, COL_WHITE_B, 0.90);
		uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 65, "Press ENTER to Try Again    |    ESC for Level Select", 0.32, cw - 60);
	}
}

//======================================================================
//  Main Render Entry Point for Level 03
//======================================================================
void level03Draw()
{
	int shaking = (gL3ScreenShake > 0.0);
	if (shaking)
	{
		double shakeX = ((rand() % 100) / 50.0 - 1.0) * 8.0 * (gL3ScreenShake / 0.25);
		double shakeY = ((rand() % 100) / 50.0 - 1.0) * 8.0 * (gL3ScreenShake / 0.25);
		glPushMatrix();
		glTranslated(shakeX, shakeY, 0.0);
	}

	// 1. Continuous World-Space Scrolling Background
	l3DrawBackground();

	// 1b. Dynamic Atmospheric Horizon Glow & Gradient
	l3DrawAtmosphericHaze();

	// 2. Lava Chasm Gaps (Cutting through terrain with boiling magma)
	l3DrawLavaGaps();

	// 3. Ambient Weather Particles (Background layer)
	l3DrawParticles();

	// 4. Start & Finish Lines
	l3DrawStartFinish();

	// 5. Obstacles
	int i;
	for (i = 0; i < gL3ObstacleCount; i++)
	{
		l3DrawObstacle(&gL3Obstacles[i]);
	}

	// 6. Pickups
	for (i = 0; i < gL3PickupCount; i++)
	{
		l3DrawPickup(&gL3Pickups[i]);
	}

	// 7. Animated Cyclist & Tire Effects
	l3DrawCyclist();

	// 8. Stunt Hop Floating Banner
	l3DrawStuntText();

	// 8b. Dynamic Biome Waypoint Milestone Banner
	l3DrawBiomeWaypointBanner();

	if (shaking)
	{
		glPopMatrix();
	}

	// 8. HUD (Fixed on screen, no shake)
	l3DrawHUD();

	// 9. Modals / Overlays
	l3DrawOverlays();
}

#endif
