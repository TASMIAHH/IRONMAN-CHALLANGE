//======================================================================
//  Level04Draw.h
//  All rendering for LEVEL 04 - SWIMMING EXPEDITION
//
//  STRICTLY IMAGE-BASED RENDERING:
//  No procedural vector drawings or iDraw polygon/shape primitives.
//  - Seamless scrolling multi-zone ocean backgrounds with smooth cross-fading
//  - Animated 7-frame photorealistic triathlon swimmer sequence
//  - Dedicated sprint surge and obstacle collision reaction sprites
//  - Realistic natural swimming obstacles (Jellyfish, Reef Sharks, Corals,
//    Driftwood logs, Sea Urchins, Sea Rocks, Whirlpools, Buoy Chains)
//  - Full image-based power-ups (Oxygen canisters, Energy gels, Speed fins,
//    Shields, Sunken gold coins, Marine First-aid kits)
//  - Textured start buoy arch and finish pontoon gantry
//  - Dynamic wake bubble stream trailing behind swimmer
//  - Glassmorphic HUD with dedicated Oxygen / Breath gauge
//======================================================================
#ifndef LEVEL04_DRAW_H
#define LEVEL04_DRAW_H

#include <math.h>
#include <stdlib.h>

//----------------------------------------------------------------------
// Small HUD helper
//----------------------------------------------------------------------
void l4Bar(int x, int y, int w, int h, double ratio, int r, int g, int b)
{
	int fill;
	if (ratio < 0.0) ratio = 0.0;
	if (ratio > 1.0) ratio = 1.0;
	fill = (int)(w * ratio);

	iSetColorA(10, 16, 30, 0.78);
	iFilledRectangle(x, y, w, h);

	if (fill > 0)
	{
		iSetColorA(r, g, b, 0.95);
		iFilledRectangle(x + 1, y + 1, fill - 2, h - 2);
	}

	iSetColorA(200, 225, 255, 0.55);
	uiBorder(x, y, w, h, 1);
}

//======================================================================
//  Biome weight calculation for smooth world-space transitions
//======================================================================
double l4GetBiomeWeight(int biome, double worldX)
{
	const double transHalf = 500.0; // 1000px total transition window
	if (biome == L4_BIOME_LAGOON) // 0 to 5000
	{
		if (worldX <= L4_BIOME_0_END - transHalf) return 1.0;
		if (worldX >= L4_BIOME_0_END + transHalf) return 0.0;
		double t = (worldX - (L4_BIOME_0_END - transHalf)) / (2.0 * transHalf);
		return 1.0 - (t * t * (3.0 - 2.0 * t));
	}
	else if (biome == L4_BIOME_DEEPBLUE) // 5000 to 10000
	{
		if (worldX <= L4_BIOME_0_END - transHalf) return 0.0;
		if (worldX < L4_BIOME_0_END + transHalf)
		{
			double t = (worldX - (L4_BIOME_0_END - transHalf)) / (2.0 * transHalf);
			return t * t * (3.0 - 2.0 * t);
		}
		if (worldX <= L4_BIOME_1_END - transHalf) return 1.0;
		if (worldX < L4_BIOME_1_END + transHalf)
		{
			double t = (worldX - (L4_BIOME_1_END - transHalf)) / (2.0 * transHalf);
			return 1.0 - (t * t * (3.0 - 2.0 * t));
		}
		return 0.0;
	}
	else if (biome == L4_BIOME_KELP) // 10000 to 15000
	{
		if (worldX <= L4_BIOME_1_END - transHalf) return 0.0;
		if (worldX < L4_BIOME_1_END + transHalf)
		{
			double t = (worldX - (L4_BIOME_1_END - transHalf)) / (2.0 * transHalf);
			return t * t * (3.0 - 2.0 * t);
		}
		if (worldX <= L4_BIOME_2_END - transHalf) return 1.0;
		if (worldX < L4_BIOME_2_END + transHalf)
		{
			double t = (worldX - (L4_BIOME_2_END - transHalf)) / (2.0 * transHalf);
			return 1.0 - (t * t * (3.0 - 2.0 * t));
		}
		return 0.0;
	}
	else // L4_BIOME_TRENCH: 15000 to 20000
	{
		if (worldX <= L4_BIOME_2_END - transHalf) return 0.0;
		if (worldX < L4_BIOME_2_END + transHalf)
		{
			double t = (worldX - (L4_BIOME_2_END - transHalf)) / (2.0 * transHalf);
			return t * t * (3.0 - 2.0 * t);
		}
		return 1.0;
	}
}

