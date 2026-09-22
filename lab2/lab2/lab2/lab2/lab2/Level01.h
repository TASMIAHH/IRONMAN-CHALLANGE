//======================================================================
//  Level01.h
//  LEVEL 01 - RUNNING TRAINING
//
//  A 2D side-scrolling endurance run. The player is pinned to a fixed
//  screen position and the world scrolls past from right to left. The
//  only movement verb is the jump; D / RIGHT pushes the pace (costing
//  stamina) and A / LEFT eases off so a jump can be timed.
//
//  Everything level-specific lives in this one header so the menu and
//  level-select code stay untouched. It is included once, from
//  iMain.cpp, after GameUI.h / GameAssets.h.
//
//  TIMING
//  iGraphics calls fixedUpdate() from a Win32 SetTimer. WM_TIMER is the
//  lowest-priority message in the queue and glutIdleFunc() keeps that
//  queue busy, so the tick actually lands nearer 40 Hz than the 62 Hz
//  the 16 ms period suggests - and the rate differs from machine to
//  machine. Every rate below is therefore expressed per SECOND and
//  scaled by the measured delta time, so the run plays at the same
//  speed everywhere and the HUD clock counts real seconds.
//
//  dt is measured with QueryPerformanceCounter rather than
//  GetTickCount(). GetTickCount() only advances in ~10-16 ms steps on
//  most Windows systems, so dt was being quantized into a handful of
//  discrete values (0, 15, 16, 31 ms, ...) instead of varying smoothly.
//  Since every motion in the level - the player's world x, the run
//  animation, obstacles - is scaled directly by dt, that quantization
//  showed up as visible stutter. QueryPerformanceCounter has
//  microsecond resolution, so dt (and therefore the motion it drives)
//  is smooth.
//======================================================================
#ifndef LEVEL01_H
#define LEVEL01_H

//----------------------------------------------------------------------
// Asset paths & Biomes
//----------------------------------------------------------------------
#define L1_PATH_BG_COAST       "Assets/level01_bg_coast.png"
#define L1_PATH_BG_FOREST      "Assets/level01_bg_forest.png"
#define L1_PATH_BG_MOUNTAIN    "Assets/level01_bg_mountain.png"
#define L1_PATH_BG_SUNSET      "Assets/level01_bg_sunset.png"

#define L1_PATH_SKY            "Assets/level01_sky.png"
#define L1_PATH_GROUND         "Assets/level01_ground.png"
#define L1_PATH_START          "Assets/start_line.png"
#define L1_PATH_FINISH         "Assets/finish_line.png"

// Biome IDs
#define L1_BIOME_COAST         0
#define L1_BIOME_FOREST        1
#define L1_BIOME_MOUNTAIN      2
#define L1_BIOME_SUNSET        3
#define L1_BIOME_COUNT         4

#define L1_BIOME_0_END     11000.0
#define L1_BIOME_1_END     22000.0
#define L1_BIOME_2_END     33000.0
#define L1_BIOME_3_END     44000.0

//----------------------------------------------------------------------
// Geometry
//----------------------------------------------------------------------
#define L1_GROUND_Y             96      // the line the runner's feet sit on
#define L1_PLAYER_SCREEN_X     150      // fixed: the player never slides around
#define L1_PLAYER_W             91
#define L1_PLAYER_H            130
#define L1_PLAYER_HIT_W         44      // forgiving hitbox, narrower than the art
#define L1_PLAYER_HIT_H        116

//----------------------------------------------------------------------
// Motion, all per second
//----------------------------------------------------------------------
#define L1_GRAVITY            1730.0    // px/s^2
#define L1_JUMP_VELOCITY       744.0    // px/s  -> apex ~160 px, airtime ~0.86 s
#define L1_BASE_SPEED          305.0    // px/s

#define L1_SPRINT_MULT           1.48
#define L1_BRAKE_MULT            0.60
#define L1_TIRED_MULT            0.65   // out of stamina
#define L1_BOOTS_MULT            1.35

#define L1_MAX_DELTA             0.10   // clamp, so a stall cannot tunnel

//----------------------------------------------------------------------
// Course and survival
//----------------------------------------------------------------------
#define L1_LENGTH            44000      // world px -> 4 full running biomes
#define L1_TIME_LIMIT          180      // seconds (3:00)

#define L1_HP_MAX              150
#define L1_STAMINA_MAX         150
#define L1_STAMINA_RUN_DRAIN     0.9    // per second, cruising (gentle)
#define L1_STAMINA_SPRINT_DRAIN  9.5    // per second, sprinting
#define L1_STAMINA_RECOVER      22.0    // per second, easing off / braking (fast recharge)
#define L1_ENERGY_REFILL        60

#define L1_HIT_DAMAGE           12      // forgiving damage: survive 12+ hits
#define L1_HIT_INVULN_SECONDS    1.6    // generous mercy window after a hit
#define L1_SHIELD_SECONDS       11.0
#define L1_BOOTS_SECONDS        11.0

