//======================================================================
//  Level03.h
//  LEVEL 03 - CYCLING EXPEDITION
//
//  An endurance cross-country cycling challenge across 4 transitioning
//  biomes: Forest Hill Tracks, Mountain Rail Tracks, Lava Hills Tracks,
//  and Snow Hills Tracks.
//
//  Features:
//  - Realistic terrain elevation matching exact artwork contours (user red line)
//  - Smooth continuous parabolic jump physics with zero vertical teleportation
//  - Lava Chasm Gaps with boiling magma pits, approach warnings, and bunny hop leaps
//  - Inertial speed transitions and exponential bike tilt smoothing
//  - Clear 160px takeoff runways and 180px landing zones around all chasms
//  - Strict image-based graphics with 0 vector primitives
//======================================================================
#ifndef LEVEL03_H
#define LEVEL03_H

#include <math.h>
#include <windows.h>
#include <stdio.h>

//----------------------------------------------------------------------
// Asset paths
//----------------------------------------------------------------------
#define L3_PATH_BG_HILL       "Assets/level03_bg_hill.png"
#define L3_PATH_BG_RAIL       "Assets/level03_bg_rail.png"
#define L3_PATH_BG_LAVA       "Assets/level03_bg_lava.png"
#define L3_PATH_BG_SNOW       "Assets/level03_bg_snow.png"

#define L3_PATH_START         "Assets/start_line.png"
#define L3_PATH_FINISH        "Assets/finish_line.png"

//----------------------------------------------------------------------
// Biomes
//----------------------------------------------------------------------
#define L3_BIOME_HILL         0
#define L3_BIOME_LAVA         1
#define L3_BIOME_SNOW         2
#define L3_BIOME_RAIL         3
#define L3_BIOME_COUNT        4

#define L3_BIOME_0_END     11000.0
#define L3_BIOME_1_END    22000.0
#define L3_BIOME_2_END    33000.0
#define L3_BIOME_3_END    44000.0

//----------------------------------------------------------------------
// Geometry
//----------------------------------------------------------------------
#define L3_PLAYER_SCREEN_X    160
#define L3_PLAYER_W           144
#define L3_PLAYER_H           124
#define L3_PLAYER_HIT_W        66
#define L3_PLAYER_HIT_H        72

//----------------------------------------------------------------------
// Motion (all scaled by delta time)
//----------------------------------------------------------------------
#define L3_GRAVITY           1680.0   // px/s^2
#define L3_JUMP_VELOCITY      680.0   // px/s (natural, responsive hop clearing all obstacles & chasms)
#define L3_BASE_SPEED         320.0   // px/s

#define L3_SPRINT_MULT          1.55
#define L3_BRAKE_MULT           0.55
#define L3_TIRED_MULT           0.65
#define L3_NITRO_MULT           1.48

#define L3_MAX_DELTA            0.10  // clamp

//----------------------------------------------------------------------
// Course & Survival (Easier & Forgiving Tuning: Finish Easily)
//----------------------------------------------------------------------
#define L3_LENGTH            44000    // world px (~137.5 s at base speed, ~2.2-2.4 min playtime)
#define L3_TIME_LIMIT          210    // seconds (3:30, ample time)

#define L3_HP_MAX              150    // Generous health pool (12+ hits buffer)
#define L3_STAMINA_MAX         140    // Large stamina pool for sprinting & climbing
#define L3_STAMINA_RUN_DRAIN    0.8   // per second cruising (gentle)
#define L3_STAMINA_SPRINT_DRAIN  9.5  // per second sprinting (accessible)
#define L3_STAMINA_RECOVER     22.0   // per second coasting/braking (rapid recharge)
#define L3_ENERGY_REFILL        60    // +60 stamina, +12 HP
#define L3_HEAL_AMOUNT          45    // HP restored by health pack (+30 stamina)

#define L3_HIT_DAMAGE           10    // Obstacle damage set to 10
#define L3_HIT_INVULN_SECONDS    1.6  // Generous mercy window after a hit
#define L3_SHIELD_SECONDS        7.0  // Shield duration set to 7s
#define L3_NITRO_SECONDS        10.0  // Extended obstacle-smashing nitro
#define L3_READY_SECONDS         2.0

//----------------------------------------------------------------------
// Scoring
//----------------------------------------------------------------------
#define L3_SCORE_OBSTACLE       60
#define L3_SCORE_PICKUP        120
#define L3_SCORE_COIN          200
#define L3_SCORE_PER_10PX        1
#define L3_SCORE_TIME_BONUS     30

//----------------------------------------------------------------------
// Entity kinds
//----------------------------------------------------------------------
#define L3_OB_LOG         0
#define L3_OB_ROCK        1
#define L3_OB_BARRIER     2
#define L3_OB_CART        3
#define L3_OB_LAVA_ROCK   4
#define L3_OB_LAVA_VENT   5
#define L3_OB_ICE_ROCK    6
#define L3_OB_SNOWDRIFT   7
#define L3_OB_KINDS       8

#define L3_PU_ENERGY      0
#define L3_PU_SHIELD      1
#define L3_PU_NITRO       2
#define L3_PU_COIN        3
#define L3_PU_HEAL        4
#define L3_PU_KINDS       5

#define L3_MAX_OBSTACLES 250
#define L3_MAX_PICKUPS   250

//----------------------------------------------------------------------
// Lava Chasm Gaps
//----------------------------------------------------------------------
#define L3_MAX_LAVA_GAPS   10
struct L3LavaGap
{
	double startX;
	double endX;
	int    cleared;
};
struct L3LavaGap gL3LavaGaps[L3_MAX_LAVA_GAPS];
int gL3LavaGapCount = 0;
int gL3LavaGapsCrossed = 0;

//----------------------------------------------------------------------
// Level states
//----------------------------------------------------------------------
#define L3_STATE_READY    0
#define L3_STATE_RUNNING  1
#define L3_STATE_PAUSED   2
#define L3_STATE_WON      3
#define L3_STATE_LOST     4

#define L3_LOSE_NONE      0
#define L3_LOSE_HEALTH    1
#define L3_LOSE_TIME      2

//----------------------------------------------------------------------
// Structures
//----------------------------------------------------------------------
struct L3Obstacle
{
	int    type;
	double x;
	double y;
	double baseY;
	double speedX;
	int    w, h;
	int    hit;
	int    scored;
	double phase;
};

