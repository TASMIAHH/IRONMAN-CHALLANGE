//======================================================================
//  Level04.h
//  LEVEL 04 - SWIMMING EXPEDITION
//
//  An endurance open-water and deep-ocean swimming challenge across 4
//  transitioning oceanic biomes: Sunlit Turquoise Lagoon, Pelagic Deep
//  Ocean, Giant Kelp Forest, and Sunken Galleon Shipwreck Trench.
//
//  Features:
//  - True 2D fluid swimming dynamics (free vertical & horizontal navigation)
//  - Realistic hydrodynamic drag, water resistance, buoyancy & inertia
//  - Dynamic Oxygen / Breath meter (recharge at surface or via oxygen tanks)
//  - Fast freestyle flutter kick sprinting, gliding, and Dolphin Kick surges
//  - Natural swimming obstacles: Jellyfish, Reef Sharks, Staghorn Corals,
//    Driftwood logs, Sea Urchins, Sea Rocks, Whirlpools, Buoy Chains
//  - Full power-up suite: Oxygen tanks, Energy gels, Speed fins, Shields,
//    Sunken Gold Coins, and Marine First-Aid packs
//  - Strictly image-based rendering with 0 procedural/cartoon vector primitives
//======================================================================
#ifndef LEVEL04_H
#define LEVEL04_H

#include <math.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

//----------------------------------------------------------------------
// Asset paths
//----------------------------------------------------------------------
#define L4_PATH_BG_LAGOON     "Assets/level04_bg_lagoon.png"
#define L4_PATH_BG_DEEPBLUE   "Assets/level04_bg_deepblue.png"
#define L4_PATH_BG_KELP       "Assets/level04_bg_kelp.png"
#define L4_PATH_BG_TRENCH     "Assets/level04_bg_trench.png"

#define L4_PATH_START         "Assets/swim_start.png"
#define L4_PATH_FINISH        "Assets/swim_finish.png"
#define L4_PATH_BUBBLE        "Assets/water_bubble.png"

//----------------------------------------------------------------------
// Biomes
//----------------------------------------------------------------------
#define L4_BIOME_LAGOON       0
#define L4_BIOME_DEEPBLUE     1
#define L4_BIOME_KELP         2
#define L4_BIOME_TRENCH       3
#define L4_BIOME_COUNT        4

#define L4_BIOME_0_END     11000.0
#define L4_BIOME_1_END    22000.0
#define L4_BIOME_2_END    33000.0
#define L4_BIOME_3_END    44000.0

//----------------------------------------------------------------------
// Geometry
//----------------------------------------------------------------------
#define L4_PLAYER_SCREEN_X    160
#define L4_PLAYER_W           216
#define L4_PLAYER_H           104
#define L4_PLAYER_HIT_W       130
#define L4_PLAYER_HIT_H        54

#define L4_WATER_SURFACE_Y    470.0
#define L4_SEABED_Y            45.0

//----------------------------------------------------------------------
// Motion (all scaled by delta time)
//----------------------------------------------------------------------
#define L4_BASE_SPEED         290.0   // px/s forward cruise
#define L4_SPRINT_MULT          1.52  // forward freestyle kick sprint
#define L4_BRAKE_MULT           0.58  // coast / glide
#define L4_TIRED_MULT           0.65  // when exhausted
#define L4_FINS_MULT            1.48  // speed fins buff

#define L4_SWIM_VERT_SPEED    260.0   // px/s up/down swimming speed (smooth & agile)
#define L4_BUOYANCY            18.0   // px/s gentle natural upward drift
#define L4_SURGE_BOOST        220.0   // Dolphin Kick forward burst

#define L4_MAX_DELTA            0.10  // clamp

//----------------------------------------------------------------------
// Course & Survival
//----------------------------------------------------------------------
#define L4_LENGTH            44000    // world px (~151 s at base speed, ~2.3-2.6 min playtime)
#define L4_TIME_LIMIT          190    // seconds (3:10)

#define L4_HP_MAX              120    // Challenging health pool (6 hits)
#define L4_STAMINA_MAX         100    // Paced stamina pool
#define L4_OXYGEN_MAX          100    // Standard lung capacity

#define L4_STAMINA_CRUISE_DRAIN 1.2   // per second cruising
#define L4_STAMINA_SPRINT_DRAIN 14.0  // per second sprinting
#define L4_STAMINA_RECOVER     16.5   // per second gliding
#define L4_STAMINA_SURGE_COST  20.0   // per dolphin kick surge

#define L4_OXYGEN_DEEP_DRAIN     5.2  // per second while submerged deep (Y < 380)
#define L4_OXYGEN_SURFACE_REFILL 65.0 // per second when near surface (Y >= 380)
#define L4_OXYGEN_TANK_REFILL    50.0 // from oxygen canister pickup
#define L4_ASPHYXIA_DAMAGE      12.0  // HP/s lost when oxygen hits 0

#define L4_HEAL_AMOUNT          35
#define L4_HIT_DAMAGE           20
#define L4_HIT_INVULN_SECONDS    1.2
#define L4_SHIELD_SECONDS        7.0
#define L4_FINS_SECONDS          7.0
#define L4_READY_SECONDS         2.0

//----------------------------------------------------------------------
// Scoring
//----------------------------------------------------------------------
#define L4_SCORE_OBSTACLE       70
#define L4_SCORE_PICKUP        120
#define L4_SCORE_COIN          250
#define L4_SCORE_PER_10PX        1
#define L4_SCORE_TIME_BONUS     35

//----------------------------------------------------------------------
// Entity kinds
//----------------------------------------------------------------------
#define L4_OB_JELLYFISH   0
#define L4_OB_SHARK       1
#define L4_OB_CORAL       2
#define L4_OB_DRIFTWOOD   3
#define L4_OB_URCHIN      4
#define L4_OB_ROCK        5
#define L4_OB_WHIRLPOOL   6
#define L4_OB_BUOY_CHAIN  7
#define L4_OB_KINDS       8

#define L4_PU_OXYGEN      0
#define L4_PU_ENERGY      1
#define L4_PU_FINS        2
#define L4_PU_SHIELD      3
#define L4_PU_COIN        4
#define L4_PU_HEAL        5
#define L4_PU_KINDS       6

//----------------------------------------------------------------------
// Game states & Lose reasons
//----------------------------------------------------------------------
#define L4_STATE_READY    0
#define L4_STATE_RUNNING  1
#define L4_STATE_PAUSED   2
#define L4_STATE_WON      3
#define L4_STATE_LOST     4

#define L4_LOSE_NONE      0
#define L4_LOSE_HEALTH    1
#define L4_LOSE_TIME      2
#define L4_LOSE_ASPHYXIA  3

//----------------------------------------------------------------------
// Entities
//----------------------------------------------------------------------
#define L4_MAX_OBSTACLES  350
#define L4_MAX_PICKUPS    300
#define L4_MAX_PARTICLES  120
#define L4_MAX_BUBBLES     80

struct L4Obstacle
{
	int type;
	double x;
	double y;
	double baseY;
	double speedX;
	double speedY;
	double phase;
	int w;
	int h;
	int hit;
};