#define L1_MOVING_PHASE_RATE     2.2    // rad/s, vertical bob
#define L1_MOVING_CLOSE_RATE    56.0    // px/s of extra closing speed
#define L1_MOVING_WAKE_RANGE  1600.0    // px: only animate once in range

#define L1_READY_SECONDS         2.0

//----------------------------------------------------------------------
// Scoring
//----------------------------------------------------------------------
#define L1_SCORE_PER_100PX       10
#define L1_SCORE_PER_10PX         1
#define L1_SCORE_OBSTACLE       100
#define L1_SCORE_PICKUP         100
#define L1_SCORE_TIME_BONUS      25     // points per second left on clock

//----------------------------------------------------------------------
// Background layers
//----------------------------------------------------------------------
#define L1_BG_TILE_W          1024
#define L1_BG_SPLIT_Y          151
#define L1_SKY_PARALLAX          0.30
#define L1_ANIM_DISTANCE        26      // world px between run frames

//----------------------------------------------------------------------
// Level state
//----------------------------------------------------------------------
#define L1_STATE_READY    0
#define L1_STATE_RUNNING  1
#define L1_STATE_PAUSED   2
#define L1_STATE_WON      3
#define L1_STATE_LOST     4

#define L1_LOSE_NONE      0
#define L1_LOSE_HEALTH    1
#define L1_LOSE_TIME      2

//----------------------------------------------------------------------
// Entity kinds: Unique, relevant obstacles for EACH running track
//----------------------------------------------------------------------
// Track 0: Sunrise Coastal Promenade
#define L1_OB_COAST_DRIFTWOOD    0
#define L1_OB_COAST_SANDBAGS     1
#define L1_OB_COAST_PUDDLE       2

// Track 1: Sunlit Redwood Forest Trail
#define L1_OB_FOREST_LOG         3
#define L1_OB_FOREST_BOULDER     4
#define L1_OB_FOREST_MUD         5

// Track 2: Alpine Mountain Valley Route
#define L1_OB_MOUNTAIN_ROCK      6
#define L1_OB_MOUNTAIN_BARRIER   7
#define L1_OB_MOUNTAIN_CREVASSE  8

// Track 3: Sunset Marathon Boulevard
#define L1_OB_MARATHON_HURDLE    9
#define L1_OB_MARATHON_TIRES    10
#define L1_OB_MARATHON_BLOCK    11
#define L1_OB_MARATHON_MOVING   12

#define L1_OB_KINDS             13

// Power-ups present across EVERY track
#define PU_ENERGY   0
#define PU_SHIELD   1
#define PU_BOOTS    2
#define PU_COIN     3
#define PU_KINDS    4

#define L1_MAX_OBSTACLES  300
#define L1_MAX_PICKUPS    250

struct L1Obstacle
{
	int    type;
	double x;          // world x of the left edge
	double y;          // current bottom edge (moving ones travel)
	double baseY;
	double phase;      // oscillation phase for OB_MOVING
	int    w, h;
	int    hit;        // already damaged the player
	int    scored;     // already paid out its "cleared" points
};

struct L1Pickup
{
	int    type;
	double x, y;
	int    w, h;
	int    taken;
};

//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Textures
//----------------------------------------------------------------------
unsigned int gL1TexRun[8];
unsigned int gL1TexObstacle[L1_OB_KINDS];
unsigned int gL1TexPickup[PU_KINDS];
unsigned int gL1TexBg[L1_BIOME_COUNT];
unsigned int gL1TexSky, gL1TexGround, gL1TexStart, gL1TexFinish;
unsigned int gL1TexSun, gL1TexShieldBubble, gL1TexPitHazard, gL1TexMedal;

//----------------------------------------------------------------------
// Ambient particles & Runner footstep dust
//----------------------------------------------------------------------
#define L1_MAX_PARTICLES 65
struct L1Particle
{
	double x, y;
	double vx, vy;
	double size;
	double alpha;
	double phase;
	int    type;
};
struct L1Particle gL1Particles[L1_MAX_PARTICLES];

#define L1_MAX_DUST 30
struct L1DustParticle
{
	double x, y;
	double vx, vy;
	double life;
	double maxLife;
	double size;
};
struct L1DustParticle gL1Dust[L1_MAX_DUST];

// Biome announcement banner
int    gL1CurrentBiome;
char   gL1BiomeBannerText[64];
double gL1BiomeBannerTimer;

//----------------------------------------------------------------------
// Runtime state
//----------------------------------------------------------------------
int    gL1State;
int    gL1LoseReason;

double gL1PlayerX;          // world x of the player's draw quad
double gL1PlayerY;          // height above the ground line
double gL1PlayerVY;
int    gL1OnGround;

double gL1Hp;
double gL1Stamina;
long   gL1Score;
double gL1DistanceScored;   // world x already converted into points

double gL1Elapsed;          // seconds spent running
double gL1ReadyTime;        // seconds spent on the GET READY card
double gL1InvulnTime;       // seconds of mercy left
double gL1ShieldTime;       // seconds of shield left
double gL1BootsTime;        // seconds of jet boots left