struct L3Pickup
{
	int    type;
	double x, y;
	int    w, h;
	int    taken;
};

//----------------------------------------------------------------------
// Textures
//----------------------------------------------------------------------
unsigned int gL3TexBg[L3_BIOME_COUNT];
unsigned int gL3TexCycle[7];
unsigned int gL3TexCycleJump;
unsigned int gL3TexCycleCrash;
unsigned int gL3TexObstacle[L3_OB_KINDS];
unsigned int gL3TexPickup[L3_PU_KINDS];
unsigned int gL3TexStart, gL3TexFinish;

// Lava Chasm Realistic Image Assets
unsigned int gL3TexLavaCliffL;
unsigned int gL3TexLavaCliffR;
unsigned int gL3TexLavaChasmBg;
unsigned int gL3TexLavaPool[4];
unsigned int gL3TexLavaEmber;
unsigned int gL3TexLeaf;
unsigned int gL3TexSnow;
unsigned int gL3TexMist;

//----------------------------------------------------------------------
// Runtime state
//----------------------------------------------------------------------
int    gL3State = L3_STATE_READY;
int    gL3LoseReason = L3_LOSE_NONE;

double gL3PlayerX = 160.0;
double gL3PlayerY = 204.0;       // Absolute world Y coordinate (wheels contact)
double gL3PlayerVY = 0.0;
int    gL3OnGround = 1;

double gL3Hp = 100.0;
double gL3Stamina = 100.0;
long   gL3Score = 0;
double gL3DistanceScored = 160.0;

double gL3Elapsed = 0.0;
double gL3ReadyTime = 0.0;
double gL3InvulnTime = 0.0;
double gL3ShieldTime = 0.0;
double gL3NitroTime = 0.0;

LARGE_INTEGER gL3Freq;
LARGE_INTEGER gL3LastCounter;

int    gL3Frame = 0;
double gL3Speed = 0.0;
double gL3Tilt = 0.0;          // bike tilt angle in degrees

struct L3Obstacle gL3Obstacles[L3_MAX_OBSTACLES];
int    gL3ObstacleCount = 0;
struct L3Pickup   gL3Pickups[L3_MAX_PICKUPS];
int    gL3PickupCount = 0;

long   gL3ParScore = 4500;
int    gL3Medal = 0;

// Ambient weather particle system
#define L3_MAX_PARTICLES 55
struct L3Particle
{
	double x, y;
	double vx, vy;
	double size;
	double alpha;
	double phase;
};
struct L3Particle gL3Particles[L3_MAX_PARTICLES];

// Stunt & Combo System
int    gL3Combo = 1;
double gL3ComboTimer = 0.0;
char   gL3StuntText[48];
double gL3StuntTimer = 0.0;
double gL3ScreenShake = 0.0;
int    gL3CurrentGear = 1;

//----------------------------------------------------------------------
// Lava Gap Detection Helpers
//----------------------------------------------------------------------
int l3GetLavaGapIndex(double worldX)
{
	int i;
	for (i = 0; i < gL3LavaGapCount; i++)
	{
		if (worldX >= gL3LavaGaps[i].startX && worldX <= gL3LavaGaps[i].endX)
			return i;
	}
	return -1;
}

//----------------------------------------------------------------------
// Terrain Elevation Functions
// Matched to the exact user-drawn red line across the hill artwork:
// - Crest under pine tree: Y = 204
// - Downward slope: smooth descent from 204 to 74
// - Valley floor: Y = 68
// - Alternating rolling hills (2048px wavelength: 0..1024 downhill, 1024..2048 uphill)
//----------------------------------------------------------------------
double l3GetHillElevation(double localX)
{
	if (localX <= 240.0)
	{
		return 204.0;
	}
	else if (localX <= 300.0)
	{
		double t = (localX - 240.0) / 60.0;
		double s = t * t * (3.0 - 2.0 * t);
		return 204.0 + s * (190.0 - 204.0);
	}
	else if (localX <= 350.0)
	{
		double t = (localX - 300.0) / 50.0;
		double s = t * t * (3.0 - 2.0 * t);
		return 190.0 + s * (171.0 - 190.0);
	}
	else if (localX <= 420.0)
	{
		double t = (localX - 350.0) / 70.0;
		double s = t * t * (3.0 - 2.0 * t);
		return 171.0 + s * (160.0 - 171.0);
	}
	else if (localX <= 500.0)
	{
		double t = (localX - 420.0) / 80.0;
		double s = t * t * (3.0 - 2.0 * t);
		return 160.0 + s * (146.0 - 160.0);
	}
	else if (localX <= 600.0)
	{
		double t = (localX - 500.0) / 100.0;
		double s = t * t * (3.0 - 2.0 * t);
		return 146.0 + s * (115.0 - 146.0);
	}
	else if (localX <= 700.0)
	{
		double t = (localX - 600.0) / 100.0;
		double s = t * t * (3.0 - 2.0 * t);
		return 115.0 + s * (95.0 - 115.0);
	}
	else if (localX <= 800.0)
	{
		double t = (localX - 700.0) / 100.0;
		double s = t * t * (3.0 - 2.0 * t);
		return 95.0 + s * (78.0 - 95.0);
	}
	else if (localX <= 880.0)
	{
		double t = (localX - 800.0) / 80.0;
		double s = t * t * (3.0 - 2.0 * t);
		return 78.0 + s * (75.0 - 78.0);
	}
	else if (localX <= 1024.0)
	{
		double t = (localX - 880.0) / 144.0;
		double s = t * t * (3.0 - 2.0 * t);
		return 75.0 + s * (68.0 - 75.0);
	}
	return 68.0;
}

// Photorealistic Lava Basin Basalt Highway (solid rock causeway over magma lake)
// Elevation sits directly on the cracked basalt road (Y = 146.0),
// well below the distant lava lake shoreline (Y = 190.0) so the cycle runs realistically ON the road.
double l3GetLavaHillElevation(double worldX)
{
	return 146.0 + sin(worldX * 0.04) * 1.2;
}