struct L4Pickup
{
	int type;
	double x;
	double y;
	int w;
	int h;
	int taken;
};

struct L4Particle
{
	double x;
	double y;
	double vx;
	double vy;
	double size;
	double alpha;
	double phase;
};

struct L4Bubble
{
	double x;
	double y;
	double vx;
	double vy;
	double size;
	double life;
	double maxLife;
	int active;
};

//----------------------------------------------------------------------
// Global Level 04 State
//----------------------------------------------------------------------
int gL4State = L4_STATE_READY;
int gL4LoseReason = L4_LOSE_NONE;

double gL4PlayerX = 160.0;
double gL4PlayerY = 320.0;
double gL4PlayerVY = 0.0;
double gL4Speed = L4_BASE_SPEED;
double gL4Tilt = 0.0;
double gL4SurgeTimer = 0.0;

double gL4Hp = L4_HP_MAX;
double gL4Stamina = L4_STAMINA_MAX;
double gL4Oxygen = L4_OXYGEN_MAX;
long gL4Score = 0;
double gL4DistanceScored = 160.0;

double gL4Elapsed = 0.0;
double gL4ReadyTime = L4_READY_SECONDS;
double gL4InvulnTime = 0.0;
double gL4ShieldTime = 0.0;
double gL4FinsTime = 0.0;

int gL4Frame = 0;
double gL4StrokeAccum = 0.0;
int gL4Medal = 0;
int gL4Combo = 1;
double gL4ComboTimer = 0.0;
double gL4ScreenShake = 0.0;

// Milestone banners
int gL4WaypointPassed[4] = { 0, 0, 0, 0 };
int gL4ActiveWaypoint = -1;
double gL4WaypointTimer = 0.0;

struct L4Obstacle gL4Obstacles[L4_MAX_OBSTACLES];
int gL4ObstacleCount = 0;

struct L4Pickup gL4Pickups[L4_MAX_PICKUPS];
int gL4PickupCount = 0;

struct L4Particle gL4Particles[L4_MAX_PARTICLES];
struct L4Bubble gL4Bubbles[L4_MAX_BUBBLES];

// Textures
unsigned int gL4TexBg[L4_BIOME_COUNT];
unsigned int gL4TexCaustics = 0;
unsigned int gL4TexSunbeams = 0;
unsigned int gL4TexSeabedSand = 0;
unsigned int gL4TexSeabedRock = 0;
unsigned int gL4TexFishSchool = 0;
unsigned int gL4TexMantaRay = 0;
unsigned int gL4TexKelpStalk = 0;
unsigned int gL4TexSplash = 0;
unsigned int gL4TexWaterSurface = 0;
unsigned int gL4TexSwim[7];
unsigned int gL4TexSwimSprint = 0;
unsigned int gL4TexSwimHurt = 0;
unsigned int gL4TexObstacle[L4_OB_KINDS];
unsigned int gL4TexPickup[L4_PU_KINDS];
unsigned int gL4TexStart = 0;
unsigned int gL4TexFinish = 0;
unsigned int gL4TexBubble = 0;

#define L4_MAX_FISH_SCHOOLS 3
struct L4FishSchool
{
	double x;
	double y;
	double speedX;
	double phase;
	double scale;
	float alpha;
	int active;
};
struct L4FishSchool gL4FishSchools[L4_MAX_FISH_SCHOOLS];

// Dynamic Rising Seabed Bubble Columns
#define L4_MAX_VENT_BUBBLES 140
struct L4VentBubble
{
	double worldX;
	double y;
	double vy;
	double baseSize;
	double phase;
	double life;
	double maxLife;
	int active;
};
struct L4VentBubble gL4VentBubbles[L4_MAX_VENT_BUBBLES];

#define L4_MAX_VENTS 30
struct L4Vent
{
	double worldX;
	double timer;
	double interval;
};
struct L4Vent gL4Vents[L4_MAX_VENTS];
int gL4VentCount = 0;

// Dynamic Surface Splash & Meniscus Rings
#define L4_MAX_SPLASHES 24
struct L4SplashRing
{
	double screenX;
	double y;
	double size;
	double maxSize;
	double life;
	double maxLife;
	int active;
};
struct L4SplashRing gL4Splashes[L4_MAX_SPLASHES];

// Dynamic Swaying Seabed Kelp Vegetation
#define L4_MAX_KELP_PLANTS 32
struct L4KelpPlant
{
	double worldX;
	double y;
	double height;
	double width;
	double phase;
	double bendSpeed;
	double bendAmp;
};
struct L4KelpPlant gL4KelpPlants[L4_MAX_KELP_PLANTS];
int gL4KelpPlantCount = 0;

// Majestic Background Pelagic Manta Ray
struct L4MantaRay
{
	double x;
	double y;
	double speedX;
	double speedY;
	double phase;
	double scale;
	float alpha;
	int active;
};
struct L4MantaRay gL4MantaRay;

LARGE_INTEGER gL4Freq;
LARGE_INTEGER gL4LastCounter;

//----------------------------------------------------------------------
// Helpers
//----------------------------------------------------------------------
int l4GetCurrentBiome(double worldX)
{
	if (worldX < L4_BIOME_0_END) return L4_BIOME_LAGOON;
	if (worldX < L4_BIOME_1_END) return L4_BIOME_DEEPBLUE;
	if (worldX < L4_BIOME_2_END) return L4_BIOME_KELP;
	return L4_BIOME_TRENCH;
}

double l4ScreenX(double worldX)
{
	return (worldX - gL4PlayerX) + L4_PLAYER_SCREEN_X;
}

void l4SpawnBubble(double x, double y, double vx, double vy, double size, double life)
{
	int i;
	for (i = 0; i < L4_MAX_BUBBLES; i++)
	{
		if (!gL4Bubbles[i].active)
		{
			gL4Bubbles[i].active = 1;
			gL4Bubbles[i].x = x;
			gL4Bubbles[i].y = y;
			gL4Bubbles[i].vx = vx;
			gL4Bubbles[i].vy = vy;
			gL4Bubbles[i].size = size;
			gL4Bubbles[i].life = life;
			gL4Bubbles[i].maxLife = life;
			return;
		}
	}
}

void l4SpawnVentBubble(double worldX, double y, double baseSize, double life)
{
	int i;
	for (i = 0; i < L4_MAX_VENT_BUBBLES; i++)
	{
		if (!gL4VentBubbles[i].active)
		{
			gL4VentBubbles[i].active = 1;
			gL4VentBubbles[i].worldX = worldX + ((rand() % 16) - 8.0);
			gL4VentBubbles[i].y = y;
			gL4VentBubbles[i].vy = 65.0 + (rand() % 45);
			gL4VentBubbles[i].baseSize = baseSize;
			gL4VentBubbles[i].phase = (double)(rand() % 100);
			gL4VentBubbles[i].life = life;
			gL4VentBubbles[i].maxLife = life;
			return;
		}
	}
}