// High-resolution timing (QueryPerformanceCounter) instead of
// GetTickCount(). See header note: GetTickCount()'s ~10-16 ms
// resolution was quantizing dt and causing visible stutter in
// every motion the level drives, including the run animation.
LARGE_INTEGER gL1Freq;         // counts per second, queried once at start
LARGE_INTEGER gL1LastCounter;  // QueryPerformanceCounter at the previous update

double gL1AnimDistance;     // no longer used to drive gL1Frame (see level01Update);
// kept declared in case anything else references it
int    gL1Frame;
double gL1Speed;            // this frame's world speed

struct L1Obstacle gL1Obstacles[L1_MAX_OBSTACLES];
int    gL1ObstacleCount;
struct L1Pickup   gL1Pickups[L1_MAX_PICKUPS];
int    gL1PickupCount;

long   gL1ParScore;         // used to decide the medal
int    gL1Medal;            // 0 none, 1 bronze, 2 silver, 3 gold

//======================================================================
//  Asset loading (call once, after iInitialize)
//======================================================================
void level01LoadAssets()
{
	char path[64];
	int i;

	for (i = 0; i < 8; i++)
	{
		sprintf(path, "Assets/run_%02d.png", i + 1);
		assetLoadTexture(path, &gL1TexRun[i]);
	}

	// Track 0: Sunrise Coastal Promenade
	assetLoadTexture("Assets/obs4_driftwood.png",    &gL1TexObstacle[L1_OB_COAST_DRIFTWOOD]);
	assetLoadTexture("Assets/l1_ob_sandbags.png",    &gL1TexObstacle[L1_OB_COAST_SANDBAGS]);
	assetLoadTexture("Assets/l1_pit_hazard.png",     &gL1TexObstacle[L1_OB_COAST_PUDDLE]);

	// Track 1: Sunlit Redwood Forest Trail
	assetLoadTexture("Assets/obs3_log.png",          &gL1TexObstacle[L1_OB_FOREST_LOG]);
	assetLoadTexture("Assets/obs3_rock.png",         &gL1TexObstacle[L1_OB_FOREST_BOULDER]);
	assetLoadTexture("Assets/l1_pit_hazard.png",     &gL1TexObstacle[L1_OB_FOREST_MUD]);

	// Track 2: Alpine Mountain Valley Route
	assetLoadTexture("Assets/obs3_ice_rock.png",     &gL1TexObstacle[L1_OB_MOUNTAIN_ROCK]);
	assetLoadTexture("Assets/l1_ob_alpine_gate.png", &gL1TexObstacle[L1_OB_MOUNTAIN_BARRIER]);
	assetLoadTexture("Assets/l1_pit_hazard.png",     &gL1TexObstacle[L1_OB_MOUNTAIN_CREVASSE]);

	// Track 3: Sunset Marathon Boulevard
	assetLoadTexture("Assets/l1_ob_hurdle.png",      &gL1TexObstacle[L1_OB_MARATHON_HURDLE]);
	assetLoadTexture("Assets/obs_tire.png",          &gL1TexObstacle[L1_OB_MARATHON_TIRES]);
	assetLoadTexture("Assets/obs_block.png",         &gL1TexObstacle[L1_OB_MARATHON_BLOCK]);
	assetLoadTexture("Assets/obs_moving.png",        &gL1TexObstacle[L1_OB_MARATHON_MOVING]);

	// Power-ups present across EVERY track
	assetLoadTexture("Assets/pu_energy.png",         &gL1TexPickup[PU_ENERGY]);
	assetLoadTexture("Assets/pu_shield.png",         &gL1TexPickup[PU_SHIELD]);
	assetLoadTexture("Assets/pu_boots.png",          &gL1TexPickup[PU_BOOTS]);
	assetLoadTexture("Assets/pu3_coin.png",          &gL1TexPickup[PU_COIN]);

	// Photorealistic running biome backgrounds
	assetLoadTexture(L1_PATH_BG_COAST,    &gL1TexBg[L1_BIOME_COAST]);
	assetLoadTexture(L1_PATH_BG_FOREST,   &gL1TexBg[L1_BIOME_FOREST]);
	assetLoadTexture(L1_PATH_BG_MOUNTAIN, &gL1TexBg[L1_BIOME_MOUNTAIN]);
	assetLoadTexture(L1_PATH_BG_SUNSET,   &gL1TexBg[L1_BIOME_SUNSET]);

	assetLoadTexture(L1_PATH_SKY, &gL1TexSky);
	assetLoadTexture(L1_PATH_GROUND, &gL1TexGround);
	assetLoadTexture(L1_PATH_START, &gL1TexStart);
	assetLoadTexture(L1_PATH_FINISH, &gL1TexFinish);

	assetLoadTexture("Assets/l1_sun.png", &gL1TexSun);
	assetLoadTexture("Assets/l1_shield_bubble.png", &gL1TexShieldBubble);
	assetLoadTexture("Assets/l1_pit_hazard.png", &gL1TexPitHazard);
	assetLoadTexture("Assets/pu3_coin.png", &gL1TexMedal);
}