// Continuous road track elevation (solid surface across all biomes)
double l3GetTrackY(double worldX)
{
	if (worldX < 0.0) worldX = 0.0;

	// BIOME 0: Forest Hills (0 to 5000)
	if (worldX < L3_BIOME_0_END - 500.0)
	{
		double cycle = fmod(worldX, 2048.0);
		if (cycle < 0.0) cycle += 2048.0;
		if (cycle <= 1024.0)
			return l3GetHillElevation(cycle);
		else
			return l3GetHillElevation(2048.0 - cycle);
	}
	else if (worldX < L3_BIOME_0_END + 500.0)
	{
		// Smooth blend from Forest Hill to Lava Basalt Highway (Y = 146.0)
		double t = (worldX - (L3_BIOME_0_END - 500.0)) / 1000.0;
		double s = t * t * (3.0 - 2.0 * t);
		double cycle = fmod(worldX, 2048.0);
		if (cycle < 0.0) cycle += 2048.0;
		double yHill = (cycle <= 1024.0) ? l3GetHillElevation(cycle) : l3GetHillElevation(2048.0 - cycle);
		double yLava = l3GetLavaHillElevation(worldX);
		return yHill * (1.0 - s) + yLava * s;
	}
	// BIOME 1: Lava Hills Tracks (5000 to 10000)
	else if (worldX < L3_BIOME_1_END - 500.0)
	{
		// Natural rolling basalt highway with level cliff takeoffs
		return l3GetLavaHillElevation(worldX);
	}
	else if (worldX < L3_BIOME_1_END + 500.0)
	{
		// Smooth blend from Lava to Snow
		double t = (worldX - (L3_BIOME_1_END - 500.0)) / 1000.0;
		double s = t * t * (3.0 - 2.0 * t);
		double yLava = l3GetLavaHillElevation(worldX);
		double cycle = fmod(worldX - L3_BIOME_1_END, 2048.0);
		if (cycle < 0.0) cycle += 2048.0;
		double ySnow = (cycle <= 1024.0) ? l3GetHillElevation(cycle) : l3GetHillElevation(2048.0 - cycle);
		return yLava * (1.0 - s) + ySnow * s;
	}
	// BIOME 2: Snow Hills Tracks (10000 to 15000)
	else if (worldX < L3_BIOME_2_END - 500.0)
	{
		// Alpine snow hills
		double cycle = fmod(worldX - L3_BIOME_1_END, 2048.0);
		if (cycle < 0.0) cycle += 2048.0;
		if (cycle <= 1024.0)
			return l3GetHillElevation(cycle);
		else
			return l3GetHillElevation(2048.0 - cycle);
	}
	else if (worldX < L3_BIOME_2_END + 500.0)
	{
		// Smooth blend from Snow to Rail Track (Y = 180.0)
		double t = (worldX - (L3_BIOME_2_END - 500.0)) / 1000.0;
		double s = t * t * (3.0 - 2.0 * t);
		double cycle = fmod(worldX - L3_BIOME_1_END, 2048.0);
		if (cycle < 0.0) cycle += 2048.0;
		double ySnow = (cycle <= 1024.0) ? l3GetHillElevation(cycle) : l3GetHillElevation(2048.0 - cycle);
		return ySnow * (1.0 - s) + 180.0 * s;
	}
	// BIOME 3: Mountain Rail Tracks (15000 to 20000)
	else
	{
		// Rail bridge steel tracks are flat and level at 180.0
		return 180.0;
	}
}

// Physical ground elevation (includes chasm pit bottom at Y=35)
double l3GetGroundY(double worldX)
{
	int gapIdx = l3GetLavaGapIndex(worldX);
	if (gapIdx >= 0)
	{
		return 35.0; // Pit bottom
	}
	return l3GetTrackY(worldX);
}

// Track slope angle (computed from continuous road track without cliff discontinuity)
double l3GetSlopeAngle(double worldX)
{
	double dy = l3GetTrackY(worldX + 24.0) - l3GetTrackY(worldX - 24.0);
	double angRad = atan2(dy, 48.0);
	return angRad * (180.0 / 3.14159265);
}

int l3GetCurrentBiome(double worldX)
{
	if (worldX < L3_BIOME_0_END) return L3_BIOME_HILL;
	if (worldX < L3_BIOME_1_END) return L3_BIOME_LAVA;
	if (worldX < L3_BIOME_2_END) return L3_BIOME_SNOW;
	return L3_BIOME_RAIL;
}

//----------------------------------------------------------------------
// Screen coordinate translation
//----------------------------------------------------------------------
double l3ScreenX(double worldX)
{
	return (worldX - gL3PlayerX) + L3_PLAYER_SCREEN_X;
}