void l4SpawnSplash(double screenX, double y, double size, double life)
{
	int i;
	for (i = 0; i < L4_MAX_SPLASHES; i++)
	{
		if (!gL4Splashes[i].active)
		{
			gL4Splashes[i].active = 1;
			gL4Splashes[i].screenX = screenX;
			gL4Splashes[i].y = y;
			gL4Splashes[i].size = size * 0.35;
			gL4Splashes[i].maxSize = size;
			gL4Splashes[i].life = life;
			gL4Splashes[i].maxLife = life;
			return;
		}
	}
}

//----------------------------------------------------------------------
// Texture Loading
//----------------------------------------------------------------------
void level04LoadAssets()
{
	char path[64];
	int i;

	// 4 Biome Backgrounds
	assetLoadTexture(L4_PATH_BG_LAGOON, &gL4TexBg[L4_BIOME_LAGOON]);
	assetLoadTexture(L4_PATH_BG_DEEPBLUE, &gL4TexBg[L4_BIOME_DEEPBLUE]);
	assetLoadTexture(L4_PATH_BG_KELP, &gL4TexBg[L4_BIOME_KELP]);
	assetLoadTexture(L4_PATH_BG_TRENCH, &gL4TexBg[L4_BIOME_TRENCH]);

	// Dynamic Underwater Environment Assets
	assetLoadTexture("Assets/l4_caustics.png", &gL4TexCaustics);
	assetLoadTexture("Assets/l4_sunbeams.png", &gL4TexSunbeams);
	assetLoadTexture("Assets/l4_seabed_sand.png", &gL4TexSeabedSand);
	assetLoadTexture("Assets/l4_seabed_rock.png", &gL4TexSeabedRock);
	assetLoadTexture("Assets/l4_fish_school.png", &gL4TexFishSchool);
	assetLoadTexture("Assets/l4_manta_ray.png", &gL4TexMantaRay);
	assetLoadTexture("Assets/l4_kelp_stalk.png", &gL4TexKelpStalk);
	assetLoadTexture("Assets/l4_surface_splash.png", &gL4TexSplash);
	assetLoadTexture("Assets/l4_water_surface.png", &gL4TexWaterSurface);

	// Swimmer 7-frame freestyle cycle
	for (i = 0; i < 7; i++)
	{
		sprintf(path, "Assets/swim_%02d.png", i + 1);
		assetLoadTexture(path, &gL4TexSwim[i]);
	}
	assetLoadTexture("Assets/swim_sprint.png", &gL4TexSwimSprint);
	assetLoadTexture("Assets/swim_hurt.png", &gL4TexSwimHurt);

	// Obstacles
	assetLoadTexture("Assets/obs4_jellyfish.png", &gL4TexObstacle[L4_OB_JELLYFISH]);
	assetLoadTexture("Assets/obs4_shark.png", &gL4TexObstacle[L4_OB_SHARK]);
	assetLoadTexture("Assets/obs4_coral.png", &gL4TexObstacle[L4_OB_CORAL]);
	assetLoadTexture("Assets/obs4_driftwood.png", &gL4TexObstacle[L4_OB_DRIFTWOOD]);
	assetLoadTexture("Assets/obs4_urchin.png", &gL4TexObstacle[L4_OB_URCHIN]);
	assetLoadTexture("Assets/obs4_rock.png", &gL4TexObstacle[L4_OB_ROCK]);
	assetLoadTexture("Assets/obs4_whirlpool.png", &gL4TexObstacle[L4_OB_WHIRLPOOL]);
	assetLoadTexture("Assets/obs4_buoy_chain.png", &gL4TexObstacle[L4_OB_BUOY_CHAIN]);

	// Pickups
	assetLoadTexture("Assets/pu4_oxygen.png", &gL4TexPickup[L4_PU_OXYGEN]);
	assetLoadTexture("Assets/pu4_energy.png", &gL4TexPickup[L4_PU_ENERGY]);
	assetLoadTexture("Assets/pu4_fins.png", &gL4TexPickup[L4_PU_FINS]);
	assetLoadTexture("Assets/pu4_shield.png", &gL4TexPickup[L4_PU_SHIELD]);
	assetLoadTexture("Assets/pu4_coin.png", &gL4TexPickup[L4_PU_COIN]);
	assetLoadTexture("Assets/pu4_heal.png", &gL4TexPickup[L4_PU_HEAL]);

	// Markers & Effects
	assetLoadTexture(L4_PATH_START, &gL4TexStart);
	assetLoadTexture(L4_PATH_FINISH, &gL4TexFinish);
	assetLoadTexture(L4_PATH_BUBBLE, &gL4TexBubble);
}

//----------------------------------------------------------------------
// Course Builder
//----------------------------------------------------------------------
void l4AddObstacle(int type, double x, double y, int w, int h, double sx, double sy)
{
	if (gL4ObstacleCount >= L4_MAX_OBSTACLES) return;
	struct L4Obstacle *o = &gL4Obstacles[gL4ObstacleCount++];
	o->type = type;
	o->x = x;
	o->y = y;
	o->baseY = y;
	o->speedX = sx;
	o->speedY = sy;
	o->phase = (double)(rand() % 100);
	o->w = w;
	o->h = h;
	o->hit = 0;
}

void l4AddPickup(int type, double x, double y, int w, int h)
{
	if (gL4PickupCount >= L4_MAX_PICKUPS) return;
	struct L4Pickup *p = &gL4Pickups[gL4PickupCount++];
	p->type = type;
	p->x = x;
	p->y = y;
	p->w = w;
	p->h = h;
	p->taken = 0;
}