//======================================================================
//  Helpers
//======================================================================
int l1Overlap(double ax, double ay, double aw, double ah,
	double bx, double by, double bw, double bh)
{
	if (ax + aw <= bx || bx + bw <= ax) return 0;
	if (ay + ah <= by || by + bh <= ay) return 0;
	return 1;
}

// World x -> screen x. The player stays pinned at L1_PLAYER_SCREEN_X.
double l1ScreenX(double worldX)
{
	return worldX - gL1PlayerX + L1_PLAYER_SCREEN_X;
}

// Deterministic generator, so the course is identical on every run and
// the difficulty ramp can actually be tuned.
unsigned int gL1Seed;

int l1Rand(int n)
{
	gL1Seed = gL1Seed * 1103515245u + 12345u;
	if (n <= 0) return 0;
	return (int)((gL1Seed >> 16) % (unsigned int)n);
}

void l1ObstacleSize(int type, int *w, int *h)
{
	switch (type)
	{
	// Track 0: Sunrise Coastal Promenade
	case L1_OB_COAST_DRIFTWOOD:   *w = 110; *h = 48; break;
	case L1_OB_COAST_SANDBAGS:    *w = 112; *h = 52; break;
	case L1_OB_COAST_PUDDLE:      *w = 118; *h = 32; break;

	// Track 1: Sunlit Redwood Forest Trail
	case L1_OB_FOREST_LOG:        *w = 116; *h = 54; break;
	case L1_OB_FOREST_BOULDER:    *w = 74;  *h = 66; break;
	case L1_OB_FOREST_MUD:        *w = 120; *h = 32; break;

	// Track 2: Alpine Mountain Valley Route
	case L1_OB_MOUNTAIN_ROCK:     *w = 76;  *h = 68; break;
	case L1_OB_MOUNTAIN_BARRIER:  *w = 104; *h = 60; break;
	case L1_OB_MOUNTAIN_CREVASSE: *w = 124; *h = 34; break;

	// Track 3: Sunset Marathon Boulevard
	case L1_OB_MARATHON_HURDLE:   *w = 96;  *h = 68; break;
	case L1_OB_MARATHON_TIRES:    *w = 66;  *h = 64; break;
	case L1_OB_MARATHON_BLOCK:    *w = 72;  *h = 68; break;
	case L1_OB_MARATHON_MOVING:   *w = 80;  *h = 64; break;

	default:                      *w = 72;  *h = 64; break;
	}
}

void l1PickupSize(int type, int *w, int *h)
{
	switch (type)
	{
	case PU_ENERGY: *w = 28; *h = 60; break;
	case PU_SHIELD: *w = 42; *h = 48; break;
	case PU_BOOTS:  *w = 48; *h = 42; break;
	case PU_COIN:   *w = 38; *h = 38; break;
	default:        *w = 36; *h = 36; break;
	}
}