//----------------------------------------------------------------------
// Texture loading
//----------------------------------------------------------------------
void level03LoadAssets()
{
	char path[64];
	int i;

	// Backgrounds
	assetLoadTexture(L3_PATH_BG_HILL, &gL3TexBg[L3_BIOME_HILL]);
	assetLoadTexture(L3_PATH_BG_LAVA, &gL3TexBg[L3_BIOME_LAVA]);
	assetLoadTexture(L3_PATH_BG_SNOW, &gL3TexBg[L3_BIOME_SNOW]);
	assetLoadTexture(L3_PATH_BG_RAIL, &gL3TexBg[L3_BIOME_RAIL]);

	// Cyclist pedaling animation frames
	for (i = 0; i < 7; i++)
	{
		sprintf(path, "Assets/cycle_%02d.png", i + 1);
		assetLoadTexture(path, &gL3TexCycle[i]);
	}

	assetLoadTexture("Assets/cycle_jump.png", &gL3TexCycleJump);
	assetLoadTexture("Assets/cycle_crash.png", &gL3TexCycleCrash);

	// Obstacles
	assetLoadTexture("Assets/obs3_log.png", &gL3TexObstacle[L3_OB_LOG]);
	assetLoadTexture("Assets/obs3_rock.png", &gL3TexObstacle[L3_OB_ROCK]);
	assetLoadTexture("Assets/obs3_barrier.png", &gL3TexObstacle[L3_OB_BARRIER]);
	assetLoadTexture("Assets/obs3_cart.png", &gL3TexObstacle[L3_OB_CART]);
	assetLoadTexture("Assets/obs3_lava_rock.png", &gL3TexObstacle[L3_OB_LAVA_ROCK]);
	assetLoadTexture("Assets/obs3_lava_vent.png", &gL3TexObstacle[L3_OB_LAVA_VENT]);
	assetLoadTexture("Assets/obs3_ice_rock.png", &gL3TexObstacle[L3_OB_ICE_ROCK]);
	assetLoadTexture("Assets/obs3_snowdrift.png", &gL3TexObstacle[L3_OB_SNOWDRIFT]);

	// Pickups
	assetLoadTexture("Assets/pu3_energy.png", &gL3TexPickup[L3_PU_ENERGY]);
	assetLoadTexture("Assets/pu3_shield.png", &gL3TexPickup[L3_PU_SHIELD]);
	assetLoadTexture("Assets/pu3_nitro.png", &gL3TexPickup[L3_PU_NITRO]);
	assetLoadTexture("Assets/pu3_coin.png", &gL3TexPickup[L3_PU_COIN]);
	assetLoadTexture("Assets/pu3_heal.png", &gL3TexPickup[L3_PU_HEAL]);

	// Line markers
	assetLoadTexture(L3_PATH_START, &gL3TexStart);
	assetLoadTexture(L3_PATH_FINISH, &gL3TexFinish);

	// Lava Chasm Realistic Image Assets
	assetLoadTexture("Assets/lava_cliff_l.png", &gL3TexLavaCliffL);
	assetLoadTexture("Assets/lava_cliff_r.png", &gL3TexLavaCliffR);
	assetLoadTexture("Assets/lava_chasm_bg.png", &gL3TexLavaChasmBg);
	assetLoadTexture("Assets/lava_pool_0.png", &gL3TexLavaPool[0]);
	assetLoadTexture("Assets/lava_pool_1.png", &gL3TexLavaPool[1]);
	assetLoadTexture("Assets/lava_pool_2.png", &gL3TexLavaPool[2]);
	assetLoadTexture("Assets/lava_pool_3.png", &gL3TexLavaPool[3]);
	assetLoadTexture("Assets/lava_ember.png", &gL3TexLavaEmber);
	assetLoadTexture("Assets/l3_leaf_particle.png", &gL3TexLeaf);
	assetLoadTexture("Assets/l2_snow_particle.png", &gL3TexSnow);
	assetLoadTexture("Assets/l2_mountain_mist.png", &gL3TexMist);
}

//----------------------------------------------------------------------
// Course builder
//----------------------------------------------------------------------
void l3AddObstacle(int type, double x, int w, int h, double sx)
{
	if (gL3ObstacleCount >= L3_MAX_OBSTACLES) return;
	struct L3Obstacle *o = &gL3Obstacles[gL3ObstacleCount++];
	o->type = type;
	o->x = x;
	o->baseY = l3GetTrackY(x);
	o->y = o->baseY;
	o->speedX = sx;
	o->w = w;
	o->h = h;
	o->hit = 0;
	o->scored = 0;
	o->phase = (double)(rand() % 100);
}

void l3AddPickup(int type, double x, double extraY, int w, int h)
{
	if (gL3PickupCount >= L3_MAX_PICKUPS) return;
	struct L3Pickup *p = &gL3Pickups[gL3PickupCount++];
	p->type = type;
	p->x = x;
	p->y = l3GetTrackY(x) + extraY;
	p->w = w;
	p->h = h;
	p->taken = 0;
}

// Spawns a golden star coin centered above an obstacle with clean, balanced elevation
void l3AddObstacleCoin(double obX, int obW, int obH)
{
	double coinW = 36.0;
	double coinH = 36.0;
	// Horizontally center coin over obstacle body
	double coinX = obX + (obW - coinW) * 0.5;
	// Vertically position coin slightly above obstacle peak (16px clearance at rest).
	// Obstacle spans from trackY to trackY + obH, so extraY = obH + 16.0 floats neatly above it.
	double coinY = (double)obH + 16.0;
	l3AddPickup(L3_PU_COIN, coinX, coinY, (int)coinW, (int)coinH);
}