void l4BuildCourse()
{
	gL4ObstacleCount = 0;
	gL4PickupCount = 0;

	double x;

	// 1. BIOME 1: TURQUOISE LAGOON & CORAL SHALLOWS (0 to 11000)
	// Obstacles: Coral pinnacles on sea floor, floating driftwood near surface, gentle jellyfish
	for (x = 600.0; x < 10600.0; x += 440.0)
	{
		int pattern = ((int)(x / 440)) % 3;
		if (pattern == 0)
		{
			// Coral pinnacle on seabed
			l4AddObstacle(L4_OB_CORAL, x, L4_SEABED_Y + 5, 88, 120, 0.0, 0.0);
			l4AddPickup(L4_PU_OXYGEN, x + 20.0, 360.0, 42, 48);
		}
		else if (pattern == 1)
		{
			// Driftwood log floating at surface
			l4AddObstacle(L4_OB_DRIFTWOOD, x, L4_WATER_SURFACE_Y - 45, 128, 55, 0.0, 0.0);
			l4AddPickup(L4_PU_COIN, x + 30.0, 220.0, 36, 36);
		}
		else
		{
			// Pulsing Lion's Mane Jellyfish mid-water
			l4AddObstacle(L4_OB_JELLYFISH, x, 240.0, 84, 115, 0.0, 24.0);
			l4AddPickup(L4_PU_ENERGY, x + 210.0, 320.0, 34, 46);
		}
	}
	l4AddPickup(L4_PU_FINS,   1400.0, 280.0, 42, 48);
	l4AddPickup(L4_PU_SHIELD, 2800.0, 320.0, 44, 44);
	l4AddPickup(L4_PU_HEAL,   4200.0, 260.0, 42, 42);
	l4AddPickup(L4_PU_ENERGY, 5800.0, 280.0, 34, 46);
	l4AddPickup(L4_PU_FINS,   7400.0, 300.0, 42, 48);
	l4AddPickup(L4_PU_SHIELD, 8800.0, 320.0, 44, 44);
	l4AddPickup(L4_PU_HEAL,  10200.0, 260.0, 42, 42);

	// 2. BIOME 2: PELAGIC DEEP OCEAN & SHARK ZONE (11000 to 22000)
	// Obstacles: Patrolling reef sharks, deep sea rocks, buoy mooring chains
	for (x = 11400.0; x < 21600.0; x += 400.0)
	{
		int pattern = ((int)(x / 400)) % 3;
		if (pattern == 0)
		{
			// Cruising Reef Shark in mid/deep water
			l4AddObstacle(L4_OB_SHARK, x, 180.0 + (rand() % 140), 160, 68, -45.0, 0.0);
			l4AddPickup(L4_PU_OXYGEN, x + 40.0, 440.0, 42, 48);
		}
		else if (pattern == 1)
		{
			// Navigation buoy with anchor chain from surface
			l4AddObstacle(L4_OB_BUOY_CHAIN, x, L4_WATER_SURFACE_Y - 120, 64, 130, 0.0, 0.0);
			l4AddPickup(L4_PU_COIN, x + 200.0, 160.0, 36, 36);
		}
		else
		{
			// Deep Sea Rock Pinnacle
			l4AddObstacle(L4_OB_ROCK, x, L4_SEABED_Y + 5, 100, 110, 0.0, 0.0);
			l4AddPickup(L4_PU_FINS, x + 190.0, 310.0, 42, 48);
		}
	}
	l4AddPickup(L4_PU_SHIELD, 12500.0, 290.0, 44, 44);
	l4AddPickup(L4_PU_HEAL,   14500.0, 350.0, 42, 42);
	l4AddPickup(L4_PU_ENERGY, 16500.0, 240.0, 34, 46);
	l4AddPickup(L4_PU_FINS,   18500.0, 310.0, 42, 48);
	l4AddPickup(L4_PU_HEAL,   20500.0, 330.0, 42, 42);

	// 3. BIOME 3: GIANT KELP FOREST & ROCKY TRENCH (22000 to 33000)
	// Obstacles: Spiny Sea Urchins on rocky bases, multiple Jellyfish, Whirlpools
	for (x = 22400.0; x < 32600.0; x += 420.0)
	{
		int pattern = ((int)(x / 420)) % 3;
		if (pattern == 0)
		{
			// Sea Urchin cluster on seabed rock
			l4AddObstacle(L4_OB_URCHIN, x, L4_SEABED_Y + 10, 72, 72, 0.0, 0.0);
			l4AddPickup(L4_PU_OXYGEN, x + 30.0, 380.0, 42, 48);
		}
		else if (pattern == 1)
		{
			// Swirling Whirlpool / Undercurrent Vortex mid-water
			l4AddObstacle(L4_OB_WHIRLPOOL, x, 220.0 + (rand() % 80), 96, 96, 0.0, 0.0);
			l4AddPickup(L4_PU_COIN, x + 180.0, 340.0, 36, 36);
		}
		else
		{
			// Descending / Ascending Jellyfish
			l4AddObstacle(L4_OB_JELLYFISH, x, 300.0, 84, 115, 0.0, 35.0);
			l4AddPickup(L4_PU_ENERGY, x + 200.0, 200.0, 34, 46);
		}
	}
	l4AddPickup(L4_PU_FINS,   23500.0, 310.0, 42, 48);
	l4AddPickup(L4_PU_SHIELD, 25500.0, 270.0, 44, 44);
	l4AddPickup(L4_PU_HEAL,   27500.0, 330.0, 42, 42);
	l4AddPickup(L4_PU_ENERGY, 29500.0, 250.0, 34, 46);
	l4AddPickup(L4_PU_SHIELD, 31500.0, 290.0, 44, 44);

	// 4. BIOME 4: SUNKEN SHIPWRECK & ABYSSAL REEF (33000 to 44000)
	// Obstacles: Dense combination of patrolling sharks, driftwood debris, urchins & whirlpools
	for (x = 33300.0; x < 43600.0; x += 390.0)
	{
		int pattern = ((int)(x / 390)) % 4;
		if (pattern == 0)
		{
			l4AddObstacle(L4_OB_SHARK, x, 240.0, 160, 68, -55.0, 0.0);
			l4AddPickup(L4_PU_OXYGEN, x + 35.0, 430.0, 42, 48);
		}
		else if (pattern == 1)
		{
			l4AddObstacle(L4_OB_DRIFTWOOD, x, L4_WATER_SURFACE_Y - 45, 128, 55, 0.0, 0.0);
			l4AddPickup(L4_PU_COIN, x + 160.0, 200.0, 36, 36);
		}
		else if (pattern == 2)
		{
			l4AddObstacle(L4_OB_WHIRLPOOL, x, 180.0, 96, 96, 0.0, 0.0);
			l4AddPickup(L4_PU_FINS, x + 180.0, 350.0, 42, 48);
		}
		else
		{
			l4AddObstacle(L4_OB_CORAL, x, L4_SEABED_Y + 5, 88, 120, 0.0, 0.0);
			l4AddPickup(L4_PU_HEAL, x + 180.0, 280.0, 42, 42);
		}
	}
	l4AddPickup(L4_PU_SHIELD, 34500.0, 310.0, 44, 44);
	l4AddPickup(L4_PU_ENERGY, 36500.0, 260.0, 34, 46);
	l4AddPickup(L4_PU_HEAL,   38500.0, 290.0, 42, 42);
	l4AddPickup(L4_PU_FINS,   40500.0, 320.0, 42, 48);
	l4AddPickup(L4_PU_SHIELD, 42500.0, 310.0, 44, 44);
	l4AddPickup(L4_PU_COIN,   43500.0, 320.0, 36, 36);
}