//======================================================================
//  Course generation: Track-specific obstacles + universal powerups
//======================================================================
void level01Generate()
{
	double x;
	int obstaclePoints = 0;
	int pickupPoints = 0;

	// Track-specific obstacle collections
	int bagCoast[3]    = { L1_OB_COAST_DRIFTWOOD, L1_OB_COAST_SANDBAGS, L1_OB_COAST_PUDDLE };
	int bagForest[3]   = { L1_OB_FOREST_LOG, L1_OB_FOREST_BOULDER, L1_OB_FOREST_MUD };
	int bagMountain[3] = { L1_OB_MOUNTAIN_ROCK, L1_OB_MOUNTAIN_BARRIER, L1_OB_MOUNTAIN_CREVASSE };
	int bagMarathon[4] = { L1_OB_MARATHON_HURDLE, L1_OB_MARATHON_TIRES, L1_OB_MARATHON_BLOCK, L1_OB_MARATHON_MOVING };

	int curCoast[3], curCoastCount = 0;
	int curForest[3], curForestCount = 0;
	int curMountain[3], curMountainCount = 0;
	int curMarathon[4], curMarathonCount = 0;

	// Universal Power-up bag: 2 energy : 1 shield : 1 boots : 2 coins = 6 items
	int pbag[6];
	int pbagCount = 0;

	gL1Seed = 20260830u;
	gL1ObstacleCount = 0;
	gL1PickupCount = 0;

	x = 1400.0;

	while (x < L1_LENGTH - 1400.0 && gL1ObstacleCount < L1_MAX_OBSTACLES)
	{
		double progress = x / (double)L1_LENGTH;      // 0 .. 1
		double spacing = 820.0 - 360.0 * progress;

		int type, w, h;
		int k, j, tmp;

		// Select obstacle specific to the current track biome
		if (x < L1_BIOME_0_END)
		{
			if (curCoastCount == 0)
			{
				for (k = 0; k < 3; k++) curCoast[k] = bagCoast[k];
				curCoastCount = 3;
				for (k = curCoastCount - 1; k > 0; k--)
				{
					j = l1Rand(k + 1);
					tmp = curCoast[k]; curCoast[k] = curCoast[j]; curCoast[j] = tmp;
				}
			}
			type = curCoast[--curCoastCount];
		}
		else if (x < L1_BIOME_1_END)
		{
			if (curForestCount == 0)
			{
				for (k = 0; k < 3; k++) curForest[k] = bagForest[k];
				curForestCount = 3;
				for (k = curForestCount - 1; k > 0; k--)
				{
					j = l1Rand(k + 1);
					tmp = curForest[k]; curForest[k] = curForest[j]; curForest[j] = tmp;
				}
			}
			type = curForest[--curForestCount];
		}
		else if (x < L1_BIOME_2_END)
		{
			if (curMountainCount == 0)
			{
				for (k = 0; k < 3; k++) curMountain[k] = bagMountain[k];
				curMountainCount = 3;
				for (k = curMountainCount - 1; k > 0; k--)
				{
					j = l1Rand(k + 1);
					tmp = curMountain[k]; curMountain[k] = curMountain[j]; curMountain[j] = tmp;
				}
			}
			type = curMountain[--curMountainCount];
		}
		else
		{
			if (curMarathonCount == 0)
			{
				for (k = 0; k < 4; k++) curMarathon[k] = bagMarathon[k];
				curMarathonCount = 4;
				for (k = curMarathonCount - 1; k > 0; k--)
				{
					j = l1Rand(k + 1);
					tmp = curMarathon[k]; curMarathon[k] = curMarathon[j]; curMarathon[j] = tmp;
				}
			}
			type = curMarathon[--curMarathonCount];
		}

		l1ObstacleSize(type, &w, &h);

		gL1Obstacles[gL1ObstacleCount].type = type;
		gL1Obstacles[gL1ObstacleCount].x = x;
		gL1Obstacles[gL1ObstacleCount].w = w;
		gL1Obstacles[gL1ObstacleCount].h = h;
		gL1Obstacles[gL1ObstacleCount].baseY = L1_GROUND_Y;
		gL1Obstacles[gL1ObstacleCount].y = L1_GROUND_Y;
		gL1Obstacles[gL1ObstacleCount].phase = l1Rand(628) / 100.0;
		gL1Obstacles[gL1ObstacleCount].hit = 0;
		gL1Obstacles[gL1ObstacleCount].scored = 0;
		gL1ObstacleCount++;
		obstaclePoints += L1_SCORE_OBSTACLE;

		// Power-up in the gap after this obstacle (80% chance across EVERY track)
		if (l1Rand(100) < 80 && gL1PickupCount < L1_MAX_PICKUPS)
		{
			int ptype;
			int pw, ph;

			if (pbagCount == 0)
			{
				pbag[0] = PU_ENERGY;  pbag[1] = PU_ENERGY;
				pbag[2] = PU_SHIELD;  pbag[3] = PU_BOOTS;
				pbag[4] = PU_COIN;    pbag[5] = PU_COIN;
				pbagCount = 6;

				for (k = pbagCount - 1; k > 0; k--)
				{
					j = l1Rand(k + 1);
					tmp = pbag[k]; pbag[k] = pbag[j]; pbag[j] = tmp;
				}
			}

			ptype = pbag[--pbagCount];
			l1PickupSize(ptype, &pw, &ph);

			gL1Pickups[gL1PickupCount].type = ptype;
			gL1Pickups[gL1PickupCount].x = x + spacing * 0.55;
			// Alternating heights: some on ground, some require jumping
			gL1Pickups[gL1PickupCount].y = (l1Rand(100) < 50)
				? (L1_GROUND_Y + 8)
				: (L1_GROUND_Y + 90);
			gL1Pickups[gL1PickupCount].w = pw;
			gL1Pickups[gL1PickupCount].h = ph;
			gL1Pickups[gL1PickupCount].taken = 0;
			gL1PickupCount++;
			pickupPoints += L1_SCORE_PICKUP;
		}

		x += spacing + l1Rand(130);
	}

	// What a strong run is worth, used for the medal thresholds.
	gL1ParScore = obstaclePoints + pickupPoints
		+ (L1_LENGTH / 10) * L1_SCORE_PER_10PX;
}