void l3BuildCourse()
{
	int i;
	double x;

	gL3ObstacleCount = 0;
	gL3PickupCount = 0;
	gL3LavaGapCount = 0;

	// 1. BIOME 1: FOREST HILL TRACKS (160 to 10800)
	// First visible hill matching user screenshot:
	l3AddObstacle(L3_OB_LOG, 280.0, 90, 46, 0.0);
	l3AddObstacle(L3_OB_ROCK, 680.0, 75, 68, 0.0);
	l3AddObstacleCoin(680.0, 75, 68); // Centered above rock peak with 16px clearance
	l3AddPickup(L3_PU_ENERGY, 880.0, 10.0, 36, 48);

	// Subsequent rolling hills:
	for (x = 1350.0; x < 10600.0; x += 480.0)
	{
		int obType = ((int)(x / 480)) % 2 == 0 ? L3_OB_LOG : L3_OB_ROCK;
		int w = (obType == L3_OB_LOG) ? 90 : 75;
		int h = (obType == L3_OB_LOG) ? 46 : 68;
		l3AddObstacle(obType, x, w, h, 0.0);

		// Place reward coin centered slightly above obstacle
		if (((int)(x / 480)) % 2 == 0)
			l3AddObstacleCoin(x, w, h);
	}
	l3AddPickup(L3_PU_SHIELD, 2200.0, 25.0, 44, 44);
	l3AddPickup(L3_PU_HEAL,   3600.0, 25.0, 44, 44); // HP Recovery Pack
	l3AddPickup(L3_PU_ENERGY, 5200.0, 15.0, 36, 48);
	l3AddPickup(L3_PU_COIN,   6800.0, 25.0, 36, 36);
	l3AddPickup(L3_PU_HEAL,   8400.0, 25.0, 44, 44);
	l3AddPickup(L3_PU_SHIELD, 9800.0, 25.0, 44, 44);

	// 2. BIOME 2: LAVA BASIN BASALT HIGHWAY (11000 to 22000)
	// 6 thrilling Lava Chasm Gaps across the raised basalt rock highway:
	gL3LavaGaps[0].startX = 12500.0; gL3LavaGaps[0].endX = 12640.0; gL3LavaGaps[0].cleared = 0; // Basalt Crevasse 1 (140px wide)
	gL3LavaGaps[1].startX = 14200.0; gL3LavaGaps[1].endX = 14340.0; gL3LavaGaps[1].cleared = 0; // Basalt Crevasse 2 (140px wide)
	gL3LavaGaps[2].startX = 15900.0; gL3LavaGaps[2].endX = 16040.0; gL3LavaGaps[2].cleared = 0; // Basalt Crevasse 3 (140px wide)
	gL3LavaGaps[3].startX = 17600.0; gL3LavaGaps[3].endX = 17740.0; gL3LavaGaps[3].cleared = 0; // Basalt Crevasse 4 (140px wide)
	gL3LavaGaps[4].startX = 19300.0; gL3LavaGaps[4].endX = 19440.0; gL3LavaGaps[4].cleared = 0; // Basalt Crevasse 5 (140px wide)
	gL3LavaGaps[5].startX = 21000.0; gL3LavaGaps[5].endX = 21140.0; gL3LavaGaps[5].cleared = 0; // Basalt Crevasse 6 (140px wide)
	gL3LavaGapCount = 6;

	// Place mid-air star coins centered across each lava gap at jump apex!
	for (i = 0; i < gL3LavaGapCount; i++)
	{
		double midX = (gL3LavaGaps[i].startX + gL3LavaGaps[i].endX) * 0.5;
		l3AddPickup(L3_PU_COIN, midX - 18.0, 65.0, 36, 36);
	}

	// Obstacles on solid basalt highway between gaps (molten rock, fire vent geyser)
	// Clear 220px takeoff runway and 240px landing buffer around each gap!
	for (x = 11400.0; x < 21700.0; x += 460.0)
	{
		int nearGap = 0;
		for (i = 0; i < gL3LavaGapCount; i++)
		{
			if (x >= gL3LavaGaps[i].startX - 220.0 && x <= gL3LavaGaps[i].endX + 240.0)
			{
				nearGap = 1;
				break;
			}
		}
		if (nearGap) continue;

		int obType = ((int)(x / 460)) % 2 == 0 ? L3_OB_LAVA_ROCK : L3_OB_LAVA_VENT;
		int w = (obType == L3_OB_LAVA_ROCK) ? 80 : 88;
		int h = (obType == L3_OB_LAVA_ROCK) ? 72 : 94;
		l3AddObstacle(obType, x, w, h, 0.0);

		// Reward coin centered above molten rock
		if (obType == L3_OB_LAVA_ROCK)
			l3AddObstacleCoin(x, w, h);
	}
	l3AddPickup(L3_PU_NITRO,  13300.0, 30.0, 40, 48);
	l3AddPickup(L3_PU_HEAL,   15000.0, 25.0, 44, 44); // HP Recovery Pack
	l3AddPickup(L3_PU_SHIELD, 16700.0, 30.0, 44, 44);
	l3AddPickup(L3_PU_ENERGY, 18400.0, 20.0, 36, 48);
	l3AddPickup(L3_PU_HEAL,   20100.0, 25.0, 44, 44);
	l3AddPickup(L3_PU_SHIELD, 21800.0, 30.0, 44, 44);

	// 3. BIOME 3: SNOW HILLS TRACKS (22000 to 33000)
	// Obstacles: Glacial ice rock (w:82, h:75), Snowdrift (w:110, h:46)
	for (x = 22400.0; x < 32600.0; x += 520.0)
	{
		int obType = ((int)(x / 520)) % 2 == 0 ? L3_OB_ICE_ROCK : L3_OB_SNOWDRIFT;
		int w = (obType == L3_OB_ICE_ROCK) ? 82 : 110;
		int h = (obType == L3_OB_ICE_ROCK) ? 75 : 46;
		l3AddObstacle(obType, x, w, h, 0.0);

		// Reward coin centered slightly above snow obstacle
		l3AddObstacleCoin(x, w, h);

		// Flat-track ride coin in open stretches
		if (((int)(x / 520)) % 2 == 1)
			l3AddPickup(L3_PU_COIN, x + 260.0, 25.0, 36, 36);
	}
	l3AddPickup(L3_PU_NITRO,  23500.0, 30.0, 40, 48);
	l3AddPickup(L3_PU_HEAL,   25200.0, 25.0, 44, 44); // HP Recovery Pack
	l3AddPickup(L3_PU_ENERGY, 27000.0, 20.0, 36, 48);
	l3AddPickup(L3_PU_SHIELD, 28800.0, 30.0, 44, 44);
	l3AddPickup(L3_PU_HEAL,   30600.0, 25.0, 44, 44);
	l3AddPickup(L3_PU_NITRO,  32200.0, 30.0, 40, 48);

	// 4. BIOME 4: MOUNTAIN RAIL TRACKS (33000 to 44000)
	// Obstacles: Rail barrier (w:104, h:78), Mine cart (w:96, h:66)
	// Generously spaced so the final sprint to the finish line is smooth & easy
	for (x = 33400.0; x < 43600.0; x += 540.0)
	{
		int obType = ((int)(x / 540)) % 2 == 0 ? L3_OB_BARRIER : L3_OB_CART;
		int w = (obType == L3_OB_BARRIER) ? 104 : 96;
		int h = (obType == L3_OB_BARRIER) ? 78 : 66;
		double sx = (obType == L3_OB_CART) ? -35.0 : 0.0; // rolling cart hazard
		l3AddObstacle(obType, x, w, h, sx);

		// Place reward coin centered slightly above barrier / cart
		if (((int)(x / 540)) % 2 == 1)
			l3AddObstacleCoin(x, w, h);
	}
	l3AddPickup(L3_PU_SHIELD, 33800.0, 30.0, 44, 44);
	l3AddPickup(L3_PU_NITRO,  35000.0, 30.0, 40, 48);
	l3AddPickup(L3_PU_HEAL,   36800.0, 25.0, 44, 44); // HP Recovery Pack
	l3AddPickup(L3_PU_ENERGY, 38500.0, 20.0, 36, 48);
	l3AddPickup(L3_PU_SHIELD, 40200.0, 30.0, 44, 44);
	l3AddPickup(L3_PU_NITRO,  41500.0, 30.0, 40, 48);
	l3AddPickup(L3_PU_HEAL,   42600.0, 25.0, 44, 44);
	l3AddPickup(L3_PU_ENERGY, 43500.0, 20.0, 36, 48);
}