//----------------------------------------------------------------------
// Startup
//----------------------------------------------------------------------
void level04Start()
{
	gL4State = L4_STATE_READY;
	gL4LoseReason = L4_LOSE_NONE;

	gL4PlayerX = 160.0;
	gL4PlayerY = 320.0;
	gL4PlayerVY = 0.0;
	gL4Speed = L4_BASE_SPEED;
	gL4Tilt = 0.0;
	gL4SurgeTimer = 0.0;

	gL4Hp = L4_HP_MAX;
	gL4Stamina = L4_STAMINA_MAX;
	gL4Oxygen = L4_OXYGEN_MAX;
	gL4Score = 0;
	gL4DistanceScored = 160.0;

	gL4Elapsed = 0.0;
	gL4ReadyTime = L4_READY_SECONDS;
	gL4InvulnTime = 0.0;
	gL4ShieldTime = 0.0;
	gL4FinsTime = 0.0;

	gL4Frame = 0;
	gL4StrokeAccum = 0.0;
	gL4Medal = 0;
	gL4Combo = 1;
	gL4ComboTimer = 0.0;
	gL4ScreenShake = 0.0;

	gL4ActiveWaypoint = -1;
	gL4WaypointTimer = 0.0;
	int w;
	for (w = 0; w < 4; w++) gL4WaypointPassed[w] = 0;

	l4BuildCourse();

	// Initialize ambient marine particles (plankton, light motes, small bubbles)
	int p;
	for (p = 0; p < L4_MAX_PARTICLES; p++)
	{
		gL4Particles[p].x = (double)(rand() % SCREEN_WIDTH);
		gL4Particles[p].y = (double)(rand() % SCREEN_HEIGHT);
		gL4Particles[p].vx = -25.0 - (rand() % 35);
		gL4Particles[p].vy = 10.0 + (rand() % 25);
		gL4Particles[p].size = 2.0 + (rand() % 4);
		gL4Particles[p].alpha = 0.35 + ((rand() % 40) / 100.0);
		gL4Particles[p].phase = (double)(rand() % 100);
	}

	for (p = 0; p < L4_MAX_BUBBLES; p++)
	{
		gL4Bubbles[p].active = 0;
	}

	// Initialize background marine life (schools of fish at depth)
	int f;
	for (f = 0; f < L4_MAX_FISH_SCHOOLS; f++)
	{
		gL4FishSchools[f].x = 240.0 + f * 340.0;
		gL4FishSchools[f].y = 110.0 + f * 105.0;
		gL4FishSchools[f].speedX = 75.0 + f * 25.0;
		gL4FishSchools[f].phase = (double)(rand() % 100);
		gL4FishSchools[f].scale = 0.50 + f * 0.18;
		gL4FishSchools[f].alpha = 0.45f + f * 0.18f;
		gL4FishSchools[f].active = 1;
	}

	// Initialize Seabed Bubble Vents
	int v;
	for (v = 0; v < L4_MAX_VENT_BUBBLES; v++)
	{
		gL4VentBubbles[v].active = 0;
	}

	gL4VentCount = 0;
	for (double vx = 500.0; vx < 19600.0; vx += 750.0)
	{
		if (gL4VentCount < L4_MAX_VENTS)
		{
			gL4Vents[gL4VentCount].worldX = vx + (rand() % 150 - 75);
			gL4Vents[gL4VentCount].timer = (rand() % 100) / 100.0 * 0.25;
			gL4Vents[gL4VentCount].interval = 0.18 + (rand() % 15) / 100.0;
			gL4VentCount++;
		}
	}

	// Initialize Surface Splash Rings
	int s;
	for (s = 0; s < L4_MAX_SPLASHES; s++)
	{
		gL4Splashes[s].active = 0;
	}

	// Initialize Seabed Kelp Vegetation (organic swaying kelp stalks)
	gL4KelpPlantCount = 0;
	for (double kx = 350.0; kx < 19700.0; kx += 420.0)
	{
		if (gL4KelpPlantCount < L4_MAX_KELP_PLANTS)
		{
			struct L4KelpPlant *kp = &gL4KelpPlants[gL4KelpPlantCount++];
			kp->worldX = kx + (rand() % 100 - 50);
			kp->y = L4_SEABED_Y - 8.0;
			kp->height = 250.0 + (rand() % 140);
			kp->width = 75.0 + (rand() % 25);
			kp->phase = (double)(rand() % 100);
			kp->bendSpeed = 1.6 + (rand() % 10) / 10.0;
			kp->bendAmp = 28.0 + (rand() % 22);
		}
	}

	// Initialize Majestic Pelagic Manta Ray Glider
	gL4MantaRay.active = 1;
	gL4MantaRay.x = SCREEN_WIDTH + 180.0;
	gL4MantaRay.y = 260.0;
	gL4MantaRay.speedX = 65.0;
	gL4MantaRay.speedY = 0.0;
	gL4MantaRay.phase = 0.0;
	gL4MantaRay.scale = 0.72;
	gL4MantaRay.alpha = 0.65f;

	QueryPerformanceFrequency(&gL4Freq);
	QueryPerformanceCounter(&gL4LastCounter);

	audioPlayLevel01Theme(); // loops energetic stage music
}

//----------------------------------------------------------------------
// Overlap Collision Test
//----------------------------------------------------------------------
int l4Overlap(double x1, double y1, double w1, double h1,
              double x2, double y2, double w2, double h2)
{
	return (x1 < x2 + w2) && (x1 + w1 > x2) &&
	       (y1 < y2 + h2) && (y1 + h1 > y2);
}