//======================================================================
//  Entering / leaving the level
//======================================================================
void level01Start()
{
	level01Generate();

	gL1State = L1_STATE_READY;
	gL1LoseReason = L1_LOSE_NONE;

	gL1PlayerX = 0.0;
	gL1PlayerY = 0.0;
	gL1PlayerVY = 0.0;
	gL1OnGround = 1;

	gL1Hp = L1_HP_MAX;
	gL1Stamina = L1_STAMINA_MAX;
	gL1Score = 0;
	gL1DistanceScored = 0.0;

	gL1Elapsed = 0.0;
	gL1ReadyTime = 0.0;
	gL1InvulnTime = 0.0;
	gL1ShieldTime = 0.0;
	gL1BootsTime = 0.0;

	QueryPerformanceFrequency(&gL1Freq);
	QueryPerformanceCounter(&gL1LastCounter);

	gL1AnimDistance = 0.0;
	gL1Frame = 0;
	gL1Speed = 0.0;
	gL1Medal = 0;

	// Initialize ambient weather particles
	int p;
	for (p = 0; p < L1_MAX_PARTICLES; p++)
	{
		gL1Particles[p].x = (double)(p * 18 % SCREEN_WIDTH);
		gL1Particles[p].y = (double)(90 + (p * 37) % (SCREEN_HEIGHT - 120));
		gL1Particles[p].vx = -30.0 - (p % 5) * 16.0;
		gL1Particles[p].vy = -8.0 + (p % 7) * 4.0;
		gL1Particles[p].size = 4.0 + (p % 4) * 2.0;
		gL1Particles[p].alpha = 0.55 + (p % 5) * 0.09;
		gL1Particles[p].phase = (p * 50) % 628 / 100.0;
		gL1Particles[p].type = p % 4;
	}
	for (p = 0; p < L1_MAX_DUST; p++)
	{
		gL1Dust[p].life = 0.0;
	}

	gL1CurrentBiome = 0;
	strcpy(gL1BiomeBannerText, "ZONE 1: SUNRISE COASTAL PROMENADE");
	gL1BiomeBannerTimer = 3.5;

	audioPlayLevel01Theme();
}

void level01Exit()
{
	audioPlayBackgroundTheme();          // back to the menu/level-select track
	gCurrentScreen = SCREEN_LEVEL_SELECT;
}

//======================================================================
//  Finishing
//======================================================================
int l1SecondsLeft()
{
	int left = L1_TIME_LIMIT - (int)gL1Elapsed;
	return (left < 0) ? 0 : left;
}

void l1Win()
{
	int left = l1SecondsLeft();

	gL1Score += (long)left * L1_SCORE_TIME_BONUS;

	if (gL1Score >= (long)(gL1ParScore * 0.85))      gL1Medal = 3;   // gold
	else if (gL1Score >= (long)(gL1ParScore * 0.65)) gL1Medal = 2;   // silver
	else                                             gL1Medal = 1;   // bronze

	gL1State = L1_STATE_WON;

	markLevelCompleted(1);               // this is what unlocks Level 02
	audioStopAllMusic();
	profileUpdateLevelScore(1, gL1Score, gL1Medal, 1);
}

void l1Lose(int reason)
{
	gL1LoseReason = reason;
	gL1Medal = 0;
	gL1State = L1_STATE_LOST;

	audioStopAllMusic();
	audioPlayGameOver();
	profileUpdateLevelScore(1, gL1Score, 0, 0);
}

void l1TakeHit(struct L1Obstacle *o)
{
	o->hit = 1;

	audioPlayCollision();                // collision sound, every collision

	if (gL1ShieldTime > 0.0)
		return;                          // shielded: noise but no damage

	gL1Hp -= L1_HIT_DAMAGE;
	gL1InvulnTime = L1_HIT_INVULN_SECONDS;

	if (gL1Hp <= 0.0)
	{
		gL1Hp = 0.0;
		l1Lose(L1_LOSE_HEALTH);
	}
}