//----------------------------------------------------------------------
// Draws a textured quad slice with per-vertex alpha cross-fading
//----------------------------------------------------------------------
void l4DrawBiomeQuad(double sx0, double sx1, double sy0, double sy1,
                     float u0, float u1, unsigned int tex, float aL, float aR)
{
	if (!tex) return;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Subtle natural optical water refraction wave
	double wave0 = sin(sx0 * 0.015 + gL4Elapsed * 1.6) * 1.8;
	double wave1 = sin(sx1 * 0.015 + gL4Elapsed * 1.6) * 1.8;

	glBegin(GL_QUADS);
		glColor4f(1.0f, 1.0f, 1.0f, aL);
		glTexCoord2f(u0, 0.0f);
		glVertex2f((GLfloat)(sx0 + wave0), (GLfloat)sy0);

		glColor4f(1.0f, 1.0f, 1.0f, aR);
		glTexCoord2f(u1, 0.0f);
		glVertex2f((GLfloat)(sx1 + wave1), (GLfloat)sy0);

		glColor4f(1.0f, 1.0f, 1.0f, aR);
		glTexCoord2f(u1, -1.0f);
		glVertex2f((GLfloat)(sx1 - wave1 * 0.4), (GLfloat)sy1);

		glColor4f(1.0f, 1.0f, 1.0f, aL);
		glTexCoord2f(u0, -1.0f);
		glVertex2f((GLfloat)(sx0 - wave0 * 0.4), (GLfloat)sy1);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

//======================================================================
//  1. Continuous Parallax Scrolling Ocean Background
//======================================================================
void l4DrawBackground()
{
	const double bgParallax = 0.22; // Vast distant ocean parallax
	double bgWorldLeft = (gL4PlayerX - L4_PLAYER_SCREEN_X) * bgParallax;

	const double sliceW = 64.0; // 16 smooth slices across 1024px screen
	int sliceCount = (int)(SCREEN_WIDTH / sliceW);

	int sl, b;
	for (sl = 0; sl < sliceCount; sl++)
	{
		double sx0 = sl * sliceW;
		double sx1 = (sl + 1) * sliceW;

		// Texture coordinates scroll continuously without resetting
		float u0 = (float)((bgWorldLeft + sx0) / 1024.0);
		float u1 = (float)((bgWorldLeft + sx1) / 1024.0);

		// Corresponding player world positions for biome cross-fading
		double wX0 = gL4PlayerX - L4_PLAYER_SCREEN_X + sx0;
		double wX1 = gL4PlayerX - L4_PLAYER_SCREEN_X + sx1;

		for (b = 0; b < L4_BIOME_COUNT; b++)
		{
			float aL = (float)l4GetBiomeWeight(b, wX0);
			float aR = (float)l4GetBiomeWeight(b, wX1);

			if (aL <= 0.001f && aR <= 0.001f) continue;

			l4DrawBiomeQuad(sx0, sx1, 0.0, (double)SCREEN_HEIGHT, u0, u1, gL4TexBg[b], aL, aR);
		}
	}
}

//======================================================================
//  1b. Volumetric Sunbeams & God Rays
//======================================================================
void l4DrawSunbeams()
{
	if (!gL4TexSunbeams) return;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gL4TexSunbeams);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Additive luminous sunlight

	double sway = sin(gL4Elapsed * 0.7) * 25.0;
	float pulse = (float)(0.38 + 0.12 * sin(gL4Elapsed * 1.5));

	// Sunlight attenuates deeper in biome 3 (Trench)
	int curBiome = l4GetCurrentBiome(gL4PlayerX);
	if (curBiome == L4_BIOME_TRENCH) pulse *= 0.35f;
	else if (curBiome == L4_BIOME_KELP) pulse *= 0.70f;

	glColor4f(0.50f, 0.85f, 1.0f, pulse);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f((GLfloat)(-30.0 + sway), 0.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex2f((GLfloat)(SCREEN_WIDTH + 30.0 + sway), 0.0f);

		glTexCoord2f(1.0f, -1.0f);
		glVertex2f((GLfloat)(SCREEN_WIDTH + 30.0 - sway * 0.4), (GLfloat)SCREEN_HEIGHT);

		glTexCoord2f(0.0f, -1.0f);
		glVertex2f((GLfloat)(-30.0 - sway * 0.4), (GLfloat)SCREEN_HEIGHT);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

//======================================================================
//  1c. Photorealistic Grounded Seabed Layer
//======================================================================
void l4DrawSeabed()
{
	if (!gL4TexSeabedSand && !gL4TexSeabedRock) return;

	double seabedWorldX = (gL4PlayerX - L4_PLAYER_SCREEN_X) * 0.85;
	float u0 = (float)(seabedWorldX / 1024.0);
	float u1 = u0 + (float)(SCREEN_WIDTH / 1024.0);

	double curX = gL4PlayerX;
	float sandWeight = 1.0f;
	if (curX > 8500.0)
	{
		if (curX >= 11500.0) sandWeight = 0.0f;
		else sandWeight = (float)(1.0 - (curX - 8500.0) / 3000.0);
	}
	float rockWeight = 1.0f - sandWeight;

	glEnable(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	double seabedH = 115.0;

	// 1. Sand Seabed (Lagoon / Deep Blue)
	if (sandWeight > 0.001f && gL4TexSeabedSand)
	{
		glBindTexture(GL_TEXTURE_2D, gL4TexSeabedSand);
		glBegin(GL_QUADS);
			glColor4f(1.0f, 1.0f, 1.0f, 0.95f * sandWeight);
			glTexCoord2f(u0, 0.0f); glVertex2f(0.0f, 0.0f);
			glTexCoord2f(u1, 0.0f); glVertex2f((GLfloat)SCREEN_WIDTH, 0.0f);

			glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
			glTexCoord2f(u1, -1.0f); glVertex2f((GLfloat)SCREEN_WIDTH, (GLfloat)seabedH);
			glTexCoord2f(u0, -1.0f); glVertex2f(0.0f, (GLfloat)seabedH);
		glEnd();
	}

	// 2. Rock Seabed (Kelp Forest / Abyssal Trench)
	if (rockWeight > 0.001f && gL4TexSeabedRock)
	{
		glBindTexture(GL_TEXTURE_2D, gL4TexSeabedRock);
		glBegin(GL_QUADS);
			glColor4f(1.0f, 1.0f, 1.0f, 0.95f * rockWeight);
			glTexCoord2f(u0, 0.0f); glVertex2f(0.0f, 0.0f);
			glTexCoord2f(u1, 0.0f); glVertex2f((GLfloat)SCREEN_WIDTH, 0.0f);

			glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
			glTexCoord2f(u1, -1.0f); glVertex2f((GLfloat)SCREEN_WIDTH, (GLfloat)seabedH);
			glTexCoord2f(u0, -1.0f); glVertex2f(0.0f, (GLfloat)seabedH);
		glEnd();
	}

	glDisable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

//======================================================================
//  1c-2. Dynamic Swaying Seabed Kelp Forest
//======================================================================
void l4DrawSwayingKelp()
{
	if (!gL4TexKelpStalk) return;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gL4TexKelpStalk);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	const int segments = 10;
	int k;
	for (k = 0; k < gL4KelpPlantCount; k++)
	{
		struct L4KelpPlant *kp = &gL4KelpPlants[k];
		double rootSx = l4ScreenX(kp->worldX);

		if (rootSx + kp->width + kp->bendAmp < -120.0 ||
		    rootSx - kp->width - kp->bendAmp > SCREEN_WIDTH + 120.0)
			continue;

		double rootSy = kp->y;
		double height = kp->height;
		double halfW = kp->width * 0.5;

		glBegin(GL_QUAD_STRIP);
		int seg;
		for (seg = 0; seg <= segments; seg++)
		{
			double t = (double)seg / segments;
			double segY = rootSy + t * height;

			// Hydrodynamic harmonic deflection:
			// Deflection increases with square of height (t^2), anchored firmly at base (t=0)
			double bend = (t * t) * (kp->bendAmp * sin(kp->phase + gL4Elapsed * kp->bendSpeed)) +
			              (t * t * t) * (6.0 * cos(t * 2.8 + gL4Elapsed * 1.8));

			double segCx = rootSx + bend;
			double segHw = halfW * (1.0 - 0.22 * t);

			// Depth shading: slightly deeper tone near ocean floor, sunlit near top
			float shade = (float)(0.72 + 0.28 * t);
			glColor4f(shade * 0.92f, shade, shade * 0.88f, 0.92f);

			float v = (float)(-t);
			glTexCoord2f(0.0f, v);
			glVertex2f((GLfloat)(segCx - segHw), (GLfloat)segY);

			glTexCoord2f(1.0f, v);
			glVertex2f((GLfloat)(segCx + segHw), (GLfloat)segY);
		}
		glEnd();
	}

	glDisable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

//======================================================================
//  1d. Dynamic Shimmering Caustics Light Web
//======================================================================
void l4DrawCaustics()
{
	if (!gL4TexCaustics) return;

	// Caustics strength naturally attenuates in deeper biomes (subtle in kelp, none in trench)
	double wLagoon   = l4GetBiomeWeight(L4_BIOME_LAGOON, gL4PlayerX);
	double wDeepBlue = l4GetBiomeWeight(L4_BIOME_DEEPBLUE, gL4PlayerX);
	double wKelp     = l4GetBiomeWeight(L4_BIOME_KELP, gL4PlayerX);
	double wTrench   = l4GetBiomeWeight(L4_BIOME_TRENCH, gL4PlayerX);

	float biomeCaustic = (float)(wLagoon * 0.42 + wDeepBlue * 0.26 + wKelp * 0.18 + wTrench * 0.02);
	if (biomeCaustic < 0.005f) return;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gL4TexCaustics);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Additive luminous caustic web

	double topY = L4_WATER_SURFACE_Y + 15.0;

	// Layer 1: Forward drifting caustic web with natural vertical depth fade
	{
		float u0 = (float)((gL4PlayerX * 0.35 + gL4Elapsed * 35.0) / 512.0);
		float u1 = u0 + (float)(SCREEN_WIDTH / 512.0f);
		float v0 = (float)(sin(gL4Elapsed * 1.6) * 16.0 / 512.0);
		float v1 = v0 + (float)(topY / 512.0f);

		float alpha = (float)((0.20 + 0.05 * sin(gL4Elapsed * 2.5)) * biomeCaustic);
		glColor4f(0.35f, 0.75f, 0.95f, alpha);

		glBegin(GL_QUADS);
			glColor4f(0.35f, 0.75f, 0.95f, alpha * 0.30f); // Seafloor fade
			glTexCoord2f(u0, v0); glVertex2f(0.0f, 0.0f);
			glTexCoord2f(u1, v0); glVertex2f((GLfloat)SCREEN_WIDTH, 0.0f);

			glColor4f(0.35f, 0.75f, 0.95f, alpha * 1.10f); // Surface peak
			glTexCoord2f(u1, v1); glVertex2f((GLfloat)SCREEN_WIDTH, (GLfloat)topY);
			glTexCoord2f(u0, v1); glVertex2f(0.0f, (GLfloat)topY);
		glEnd();
	}

	// Layer 2: Opposing drift creating authentic caustic interference
	{
		float u0 = (float)((gL4PlayerX * 0.22 - gL4Elapsed * 25.0 + 128.0) / 384.0);
		float u1 = u0 + (float)(SCREEN_WIDTH / 384.0f);
		float v0 = (float)(cos(gL4Elapsed * 1.9) * 12.0 / 384.0);
		float v1 = v0 + (float)(topY / 384.0f);

		float alpha = (float)((0.15 + 0.04 * cos(gL4Elapsed * 3.1)) * biomeCaustic);

		glBegin(GL_QUADS);
			glColor4f(0.20f, 0.65f, 0.85f, alpha * 0.30f);
			glTexCoord2f(u0, v0); glVertex2f(0.0f, 0.0f);
			glTexCoord2f(u1, v0); glVertex2f((GLfloat)SCREEN_WIDTH, 0.0f);

			glColor4f(0.20f, 0.65f, 0.85f, alpha * 1.10f);
			glTexCoord2f(u1, v1); glVertex2f((GLfloat)SCREEN_WIDTH, (GLfloat)topY);
			glTexCoord2f(u0, v1); glVertex2f(0.0f, (GLfloat)topY);
		glEnd();
	}

	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

//======================================================================
//  1d-2. Dynamic Rising Seabed Bubble Columns
//======================================================================
void l4DrawSeabedBubbleColumns()
{
	if (!gL4TexBubble) return;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gL4TexBubble);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	int vb;
	for (vb = 0; vb < L4_MAX_VENT_BUBBLES; vb++)
	{
		struct L4VentBubble *bub = &gL4VentBubbles[vb];
		if (!bub->active) continue;

		double sx = l4ScreenX(bub->worldX);
		if (sx < -40.0 || sx > SCREEN_WIDTH + 40.0) continue;

		// Hydrodynamic turbulence & vortex shedding lateral wobble
		double wobble = sin(bub->phase + gL4Elapsed * 6.5) * (3.0 + (bub->y - L4_SEABED_Y) * 0.007);
		double drawX = sx + wobble;

		// Hydrostatic expansion (Boyle's law: bubbles expand as water pressure drops)
		double depthFraction = (bub->y - L4_SEABED_Y) / (L4_WATER_SURFACE_Y - L4_SEABED_Y);
		if (depthFraction < 0.0) depthFraction = 0.0;
		if (depthFraction > 1.0) depthFraction = 1.0;

		double sz = bub->baseSize * (1.0 + 0.70 * depthFraction);
		float alpha = (float)(0.55 + 0.38 * depthFraction);
		double hsz = sz * 0.5;

		glColor4f(0.85f, 0.96f, 1.0f, alpha);
		glTexCoord2f(0.0f, 0.0f);  glVertex2f((GLfloat)(drawX - hsz), (GLfloat)(bub->y - hsz));
		glTexCoord2f(1.0f, 0.0f);  glVertex2f((GLfloat)(drawX + hsz), (GLfloat)(bub->y - hsz));
		glTexCoord2f(1.0f, -1.0f); glVertex2f((GLfloat)(drawX + hsz), (GLfloat)(bub->y + hsz));
		glTexCoord2f(0.0f, -1.0f); glVertex2f((GLfloat)(drawX - hsz), (GLfloat)(bub->y + hsz));
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

//======================================================================
//  1d-3. Atmospheric Ocean Depth Haze (Volumetric Rayleigh Absorption)
//======================================================================
void l4DrawOceanAtmosphere()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	double w0 = l4GetBiomeWeight(L4_BIOME_LAGOON, gL4PlayerX);
	double w1 = l4GetBiomeWeight(L4_BIOME_DEEPBLUE, gL4PlayerX);
	double w2 = l4GetBiomeWeight(L4_BIOME_KELP, gL4PlayerX);
	double w3 = l4GetBiomeWeight(L4_BIOME_TRENCH, gL4PlayerX);

	float r = (float)(w0 * 0.08 + w1 * 0.02 + w2 * 0.04 + w3 * 0.01);
	float g = (float)(w0 * 0.48 + w1 * 0.18 + w2 * 0.30 + w3 * 0.05);
	float b = (float)(w0 * 0.58 + w1 * 0.48 + w2 * 0.32 + w3 * 0.18);
	float alpha = (float)(w0 * 0.10 + w1 * 0.15 + w2 * 0.13 + w3 * 0.20);

	glBegin(GL_QUADS);
		// Bottom is deeper and denser oceanic absorption
		glColor4f(r * 0.7f, g * 0.7f, b * 0.7f, alpha * 1.25f);
		glVertex2f(0.0f, 0.0f);
		glVertex2f((GLfloat)SCREEN_WIDTH, 0.0f);

		// Surface is bright and clear
		glColor4f(r * 1.2f, g * 1.2f, b * 1.2f, alpha * 0.55f);
		glVertex2f((GLfloat)SCREEN_WIDTH, (GLfloat)L4_WATER_SURFACE_Y);
		glVertex2f(0.0f, (GLfloat)L4_WATER_SURFACE_Y);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

//======================================================================
//  1e. Majestic Pelagic Manta Ray Glider
//======================================================================
void l4DrawMantaRay()
{
	if (!gL4TexMantaRay || !gL4MantaRay.active) return;

	double cx = gL4MantaRay.x;
	double cy = gL4MantaRay.y;

	if (cx < -300.0 || cx > SCREEN_WIDTH + 300.0) return;

	double w = 270.0 * gL4MantaRay.scale;
	double h = 150.0 * gL4MantaRay.scale;

	// Undulating wing flap frequency & vertical compression
	double wingFlap = 1.0 + 0.15 * sin(gL4MantaRay.phase * 2.4);
	double pitch = -cos(gL4MantaRay.phase * 0.5) * 5.5;

	glPushMatrix();
	glTranslated(cx, cy, 0.0);
	glRotated(pitch, 0.0, 0.0, 1.0);
	glScaled(1.0, wingFlap, 1.0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gL4TexMantaRay);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(0.68f, 0.90f, 1.0f, gL4MantaRay.alpha);

	double hw = w * 0.5;
	double hh = h * 0.5;

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);  glVertex2f((GLfloat)-hw, (GLfloat)-hh);
		glTexCoord2f(1.0f, 0.0f);  glVertex2f((GLfloat) hw, (GLfloat)-hh);
		glTexCoord2f(1.0f, -1.0f); glVertex2f((GLfloat) hw, (GLfloat) hh);
		glTexCoord2f(0.0f, -1.0f); glVertex2f((GLfloat)-hw, (GLfloat) hh);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

//======================================================================
//  1e-2. Background Marine Life (Schools of Fish)
//======================================================================
void l4DrawMarineLife()
{
	if (!gL4TexFishSchool) return;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gL4TexFishSchool);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	int f;
	for (f = 0; f < L4_MAX_FISH_SCHOOLS; f++)
	{
		struct L4FishSchool *fs = &gL4FishSchools[f];
		if (!fs->active) continue;

		double drawY = fs->y + sin(fs->phase) * 12.0;
		float dw = (float)(320.0 * fs->scale);
		float dh = (float)(160.0 * fs->scale);
		float fx = (float)fs->x;
		float fy = (float)drawY;

		glColor4f(0.65f, 0.90f, 1.0f, fs->alpha);
		glTexCoord2f(0.0f, 0.0f);  glVertex2f(fx, fy);
		glTexCoord2f(1.0f, 0.0f);  glVertex2f(fx + dw, fy);
		glTexCoord2f(1.0f, -1.0f); glVertex2f(fx + dw, fy + dh);
		glTexCoord2f(0.0f, -1.0f); glVertex2f(fx, fy + dh);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}


//======================================================================
//  1f. Realistic Water Surface & Undulating Meniscus (Textured)
//======================================================================
void l4DrawWaterSurface()
{
	if (!gL4TexWaterSurface) return;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Additive luminous water sheen
	glBindTexture(GL_TEXTURE_2D, gL4TexWaterSurface);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glColor4f(0.85f, 0.95f, 1.0f, 0.85f);

	double scrollX = fmod(gL4PlayerX * 0.45 + gL4Elapsed * 35.0, 1024.0);
	double u0 = scrollX / 1024.0;
	double u1 = u0 + (double)SCREEN_WIDTH / 1024.0;

	double surfY = L4_WATER_SURFACE_Y - 64.0;
	double surfH = 128.0;

	glBegin(GL_QUADS);
		glTexCoord2f((GLfloat)u0, 0.0f);  glVertex2f(0.0f, (GLfloat)surfY);
		glTexCoord2f((GLfloat)u1, 0.0f);  glVertex2f((GLfloat)SCREEN_WIDTH, (GLfloat)surfY);
		glTexCoord2f((GLfloat)u1, -1.0f); glVertex2f((GLfloat)SCREEN_WIDTH, (GLfloat)(surfY + surfH));
		glTexCoord2f((GLfloat)u0, -1.0f); glVertex2f(0.0f, (GLfloat)(surfY + surfH));
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

//======================================================================
//  1g. Dynamic Water Surface Splashes & Meniscus Rings
//======================================================================
void l4DrawSurfaceSplashes()
{
	if (!gL4TexSplash) return;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gL4TexSplash);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Additive luminous surface splash sheen

	glBegin(GL_QUADS);
	int sr;
	for (sr = 0; sr < L4_MAX_SPLASHES; sr++)
	{
		struct L4SplashRing *s = &gL4Splashes[sr];
		if (!s->active) continue;

		if (s->screenX < -80.0 || s->screenX > SCREEN_WIDTH + 80.0) continue;

		float lifeRatio = (float)(s->life / s->maxLife);
		if (lifeRatio <= 0.0f) continue;

		float alpha = lifeRatio * 0.85f;
		double halfW = s->size * 1.25;
		double halfH = s->size * 0.55;

		glColor4f(0.85f, 0.98f, 1.0f, alpha);
		glTexCoord2f(0.0f, 0.0f);  glVertex2f((GLfloat)(s->screenX - halfW), (GLfloat)(s->y - halfH));
		glTexCoord2f(1.0f, 0.0f);  glVertex2f((GLfloat)(s->screenX + halfW), (GLfloat)(s->y - halfH));
		glTexCoord2f(1.0f, -1.0f); glVertex2f((GLfloat)(s->screenX + halfW), (GLfloat)(s->y + halfH));
		glTexCoord2f(0.0f, -1.0f); glVertex2f((GLfloat)(s->screenX - halfW), (GLfloat)(s->y + halfH));
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

//======================================================================
//  2. Ambient Marine Particles (Plankton / Micro-bubbles)
//======================================================================
void l4DrawParticles()
{
	if (!gL4TexBubble) return;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gL4TexBubble);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Additive luminous motes

	glBegin(GL_QUADS);
	int i;
	for (i = 0; i < L4_MAX_PARTICLES; i++)
	{
		struct L4Particle *p = &gL4Particles[i];
		int px = (int)p->x;
		int py = (int)p->y;
		if (px < -10 || px > SCREEN_WIDTH + 10 || py < -10 || py > SCREEN_HEIGHT + 10)
			continue;

		float alpha = (float)(p->alpha * (0.60 + 0.40 * sin(p->phase + gL4Elapsed * 2.8)));
		glColor4f(0.55f, 0.85f, 1.0f, alpha);
		float sz = (float)p->size;
		if (sz < 3.0f) sz = 3.0f;

		glTexCoord2f(0.0f, 0.0f);  glVertex2f((GLfloat)px, (GLfloat)py);
		glTexCoord2f(1.0f, 0.0f);  glVertex2f((GLfloat)(px + sz), (GLfloat)py);
		glTexCoord2f(1.0f, -1.0f); glVertex2f((GLfloat)(px + sz), (GLfloat)(py + sz));
		glTexCoord2f(0.0f, -1.0f); glVertex2f((GLfloat)px, (GLfloat)(py + sz));
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

//======================================================================
//  2b. Dynamic Bubble Stream Trailing Swimmer
//======================================================================
void l4DrawBubbles()
{
	if (!gL4TexBubble) return;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gL4TexBubble);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	int i;
	for (i = 0; i < L4_MAX_BUBBLES; i++)
	{
		struct L4Bubble *b = &gL4Bubbles[i];
		if (!b->active) continue;

		float alpha = (float)(b->life / b->maxLife);
		if (alpha < 0.0f) alpha = 0.0f;
		if (alpha > 1.0f) alpha = 1.0f;

		glColor4f(0.90f, 0.96f, 1.0f, alpha * 0.85f);
		float bx = (float)b->x;
		float by = (float)b->y;
		float bsz = (float)b->size;

		glTexCoord2f(0.0f, 0.0f);  glVertex2f(bx, by);
		glTexCoord2f(1.0f, 0.0f);  glVertex2f(bx + bsz, by);
		glTexCoord2f(1.0f, -1.0f); glVertex2f(bx + bsz, by + bsz);
		glTexCoord2f(0.0f, -1.0f); glVertex2f(bx, by + bsz);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

//======================================================================
//  3. World Entities & Markers
//======================================================================
void l4DrawStartFinish()
{
	// Start buoy arch
	double startSx = l4ScreenX(60.0);
	if (startSx > -150 && startSx < SCREEN_WIDTH + 150)
	{
		iShowImage((int)startSx, (int)(L4_WATER_SURFACE_Y - 120), 100, 180, gL4TexStart);
	}

	// Finish pontoon arch
	double finishSx = l4ScreenX(L4_LENGTH);
	if (finishSx > -200 && finishSx < SCREEN_WIDTH + 200)
	{
		iShowImage((int)finishSx, (int)(L4_WATER_SURFACE_Y - 160), 140, 240, gL4TexFinish);
	}
}

void l4DrawObstacle(struct L4Obstacle *o)
{
	double sx = l4ScreenX(o->x);
	if (sx + o->w < -80 || sx > SCREEN_WIDTH + 80) return;

	int drawH = o->h;
	int drawY = (int)o->y;

	// Dynamic pulsing for Jellyfish bell
	if (o->type == L4_OB_JELLYFISH)
	{
		int pulse = (int)(sin(o->phase) * 6.0);
		drawH += pulse;
	}

	// Dynamic rotation for Whirlpool
	if (o->type == L4_OB_WHIRLPOOL)
	{
		glPushMatrix();
		glTranslated(sx + o->w / 2.0, drawY + o->h / 2.0, 0.0);
		glRotated(o->phase * 25.0, 0.0, 0.0, 1.0);
		glTranslated(-(sx + o->w / 2.0), -(drawY + o->h / 2.0), 0.0);
		iShowImage((int)sx, drawY, o->w, drawH, gL4TexObstacle[o->type]);
		glPopMatrix();
		return;
	}

	iShowImage((int)sx, drawY, o->w, drawH, gL4TexObstacle[o->type]);
}

void l4DrawPickup(struct L4Pickup *p)
{
	if (p->taken) return;

	double sx = l4ScreenX(p->x);
	if (sx + p->w < -60 || sx > SCREEN_WIDTH + 60) return;

	// Gentle floating bobbing
	int floatY = (int)(p->y + sin(gL4Elapsed * 3.8 + p->x * 0.05) * 5.0);

	iShowImage((int)sx, floatY, p->w, p->h, gL4TexPickup[p->type]);
}

//======================================================================
//  4. Animated Swimmer Character Rendering
//======================================================================
void l4DrawSwimmerSprite(double x, double y, double w, double h, unsigned int tex, double angle, float alpha)
{
	if (tex == 0) return;

	double cx = x + w * 0.5;
	double cy = y + h * 0.5;

	glPushMatrix();
	glTranslated(cx, cy, 0.0);
	if (fabs(angle) > 0.01)
	{
		glRotated(angle, 0.0, 0.0, 1.0);
	}
	glTranslated(-cx, -cy, 0.0);

	if (alpha < 0.98f)
	{
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, tex);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glColor4f(1.0f, 1.0f, 1.0f, alpha);

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);  glVertex2f((GLfloat)x, (GLfloat)y);
			glTexCoord2f(1.0f, 0.0f);  glVertex2f((GLfloat)(x + w), (GLfloat)y);
			glTexCoord2f(1.0f, -1.0f); glVertex2f((GLfloat)(x + w), (GLfloat)(y + h));
			glTexCoord2f(0.0f, -1.0f); glVertex2f((GLfloat)x, (GLfloat)(y + h));
		glEnd();

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glDisable(GL_TEXTURE_2D);
	}
	else
	{
		iShowImage((int)x, (int)y, (int)w, (int)h, tex);
	}

	glPopMatrix();
}

void l4DrawSwimmer()
{
	int px = L4_PLAYER_SCREEN_X;
	int py = (int)(gL4PlayerY + 0.5);

	// Dynamic body roll along swimming axis
	double bodyRoll = sin(gL4StrokeAccum * 2.0 * 3.14159265) * 2.5;
	// Subtle vertical swimming undulation
	double waveY = sin(gL4StrokeAccum * 2.0 * 3.14159265) * 1.5;

	// Flashing mercy window: never vanishes, smoothly pulses alpha
	float swimmerAlpha = 1.0f;
	if (gL4InvulnTime > 0.0)
	{
		swimmerAlpha = (((int)(gL4InvulnTime * 14.0)) % 2 == 0) ? 0.38f : 0.88f;
	}

	double drawAngle = gL4Tilt + bodyRoll;
	double drawX = (double)px;
	double drawY = (double)py + waveY;

	// Protective Pearl Shield Aura (Rendered as glowing textured bubble sphere)
	if (gL4ShieldTime > 0.0)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Additive luminous bubble aura
		float shieldAlpha = (float)(0.55 + sin(gL4Elapsed * 8.0) * 0.18);
		glColor4f(0.45f, 0.85f, 1.0f, shieldAlpha);
		int cx = px + L4_PLAYER_W / 2;
		int cy = (int)(py + waveY + L4_PLAYER_H / 2.0);
		int sz = 164;
		iShowImage(cx - sz / 2, cy - sz / 2, sz, sz, gL4TexBubble);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// Draw Swimmer character sprite
	if (gL4InvulnTime > 0.85 && gL4TexSwimHurt)
	{
		l4DrawSwimmerSprite(drawX, drawY, L4_PLAYER_W, L4_PLAYER_H, gL4TexSwimHurt, drawAngle, swimmerAlpha);
	}
	else if (gL4SurgeTimer > 0.0 && gL4TexSwimSprint)
	{
		// Torpedo dolphin kick posture during high-speed surge
		l4DrawSwimmerSprite(drawX, drawY, L4_PLAYER_W, L4_PLAYER_H, gL4TexSwimSprint, drawAngle, swimmerAlpha);
	}
	else
	{
		int frame = gL4Frame % 7;
		if (frame < 0) frame = 0;
		unsigned int tex = gL4TexSwim[frame];
		if (!tex) tex = gL4TexSwim[0];
		l4DrawSwimmerSprite(drawX, drawY, L4_PLAYER_W, L4_PLAYER_H, tex, drawAngle, swimmerAlpha);
	}

	// Speed Fins Slipstream (Rendered as trailing hydrodynamic bubble wake)
	if (gL4FinsTime > 0.0)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		int s;
		for (s = 0; s < 5; s++)
		{
			double offX = -18.0 - s * 22.0;
			double offY = 44.0 + sin(gL4Elapsed * 16.0 + s * 1.5) * 8.0;
			float alpha = (float)(0.70 - s * 0.12);
			int bsz = 14 - s * 2;
			if (bsz < 6) bsz = 6;
			glColor4f(0.35f, 0.85f, 1.0f, alpha);
			iShowImage((int)(px + offX), (int)(py + waveY + offY), bsz, bsz, gL4TexBubble);
		}
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

//======================================================================
//  5. Biome Milestone Banner
//======================================================================
void l4DrawBiomeWaypointBanner()
{
	if (gL4WaypointTimer <= 0.0 || gL4ActiveWaypoint < 0) return;

	float alpha = 1.0f;
	if (gL4WaypointTimer > 2.8)
		alpha = (float)((3.5 - gL4WaypointTimer) / 0.7);
	else if (gL4WaypointTimer < 0.8)
		alpha = (float)(gL4WaypointTimer / 0.8);

	const char *name = "";
	const char *sub = "";

	if (gL4ActiveWaypoint == L4_BIOME_LAGOON)
	{
		name = "ZONE 1: TURQUOISE LAGOON & CORAL SHALLOWS";
		sub  = "Surface freely to breathe  |  Avoid razor corals & jellyfish";
	}
	else if (gL4ActiveWaypoint == L4_BIOME_DEEPBLUE)
	{
		name = "ZONE 2: PELAGIC DEEP OCEAN - SHARK PATROL";
		sub  = "Watch for incoming reef sharks  |  Collect oxygen canisters";
	}
	else if (gL4ActiveWaypoint == L4_BIOME_KELP)
	{
		name = "ZONE 3: GIANT KELP FOREST & ROCKY TRENCH";
		sub  = "Navigate around toxic sea urchins  |  Resist whirlpool suction";
	}
	else
	{
		name = "ZONE 4: SUNKEN GALLEON & ABYSSAL REEF";
		sub  = "Final endurance sprint  |  Reach the finish pontoon platform!";
	}

	int bw = 640;
	int bh = 54;
	int bx = (SCREEN_WIDTH - bw) / 2;
	int by = 380;

	iSetColorA(COL_PANEL_R, COL_PANEL_G, COL_PANEL_B, 0.88f * alpha);
	iFilledRectangle(bx, by, bw, bh);

	iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.95f * alpha);
	uiBorder(bx, by, bw, bh, 2);

	iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, alpha);
	uiTextClampedCentered(SCREEN_WIDTH / 2, by + 32, name, 0.40, bw - 20);

	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.90f * alpha);
	uiTextClampedCentered(SCREEN_WIDTH / 2, by + 12, sub, 0.28, bw - 20);
}

//======================================================================
//  6. Sleek Glassmorphic HUD
//======================================================================
void l4DrawHUD()
{
	char buf[64];

	// 1. Photorealistic Smoked Frosted Glass HUD Header Banner
	uiDrawPictureBanner(0, SCREEN_HEIGHT - 80, SCREEN_WIDTH, 80, gTexHudBanner);

	// ---- Left: Health & Stamina Bars (Photorealistic Picture Textures) ----
	// 1. Health Bar with Heart Badge & Liquid Ruby Vitality Texture
	uiDrawPictureBar(16, SCREEN_HEIGHT - 42, 170, 24, gL4Hp / (double)L4_HP_MAX,
	                 gTexHudHpFill, gTexHudBarFrame, gTexHudHpIcon, 24, 24);
	sprintf(buf, "%d", (int)(gL4Hp + 0.5));
	iSetColorA(COL_WHITE_R, COL_WHITE_G, COL_WHITE_B, 0.95);
	uiText(192, SCREEN_HEIGHT - 36, buf, GLUT_BITMAP_HELVETICA_10);

	// 2. Stamina Bar with Lightning Badge & Liquid Golden Energy Texture
	uiDrawPictureBar(16, SCREEN_HEIGHT - 70, 170, 24, gL4Stamina / (double)L4_STAMINA_MAX,
	                 gTexHudStaminaFill, gTexHudBarFrame, gTexHudStaminaIcon, 24, 24);
	sprintf(buf, "%d", (int)(gL4Stamina + 0.5));
	iSetColorA(COL_WHITE_R, COL_WHITE_G, COL_WHITE_B, 0.95);
	uiText(192, SCREEN_HEIGHT - 64, buf, GLUT_BITMAP_HELVETICA_10);

	// 3. OXYGEN / BREATH GAUGE with Diving Bubble Badge & Azure Liquid Texture
	uiDrawPictureBar(226, SCREEN_HEIGHT - 42, 190, 24, gL4Oxygen / (double)L4_OXYGEN_MAX,
	                 gTexHudOxygenFill, gTexHudBarFrame, gTexHudOxygenIcon, 24, 24);
	sprintf(buf, "%d%%", (int)gL4Oxygen);
	iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.95);
	uiText(422, SCREEN_HEIGHT - 36, buf, GLUT_BITMAP_HELVETICA_10);

	if (gL4Oxygen <= 0.0)
	{
		iSetColorA(COL_RED_R, COL_RED_G, COL_RED_B, 1.0);
		uiText(230, SCREEN_HEIGHT - 64, "ASPHYXIA! SURFACE TO BREATHE!", GLUT_BITMAP_HELVETICA_10);
	}
	else if (gL4Oxygen < 30.0)
	{
		iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
		uiText(230, SCREEN_HEIGHT - 64, "OXYGEN LOW - SWIM UP TO SURFACE", GLUT_BITMAP_HELVETICA_10);
	}
	else
	{
		iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.85);
		uiText(230, SCREEN_HEIGHT - 64, "BREATHING NORMAL", GLUT_BITMAP_HELVETICA_10);
	}

	// 4. Dolphin Kick Surge indicator
	iSetColorA(COL_WHITE_R, COL_WHITE_G, COL_WHITE_B, 0.85);
	uiText(470, SCREEN_HEIGHT - 32, "DOLPHIN SURGE", GLUT_BITMAP_HELVETICA_10);
	if (gL4Stamina >= L4_STAMINA_SURGE_COST && gL4SurgeTimer <= 0.0)
	{
		iSetColorA(90, 240, 140, 1.0);
		uiText(470, SCREEN_HEIGHT - 50, "[SPACE] READY", GLUT_BITMAP_HELVETICA_12);
	}
	else
	{
		iSetColorA(COL_GREY_R, COL_GREY_G, COL_GREY_B, 0.90);
		uiText(470, SCREEN_HEIGHT - 50, "RECHARGING...", GLUT_BITMAP_HELVETICA_12);
	}

	// 5. Course Distance Tracker
	iSetColorA(COL_WHITE_R, COL_WHITE_G, COL_WHITE_B, 0.90);
	sprintf(buf, "DISTANCE: %d M / %d M", (int)gL4PlayerX, L4_LENGTH);
	uiText(610, SCREEN_HEIGHT - 32, buf, GLUT_BITMAP_HELVETICA_12);
	l4Bar(610, SCREEN_HEIGHT - 52, 160, 10, gL4PlayerX / (double)L4_LENGTH, 90, 220, 130);

	// 6. Score & Time Remaining
	iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
	sprintf(buf, "SCORE: %ld", gL4Score);
	uiText(800, SCREEN_HEIGHT - 32, buf, GLUT_BITMAP_HELVETICA_12);

	long l4Best = profileGetLevelHighScore(4);
	if (l4Best > 0)
	{
		sprintf(buf, "BEST: %ld", l4Best);
		iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.95);
		uiText(910, SCREEN_HEIGHT - 32, buf, GLUT_BITMAP_HELVETICA_12);
	}

	double tRemain = L4_TIME_LIMIT - gL4Elapsed;
	if (tRemain < 0.0) tRemain = 0.0;
	int mins = (int)tRemain / 60;
	int secs = (int)tRemain % 60;
	sprintf(buf, "TIME: %02d:%02d", mins, secs);

	if (tRemain < 15.0)
		iSetColorA(COL_RED_R, COL_RED_G, COL_RED_B, 1.0);
	else
		iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 1.0);
	uiText(800, SCREEN_HEIGHT - 52, buf, GLUT_BITMAP_HELVETICA_12);

	// Combo badge
	if (gL4Combo > 1)
	{
		iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
		sprintf(buf, "%dx", gL4Combo);
		uiText(930, SCREEN_HEIGHT - 42, buf, GLUT_BITMAP_TIMES_ROMAN_24);
	}
}

//======================================================================
//  7. Modals / Overlays
//======================================================================
void l4DrawOverlays()
{
	char buf[128];

	if (gL4State == L4_STATE_READY)
	{
		uiDimScreen(0.55);
		iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
		uiTextScaledCentered(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 30, "READY! DIVE IN!", 0.50, 3.5);

		iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.95);
		uiTextCentered(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 20,
			"W/S: Dive & Surface    |    D: Sprint Kick    |    SPACE: Dolphin Surge",
			GLUT_BITMAP_HELVETICA_18);
	}
	else if (gL4State == L4_STATE_PAUSED)
	{
		uiDimScreen(0.70);
		iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
		uiTextScaledCentered(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 20, "PAUSED", 0.50, 3.5);

		iSetColorA(COL_WHITE_R, COL_WHITE_G, COL_WHITE_B, 0.90);
		uiTextCentered(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 25,
			"Press P to resume    |    ESC to return to Level Select",
			GLUT_BITMAP_HELVETICA_18);
	}
	else if (gL4State == L4_STATE_WON)
	{
		uiDimScreen(0.82);
		int cw = 640, ch = 340;
		int cx = (SCREEN_WIDTH - cw) / 2;
		int cy = (SCREEN_HEIGHT - ch) / 2;

		iSetColorA(COL_PANEL_R, COL_PANEL_G, COL_PANEL_B, 0.95);
		iFilledRectangle(cx, cy, cw, ch);
		iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
		uiBorder(cx, cy, cw, ch, 3);

		iSetColorA(90, 220, 130, 1.0);
		uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 280, "EXPEDITION COMPLETE!", 0.52, cw - 60);

		if (gL4Medal == 3) sprintf(buf, "MEDAL EARNED: GOLD (MASTER OF THE SEAS)");
		else if (gL4Medal == 2) sprintf(buf, "MEDAL EARNED: SILVER (OCEAN VOYAGER)");
		else sprintf(buf, "MEDAL EARNED: BRONZE (SURVIVOR)");

		iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
		uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 225, buf, 0.40, cw - 60);

		sprintf(buf, "FINAL SCORE: %ld", gL4Score);
		iSetColorA(COL_WHITE_R, COL_WHITE_G, COL_WHITE_B, 1.0);
		uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 185, buf, 0.44, cw - 60);

		long l4Best = profileGetLevelHighScore(4);
		if (gNewHighScoreAchieved)
		{
			iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
			uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 164, "★ NEW PERSONAL BEST RECORD! ★", 0.38, cw - 60);
		}
		else if (l4Best > 0)
		{
			char pbBuf[32];
			sprintf(pbBuf, "PERSONAL BEST: %ld PTS", l4Best);
			iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.85);
			uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 164, pbBuf, 0.30, cw - 60);
		}

		sprintf(buf, "TIME ELAPSED: %02d:%02d", (int)gL4Elapsed / 60, (int)gL4Elapsed % 60);
		iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.95);
		uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 144, buf, 0.36, cw - 60);

		uiRule(cx + 50, cy + 115, cw - 100, COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.65);

		iSetColorA(COL_WHITE_R, COL_WHITE_G, COL_WHITE_B, 0.90);
		uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 65, "Press ENTER to Replay    |    ESC for Level Select", 0.32, cw - 60);
	}
	else if (gL4State == L4_STATE_LOST)
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
		if (gL4LoseReason == L4_LOSE_ASPHYXIA)
			uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 232, "Asphyxiation! Ran out of breath underwater!", 0.36, cw - 60);
		else if (gL4LoseReason == L4_LOSE_HEALTH)
			uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 232, "Defeated from too many sea hazard collisions!", 0.36, cw - 60);
		else
			uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 232, "Time Expired! The stage timer ran out!", 0.36, cw - 60);

		sprintf(buf, "DISTANCE REACHED: %d M / %d M", (int)gL4PlayerX, L4_LENGTH);
		iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 0.95);
		uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 182, buf, 0.38, cw - 60);

		sprintf(buf, "FINAL SCORE: %ld", gL4Score);
		iSetColorA(COL_WHITE_R, COL_WHITE_G, COL_WHITE_B, 1.0);
		uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 152, buf, 0.38, cw - 60);

		long l4Best = profileGetLevelHighScore(4);
		if (gNewHighScoreAchieved)
		{
			iSetColorA(COL_GOLD_R, COL_GOLD_G, COL_GOLD_B, 1.0);
			uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 132, "★ NEW PERSONAL BEST RECORD! ★", 0.34, cw - 60);
		}
		else if (l4Best > 0)
		{
			char pbBuf[32];
			sprintf(pbBuf, "PERSONAL BEST: %ld PTS", l4Best);
			iSetColorA(COL_CYAN_R, COL_CYAN_G, COL_CYAN_B, 0.85);
			uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 132, pbBuf, 0.28, cw - 60);
		}

		uiRule(cx + 50, cy + 112, cw - 100, COL_RED_R, COL_RED_G, COL_RED_B, 0.65);

		iSetColorA(COL_WHITE_R, COL_WHITE_G, COL_WHITE_B, 0.90);
		uiTextClampedCentered(SCREEN_WIDTH / 2, cy + 65, "Press ENTER to Try Again    |    ESC for Level Select", 0.32, cw - 60);
	}
}