//----------------------------------------------------------------------
// Startup
//----------------------------------------------------------------------
void level03Start()
{
	gL3State = L3_STATE_READY;
	gL3LoseReason = L3_LOSE_NONE;

	gL3PlayerX = 160.0;
	gL3PlayerY = l3GetTrackY(160.0);
	gL3PlayerVY = 0.0;
	gL3OnGround = 1;

	gL3Hp = L3_HP_MAX;
	gL3Stamina = L3_STAMINA_MAX;
	gL3Score = 0;
	gL3DistanceScored = 160.0;

	gL3Elapsed = 0.0;
	gL3ReadyTime = L3_READY_SECONDS;
	gL3InvulnTime = 0.0;
	gL3ShieldTime = 0.0;
	gL3NitroTime = 0.0;

	gL3Frame = 0;
	gL3Speed = L3_BASE_SPEED;
	gL3Tilt = 0.0;
	gL3Medal = 0;

	l3BuildCourse();

	// Initialize ambient weather particles
	int p;
	for (p = 0; p < L3_MAX_PARTICLES; p++)
	{
		gL3Particles[p].x = (double)(rand() % SCREEN_WIDTH);
		gL3Particles[p].y = (double)(rand() % SCREEN_HEIGHT);
		gL3Particles[p].vx = -30.0 - (rand() % 40);
		gL3Particles[p].vy = 20.0 + (rand() % 35);
		gL3Particles[p].size = 3.0 + (rand() % 4);
		gL3Particles[p].alpha = 0.4 + ((rand() % 40) / 100.0);
		gL3Particles[p].phase = (double)(rand() % 100);
	}

	QueryPerformanceFrequency(&gL3Freq);
	QueryPerformanceCounter(&gL3LastCounter);

	audioPlayLevel03Theme();
}

//----------------------------------------------------------------------
// Helpers
//----------------------------------------------------------------------
int l3Overlap(double x1, double y1, double w1, double h1,
              double x2, double y2, double w2, double h2)
{
	return (x1 < x2 + w2) && (x1 + w1 > x2) &&
	       (y1 < y2 + h2) && (y1 + h1 > y2);
}