//======================================================================
//  Update. Called from fixedUpdate(); every rate is scaled by the real
//  elapsed time, so the pace does not depend on how often it fires.
//======================================================================
void level01Update(int jumpPressed, int pausePressed, int backPressed,
	int confirmPressed)
{
	LARGE_INTEGER now;
	double dt;
	int sprint, brake;
	double speed;
	double px, py;
	int i;

	// ---- delta time ---------------------------------------------------
	// QueryPerformanceCounter gives microsecond-scale resolution, unlike
	// GetTickCount()'s ~10-16 ms steps, so dt varies smoothly instead of
	// being quantized - this is what removes the stutter from every
	// motion in the level, including the run animation.
	QueryPerformanceCounter(&now);
	dt = (double)(now.QuadPart - gL1LastCounter.QuadPart) / (double)gL1Freq.QuadPart;
	gL1LastCounter = now;

	if (dt < 0.0) dt = 0.0;                     // counter wrapped/reset
	if (dt > L1_MAX_DELTA) dt = L1_MAX_DELTA;   // stalled or just resumed

	// ---- states that are not simulating -------------------------------
	if (gL1State == L1_STATE_WON || gL1State == L1_STATE_LOST)
	{
		if (confirmPressed) level01Start();      // retry
		else if (backPressed) level01Exit();
		return;
	}

	if (gL1State == L1_STATE_PAUSED)
	{
		if (pausePressed) gL1State = L1_STATE_RUNNING;
		else if (backPressed) level01Exit();
		return;
	}

	if (gL1State == L1_STATE_READY)
	{
		if (backPressed) { level01Exit(); return; }

		gL1ReadyTime += dt;
		if (gL1ReadyTime >= L1_READY_SECONDS)
			gL1State = L1_STATE_RUNNING;
		return;
	}

	// ---- running -------------------------------------------------------
	if (backPressed)  { level01Exit(); return; }
	if (pausePressed) { gL1State = L1_STATE_PAUSED; return; }

	gL1Elapsed += dt;

	// GLUT never reports Shift on its own, so the pace keys are D / A.
	sprint = isKeyPressed('d') || isKeyPressed('D') || isSpecialKeyPressed(GLUT_KEY_RIGHT);
	brake = isKeyPressed('a') || isKeyPressed('A') || isSpecialKeyPressed(GLUT_KEY_LEFT);

	// ---- speed and stamina ---------------------------------------------
	speed = L1_BASE_SPEED;

	if (gL1BootsTime > 0.0)
		speed *= L1_BOOTS_MULT;

	if (sprint && gL1Stamina > 0.0)
	{
		speed *= L1_SPRINT_MULT;
		gL1Stamina -= L1_STAMINA_SPRINT_DRAIN * dt;
	}
	else if (brake)
	{
		speed *= L1_BRAKE_MULT;
		// Tactical endurance recovery: easing off restores stamina
		gL1Stamina += L1_STAMINA_RECOVER * dt;
		if (gL1Stamina > L1_STAMINA_MAX) gL1Stamina = L1_STAMINA_MAX;
	}
	else
	{
		gL1Stamina -= L1_STAMINA_RUN_DRAIN * dt;
	}

	if (gL1Stamina <= 0.0)
	{
		gL1Stamina = 0.0;
		speed *= L1_TIRED_MULT;          // exhausted: the pace collapses
	}

	gL1Speed = speed;
	gL1PlayerX += speed * dt;

	// ---- biome progression & announcement banner ------------------------
	{
		int targetBiome = 0;
		if (gL1PlayerX >= L1_BIOME_2_END)      targetBiome = 3;
		else if (gL1PlayerX >= L1_BIOME_1_END) targetBiome = 2;
		else if (gL1PlayerX >= L1_BIOME_0_END) targetBiome = 1;

		if (targetBiome != gL1CurrentBiome)
		{
			gL1CurrentBiome = targetBiome;
			gL1BiomeBannerTimer = 3.6;
			if (targetBiome == 1)      strcpy(gL1BiomeBannerText, "ZONE 2: SUNLIT REDWOOD FOREST TRAIL");
			else if (targetBiome == 2) strcpy(gL1BiomeBannerText, "ZONE 3: ALPINE MOUNTAIN VALLEY");
			else if (targetBiome == 3) strcpy(gL1BiomeBannerText, "ZONE 4: SUNSET MARATHON BOULEVARD");
		}
		if (gL1BiomeBannerTimer > 0.0)
			gL1BiomeBannerTimer -= dt;
	}

	// ---- runner footstep dust puffs --------------------------------------
	if (gL1OnGround && speed > 100.0)
	{
		static double dustSpawnTimer = 0.0;
		dustSpawnTimer += dt;
		double interval = (sprint) ? 0.07 : 0.14;
		if (dustSpawnTimer >= interval)
		{
			dustSpawnTimer = 0.0;
			for (i = 0; i < L1_MAX_DUST; i++)
			{
				if (gL1Dust[i].life <= 0.0)
				{
					gL1Dust[i].x = L1_PLAYER_SCREEN_X + 16.0;
					gL1Dust[i].y = L1_GROUND_Y + 2.0;
					gL1Dust[i].vx = -speed * 0.30 - (rand() % 40);
					gL1Dust[i].vy = 10.0 + (rand() % 24);
					gL1Dust[i].maxLife = 0.35 + (rand() % 20) / 100.0;
					gL1Dust[i].life = gL1Dust[i].maxLife;
					gL1Dust[i].size = 5.0 + (rand() % 6);
					break;
				}
			}
		}
	}

	for (i = 0; i < L1_MAX_DUST; i++)
	{
		if (gL1Dust[i].life > 0.0)
		{
			gL1Dust[i].life -= dt;
			gL1Dust[i].x += gL1Dust[i].vx * dt;
			gL1Dust[i].y += gL1Dust[i].vy * dt;
			gL1Dust[i].size += 7.0 * dt;
		}
	}

	// ---- ambient weather particles simulation -----------------------------
	for (i = 0; i < L1_MAX_PARTICLES; i++)
	{
		gL1Particles[i].phase += dt * 2.2;
		gL1Particles[i].x += (gL1Particles[i].vx - speed * 0.18) * dt;
		gL1Particles[i].y += gL1Particles[i].vy * dt + sin(gL1Particles[i].phase) * 0.7;

		if (gL1Particles[i].x < -30.0)
		{
			gL1Particles[i].x = (double)SCREEN_WIDTH + 30.0;
			gL1Particles[i].y = 80.0 + (rand() % (SCREEN_HEIGHT - 120));
		}
		else if (gL1Particles[i].x > (double)SCREEN_WIDTH + 40.0)
		{
			gL1Particles[i].x = -20.0;
		}
		if (gL1Particles[i].y < 70.0) gL1Particles[i].y = (double)(SCREEN_HEIGHT - 20);
		else if (gL1Particles[i].y > (double)SCREEN_HEIGHT + 10.0) gL1Particles[i].y = 80.0;
	}

	// ---- run cycle ------------------------------------------------------
	// Frame index is derived directly from world position instead of
	// accumulated per-tick. An accumulator carries timing error forward,
	// so it makes it skip a frame on a long tick and barely advance on
	// the next - visible as a stutter. Deriving it straight from
	// gL1PlayerX means the frame shown is always exactly "correct" for
	// the current position, no matter how irregular the ticks were.
	{
		double cycleLen = L1_ANIM_DISTANCE * 8.0;
		double posInCycle = fmod(gL1PlayerX, cycleLen);
		if (posInCycle < 0.0) posInCycle += cycleLen;
		gL1Frame = (int)(posInCycle / L1_ANIM_DISTANCE) % 8;
	}

	// ---- jump physics (single jump, no double jump) ---------------------
	if (jumpPressed && gL1OnGround)
	{
		gL1PlayerVY = L1_JUMP_VELOCITY * ((gL1BootsTime > 0.0) ? 1.15 : 1.0);
		gL1OnGround = 0;
	}

	if (!gL1OnGround)
	{
		gL1PlayerY += gL1PlayerVY * dt;
		gL1PlayerVY -= L1_GRAVITY * dt;

		if (gL1PlayerY <= 0.0)
		{
			gL1PlayerY = 0.0;
			gL1PlayerVY = 0.0;
			gL1OnGround = 1;
		}
	}

	// ---- power-up timers -------------------------------------------------
	if (gL1InvulnTime > 0.0) gL1InvulnTime -= dt;
	if (gL1ShieldTime > 0.0) gL1ShieldTime -= dt;
	if (gL1BootsTime  > 0.0) gL1BootsTime -= dt;

	// ---- distance score --------------------------------------------------
	while (gL1PlayerX - gL1DistanceScored >= 10.0)
	{
		gL1DistanceScored += 10.0;
		gL1Score += L1_SCORE_PER_10PX;
	}

	// ---- player hitbox in world space -------------------------------------
	px = gL1PlayerX + (L1_PLAYER_W - L1_PLAYER_HIT_W) / 2.0;
	py = L1_GROUND_Y + gL1PlayerY;

	// ---- obstacles ---------------------------------------------------------
	for (i = 0; i < gL1ObstacleCount; i++)
	{
		struct L1Obstacle *o = &gL1Obstacles[i];

		// Only animate a moving obstacle once the player is near it.
		// Without this the extra closing speed would accumulate from the
		// moment the level starts, dragging far-off obstacles thousands
		// of pixels out of position and piling them onto each other.
		if (o->type == L1_OB_MARATHON_MOVING && o->x - px < L1_MOVING_WAKE_RANGE &&
			o->x + o->w > px - 200.0)
		{
			o->phase += L1_MOVING_PHASE_RATE * dt;
			o->y = o->baseY + 23.0 + 23.0 * sin(o->phase);
			o->x -= L1_MOVING_CLOSE_RATE * dt;   // closes on the player faster
		}

		if (!o->hit && gL1InvulnTime <= 0.0 &&
			l1Overlap(px, py, L1_PLAYER_HIT_W, L1_PLAYER_HIT_H,
			o->x, o->y, (double)o->w, (double)o->h))
		{
			l1TakeHit(o);
			if (gL1State == L1_STATE_LOST) return;
		}

		// cleared it without being hit
		if (!o->scored && o->x + o->w < px)
		{
			o->scored = 1;
			if (!o->hit)
				gL1Score += L1_SCORE_OBSTACLE;
		}
	}

	// ---- power-ups ----------------------------------------------------------
	for (i = 0; i < gL1PickupCount; i++)
	{
		struct L1Pickup *p = &gL1Pickups[i];

		if (p->taken) continue;

		if (l1Overlap(px, py, L1_PLAYER_HIT_W, L1_PLAYER_HIT_H,
			p->x - 6.0, p->y - 6.0, p->w + 12.0, p->h + 12.0))
		{
			p->taken = 1;
			gL1Score += L1_SCORE_PICKUP;

			if (p->type == PU_ENERGY)
			{
				gL1Stamina += L1_ENERGY_REFILL;
				if (gL1Stamina > L1_STAMINA_MAX) gL1Stamina = L1_STAMINA_MAX;
				gL1Hp += 20.0;
				if (gL1Hp > L1_HP_MAX) gL1Hp = L1_HP_MAX;
			}
			else if (p->type == PU_SHIELD)
			{
				gL1ShieldTime = L1_SHIELD_SECONDS;
			}
			else if (p->type == PU_BOOTS)
			{
				gL1BootsTime = L1_BOOTS_SECONDS;
			}
			else if (p->type == PU_COIN)
			{
				gL1Score += 250;
				gL1Hp += 8.0;
				if (gL1Hp > L1_HP_MAX) gL1Hp = L1_HP_MAX;
			}
		}
	}

	// ---- win / lose ---------------------------------------------------------
	if (gL1PlayerX >= (double)L1_LENGTH)
	{
		l1Win();
		return;
	}

	if (l1SecondsLeft() <= 0)
		l1Lose(L1_LOSE_TIME);
}

#endif