//----------------------------------------------------------------------
// Physics & Logic Update
//----------------------------------------------------------------------
void level04Update(int surgePressed, int pausePressed, int backPressed, int confirmPressed)
{
	if (backPressed)
	{
		audioPlayIntroTheme();
		gCurrentScreen = SCREEN_LEVEL_SELECT;
		return;
	}

	if (pausePressed && (gL4State == L4_STATE_RUNNING || gL4State == L4_STATE_PAUSED))
	{
		gL4State = (gL4State == L4_STATE_RUNNING) ? L4_STATE_PAUSED : L4_STATE_RUNNING;
		QueryPerformanceCounter(&gL4LastCounter);
		return;
	}

	if ((gL4State == L4_STATE_WON || gL4State == L4_STATE_LOST) && confirmPressed)
	{
		level04Start();
		return;
	}

	if (gL4State == L4_STATE_PAUSED || gL4State == L4_STATE_WON || gL4State == L4_STATE_LOST)
		return;

	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	double dt = (double)(now.QuadPart - gL4LastCounter.QuadPart) / (double)gL4Freq.QuadPart;
	gL4LastCounter = now;

	if (dt > L4_MAX_DELTA) dt = L4_MAX_DELTA;
	if (dt <= 0.0) return;

	// Ready countdown
	if (gL4State == L4_STATE_READY)
	{
		gL4ReadyTime -= dt;
		if (gL4ReadyTime <= 0.0)
		{
			gL4State = L4_STATE_RUNNING;
			gL4ReadyTime = 0.0;
		}
		return;
	}

	if (gL4State != L4_STATE_RUNNING) return;

	gL4Elapsed += dt;

	// Timers
	if (gL4ComboTimer > 0.0)
	{
		gL4ComboTimer -= dt;
		if (gL4ComboTimer <= 0.0) gL4Combo = 1;
	}
	if (gL4ScreenShake > 0.0)
	{
		gL4ScreenShake -= dt;
		if (gL4ScreenShake < 0.0) gL4ScreenShake = 0.0;
	}
	if (gL4WaypointTimer > 0.0) gL4WaypointTimer -= dt;
	if (gL4SurgeTimer > 0.0) gL4SurgeTimer -= dt;

	// Time limit check
	if (gL4Elapsed >= L4_TIME_LIMIT)
	{
		gL4State = L4_STATE_LOST;
		gL4LoseReason = L4_LOSE_TIME;
		audioPlayGameOver();
		profileUpdateLevelScore(4, gL4Score, 0, 0);
		return;
	}

	// Buff timers
	if (gL4InvulnTime > 0.0) gL4InvulnTime -= dt;
	if (gL4ShieldTime > 0.0) gL4ShieldTime -= dt;
	if (gL4FinsTime > 0.0)   gL4FinsTime -= dt;

	// Keyboard Input Polling
	int moveUp   = isKeyPressed('w') || isKeyPressed('W') || isSpecialKeyPressed(GLUT_KEY_UP);
	int moveDown = isKeyPressed('s') || isKeyPressed('S') || isSpecialKeyPressed(GLUT_KEY_DOWN);
	int moveFwd  = isKeyPressed('d') || isKeyPressed('D') || isSpecialKeyPressed(GLUT_KEY_RIGHT);
	int moveBack = isKeyPressed('a') || isKeyPressed('A') || isSpecialKeyPressed(GLUT_KEY_LEFT);

	// Dolphin Kick Surge Trigger
	if (surgePressed && gL4Stamina >= L4_STAMINA_SURGE_COST && gL4SurgeTimer <= 0.0)
	{
		gL4SurgeTimer = 1.2;
		gL4Stamina -= L4_STAMINA_SURGE_COST;
		if (gL4Stamina < 0.0) gL4Stamina = 0.0;

		// Spawn burst of wake bubbles
		int b;
		for (b = 0; b < 12; b++)
		{
			double bx = L4_PLAYER_SCREEN_X - 10.0 - (rand() % 30);
			double by = gL4PlayerY + 25.0 + (rand() % 24 - 12);
			l4SpawnBubble(bx, by, -120.0 - (rand() % 80), 20.0 + (rand() % 30), 8.0 + (rand() % 10), 0.6 + (rand() % 40) / 100.0);
		}
	}

	// Horizontal Pace & Speed
	double speedMult = 1.0;
	if (gL4FinsTime > 0.0)
	{
		speedMult = L4_FINS_MULT;
		if (moveFwd) speedMult *= 1.15;
	}
	else if (moveFwd && gL4Stamina > 0.0)
	{
		speedMult = L4_SPRINT_MULT;
		gL4Stamina -= L4_STAMINA_SPRINT_DRAIN * dt;
		if (gL4Stamina < 0.0) gL4Stamina = 0.0;
	}
	else if (moveBack)
	{
		speedMult = L4_BRAKE_MULT;
		gL4Stamina += L4_STAMINA_RECOVER * dt;
		if (gL4Stamina > L4_STAMINA_MAX) gL4Stamina = L4_STAMINA_MAX;
	}
	else
	{
		// Cruising
		gL4Stamina -= L4_STAMINA_CRUISE_DRAIN * dt;
		if (gL4Stamina < 0.0) gL4Stamina = 0.0;
	}

	if (gL4Stamina <= 0.0 && gL4FinsTime <= 0.0)
	{
		speedMult = L4_TIRED_MULT;
	}

	double surgeAdd = (gL4SurgeTimer > 0.0) ? (L4_SURGE_BOOST * (gL4SurgeTimer / 1.2)) : 0.0;
	double targetSpeed = (L4_BASE_SPEED * speedMult) + surgeAdd;

	gL4Speed += (targetSpeed - gL4Speed) * (1.0 - exp(-dt * 8.0));
	gL4PlayerX += gL4Speed * dt;

	// Vertical Swimming Movement & Buoyancy (Smooth, responsive, and natural)
	double vertSpeed = L4_SWIM_VERT_SPEED;
	if (gL4FinsTime > 0.0) vertSpeed *= 1.25;

	double targetVY = 0.0;
	if (moveUp && !moveDown)
	{
		targetVY = vertSpeed;
	}
	else if (moveDown && !moveUp)
	{
		targetVY = -vertSpeed;
	}
	else
	{
		// Gentle natural neutral buoyancy drift when not steering
		targetVY = L4_BUOYANCY * 0.35;
	}

	// Playable water column bounds with realistic surface waterline
	double maxSurfaceY = L4_WATER_SURFACE_Y - 46.0; // Swimmer upper body crests through surface (Y ~ 424)
	double minSeabedY  = L4_SEABED_Y + 15.0;        // Above seabed rocks (Y ~ 60)

	// Silky smooth buoyant deceleration cushioning near boundaries
	double distToSurface = maxSurfaceY - gL4PlayerY;
	if (distToSurface < 50.0 && targetVY > 0.0)
	{
		double t = distToSurface / 50.0;
		if (t < 0.0) t = 0.0;
		targetVY *= (0.15 + 0.85 * t * t);
	}
	double distToSeabed = gL4PlayerY - minSeabedY;
	if (distToSeabed < 40.0 && targetVY < 0.0)
	{
		double t = distToSeabed / 40.0;
		if (t < 0.0) t = 0.0;
		targetVY *= (0.15 + 0.85 * t * t);
	}

	double smoothRate = (moveUp || moveDown) ? 12.0 : 7.0;
	gL4PlayerVY += (targetVY - gL4PlayerVY) * (1.0 - exp(-dt * smoothRate));
	gL4PlayerY += gL4PlayerVY * dt;

	if (gL4PlayerY > maxSurfaceY)
	{
		gL4PlayerY = maxSurfaceY;
		if (gL4PlayerVY > 0.0) gL4PlayerVY = 0.0;
	}
	if (gL4PlayerY < minSeabedY)
	{
		gL4PlayerY = minSeabedY;
		if (gL4PlayerVY < 0.0) gL4PlayerVY = 0.0;
	}

	// Swimmer continuous body tilt smoothing based on vertical velocity
	double targetTilt = (gL4PlayerVY / vertSpeed) * 14.0;
	if (targetTilt > 14.0) targetTilt = 14.0;
	if (targetTilt < -14.0) targetTilt = -14.0;
	gL4Tilt += (targetTilt - gL4Tilt) * (1.0 - exp(-dt * 9.0));

	// Continuous water bubble emission from feet
	static double bubbleAccum = 0.0;
	bubbleAccum += dt * (gL4Speed / 80.0);
	if (bubbleAccum >= 0.12)
	{
		bubbleAccum = 0.0;
		double footX = L4_PLAYER_SCREEN_X - 6.0;
		double footY = gL4PlayerY + 48.0;
		l4SpawnBubble(footX, footY, -50.0 - (rand() % 40), 15.0 + (rand() % 20), 4.0 + (rand() % 6), 0.5 + (rand() % 30) / 100.0);
	}

	// Swimmer periodic underwater breathing exhalation bubbles
	static double breathTimer = 0.0;
	breathTimer += dt;
	if (breathTimer >= 1.7)
	{
		breathTimer = 0.0;
		if (gL4PlayerY < maxSurfaceY - 15.0)
		{
			int puffs = 3 + (rand() % 3);
			int k;
			for (k = 0; k < puffs; k++)
			{
				double bx = L4_PLAYER_SCREEN_X + 175.0 - (rand() % 16);
				double by = gL4PlayerY + 52.0 + (rand() % 12 - 6);
				double bvx = -45.0 - (rand() % 45); // drifts backward against swim speed
				double bvy = 28.0 + (rand() % 35);  // rises
				double bsz = 5.0 + (rand() % 6);
				l4SpawnBubble(bx, by, bvx, bvy, bsz, 1.1 + (rand() % 40) / 100.0);
			}
		}
	}

	// Surface breach splash when swimming right at water boundary
	if (gL4PlayerY >= maxSurfaceY - 12.0)
	{
		static double splashTimer = 0.0;
		splashTimer += dt;
		if (splashTimer >= 0.20)
		{
			splashTimer = 0.0;
			double splashX = L4_PLAYER_SCREEN_X + 110.0 + (rand() % 40 - 20);
			l4SpawnSplash(splashX, L4_WATER_SURFACE_Y + 2.0, 32.0, 0.45);
		}
	}

	// Update active bubbles (swimmer wake & breathing)
	int b;
	for (b = 0; b < L4_MAX_BUBBLES; b++)
	{
		if (gL4Bubbles[b].active)
		{
			gL4Bubbles[b].life -= dt;
			if (gL4Bubbles[b].life <= 0.0)
			{
				gL4Bubbles[b].active = 0;
			}
			else
			{
				gL4Bubbles[b].x += gL4Bubbles[b].vx * dt;
				gL4Bubbles[b].y += gL4Bubbles[b].vy * dt;
				// Wobble
				gL4Bubbles[b].x += sin(gL4Elapsed * 10.0 + b) * 0.5;

				// Surface pop
				if (gL4Bubbles[b].y >= L4_WATER_SURFACE_Y)
				{
					gL4Bubbles[b].active = 0;
					l4SpawnSplash(gL4Bubbles[b].x, L4_WATER_SURFACE_Y, 18.0, 0.35);
				}
			}
		}
	}

	// Update Seabed Vents & Emit Rising Vent Bubbles
	double viewLeft = gL4PlayerX - L4_PLAYER_SCREEN_X - 100.0;
	double viewRight = viewLeft + SCREEN_WIDTH + 200.0;
	int v;
	for (v = 0; v < gL4VentCount; v++)
	{
		struct L4Vent *vent = &gL4Vents[v];
		if (vent->worldX >= viewLeft && vent->worldX <= viewRight)
		{
			vent->timer -= dt;
			if (vent->timer <= 0.0)
			{
				vent->timer = vent->interval;
				l4SpawnVentBubble(vent->worldX, L4_SEABED_Y + 5.0, 5.0 + (rand() % 7), 5.5);
			}
		}
	}

	// Update Rising Vent Bubbles
	int vb;
	for (vb = 0; vb < L4_MAX_VENT_BUBBLES; vb++)
	{
		struct L4VentBubble *bub = &gL4VentBubbles[vb];
		if (!bub->active) continue;

		bub->life -= dt;
		if (bub->life <= 0.0)
		{
			bub->active = 0;
			continue;
		}

		// Buoyancy acceleration upward
		bub->vy += 38.0 * dt;
		bub->y += bub->vy * dt;

		// Surface burst & splash
		if (bub->y >= L4_WATER_SURFACE_Y)
		{
			bub->active = 0;
			double popSx = l4ScreenX(bub->worldX);
			l4SpawnSplash(popSx, L4_WATER_SURFACE_Y, 22.0, 0.40);
		}
	}

	// Update Surface Splash Rings
	int sr;
	for (sr = 0; sr < L4_MAX_SPLASHES; sr++)
	{
		struct L4SplashRing *s = &gL4Splashes[sr];
		if (!s->active) continue;

		s->life -= dt;
		if (s->life <= 0.0)
		{
			s->active = 0;
		}
		else
		{
			// Expands smoothly
			s->size += (s->maxSize - s->size) * (1.0 - exp(-dt * 7.0));
			s->screenX -= gL4Speed * 0.4 * dt;
		}
	}

	// Update particles (marine snow & plankton)
	int p;
	for (p = 0; p < L4_MAX_PARTICLES; p++)
	{
		gL4Particles[p].x += gL4Particles[p].vx * dt;
		gL4Particles[p].y += gL4Particles[p].vy * dt;
		if (gL4Particles[p].x < -20.0)
		{
			gL4Particles[p].x = SCREEN_WIDTH + 20.0;
			gL4Particles[p].y = (double)(rand() % SCREEN_HEIGHT);
		}
		if (gL4Particles[p].y > SCREEN_HEIGHT + 20.0)
		{
			gL4Particles[p].y = -10.0;
			gL4Particles[p].x = (double)(rand() % SCREEN_WIDTH);
		}
	}

	// Update Background Marine Life (schools of fish swimming at depth)
	int f;
	for (f = 0; f < L4_MAX_FISH_SCHOOLS; f++)
	{
		struct L4FishSchool *fs = &gL4FishSchools[f];
		if (!fs->active) continue;
		fs->x -= (fs->speedX + gL4Speed * 0.28) * dt;
		fs->phase += dt * 2.6;
		if (fs->x < -380.0)
		{
			fs->x = SCREEN_WIDTH + 80.0 + (rand() % 240);
			fs->y = 80.0 + (rand() % 280);
		}
	}

	// Update Pelagic Manta Ray Glider
	if (gL4MantaRay.active)
	{
		gL4MantaRay.x -= (gL4MantaRay.speedX + gL4Speed * 0.18) * dt;
		gL4MantaRay.phase += dt * 1.8;
		gL4MantaRay.y = 250.0 + sin(gL4MantaRay.phase * 0.5) * 35.0;

		if (gL4MantaRay.x < -420.0)
		{
			gL4MantaRay.x = SCREEN_WIDTH + 180.0 + (rand() % 350);
			gL4MantaRay.y = 170.0 + (rand() % 170);
			gL4MantaRay.scale = 0.58 + (rand() % 28) / 100.0;
			gL4MantaRay.alpha = 0.48f + (rand() % 25) / 100.0;
		}
	}

	// Oxygen / Breath System
	if (gL4PlayerY >= 350.0)
	{
		// Near or at the surface: breathing oxygen naturally
		gL4Oxygen += L4_OXYGEN_SURFACE_REFILL * dt;
		if (gL4Oxygen > L4_OXYGEN_MAX) gL4Oxygen = L4_OXYGEN_MAX;
	}
	else
	{
		// Submerged deep: oxygen depletes steadily
		gL4Oxygen -= L4_OXYGEN_DEEP_DRAIN * dt;
		if (gL4Oxygen <= 0.0)
		{
			gL4Oxygen = 0.0;
			// Asphyxiation health damage!
			gL4Hp -= L4_ASPHYXIA_DAMAGE * dt;
			gL4ScreenShake = 0.15;
			if (gL4Hp <= 0.0)
			{
				gL4Hp = 0.0;
				gL4State = L4_STATE_LOST;
				gL4LoseReason = L4_LOSE_ASPHYXIA;
				audioPlayGameOver();
				profileUpdateLevelScore(4, gL4Score, 0, 0);
				return;
			}
		}
	}

	// Distance Scoring
	double dDist = gL4PlayerX - gL4DistanceScored;
	if (dDist >= 10.0)
	{
		int pts = (int)(dDist / 10.0) * L4_SCORE_PER_10PX;
		gL4Score += pts;
		gL4DistanceScored += (int)(dDist / 10.0) * 10.0;
	}

	// Biome Milestones
	int curBiome = l4GetCurrentBiome(gL4PlayerX);
	if (!gL4WaypointPassed[curBiome])
	{
		gL4WaypointPassed[curBiome] = 1;
		gL4ActiveWaypoint = curBiome;
		gL4WaypointTimer = 3.5;
	}

	// Smooth continuous swimming stroke cadence controller
	{
		double strokeRate = 1.15; // baseline cycles per second (~1.1s per full 7-frame stroke)
		if (gL4State == L4_STATE_READY)
		{
			strokeRate = 0.45; // gentle idle treading / floating
		}
		else if (gL4FinsTime > 0.0)
		{
			strokeRate = 1.85; // high-speed fin surge
		}
		else if (moveFwd && gL4Stamina > 0.0)
		{
			strokeRate = 1.60; // vigorous sprint
		}
		else if (moveBack)
		{
			strokeRate = 0.55; // relaxed glide recovery
		}
		else if (gL4Stamina <= 0.0)
		{
			strokeRate = 0.70; // tired stroke
		}

		if (moveUp || moveDown)
		{
			strokeRate += 0.35; // active vertical propulsion effort
		}

		gL4StrokeAccum += strokeRate * dt;
		gL4Frame = (int)(fmod(gL4StrokeAccum * 7.0, 7.0));
		if (gL4Frame < 0) gL4Frame = 0;
	}

	// Player Hitbox
	double px = L4_PLAYER_SCREEN_X + 42.0;
	double py = gL4PlayerY + 24.0;
	double pw = L4_PLAYER_HIT_W;
	double ph = L4_PLAYER_HIT_H;

	// Update Dynamic Obstacles & Collision
	int i;
	for (i = 0; i < gL4ObstacleCount; i++)
	{
		struct L4Obstacle *o = &gL4Obstacles[i];

		// Dynamic animation/movement per obstacle kind
		if (o->type == L4_OB_JELLYFISH)
		{
			o->phase += dt * 2.8;
			o->y = o->baseY + sin(o->phase) * 35.0;
		}
		else if (o->type == L4_OB_SHARK)
		{
			o->x += o->speedX * dt; // cruises against swimmer
			o->phase += dt * 4.0;
			o->y = o->baseY + sin(o->phase) * 12.0;
		}
		else if (o->type == L4_OB_WHIRLPOOL)
		{
			o->phase += dt * 5.0; // rotation phase
			// Whirlpool suction force when swimmer is nearby!
			double sx = l4ScreenX(o->x);
			double distToPlayer = sqrt((sx - L4_PLAYER_SCREEN_X) * (sx - L4_PLAYER_SCREEN_X) + (o->y - gL4PlayerY) * (o->y - gL4PlayerY));
			if (distToPlayer < 140.0)
			{
				double pullDir = (o->y > gL4PlayerY) ? 1.0 : -1.0;
				gL4PlayerY += pullDir * 60.0 * dt;
			}
		}

		double osx = l4ScreenX(o->x);
		if (osx + o->w < -100 || osx > SCREEN_WIDTH + 100) continue;

		// Collision check with player
		if (!o->hit && gL4InvulnTime <= 0.0)
		{
			// Tight hitbox inside obstacle
			double ox = osx + o->w * 0.15;
			double oy = o->y + o->h * 0.15;
			double ow = o->w * 0.70;
			double oh = o->h * 0.70;

			if (l4Overlap(px, py, pw, ph, ox, oy, ow, oh))
			{
				if (gL4ShieldTime > 0.0)
				{
					// Shield deflects obstacle
					o->hit = 1;
					gL4Score += 100;
				}
				else
				{
					// Player hit
					o->hit = 1;
					gL4Hp -= L4_HIT_DAMAGE;
					gL4InvulnTime = L4_HIT_INVULN_SECONDS;
					gL4ScreenShake = 0.28;
					gL4Combo = 1;
					audioPlayCollision();

					if (gL4Hp <= 0.0)
					{
						gL4Hp = 0.0;
						gL4State = L4_STATE_LOST;
						gL4LoseReason = L4_LOSE_HEALTH;
						audioPlayGameOver();
						profileUpdateLevelScore(4, gL4Score, 0, 0);
						return;
					}
				}
			}
		}
	}

	// Pickups Collection
	for (i = 0; i < gL4PickupCount; i++)
	{
		struct L4Pickup *pk = &gL4Pickups[i];
		if (pk->taken) continue;

		double psx = l4ScreenX(pk->x);
		if (psx + pk->w < -50 || psx > SCREEN_WIDTH + 50) continue;

		if (l4Overlap(px, py, pw, ph, psx, pk->y, pk->w, pk->h))
		{
			pk->taken = 1;
			gL4Score += L4_SCORE_PICKUP * gL4Combo;
			gL4Combo = (gL4Combo < 4) ? (gL4Combo + 1) : 4;
			gL4ComboTimer = 5.0;

			if (pk->type == L4_PU_OXYGEN)
			{
				gL4Oxygen += L4_OXYGEN_TANK_REFILL;
				if (gL4Oxygen > L4_OXYGEN_MAX) gL4Oxygen = L4_OXYGEN_MAX;
				gL4Hp += 5.0;
				if (gL4Hp > L4_HP_MAX) gL4Hp = L4_HP_MAX;
			}
			else if (pk->type == L4_PU_ENERGY)
			{
				gL4Stamina += 45.0;
				if (gL4Stamina > L4_STAMINA_MAX) gL4Stamina = L4_STAMINA_MAX;
				gL4Hp += 8.0;
				if (gL4Hp > L4_HP_MAX) gL4Hp = L4_HP_MAX;
			}
			else if (pk->type == L4_PU_FINS)
			{
				gL4FinsTime = L4_FINS_SECONDS;
			}
			else if (pk->type == L4_PU_SHIELD)
			{
				gL4ShieldTime = L4_SHIELD_SECONDS;
			}
			else if (pk->type == L4_PU_COIN)
			{
				gL4Hp += 3.0;
				if (gL4Hp > L4_HP_MAX) gL4Hp = L4_HP_MAX;
				gL4Score += L4_SCORE_COIN;
			}
			else if (pk->type == L4_PU_HEAL)
			{
				gL4Hp += L4_HEAL_AMOUNT;
				if (gL4Hp > L4_HP_MAX) gL4Hp = L4_HP_MAX;
				gL4Stamina += 20.0;
				if (gL4Stamina > L4_STAMINA_MAX) gL4Stamina = L4_STAMINA_MAX;
				gL4Oxygen += 15.0;
				if (gL4Oxygen > L4_OXYGEN_MAX) gL4Oxygen = L4_OXYGEN_MAX;
			}
		}
	}

	// Win Check
	if (gL4PlayerX >= L4_LENGTH)
	{
		gL4PlayerX = L4_LENGTH;
		gL4State = L4_STATE_WON;

		double timeRemaining = L4_TIME_LIMIT - gL4Elapsed;
		if (timeRemaining > 0.0)
		{
			gL4Score += (long)(timeRemaining * L4_SCORE_TIME_BONUS);
		}

		if (gL4Elapsed <= 58.0)      gL4Medal = 3; // Gold
		else if (gL4Elapsed <= 72.0) gL4Medal = 2; // Silver
		else                         gL4Medal = 1; // Bronze

		markLevelCompleted(4);
		profileUpdateLevelScore(4, gL4Score, gL4Medal, 1);
	}
}

#endif