//----------------------------------------------------------------------
// Physics update
//----------------------------------------------------------------------
void level03Update(int jumpPressed, int pausePressed, int backPressed, int confirmPressed)
{
	if (backPressed)
	{
		audioPlayIntroTheme();
		gCurrentScreen = SCREEN_LEVEL_SELECT;
		return;
	}

	if (pausePressed && (gL3State == L3_STATE_RUNNING || gL3State == L3_STATE_PAUSED))
	{
		gL3State = (gL3State == L3_STATE_RUNNING) ? L3_STATE_PAUSED : L3_STATE_RUNNING;
		QueryPerformanceCounter(&gL3LastCounter);
		return;
	}

	if ((gL3State == L3_STATE_WON || gL3State == L3_STATE_LOST) && confirmPressed)
	{
		level03Start();
		return;
	}

	if (gL3State == L3_STATE_PAUSED || gL3State == L3_STATE_WON || gL3State == L3_STATE_LOST)
		return;

	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	double dt = (double)(now.QuadPart - gL3LastCounter.QuadPart) / (double)gL3Freq.QuadPart;
	gL3LastCounter = now;

	if (dt > L3_MAX_DELTA) dt = L3_MAX_DELTA;
	if (dt <= 0.0) return;

	// Ready countdown
	if (gL3State == L3_STATE_READY)
	{
		gL3ReadyTime -= dt;
		if (gL3ReadyTime <= 0.0)
		{
			gL3State = L3_STATE_RUNNING;
			gL3ReadyTime = 0.0;
		}
		return;
	}

	if (gL3State != L3_STATE_RUNNING) return;

	gL3Elapsed += dt;

	// Combo timer
	if (gL3ComboTimer > 0.0)
	{
		gL3ComboTimer -= dt;
		if (gL3ComboTimer <= 0.0) gL3Combo = 1;
	}
	if (gL3StuntTimer > 0.0) gL3StuntTimer -= dt;
	if (gL3ScreenShake > 0.0)
	{
		gL3ScreenShake -= dt;
		if (gL3ScreenShake < 0.0) gL3ScreenShake = 0.0;
	}

	// Time limit check
	if (gL3Elapsed >= L3_TIME_LIMIT)
	{
		gL3State = L3_STATE_LOST;
		gL3LoseReason = L3_LOSE_TIME;
		audioPlayGameOver();
		profileUpdateLevelScore(3, gL3Score, 0, 0);
		return;
	}

	// Buff timers
	if (gL3InvulnTime > 0.0) gL3InvulnTime -= dt;
	if (gL3ShieldTime > 0.0) gL3ShieldTime -= dt;
	if (gL3NitroTime > 0.0)  gL3NitroTime -= dt;

	// Player movement controls
	int sprintKey = isKeyPressed('d') || isKeyPressed('D') || isSpecialKeyPressed(GLUT_KEY_RIGHT);
	int brakeKey  = isKeyPressed('a') || isKeyPressed('A') || isSpecialKeyPressed(GLUT_KEY_LEFT);

	double speedMult = 1.0;

	if (gL3NitroTime > 0.0)
	{
		speedMult = L3_NITRO_MULT;
		if (sprintKey) speedMult *= 1.15;
	}
	else if (sprintKey && gL3Stamina > 0.0)
	{
		speedMult = L3_SPRINT_MULT;
		gL3Stamina -= L3_STAMINA_SPRINT_DRAIN * dt;
		if (gL3Stamina < 0.0) gL3Stamina = 0.0;
	}
	else if (brakeKey)
	{
		speedMult = L3_BRAKE_MULT;
		gL3Stamina += L3_STAMINA_RECOVER * dt;
		if (gL3Stamina > L3_STAMINA_MAX) gL3Stamina = L3_STAMINA_MAX;
	}
	else
	{
		// Cruising
		gL3Stamina -= L3_STAMINA_RUN_DRAIN * dt;
		if (gL3Stamina < 0.0) gL3Stamina = 0.0;
	}

	if (gL3Stamina <= 0.0 && gL3NitroTime <= 0.0)
	{
		speedMult = L3_TIRED_MULT;
	}

	// Slope resistance: uphill slightly slows, downhill speeds up!
	double groundAngle = l3GetSlopeAngle(gL3PlayerX);
	double slopeEffect = 1.0 - (groundAngle / 90.0) * 0.28;
	if (slopeEffect < 0.72) slopeEffect = 0.72;
	if (slopeEffect > 1.32) slopeEffect = 1.32;

	double targetSpeed = L3_BASE_SPEED * speedMult * slopeEffect;
	// Smooth speed transitions (inertia / momentum)
	gL3Speed += (targetSpeed - gL3Speed) * (1.0 - exp(-dt * 9.0));
	gL3PlayerX += gL3Speed * dt;

	// Distance scoring
	double dDist = gL3PlayerX - gL3DistanceScored;
	if (dDist >= 10.0)
	{
		int pts = (int)(dDist / 10.0) * L3_SCORE_PER_10PX;
		gL3Score += pts;
		gL3DistanceScored += (int)(dDist / 10.0) * 10.0;
	}

	// Jumping & Vertical Physics
	if (jumpPressed && gL3OnGround)
	{
		gL3PlayerVY = L3_JUMP_VELOCITY;
		gL3OnGround = 0;
	}

	int gapIdx = l3GetLavaGapIndex(gL3PlayerX);
	int inGap = (gapIdx >= 0);
	double trackY = l3GetTrackY(gL3PlayerX);

	if (gL3OnGround)
	{
		if (inGap)
		{
			// Rode off the edge of a lava chasm without jumping!
			gL3OnGround = 0;
			gL3PlayerVY = 0.0;
		}
		else
		{
			gL3PlayerY = trackY;
			gL3PlayerVY = 0.0;
		}
	}
	else
	{
		// Airborne physics: smooth continuous parabolic leap
		gL3PlayerY += gL3PlayerVY * dt;
		gL3PlayerVY -= L3_GRAVITY * dt;

		if (!inGap)
		{
			// Over solid ground: check for landing
			if (gL3PlayerY <= trackY && gL3PlayerVY <= 0.0)
			{
				gL3PlayerY = trackY;
				gL3PlayerVY = 0.0;
				gL3OnGround = 1;

				// Check if successfully completed a lava chasm leap
				if (gL3LavaGapsCrossed)
				{
					gL3Combo++;
					if (gL3Combo > 4) gL3Combo = 4;
					gL3ComboTimer = 4.0;
					int bonus = 300 * gL3Combo;
					gL3Score += bonus;
					sprintf(gL3StuntText, "CHASM JUMP! +%d (x%d)", bonus, gL3Combo);
					gL3StuntTimer = 1.6;
					gL3LavaGapsCrossed = 0;
				}
			}
		}
		else
		{
			// Currently soaring over a lava chasm!
			if (gL3PlayerY >= trackY - 20.0)
			{
				// In safe leap envelope above the chasm
				if (!gL3LavaGaps[gapIdx].cleared)
				{
					gL3LavaGaps[gapIdx].cleared = 1;
					gL3LavaGapsCrossed = 1;
				}
			}
			else if (gL3PlayerY <= 50.0)
			{
				// Plunged into the boiling magma pool!
				if (gL3ShieldTime > 0.0)
				{
					// Kinetic shield bounce
					gL3PlayerX = gL3LavaGaps[gapIdx].endX + 35.0;
					gL3PlayerY = l3GetTrackY(gL3PlayerX) + 20.0;
					gL3PlayerVY = 260.0;
					gL3OnGround = 0;
					sprintf(gL3StuntText, "SHIELD BOUNCE OVER LAVA!");
					gL3StuntTimer = 1.4;
				}
				else if (gL3InvulnTime <= 0.0)
				{
					gL3Hp -= 10.0;
					gL3InvulnTime = 1.8;
					gL3ScreenShake = 0.38;
					gL3Combo = 1;
					audioPlayCollision();

					// Rebound safely onto landing ledge
					gL3PlayerX = gL3LavaGaps[gapIdx].endX + 35.0;
					gL3PlayerY = l3GetTrackY(gL3PlayerX);
					gL3PlayerVY = 0.0;
					gL3OnGround = 1;

					if (gL3Hp <= 0.0)
					{
						gL3Hp = 0.0;
						gL3State = L3_STATE_LOST;
						gL3LoseReason = L3_LOSE_HEALTH;
						audioPlayGameOver();
						return;
					}
				}
			}
		}
	}

	// Cycling frame animation based on distance travelled (pedal cadence)
	if (gL3OnGround)
	{
		gL3Frame = ((int)(gL3PlayerX / 22.0)) % 7;
	}

	// Smooth bike tilt with realistic angular momentum
	double targetTilt = 0.0;
	if (gL3OnGround)
	{
		targetTilt = groundAngle * 0.82;
		if (targetTilt < -22.0) targetTilt = -22.0;
		if (targetTilt >  22.0) targetTilt =  22.0;
	}
	else
	{
		// Airborne pitch: tilts up on ascent, levels at peak, pitches down on descent
		targetTilt = (gL3PlayerVY / L3_JUMP_VELOCITY) * 16.0;
		if (targetTilt < -18.0) targetTilt = -18.0;
		if (targetTilt >  18.0) targetTilt =  18.0;
	}
	double tiltBlend = 1.0 - exp(-dt * 12.0);
	gL3Tilt += (targetTilt - gL3Tilt) * tiltBlend;

	// Update moving entities
	int i;
	for (i = 0; i < gL3ObstacleCount; i++)
	{
		struct L3Obstacle *o = &gL3Obstacles[i];
		if (o->speedX != 0.0)
		{
			o->x += o->speedX * dt;
		}
		o->baseY = l3GetTrackY(o->x);
		o->y = o->baseY;
		o->phase += dt * 3.5;
	}

	// Collision detection
	double px = L3_PLAYER_SCREEN_X + (L3_PLAYER_W - L3_PLAYER_HIT_W) / 2;
	double py = gL3PlayerY + 8;
	double pw = L3_PLAYER_HIT_W;
	double ph = L3_PLAYER_HIT_H;

	// Pickups
	for (i = 0; i < gL3PickupCount; i++)
	{
		struct L3Pickup *p = &gL3Pickups[i];
		if (p->taken) continue;

		double sx = l3ScreenX(p->x);
		if (sx < -100 || sx > SCREEN_WIDTH + 100) continue;

		if (l3Overlap(px, py, pw, ph, sx, p->y, p->w, p->h))
		{
			p->taken = 1;
			if (p->type == L3_PU_ENERGY)
			{
				gL3Stamina += L3_ENERGY_REFILL;
				if (gL3Stamina > L3_STAMINA_MAX) gL3Stamina = L3_STAMINA_MAX;
				gL3Hp += 12.0;
				if (gL3Hp > L3_HP_MAX) gL3Hp = L3_HP_MAX;
				gL3Score += L3_SCORE_PICKUP * gL3Combo;
			}
			else if (p->type == L3_PU_SHIELD)
			{
				gL3ShieldTime = L3_SHIELD_SECONDS;
				gL3Score += L3_SCORE_PICKUP * gL3Combo;
			}
			else if (p->type == L3_PU_NITRO)
			{
				gL3NitroTime = L3_NITRO_SECONDS;
				gL3Score += L3_SCORE_PICKUP * gL3Combo;
			}
			else if (p->type == L3_PU_COIN)
			{
				gL3Hp += 5.0;
				if (gL3Hp > L3_HP_MAX) gL3Hp = L3_HP_MAX;
				gL3Score += L3_SCORE_COIN * gL3Combo;
			}
			else if (p->type == L3_PU_HEAL)
			{
				gL3Hp += L3_HEAL_AMOUNT;
				if (gL3Hp > L3_HP_MAX) gL3Hp = L3_HP_MAX;
				gL3Stamina += 30.0;
				if (gL3Stamina > L3_STAMINA_MAX) gL3Stamina = L3_STAMINA_MAX;
				gL3Score += L3_SCORE_PICKUP * gL3Combo;
				sprintf(gL3StuntText, "HP RESTORED! +%d HP", L3_HEAL_AMOUNT);
				gL3StuntTimer = 1.6;
			}
		}
	}

	// Obstacles
	for (i = 0; i < gL3ObstacleCount; i++)
	{
		struct L3Obstacle *o = &gL3Obstacles[i];
		double sx = l3ScreenX(o->x);
		if (sx + o->w < -100 || sx > SCREEN_WIDTH + 100) continue;

		// Passed safely?
		if (!o->scored && (sx + o->w < px))
		{
			o->scored = 1;
			if (!gL3OnGround && (gL3PlayerY - l3GetTrackY(gL3PlayerX)) > 15.0)
			{
				gL3Combo++;
				if (gL3Combo > 4) gL3Combo = 4;
				gL3ComboTimer = 4.0;
				int bonus = 120 * gL3Combo;
				gL3Score += bonus;
				sprintf(gL3StuntText, "STUNT HOP! +%d (x%d)", bonus, gL3Combo);
				gL3StuntTimer = 1.4;
			}
			else
			{
				gL3Score += L3_SCORE_OBSTACLE * gL3Combo;
			}
		}

		if (o->hit) continue;

		// Accurate entity collision box
		double oy = o->y;
		if (l3Overlap(px, py, pw, ph, sx + 8, oy + 4, o->w - 16, o->h - 8))
		{
			if (gL3ShieldTime > 0.0 || gL3NitroTime > 0.0)
			{
				// Shield or Nitro smash
				o->hit = 1;
				audioPlayCollision();
			}
			else if (gL3InvulnTime <= 0.0)
			{
				o->hit = 1;
				gL3Hp -= L3_HIT_DAMAGE;
				gL3InvulnTime = L3_HIT_INVULN_SECONDS;
				gL3ScreenShake = 0.25;
				gL3Combo = 1;
				audioPlayCollision();

				if (gL3Hp <= 0.0)
				{
					gL3Hp = 0.0;
					gL3State = L3_STATE_LOST;
					gL3LoseReason = L3_LOSE_HEALTH;
					audioPlayGameOver();
					profileUpdateLevelScore(3, gL3Score, 0, 0);
					return;
				}
			}
		}
	}

	// Update ambient weather particles
	int curBio = l3GetCurrentBiome(gL3PlayerX);
	int ptIdx;
	for (ptIdx = 0; ptIdx < L3_MAX_PARTICLES; ptIdx++)
	{
		struct L3Particle *pt = &gL3Particles[ptIdx];
		if (curBio == L3_BIOME_HILL)
		{
			pt->x += (pt->vx + gL3Speed * 0.25) * dt;
			pt->y -= pt->vy * 0.7 * dt;
		}
		else if (curBio == L3_BIOME_LAVA)
		{
			pt->x += (pt->vx + gL3Speed * 0.20) * dt;
			pt->y += pt->vy * 1.3 * dt;
		}
		else if (curBio == L3_BIOME_SNOW)
		{
			pt->x += (pt->vx + gL3Speed * 0.30) * dt;
			pt->y -= pt->vy * 1.5 * dt;
		}
		else // L3_BIOME_RAIL
		{
			pt->x += (pt->vx + gL3Speed * 0.35) * dt;
			pt->y += sin(gL3Elapsed * 2.5 + pt->phase) * 6.0 * dt;
		}

		if (pt->x > SCREEN_WIDTH + 20)
		{
			pt->x = -20;
			pt->y = (double)(rand() % SCREEN_HEIGHT);
		}
		if (pt->y < -20)
		{
			pt->y = SCREEN_HEIGHT + 20;
			pt->x = (double)(rand() % SCREEN_WIDTH);
		}
		if (pt->y > SCREEN_HEIGHT + 20)
		{
			pt->y = -20;
			pt->x = (double)(rand() % SCREEN_WIDTH);
		}
	}

	// Gear shift calculation
	int kmh = (int)(gL3Speed * 0.12);
	if (gL3NitroTime > 0.0) gL3CurrentGear = 5;
	else if (kmh >= 46) gL3CurrentGear = 4;
	else if (kmh >= 34) gL3CurrentGear = 3;
	else if (kmh >= 20) gL3CurrentGear = 2;
	else gL3CurrentGear = 1;

	// Win condition
	if (gL3PlayerX >= L3_LENGTH)
	{
		gL3PlayerX = L3_LENGTH;
		gL3State = L3_STATE_WON;
		markLevelCompleted(3);

		// Time bonus
		long timeLeft = (long)(L3_TIME_LIMIT - gL3Elapsed);
		if (timeLeft > 0)
			gL3Score += timeLeft * L3_SCORE_TIME_BONUS;

		if (gL3Score >= gL3ParScore + 1000) gL3Medal = 3;      // Gold
		else if (gL3Score >= gL3ParScore)   gL3Medal = 2;      // Silver
		else                                gL3Medal = 1;      // Bronze

		profileUpdateLevelScore(3, gL3Score, gL3Medal, 1);
	}
}

#endif