//======================================================================
//  Main Render Entry Point for Level 04
//======================================================================
void level04Draw()
{
	int shaking = (gL4ScreenShake > 0.0);
	if (shaking)
	{
		double shakeX = ((rand() % 100) / 50.0 - 1.0) * 8.0 * (gL4ScreenShake / 0.28);
		double shakeY = ((rand() % 100) / 50.0 - 1.0) * 8.0 * (gL4ScreenShake / 0.28);
		glPushMatrix();
		glTranslated(shakeX, shakeY, 0.0);
	}

	// 1. Continuous Parallax Scrolling Ocean Background
	l4DrawBackground();

	// 1b. Volumetric Sunbeams & God Rays
	l4DrawSunbeams();

	// 1c. Majestic Pelagic Manta Ray Glider (Deep water layer)
	l4DrawMantaRay();

	// 1d. Background Marine Life (Schools of fish at depth)
	l4DrawMarineLife();

	// 1e. Photorealistic Grounded Seabed (Sand & Rock)
	l4DrawSeabed();

	// 1f. Dynamic Swaying Seabed Kelp Forest
	l4DrawSwayingKelp();

	// 1g. Dynamic Shimmering Caustics Light Web
	l4DrawCaustics();

	// 1h. Dynamic Rising Seabed Bubble Columns (Hydrostatic expansion)
	l4DrawSeabedBubbleColumns();

	// 1i. Atmospheric Ocean Depth Haze (Volumetric Rayleigh Scattering)
	l4DrawOceanAtmosphere();

	// 2. Start & Finish Lines
	l4DrawStartFinish();

	// 3. Obstacles
	int i;
	for (i = 0; i < gL4ObstacleCount; i++)
	{
		l4DrawObstacle(&gL4Obstacles[i]);
	}

	// 4. Pickups
	for (i = 0; i < gL4PickupCount; i++)
	{
		l4DrawPickup(&gL4Pickups[i]);
	}

	// 5. Animated Swimmer Character
	l4DrawSwimmer();

	// 6. Dynamic Swimmer Wake & Breathing Exhalation Bubbles
	l4DrawBubbles();

	// 7. Realistic Water Surface & Undulating Meniscus
	l4DrawWaterSurface();

	// 7b. Dynamic Surface Splashes & Meniscus Burst Rings
	l4DrawSurfaceSplashes();

	// 8. Ambient Marine Particles (Plankton / Micro-bubbles)
	l4DrawParticles();

	// 9. Milestone Banner
	l4DrawBiomeWaypointBanner();

	if (shaking)
	{
		glPopMatrix();
	}

	// 10. HUD (Fixed on screen)
	l4DrawHUD();

	// 11. Modals / Overlays
	l4DrawOverlays();
}

#endif
