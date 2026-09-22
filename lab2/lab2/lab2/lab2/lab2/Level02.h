//======================================================================
//  Level02.h
//  LEVEL 02 - CLIMBING & PLATFORMING CHALLENGE
//
//  A vertical-scrolling climbing & platforming level. The player jumps
//  across moving and static stone blocks while scaling the mountain.
//  Full jumping physics and jump animations are integrated with
//  climbing and rope mechanics.
//======================================================================
#ifndef LEVEL02_H
#define LEVEL02_H

#include <math.h>

//----------------------------------------------------------------------
// Asset paths
//
// To change the Level 02 background, just replace the file at this
// path in the Assets folder with a new image of the same name - no
// code changes needed. Keep the filename identical (level02_background.png).
//----------------------------------------------------------------------
#define L2_PATH_BG      "Assets/level02_background.png"
#define L2_PATH_CHAR    "Assets/char_mountaineer.png"

////----------------------------------------------------------------------
// World geometry
//----------------------------------------------------------------------
#define L2_WORLD_HEIGHT      7200
#define L2_SUMMIT_Y          6200      // reach this height to win (accessible & fun!)
#define L2_PLAY_LEFT           30      // left edge of the full-screen playable area
#define L2_PLAY_RIGHT         994      // right edge of the full-screen playable area
#define L2_WALL_W               0      // no right-side wall

//----------------------------------------------------------------------
// Player geometry
//----------------------------------------------------------------------
#define L2_PLAYER_W            36
#define L2_PLAYER_H            56
#define L2_PLAYER_HIT_W        30
#define L2_PLAYER_HIT_H        50

//----------------------------------------------------------------------
// Motion (all per second, scaled by dt) - smooth, responsive & forgiving
//----------------------------------------------------------------------
#define L2_GRAVITY           1200.0   // floatier, easier jump trajectory
#define L2_JUMP_VEL           680.0   // generous jump height
#define L2_MOVE_SPEED         240.0   // responsive horizontal movement
#define L2_AIR_CONTROL          0.90   // strong mid-air steering to land on blocks
#define L2_CLIMB_SPEED        200.0
#define L2_CLIMB_DOWN_SPEED   110.0
#define L2_WALL_JUMP_VX       290.0
#define L2_WALL_JUMP_VY       540.0
#define L2_BOOTS_JUMP_MULT      1.30
#define L2_MAX_DELTA            0.10   // clamp to avoid tunnelling

//----------------------------------------------------------------------
// Survival - relaxed time limit, gentle hazards, safe falls
//----------------------------------------------------------------------
#define L2_TIME_LIMIT         270      // seconds (4:30 - plenty of time)
#define L2_HP_MAX             100.0
#define L2_STAMINA_MAX        100.0

#define L2_STAM_CLIMB_DRAIN     3.0   // climbing stamina drain requires ledge planning
#define L2_STAM_WALK_DRAIN      0.2
#define L2_STAM_RECOVER        30.0   // stamina recovery on solid ledges

#define L2_FALL_SAFE         1000.0   // 1000px safe fall distance
#define L2_FALL_DMG_RATE       0.02   // fall scratch

#define L2_HIT_DAMAGE           6.0   // hazard damage
#define L2_HIT_INVULN_SEC       1.5   // invulnerability window
#define L2_LAVA_DPS             8.0
#define L2_SHIELD_SECONDS       8.0
#define L2_BOOTS_SECONDS        8.0
#define L2_ENERGY_REFILL       45.0

//----------------------------------------------------------------------
// Scoring
//----------------------------------------------------------------------
#define L2_SCORE_PER_100PX     10
#define L2_SCORE_PICKUP       100
#define L2_SCORE_TIME_BONUS    25

//----------------------------------------------------------------------
// Entity limits
//----------------------------------------------------------------------
#define L2_MAX_PLATFORMS      260
#define L2_MAX_ROCKS           50
#define L2_MAX_PICKUPS        150

//----------------------------------------------------------------------
// Platform types
//----------------------------------------------------------------------
#define L2_PLAT_STATIC         0
#define L2_PLAT_MOVING         1
#define L2_PLAT_NARROW         2
#define L2_PLAT_DANGEROUS      3
#define L2_PLAT_CRUMBLING      4
#define L2_PLAT_TYPES          5

//----------------------------------------------------------------------
// Spinning gear obstacles
//----------------------------------------------------------------------
#define L2_MAX_GEARS       25
#define L2_GEAR_SIZE       28
#define L2_GEAR_DAMAGE      5.0
#define L2_GEAR_SPEED       2.0

//----------------------------------------------------------------------
// Winter Power-up types
//----------------------------------------------------------------------
#define PU_WINTER_HEAL         3
#define PU_WINTER_STAMINA      4

//----------------------------------------------------------------------
// Snow Trap Hazard
//----------------------------------------------------------------------
#define L2_MAX_SNOW_CHUNKS     20
#define L2_SNOW_CHUNK_SIZE     36
#define L2_SNOW_CHUNK_DAMAGE    8.0
#define L2_SNOW_SPAWN_BASE      7.0
#define L2_SNOW_SPAWN_MIN       4.0

//----------------------------------------------------------------------
// Player states
//----------------------------------------------------------------------
#define L2_PS_STAND            0
#define L2_PS_CLIMB            1
#define L2_PS_AIR              2
#define L2_PS_ROPE             3

//----------------------------------------------------------------------
// Mountain Rope Climbing (Dynamic, Fast & Forgiving)
//----------------------------------------------------------------------
#define L2_ROPE_LANES          3
#define L2_ROPE_X_LEFT       280.0
#define L2_ROPE_X_MID        512.0
#define L2_ROPE_X_RIGHT      744.0

#define L2_ROPE_DURATION     10.0
#define L2_ROPE_SPEED_START 240.0
#define L2_ROPE_SPEED_MAX   400.0

#define L2_ROPE_SEC1_START   1800.0
#define L2_ROPE_SEC1_END     2600.0
#define L2_ROPE_SEC2_START   4200.0
#define L2_ROPE_SEC2_END     5000.0

#define L2_RTRAP_ICICLE        0
#define L2_RTRAP_AVALANCHE     1
#define L2_RTRAP_FROSTWIND     2
#define L2_MAX_ROPE_TRAPS     10

struct L2RopeTrap
{
	int    type;
	int    lane;
	double x, y;
	double vy, vx;
	double size;
	double rot;
	int    active;
	int    hit;
};

//----------------------------------------------------------------------
// Level states
//----------------------------------------------------------------------
#define L2_STATE_READY         0
#define L2_STATE_CLIMBING      1
#define L2_STATE_PAUSED        2
#define L2_STATE_WON           3
#define L2_STATE_LOST          4

#define L2_LOSE_NONE           0
#define L2_LOSE_HEALTH         1
#define L2_LOSE_TIME           2

#define L2_READY_SECONDS       2.0

//----------------------------------------------------------------------
// Hazards & Moving blocks
//----------------------------------------------------------------------
#define L2_ROCK_SPAWN_BASE     6.0
#define L2_ROCK_SPAWN_MIN      3.5
#define L2_ROCK_FALL_MIN     140.0
#define L2_ROCK_FALL_MAX     260.0

#define L2_MOVING_AMP         60.0
#define L2_MOVING_RATE         1.2
#define L2_CRUMBLE_TIME        3.5

//----------------------------------------------------------------------
// Structs
//----------------------------------------------------------------------
struct L2Platform
{
	int    type;
	double x, y;
	double baseX;
	int    w, h;
	double phase;
	double crumbleTimer;
	int    active;
};

struct L2FallingRock
{
	double x, y;
	double vy;
	int    w, h;
	int    active;
	int    hit;
};

struct L2Pickup
{
	int    type;
	double x, y;
	int    w, h;
	int    taken;
};

struct L2Gear
{
	double x, y;
	int    size;
	double angle;
	double speed;
	int    active;
};

struct L2SnowChunk
{
	double x, y;
	double vx, vy;
	double size;
	double angle;
	double rotSpeed;
	int    active;
	int    hit;
};

//----------------------------------------------------------------------
// Textures
//----------------------------------------------------------------------
unsigned int gL2TexBg;          // background image (see L2_PATH_BG above)
unsigned int gL2TexChar;        // mountaineer character sprite (see L2_PATH_CHAR above)
unsigned int gL2TexPlatStone;   // Assets/l2_platform_stone.png
unsigned int gL2TexPlatIce;     // Assets/l2_platform_ice.png
unsigned int gL2TexPlatCrumble; // Assets/l2_platform_crumble.png
unsigned int gL2TexPlatTerrace; // Assets/l2_platform_terrace.png
unsigned int gL2TexFrostSpike;  // Assets/l2_frost_spike.png
unsigned int gL2TexFallingRock; // Assets/l2_falling_rock.png
unsigned int gL2TexSnowBoulder; // Assets/l2_snow_boulder.png
unsigned int gL2TexPuHeal;      // Assets/l2_pu_heal.png
unsigned int gL2TexPuStamina;   // Assets/l2_pu_stamina.png
unsigned int gL2TexPuShield;    // Assets/l2_pu_shield.png
unsigned int gL2TexPuBoots;     // Assets/l2_pu_boots.png
unsigned int gL2TexClimbRope;   // Assets/l2_climb_rope.png
unsigned int gL2TexRopeAnchor;  // Assets/l2_rope_anchor.png
unsigned int gL2TexSummitCairn; // Assets/l2_summit_cairn.png
unsigned int gL2TexIcicleTrap;  // Assets/l2_icicle_trap.png
unsigned int gL2TexMist;        // Assets/l2_mountain_mist.png
unsigned int gL2TexSnowParticle;// Assets/l2_snow_particle.png
unsigned int gL2TexShieldBubble;// Assets/l1_shield_bubble.png

//----------------------------------------------------------------------
// Runtime state
//----------------------------------------------------------------------
int    gL2State;
int    gL2LoseReason;

double gL2PlayerX;         // world x (left edge of draw quad)
double gL2PlayerY;         // world y (bottom edge)
double gL2PlayerVX;
double gL2PlayerVY;
int    gL2PlayerState;     // L2_PS_STAND / CLIMB / AIR
int    gL2ClimbWall;       // 0 = none, 1 = left wall, 2 = right wall
int    gL2FacingRight;

double gL2AnimTimer;
int    gL2RunFrame;
double gL2ClimbAnimTimer;
int    gL2ClimbFrame;

double gL2Hp;
double gL2Stamina;
long   gL2Score;
double gL2HeightScored;    // highest world-y already converted into points

double gL2Elapsed;
double gL2ReadyTime;
double gL2InvulnTime;
double gL2ShieldTime;
double gL2BootsTime;

double gL2HighestY;        // for fall-damage: peak height since last ground
int    gL2OnPlatIdx;       // index of platform the player is standing on (-1 = ground/none)

double gL2CameraY;         // world y of the bottom edge of the viewport

double gL2RockTimer;       // countdown to next rock spawn
double gL2SnowTrapTimer;   // countdown to next big snow chunk falling from sky

unsigned long gL2LastMs;   // GetTickCount at the previous update

struct L2Platform     gL2Platforms[L2_MAX_PLATFORMS];
int                   gL2PlatformCount;
struct L2FallingRock  gL2Rocks[L2_MAX_ROCKS];
struct L2Pickup       gL2Pickups[L2_MAX_PICKUPS];
int                   gL2PickupCount;

long   gL2ParScore;
int    gL2Medal;           // 0 none, 1 bronze, 2 silver, 3 gold

struct L2Gear         gL2Gears[L2_MAX_GEARS];
int                   gL2GearCount;
struct L2SnowChunk    gL2SnowChunks[L2_MAX_SNOW_CHUNKS];

// Mountain Rope Climbing Runtime State
int                   gL2RopeActive;
int                   gL2RopeSection;
int                   gL2RopeLane;
double                gL2RopePlayerX;
double                gL2RopeElapsed;
double                gL2RopeClimbSpeed;
double                gL2RopeTrapTimer;
double                gL2RopeBannerTimer;
double                gL2RopeStartY;
double                gL2RopeTargetY;
int                   gL2RopeSec1Done;
int                   gL2RopeSec2Done;
struct L2RopeTrap     gL2RopeTraps[L2_MAX_ROPE_TRAPS];

//======================================================================
//  Asset loading (call once, after iInitialize - same pattern as
//  level01LoadAssets()). To swap the background later, just replace
//  the image file at L2_PATH_BG in the Assets folder; nothing here
//  needs to change.
//======================================================================
void level02LoadAssets()
{
	assetLoadTexture(L2_PATH_BG, &gL2TexBg);
	assetLoadTexture(L2_PATH_CHAR, &gL2TexChar);
	assetLoadTexture("Assets/l2_platform_stone.png", &gL2TexPlatStone);
	assetLoadTexture("Assets/l2_platform_ice.png", &gL2TexPlatIce);
	assetLoadTexture("Assets/l2_platform_crumble.png", &gL2TexPlatCrumble);
	assetLoadTexture("Assets/l2_platform_terrace.png", &gL2TexPlatTerrace);
	assetLoadTexture("Assets/l2_frost_spike.png", &gL2TexFrostSpike);
	assetLoadTexture("Assets/l2_falling_rock.png", &gL2TexFallingRock);
	assetLoadTexture("Assets/l2_snow_boulder.png", &gL2TexSnowBoulder);
	assetLoadTexture("Assets/l2_pu_heal.png", &gL2TexPuHeal);
	assetLoadTexture("Assets/l2_pu_stamina.png", &gL2TexPuStamina);
	assetLoadTexture("Assets/l2_pu_shield.png", &gL2TexPuShield);
	assetLoadTexture("Assets/l2_pu_boots.png", &gL2TexPuBoots);
	assetLoadTexture("Assets/l2_climb_rope.png", &gL2TexClimbRope);
	assetLoadTexture("Assets/l2_rope_anchor.png", &gL2TexRopeAnchor);
	assetLoadTexture("Assets/l2_summit_cairn.png", &gL2TexSummitCairn);
	assetLoadTexture("Assets/l2_icicle_trap.png", &gL2TexIcicleTrap);
	assetLoadTexture("Assets/l2_mountain_mist.png", &gL2TexMist);
	assetLoadTexture("Assets/l2_snow_particle.png", &gL2TexSnowParticle);
	assetLoadTexture("Assets/l1_shield_bubble.png", &gL2TexShieldBubble);
}

//======================================================================
// Deterministic RNG
//======================================================================
unsigned int gL2Seed;

int l2Rand(int n)
{
	gL2Seed = gL2Seed * 1103515245u + 12345u;
	if (n <= 0) return 0;
	return (int)((gL2Seed >> 16) % (unsigned int)n);
}

int l2Overlap(double ax, double ay, double aw, double ah,
	double bx, double by, double bw, double bh)
{
	if (ax + aw <= bx || bx + bw <= ax) return 0;
	if (ay + ah <= by || by + bh <= ay) return 0;
	return 1;
}

double l2ScreenY(double worldY)
{
	return worldY - gL2CameraY;
}

//======================================================================
// Course generation (Jumpable block spacing)
//======================================================================
void level02Generate()
{
	double y;
	int i;

	gL2Seed = 20260901u;
	gL2PlatformCount = 0;
	gL2PickupCount = 0;
	gL2GearCount = 0;

	for (i = 0; i < L2_MAX_ROCKS; i++)
		gL2Rocks[i].active = 0;

	for (i = 0; i < L2_MAX_GEARS; i++)
		gL2Gears[i].active = 0;

	for (i = 0; i < L2_MAX_SNOW_CHUNKS; i++)
		gL2SnowChunks[i].active = 0;
	gL2SnowTrapTimer = 2.0;

	int pbag[6];
	int pbagCount = 0;

	y = 110.0;
	int sec1Placed = 0, sec2Placed = 0;

	while (y < (double)L2_SUMMIT_Y - 200.0 && gL2PlatformCount < L2_MAX_PLATFORMS - 4)
	{
		// Section 1 Sheer Mountain Cliff
		if (!sec1Placed && y >= 1750.0)
		{
			struct L2Platform *p = &gL2Platforms[gL2PlatformCount++];
			p->type = L2_PLAT_STATIC;
			p->w = 560; p->h = 24;
			p->x = (SCREEN_WIDTH - p->w) / 2.0;
			p->y = 1800.0;
			p->baseX = p->x;
			p->phase = 0.0;
			p->crumbleTimer = -1.0;
			p->active = 1;

			p = &gL2Platforms[gL2PlatformCount++];
			p->type = L2_PLAT_STATIC;
			p->w = 560; p->h = 24;
			p->x = (SCREEN_WIDTH - p->w) / 2.0;
			p->y = 2600.0;
			p->baseX = p->x;
			p->phase = 0.0;
			p->crumbleTimer = -1.0;
			p->active = 1;

			if (gL2PickupCount < L2_MAX_PICKUPS - 4)
			{
				struct L2Pickup *pu1 = &gL2Pickups[gL2PickupCount++];
				pu1->type = PU_WINTER_HEAL;
				pu1->w = 24; pu1->h = 24;
				pu1->x = p->x + 50.0; pu1->y = p->y + p->h + 6.0;
				pu1->taken = 0;

				struct L2Pickup *pu2 = &gL2Pickups[gL2PickupCount++];
				pu2->type = PU_WINTER_STAMINA;
				pu2->w = 24; pu2->h = 24;
				pu2->x = p->x + 130.0; pu2->y = p->y + p->h + 6.0;
				pu2->taken = 0;

				struct L2Pickup *pu3 = &gL2Pickups[gL2PickupCount++];
				pu3->type = PU_SHIELD;
				pu3->w = 24; pu3->h = 24;
				pu3->x = p->x + p->w - 150.0; pu3->y = p->y + p->h + 6.0;
				pu3->taken = 0;

				struct L2Pickup *pu4 = &gL2Pickups[gL2PickupCount++];
				pu4->type = PU_BOOTS;
				pu4->w = 24; pu4->h = 24;
				pu4->x = p->x + p->w - 70.0; pu4->y = p->y + p->h + 6.0;
				pu4->taken = 0;
			}

			sec1Placed = 1;
			y = 2690.0;
			continue;
		}

		// Section 2 Summit Headwall Sheer Cliff
		if (!sec2Placed && y >= 4150.0)
		{
			struct L2Platform *p = &gL2Platforms[gL2PlatformCount++];
			p->type = L2_PLAT_STATIC;
			p->w = 560; p->h = 24;
			p->x = (SCREEN_WIDTH - p->w) / 2.0;
			p->y = 4200.0;
			p->baseX = p->x;
			p->phase = 0.0;
			p->crumbleTimer = -1.0;
			p->active = 1;

			p = &gL2Platforms[gL2PlatformCount++];
			p->type = L2_PLAT_STATIC;
			p->w = 560; p->h = 24;
			p->x = (SCREEN_WIDTH - p->w) / 2.0;
			p->y = 5000.0;
			p->baseX = p->x;
			p->phase = 0.0;
			p->crumbleTimer = -1.0;
			p->active = 1;

			if (gL2PickupCount < L2_MAX_PICKUPS - 4)
			{
				struct L2Pickup *pu1 = &gL2Pickups[gL2PickupCount++];
				pu1->type = PU_WINTER_HEAL;
				pu1->w = 24; pu1->h = 24;
				pu1->x = p->x + 50.0; pu1->y = p->y + p->h + 6.0;
				pu1->taken = 0;

				struct L2Pickup *pu2 = &gL2Pickups[gL2PickupCount++];
				pu2->type = PU_WINTER_STAMINA;
				pu2->w = 24; pu2->h = 24;
				pu2->x = p->x + 130.0; pu2->y = p->y + p->h + 6.0;
				pu2->taken = 0;

				struct L2Pickup *pu3 = &gL2Pickups[gL2PickupCount++];
				pu3->type = PU_SHIELD;
				pu3->w = 24; pu3->h = 24;
				pu3->x = p->x + p->w - 150.0; pu3->y = p->y + p->h + 6.0;
				pu3->taken = 0;

				struct L2Pickup *pu4 = &gL2Pickups[gL2PickupCount++];
				pu4->type = PU_BOOTS;
				pu4->w = 24; pu4->h = 24;
				pu4->x = p->x + p->w - 70.0; pu4->y = p->y + p->h + 6.0;
				pu4->taken = 0;
			}

			sec2Placed = 1;
			y = 5090.0;
			continue;
		}

		double progress = y / (double)L2_SUMMIT_Y;
		int numPlats = 2, j;
		double spacing;

		for (j = 0; j < numPlats && gL2PlatformCount < L2_MAX_PLATFORMS; j++)
		{
			struct L2Platform *p = &gL2Platforms[gL2PlatformCount];
			int type, w;

			// Generously wide platforms (easy to jump & land)
			w = 160 + l2Rand(40);
			if (w < 140) w = 140;
			if (w > 220) w = 220;

			// Mostly solid, static platforms with a few gentle moving ones
			type = L2_PLAT_STATIC;
			if (l2Rand(100) < 25)
				type = L2_PLAT_MOVING;

			p->type = type;
			p->w = w;
			p->h = 20;

			int maxAvail = (L2_PLAY_RIGHT - 30) - (L2_PLAY_LEFT + 30) - w;
			if (maxAvail < 10) maxAvail = 10;
			if (j == 0)
				p->x = (double)(L2_PLAY_LEFT + 30 + l2Rand(maxAvail / 2));
			else
				p->x = (double)(L2_PLAY_LEFT + 30 + maxAvail / 2 + l2Rand(maxAvail / 2));

			if (type == L2_PLAT_MOVING)
			{
				if (p->x < L2_PLAY_LEFT + L2_MOVING_AMP + 10)
					p->x = L2_PLAY_LEFT + L2_MOVING_AMP + 10;
				if (p->x + w > L2_PLAY_RIGHT - L2_MOVING_AMP - 10)
					p->x = L2_PLAY_RIGHT - L2_MOVING_AMP - w - 10;
			}

			p->y = y;
			p->baseX = p->x;
			p->phase = l2Rand(628) / 100.0;
			p->crumbleTimer = -1.0;
			p->active = 1;
			gL2PlatformCount++;

			// Abundant Pickups (healing, stamina, shields & boots) - 75% chance
			if (l2Rand(100) < 75 && gL2PickupCount < L2_MAX_PICKUPS)
			{
				struct L2Pickup *pu = &gL2Pickups[gL2PickupCount];
				int ptype, k, tmp;

				if (pbagCount == 0)
				{
					pbag[0] = PU_WINTER_HEAL;
					pbag[1] = PU_WINTER_STAMINA;
					pbag[2] = PU_SHIELD;
					pbag[3] = PU_BOOTS;
					pbag[4] = PU_WINTER_HEAL;
					pbag[5] = PU_WINTER_STAMINA;
					pbagCount = 6;
					for (k = pbagCount - 1; k > 0; k--)
					{
						int r = l2Rand(k + 1);
						tmp = pbag[k]; pbag[k] = pbag[r]; pbag[r] = tmp;
					}
				}
				ptype = pbag[--pbagCount];

				pu->type = ptype;
				pu->w = 24; pu->h = 24;
				pu->x = p->x + (p->w > 40 ? 15 + l2Rand(p->w - 30) : 10);
				pu->y = p->y + p->h + 6;
				pu->taken = 0;
				gL2PickupCount++;
			}

			// Few, gentle gears
			if (progress > 0.40 && l2Rand(100) < 10
				&& gL2GearCount < L2_MAX_GEARS)
			{
				struct L2Gear *g = &gL2Gears[gL2GearCount];
				g->x = p->x + p->w / 2.0;
				g->y = p->y - 8.0;
				g->size = 12;
				g->angle = l2Rand(628) / 100.0;
				g->speed = (l2Rand(2) == 0 ? 1 : -1) * L2_GEAR_SPEED;
				g->active = 1;
				gL2GearCount++;
			}
		}

		// Gentle vertical step distance between blocks (effortless jumps)
		spacing = 68.0 + l2Rand(14);
		y += spacing;
	}

	gL2ParScore = (long)(L2_SUMMIT_Y / 100.0) * L2_SCORE_PER_100PX
		+ gL2PickupCount * L2_SCORE_PICKUP;
}

//======================================================================
// Start / Exit
//======================================================================
void level02Start()
{
	level02Generate();

	gL2State = L2_STATE_READY;
	gL2LoseReason = L2_LOSE_NONE;

	gL2PlayerX = (L2_PLAY_LEFT + L2_PLAY_RIGHT) / 2.0 - L2_PLAYER_W / 2.0;
	gL2PlayerY = 0.0;
	gL2PlayerVX = 0.0;
	gL2PlayerVY = 0.0;
	gL2PlayerState = L2_PS_STAND;
	gL2ClimbWall = 0;
	gL2FacingRight = 1;

	gL2AnimTimer = 0.0;
	gL2RunFrame = 0;
	gL2ClimbAnimTimer = 0.0;
	gL2ClimbFrame = 0;

	gL2Hp = L2_HP_MAX;
	gL2Stamina = L2_STAMINA_MAX;
	gL2Score = 0;
	gL2HeightScored = 0.0;
	gL2Elapsed = 0.0;
	gL2ReadyTime = 0.0;
	gL2InvulnTime = 0.0;
	gL2ShieldTime = 0.0;
	gL2BootsTime = 0.0;

	gL2HighestY = 0.0;
	gL2OnPlatIdx = -1;
	gL2CameraY = 0.0;
	gL2RockTimer = L2_ROCK_SPAWN_BASE;
	gL2LastMs = GetTickCount();
	gL2Medal = 0;

	gL2RopeActive = 0;
	gL2RopeSection = 0;
	gL2RopeLane = 1;
	gL2RopePlayerX = L2_ROPE_X_MID;
	gL2RopeElapsed = 0.0;
	gL2RopeClimbSpeed = L2_ROPE_SPEED_START;
	gL2RopeTrapTimer = 1.0;
	gL2RopeBannerTimer = 0.0;
	gL2RopeStartY = 0.0;
	gL2RopeTargetY = 0.0;
	gL2RopeSec1Done = 0;
	gL2RopeSec2Done = 0;
	{
		int k;
		for (k = 0; k < L2_MAX_ROPE_TRAPS; k++)
			gL2RopeTraps[k].active = 0;
	}

	audioPlayBackgroundTheme();
}

void level02Exit()
{
	audioPlayBackgroundTheme();
	gCurrentScreen = SCREEN_LEVEL_SELECT;
}

int l2SecondsLeft()
{
	int left = L2_TIME_LIMIT - (int)gL2Elapsed;
	return (left < 0) ? 0 : left;
}

void l2Win()
{
	int left = l2SecondsLeft();
	gL2Score += (long)left * L2_SCORE_TIME_BONUS;

	if (gL2Score >= (long)(gL2ParScore * 0.85))      gL2Medal = 3;
	else if (gL2Score >= (long)(gL2ParScore * 0.65))  gL2Medal = 2;
	else                                              gL2Medal = 1;

	gL2State = L2_STATE_WON;
	markLevelCompleted(2);
	audioStopAllMusic();
	profileUpdateLevelScore(2, gL2Score, gL2Medal, 1);
}

void l2Lose(int reason)
{
	gL2LoseReason = reason;
	gL2Medal = 0;
	gL2State = L2_STATE_LOST;
	audioStopAllMusic();
	audioPlayGameOver();
	profileUpdateLevelScore(2, gL2Score, 0, 0);
}

void l2TakeHit(double damage)
{
	if (gL2ShieldTime > 0.0)
	{
		audioPlayCollision();
		return;
	}
	gL2Hp -= damage;
	gL2InvulnTime = L2_HIT_INVULN_SEC;
	audioPlayCollision();

	if (gL2Hp <= 0.0)
	{
		gL2Hp = 0.0;
		l2Lose(L2_LOSE_HEALTH);
	}
}

void l2SpawnRock()
{
	int i;
	for (i = 0; i < L2_MAX_ROCKS; i++)
	{
		if (!gL2Rocks[i].active)
		{
			gL2Rocks[i].w = 28 + l2Rand(22);
			gL2Rocks[i].h = 26 + l2Rand(20);
			gL2Rocks[i].x = L2_PLAY_LEFT + l2Rand(L2_PLAY_RIGHT - L2_PLAY_LEFT - gL2Rocks[i].w);
			gL2Rocks[i].y = gL2CameraY + SCREEN_HEIGHT + 40.0;
			gL2Rocks[i].vy = -(L2_ROCK_FALL_MIN + l2Rand((int)(L2_ROCK_FALL_MAX - L2_ROCK_FALL_MIN)));
			gL2Rocks[i].active = 1;
			gL2Rocks[i].hit = 0;
			return;
		}
	}
}

void l2SpawnSnowChunk()
{
	int i;
	for (i = 0; i < L2_MAX_SNOW_CHUNKS; i++)
	{
		if (!gL2SnowChunks[i].active)
		{
			gL2SnowChunks[i].size = 20.0 + l2Rand(8);
			gL2SnowChunks[i].x = L2_PLAY_LEFT + 20 + l2Rand(L2_PLAY_RIGHT - L2_PLAY_LEFT - 40);
			gL2SnowChunks[i].y = gL2CameraY + SCREEN_HEIGHT + 70.0;
			gL2SnowChunks[i].vy = -(210.0 + l2Rand(160));
			gL2SnowChunks[i].vx = -30.0 + l2Rand(60);
			gL2SnowChunks[i].angle = l2Rand(628) / 100.0;
			gL2SnowChunks[i].rotSpeed = (l2Rand(2) == 0 ? 1 : -1) * (1.5 + l2Rand(200) / 100.0);
			gL2SnowChunks[i].active = 1;
			gL2SnowChunks[i].hit = 0;
			return;
		}
	}
}

//======================================================================
// Main update
//======================================================================
void level02Update(int jumpPressed, int pausePressed,
	int backPressed, int confirmPressed)
{
	unsigned long now;
	double dt;
	int moveL, moveR, moveU, moveD;
	int i;
	double px, py;

	now = GetTickCount();
	dt = (now - gL2LastMs) / 1000.0;
	gL2LastMs = now;
	if (dt < 0.0) dt = 0.0;
	if (dt > L2_MAX_DELTA) dt = L2_MAX_DELTA;

	if (gL2State == L2_STATE_WON || gL2State == L2_STATE_LOST)
	{
		if (confirmPressed) level02Start();
		else if (backPressed) level02Exit();
		return;
	}
	if (gL2State == L2_STATE_PAUSED)
	{
		if (pausePressed) gL2State = L2_STATE_CLIMBING;
		else if (backPressed) level02Exit();
		return;
	}
	if (gL2State == L2_STATE_READY)
	{
		if (backPressed) { level02Exit(); return; }
		gL2ReadyTime += dt;
		if (gL2ReadyTime >= L2_READY_SECONDS)
			gL2State = L2_STATE_CLIMBING;
		return;
	}

	if (backPressed)  { level02Exit(); return; }
	if (pausePressed) { gL2State = L2_STATE_PAUSED; return; }

	gL2Elapsed += dt;

	moveL = isKeyPressed('a') || isKeyPressed('A') || isSpecialKeyPressed(GLUT_KEY_LEFT);
	moveR = isKeyPressed('d') || isKeyPressed('D') || isSpecialKeyPressed(GLUT_KEY_RIGHT);
	moveU = isKeyPressed('w') || isKeyPressed('W') || isSpecialKeyPressed(GLUT_KEY_UP);
	moveD = isKeyPressed('s') || isKeyPressed('S') || isSpecialKeyPressed(GLUT_KEY_DOWN);

	// 1. Update moving platforms
	for (i = 0; i < gL2PlatformCount; i++)
	{
		struct L2Platform *p = &gL2Platforms[i];
		if (p->type == L2_PLAT_MOVING && p->active)
		{
			double oldX = p->x;
			p->phase += L2_MOVING_RATE * dt;
			p->x = p->baseX + L2_MOVING_AMP * sin(p->phase);
			double platDX = p->x - oldX;

			if (gL2PlayerState == L2_PS_STAND && gL2OnPlatIdx == i)
				gL2PlayerX += platDX;
		}
	}

		// ==================================================================
	// 2. Player movement based on current state
	// ==================================================================

	// ---- STAND --------------------------------------------------------
	if (gL2PlayerState == L2_PS_STAND)
	{
		// Horizontal
		gL2PlayerVX = 0.0;
		if (moveL) { gL2PlayerVX = -L2_MOVE_SPEED; gL2FacingRight = 0; }
		if (moveR) { gL2PlayerVX = L2_MOVE_SPEED; gL2FacingRight = 1; }

		// Running animation cadence
		if (fabs(gL2PlayerVX) > 10.0)
		{
			gL2AnimTimer += dt * 8.0;
			if (gL2AnimTimer >= 1.0)
			{
				gL2AnimTimer -= 1.0;
				gL2RunFrame = (gL2RunFrame + 1) % 4;
			}
		}
		else
		{
			gL2AnimTimer = 0.0;
			gL2RunFrame = 0;
		}

		// Jump
		if (jumpPressed)
		{
			double jv = L2_JUMP_VEL;
			if (gL2BootsTime > 0.0) jv *= L2_BOOTS_JUMP_MULT;
			// Jumping consumes stamina; standing recovers stamina
			if (gL2Stamina <= 0.0) jv *= 0.80; // weaker jump if exhausted
			else gL2Stamina -= 6.0;
			if (gL2Stamina < 0.0) gL2Stamina = 0.0;

			gL2PlayerVY = jv;
			gL2PlayerState = L2_PS_AIR;
			gL2HighestY = gL2PlayerY;
			gL2OnPlatIdx = -1;
		}
		else
		{
			// Recover stamina while resting on platforms
			gL2Stamina += 15.0 * dt;
			if (gL2Stamina > L2_STAMINA_MAX) gL2Stamina = L2_STAMINA_MAX;
		}
	}

	// ---- CLIMB --------------------------------------------------------
	else if (gL2PlayerState == L2_PS_CLIMB)
	{
		gL2PlayerVX = 0.0;
		gL2PlayerVY = 0.0;

		// Move up / down along wall
		if (moveU) gL2PlayerVY = L2_CLIMB_SPEED;
		if (moveD) gL2PlayerVY = -L2_CLIMB_DOWN_SPEED;

		// Wall climbing animation cadence
		if (fabs(gL2PlayerVY) > 10.0)
		{
			gL2ClimbAnimTimer += dt * 6.0;
			if (gL2ClimbAnimTimer >= 1.0)
			{
				gL2ClimbAnimTimer -= 1.0;
				gL2ClimbFrame = (gL2ClimbFrame + 1) % 4;
			}
		}

		// Wall jump
		if (jumpPressed)
		{
			double jv = L2_WALL_JUMP_VY;
			if (gL2BootsTime > 0.0) jv *= L2_BOOTS_JUMP_MULT;

			gL2PlayerVY = jv;
			gL2PlayerVX = (gL2ClimbWall == 1) ? L2_WALL_JUMP_VX
				: -L2_WALL_JUMP_VX;
			gL2PlayerState = L2_PS_AIR;
			gL2ClimbWall = 0;
			gL2FacingRight = (gL2PlayerVX > 0) ? 1 : 0;
			gL2HighestY = gL2PlayerY;
		}
		// Detach if pressing away from wall
		else if ((gL2ClimbWall == 1 && moveR) || (gL2ClimbWall == 2 && moveL))
		{
			gL2PlayerState = L2_PS_AIR;
			gL2PlayerVY = 0.0;
			gL2PlayerVX = (gL2ClimbWall == 1) ? L2_MOVE_SPEED : -L2_MOVE_SPEED;
			gL2ClimbWall = 0;
			gL2FacingRight = (gL2PlayerVX > 0) ? 1 : 0;
			gL2HighestY = gL2PlayerY;
		}
		// Out of stamina ??? fall
		else if (gL2Stamina <= 0.0)
		{
			gL2PlayerState = L2_PS_AIR;
			gL2PlayerVY = 0.0;
			gL2ClimbWall = 0;
			gL2HighestY = gL2PlayerY;
		}
	}

	// ---- ROPE CLIMBING ------------------------------------------------
	else if (gL2PlayerState == L2_PS_ROPE)
	{
		gL2RopeElapsed += dt;
		double speedFactor = gL2RopeElapsed / L2_ROPE_DURATION;
		if (speedFactor > 1.0) speedFactor = 1.0;

		// Accelerating climb speed as time increases
		gL2RopeClimbSpeed = L2_ROPE_SPEED_START + (L2_ROPE_SPEED_MAX - L2_ROPE_SPEED_START) * speedFactor;
		if (moveU) gL2RopeClimbSpeed += 55.0;
		if (moveD) gL2RopeClimbSpeed -= 55.0;
		if (gL2RopeClimbSpeed < 110.0) gL2RopeClimbSpeed = 110.0;

		gL2PlayerVY = gL2RopeClimbSpeed;
		gL2PlayerVX = 0.0;

		// Smooth steering across ropes (A / D or Left / Right)
		double steerSpeed = 380.0;
		if (moveL) gL2RopePlayerX -= steerSpeed * dt;
		if (moveR) gL2RopePlayerX += steerSpeed * dt;

		// Quick-leap between ropes with Jump key + Direction
		if (jumpPressed)
		{
			if (moveL) gL2RopePlayerX -= 140.0;
			else if (moveR) gL2RopePlayerX += 140.0;
		}

		if (gL2RopePlayerX < L2_ROPE_X_LEFT - 35.0) gL2RopePlayerX = L2_ROPE_X_LEFT - 35.0;
		if (gL2RopePlayerX > L2_ROPE_X_RIGHT + 35.0) gL2RopePlayerX = L2_ROPE_X_RIGHT + 35.0;

		gL2PlayerX = gL2RopePlayerX - L2_PLAYER_W / 2.0;

		// Determine current lane
		double dL = fabs(gL2RopePlayerX - L2_ROPE_X_LEFT);
		double dM = fabs(gL2RopePlayerX - L2_ROPE_X_MID);
		double dR = fabs(gL2RopePlayerX - L2_ROPE_X_RIGHT);
		if (dL < dM && dL < dR) gL2RopeLane = 0;
		else if (dR < dL && dR < dM) gL2RopeLane = 2;
		else gL2RopeLane = 1;

		// Stamina consumption while climbing
		gL2Stamina -= 3.0 * dt;
		if (gL2Stamina < 0.0) gL2Stamina = 0.0;

		// Check completion of rope section
		if (gL2PlayerY >= gL2RopeTargetY || gL2RopeElapsed >= L2_ROPE_DURATION)
		{
			gL2RopeActive = 0;
			if (gL2RopeSection == 1) gL2RopeSec1Done = 1;
			if (gL2RopeSection == 2) gL2RopeSec2Done = 1;

			gL2PlayerY = gL2RopeTargetY + 2.0;
			gL2PlayerVY = 0.0;
			gL2PlayerVX = 0.0;
			gL2PlayerState = L2_PS_STAND;
			gL2Score += 600;
			gL2Stamina = L2_STAMINA_MAX;

			// Clear active rope traps
			{
				int k;
				for (k = 0; k < L2_MAX_ROPE_TRAPS; k++)
					gL2RopeTraps[k].active = 0;
			}
		}
	}

	// ---- AIR ----------------------------------------------------------
	else /* L2_PS_AIR */
	{
		// Horizontal air control (reduced)
		if (moveL) { gL2PlayerVX = -L2_MOVE_SPEED * L2_AIR_CONTROL; gL2FacingRight = 0; }
		else if (moveR) { gL2PlayerVX = L2_MOVE_SPEED * L2_AIR_CONTROL; gL2FacingRight = 1; }
		else gL2PlayerVX = gL2PlayerVX * 0.92; // friction

		// Gravity
		gL2PlayerVY -= L2_GRAVITY * dt;

		// Track peak for fall damage
		if (gL2PlayerY > gL2HighestY)
			gL2HighestY = gL2PlayerY;
	}

	// ==================================================================
	// 3. Apply velocity
	// ==================================================================
	if (gL2PlayerState != L2_PS_ROPE)
	{
		gL2PlayerX += gL2PlayerVX * dt;
		gL2PlayerY += gL2PlayerVY * dt;

		// Clamp to play area
		if (gL2PlayerX < L2_PLAY_LEFT)
			gL2PlayerX = L2_PLAY_LEFT;
		if (gL2PlayerX + L2_PLAYER_W > L2_PLAY_RIGHT)
			gL2PlayerX = L2_PLAY_RIGHT - L2_PLAYER_W;
	}
	else
	{
		gL2PlayerY += gL2PlayerVY * dt;
	}

	// ==================================================================
	// Check trigger for sheer mountain rope sections
	// ==================================================================
	if (!gL2RopeSec1Done && !gL2RopeActive && gL2PlayerY >= L2_ROPE_SEC1_START - 40.0 && gL2PlayerY < L2_ROPE_SEC1_START + 120.0)
	{
		gL2RopeActive = 1;
		gL2RopeSection = 1;
		gL2RopeElapsed = 0.0;
		gL2RopeStartY = L2_ROPE_SEC1_START;
		gL2RopeTargetY = L2_ROPE_SEC1_END;
		gL2RopeLane = 1;
		gL2RopePlayerX = L2_ROPE_X_MID;
		gL2PlayerX = L2_ROPE_X_MID - L2_PLAYER_W / 2.0;
		gL2PlayerY = L2_ROPE_SEC1_START + 10.0;
		gL2PlayerState = L2_PS_ROPE;
		gL2RopeBannerTimer = 3.5;
		gL2RopeTrapTimer = 1.0;
		gL2OnPlatIdx = -1;
	}
	else if (!gL2RopeSec2Done && !gL2RopeActive && gL2PlayerY >= L2_ROPE_SEC2_START - 40.0 && gL2PlayerY < L2_ROPE_SEC2_START + 120.0)
	{
		gL2RopeActive = 1;
		gL2RopeSection = 2;
		gL2RopeElapsed = 0.0;
		gL2RopeStartY = L2_ROPE_SEC2_START;
		gL2RopeTargetY = L2_ROPE_SEC2_END;
		gL2RopeLane = 1;
		gL2RopePlayerX = L2_ROPE_X_MID;
		gL2PlayerX = L2_ROPE_X_MID - L2_PLAYER_W / 2.0;
		gL2PlayerY = L2_ROPE_SEC2_START + 10.0;
		gL2PlayerState = L2_PS_ROPE;
		gL2RopeBannerTimer = 3.5;
		gL2RopeTrapTimer = 1.0;
		gL2OnPlatIdx = -1;
	}

	// ==================================================================
	// 4. Platform collision (one-way: pass through from below)
	// ==================================================================
	if (gL2PlayerState == L2_PS_AIR && gL2PlayerVY <= 0.0)
	{
		for (i = 0; i < gL2PlatformCount; i++)
		{
			struct L2Platform *p = &gL2Platforms[i];
			double platTop;

			if (!p->active) continue;

			platTop = p->y + p->h;

			// Player's feet within a window around platform top
			if (gL2PlayerY >= platTop - 12.0 && gL2PlayerY <= platTop + 6.0 &&
				gL2PlayerX + L2_PLAYER_W > p->x + 4 &&
				gL2PlayerX < p->x + p->w - 4)
			{
				// ---- fall damage ----------------------------------------
				{
					double fallDist = gL2HighestY - platTop;
					if (fallDist > L2_FALL_SAFE && gL2InvulnTime <= 0.0)
					{
						double dmg = (fallDist - L2_FALL_SAFE) * L2_FALL_DMG_RATE;
						l2TakeHit(dmg);
						if (gL2State == L2_STATE_LOST) return;
					}
				}

				gL2PlayerY = platTop;
				gL2PlayerVY = 0.0;
				gL2PlayerVX = 0.0;
				gL2PlayerState = L2_PS_STAND;
				gL2OnPlatIdx = i;
				gL2HighestY = gL2PlayerY;
				break;
			}
		}
	}

	// Ground collision
	if (gL2PlayerY <= 0.0 && gL2PlayerState == L2_PS_AIR)
	{
		double fallDist = gL2HighestY - 0.0;
		if (fallDist > L2_FALL_SAFE && gL2InvulnTime <= 0.0)
		{
			double dmg = (fallDist - L2_FALL_SAFE) * L2_FALL_DMG_RATE;
			l2TakeHit(dmg);
			if (gL2State == L2_STATE_LOST) return;
		}
		gL2PlayerY = 0.0;
		gL2PlayerVY = 0.0;
		gL2PlayerVX = 0.0;
		gL2PlayerState = L2_PS_STAND;
		gL2OnPlatIdx = -1;
		gL2HighestY = 0.0;
	}

	// Check if player walked off a platform
	if (gL2PlayerState == L2_PS_STAND && gL2OnPlatIdx >= 0)
	{
		struct L2Platform *p = &gL2Platforms[gL2OnPlatIdx];
		if (!p->active ||
			gL2PlayerX + L2_PLAYER_W <= p->x + 4 ||
			gL2PlayerX >= p->x + p->w - 4)
		{
			gL2PlayerState = L2_PS_AIR;
			gL2OnPlatIdx = -1;
			gL2HighestY = gL2PlayerY;
		}
	}

	// 6. Crumbling platforms
	for (i = 0; i < gL2PlatformCount; i++)
	{
		struct L2Platform *p = &gL2Platforms[i];
		if (p->type == L2_PLAT_CRUMBLING && p->active)
		{
			if (gL2PlayerState == L2_PS_STAND && gL2OnPlatIdx == i && p->crumbleTimer < 0.0)
				p->crumbleTimer = L2_CRUMBLE_TIME;

			if (p->crumbleTimer >= 0.0)
			{
				p->crumbleTimer -= dt;
				if (p->crumbleTimer <= 0.0)
				{
					p->active = 0;
					if (gL2OnPlatIdx == i)
					{
						gL2PlayerState = L2_PS_AIR;
						gL2OnPlatIdx = -1;
						gL2HighestY = gL2PlayerY;
					}
				}
			}
		}
	}

	// 7. Dangerous platform damage
	if (gL2PlayerState == L2_PS_STAND && gL2OnPlatIdx >= 0)
	{
		struct L2Platform *p = &gL2Platforms[gL2OnPlatIdx];
		if (p->type == L2_PLAT_DANGEROUS && gL2InvulnTime <= 0.0)
		{
			gL2Hp -= L2_LAVA_DPS * dt;
			if (gL2Hp <= 0.0)
			{
				gL2Hp = 0.0;
				l2Lose(L2_LOSE_HEALTH);
				return;
			}
		}
	}

	// 8. Falling rocks & snow chunks
	if (!gL2RopeActive)
	{
		double progress = gL2PlayerY / (double)L2_SUMMIT_Y;
		if (progress < 0.0) progress = 0.0;
		if (progress > 1.0) progress = 1.0;
		double interval = L2_ROCK_SPAWN_BASE - (L2_ROCK_SPAWN_BASE - L2_ROCK_SPAWN_MIN) * progress;

		gL2RockTimer -= dt;
		if (gL2RockTimer <= 0.0)
		{
			int count = 1 + l2Rand(2);
			int c;
			for (c = 0; c < count; c++)
				l2SpawnRock();
			gL2RockTimer = interval;
		}

		double sInterval = L2_SNOW_SPAWN_BASE - (L2_SNOW_SPAWN_BASE - L2_SNOW_SPAWN_MIN) * progress;
		gL2SnowTrapTimer -= dt;
		if (gL2SnowTrapTimer <= 0.0)
		{
			l2SpawnSnowChunk();
			if (progress > 0.35 && l2Rand(100) < 35)
				l2SpawnSnowChunk();
			gL2SnowTrapTimer = sInterval;
		}
	}

	px = gL2PlayerX + (L2_PLAYER_W - L2_PLAYER_HIT_W) / 2.0;
	py = gL2PlayerY;

	for (i = 0; i < L2_MAX_ROCKS; i++)
	{
		struct L2FallingRock *r = &gL2Rocks[i];
		if (!r->active) continue;

		r->y += r->vy * dt;
		if (r->y + r->h < gL2CameraY - 100.0)
		{
			r->active = 0;
			continue;
		}

		if (!r->hit && gL2InvulnTime <= 0.0 &&
			l2Overlap(px, py, L2_PLAYER_HIT_W, L2_PLAYER_HIT_H,
			r->x, r->y, (double)r->w, (double)r->h))
		{
			r->hit = 1;
			l2TakeHit(L2_HIT_DAMAGE);
			if (gL2State == L2_STATE_LOST) return;
		}
	}

	for (i = 0; i < L2_MAX_SNOW_CHUNKS; i++)
	{
		struct L2SnowChunk *sc = &gL2SnowChunks[i];
		if (!sc->active) continue;

		sc->x += sc->vx * dt;
		sc->y += sc->vy * dt;
		sc->angle += sc->rotSpeed * dt;

		if (sc->y + sc->size < gL2CameraY - 100.0)
		{
			sc->active = 0;
			continue;
		}

		if (!sc->hit && gL2InvulnTime <= 0.0)
		{
			double pcx = px + L2_PLAYER_HIT_W / 2.0;
			double pcy = py + L2_PLAYER_HIT_H / 2.0;
			double dx = pcx - sc->x;
			double dy = pcy - sc->y;
			double dist = sqrt(dx * dx + dy * dy);
			if (dist < sc->size + L2_PLAYER_HIT_W / 2.0 - 2.0)
			{
				sc->hit = 1;
				l2TakeHit(L2_SNOW_CHUNK_DAMAGE);
				if (gL2State == L2_STATE_LOST) return;
			}
		}
	}

	// 9. Rope traps
	if (gL2RopeActive)
	{
		if (gL2RopeBannerTimer > 0.0) gL2RopeBannerTimer -= dt;

		gL2RopeTrapTimer -= dt;
		if (gL2RopeTrapTimer <= 0.0)
		{
			double speedFactor = gL2RopeElapsed / L2_ROPE_DURATION;
			if (speedFactor > 1.0) speedFactor = 1.0;
			gL2RopeTrapTimer = (2.2 - 0.6 * speedFactor) + (l2Rand(30) / 100.0);

			for (i = 0; i < L2_MAX_ROPE_TRAPS; i++)
			{
				if (!gL2RopeTraps[i].active)
				{
					struct L2RopeTrap *rt = &gL2RopeTraps[i];
					int targetLane = (l2Rand(100) < 60) ? gL2RopeLane : l2Rand(L2_ROPE_LANES);

					rt->lane = targetLane;
					rt->x = (targetLane == 0 ? L2_ROPE_X_LEFT : (targetLane == 1 ? L2_ROPE_X_MID : L2_ROPE_X_RIGHT)) + (l2Rand(30) - 15);
					rt->y = gL2CameraY + SCREEN_HEIGHT + 30.0;
					rt->hit = 0;
					rt->rot = 0.0;

					int roll = l2Rand(100);
					if (roll < 45)
					{
						rt->type = L2_RTRAP_ICICLE;
						rt->vy = -(300.0 + 100.0 * speedFactor + l2Rand(40));
						rt->vx = 0.0;
						rt->size = 18.0;
					}
					else if (roll < 75)
					{
						rt->type = L2_RTRAP_AVALANCHE;
						rt->vy = -(240.0 + 80.0 * speedFactor + l2Rand(30));
						rt->vx = (l2Rand(2) == 0 ? 15.0 : -15.0);
						rt->size = 24.0;
					}
					else
					{
						rt->type = L2_RTRAP_FROSTWIND;
						rt->vy = -(180.0 + 60.0 * speedFactor);
						rt->vx = (targetLane == 0 ? 120.0 : (targetLane == 2 ? -120.0 : (l2Rand(2) == 0 ? 120.0 : -120.0)));
						rt->size = 28.0;
					}
					rt->active = 1;
					break;
				}
			}
		}

		for (i = 0; i < L2_MAX_ROPE_TRAPS; i++)
		{
			struct L2RopeTrap *rt = &gL2RopeTraps[i];
			if (!rt->active) continue;

			rt->y += rt->vy * dt;
			rt->x += rt->vx * dt;
			rt->rot += 4.5 * dt;

			if (!rt->hit && gL2InvulnTime <= 0.0)
			{
				double pcx = gL2PlayerX + L2_PLAYER_W / 2.0;
				double pcy = gL2PlayerY + L2_PLAYER_H / 2.0;
				double dx = fabs(rt->x - pcx);
				double dy = fabs(rt->y - pcy);

				if (dx < (rt->size + L2_PLAYER_HIT_W / 2.0) &&
					dy < (rt->size + L2_PLAYER_HIT_H / 2.0))
				{
					if (gL2ShieldTime <= 0.0)
					{
						gL2Hp -= 5.0;
						gL2InvulnTime = 1.5;
						gL2Stamina -= 4.0;
						gL2PlayerY -= 8.0;
						if (gL2Hp <= 0.0)
						{
							gL2Hp = 0.0;
							gL2State = L2_STATE_LOST;
							gL2LoseReason = L2_LOSE_HEALTH;
							return;
						}
					}
					rt->hit = 1;
				}
			}

			if (rt->y < gL2CameraY - 60.0)
				rt->active = 0;
		}
	}

	// 10. Pickups
	for (i = 0; i < gL2PickupCount; i++)
	{
		struct L2Pickup *pu = &gL2Pickups[i];
		if (pu->taken) continue;

		if (l2Overlap(px, py, L2_PLAYER_HIT_W, L2_PLAYER_HIT_H,
			pu->x - 6.0, pu->y - 6.0,
			pu->w + 12.0, pu->h + 12.0))
		{
			pu->taken = 1;
			gL2Score += L2_SCORE_PICKUP;

			if (pu->type == PU_WINTER_HEAL)
			{
				gL2Hp += 40.0;
				if (gL2Hp > L2_HP_MAX) gL2Hp = L2_HP_MAX;
				gL2Stamina += 20.0;
				if (gL2Stamina > L2_STAMINA_MAX) gL2Stamina = L2_STAMINA_MAX;
			}
			else if (pu->type == PU_WINTER_STAMINA || pu->type == PU_ENERGY)
			{
				gL2Stamina += (pu->type == PU_WINTER_STAMINA ? 60.0 : L2_ENERGY_REFILL);
				if (gL2Stamina > L2_STAMINA_MAX) gL2Stamina = L2_STAMINA_MAX;
				gL2Hp += 8.0;
				if (gL2Hp > L2_HP_MAX) gL2Hp = L2_HP_MAX;
			}
			else if (pu->type == PU_SHIELD)
			{
				gL2ShieldTime = L2_SHIELD_SECONDS;
			}
			else
			{
				gL2BootsTime = L2_BOOTS_SECONDS;
			}
		}
	}

	// 11. Gears
	for (i = 0; i < gL2GearCount; i++)
	{
		struct L2Gear *g = &gL2Gears[i];
		if (!g->active) continue;

		g->angle += g->speed * dt;

		if (gL2InvulnTime <= 0.0)
		{
			double dx = (px + L2_PLAYER_HIT_W / 2.0) - g->x;
			double dy = (py + L2_PLAYER_HIT_H / 2.0) - g->y;
			double dist = sqrt(dx * dx + dy * dy);
			if (dist < g->size + L2_PLAYER_HIT_W / 2.0 - 4.0)
			{
				l2TakeHit(L2_GEAR_DAMAGE);
				if (gL2State == L2_STATE_LOST) return;
			}
		}
	}

	if (gL2InvulnTime > 0.0) gL2InvulnTime -= dt;
	if (gL2ShieldTime > 0.0) gL2ShieldTime -= dt;
	if (gL2BootsTime  > 0.0) gL2BootsTime -= dt;

	if (gL2PlayerState == L2_PS_STAND && gL2OnPlatIdx >= 0 && gL2Platforms[gL2OnPlatIdx].type == L2_PLAT_DANGEROUS)
	{
		gL2Stamina -= L2_STAM_WALK_DRAIN * 2.0 * dt;
	}

	if (gL2Stamina < 0.0) gL2Stamina = 0.0;
	if (gL2Stamina > L2_STAMINA_MAX) gL2Stamina = L2_STAMINA_MAX;

	while (gL2PlayerY - gL2HeightScored >= 100.0)
	{
		gL2HeightScored += 100.0;
		gL2Score += L2_SCORE_PER_100PX;
	}

	// Camera follow
	{
		double targetCam = gL2PlayerY - SCREEN_HEIGHT * 0.38;
		if (targetCam < 0.0) targetCam = 0.0;
		if (targetCam > L2_WORLD_HEIGHT - SCREEN_HEIGHT)
			targetCam = L2_WORLD_HEIGHT - SCREEN_HEIGHT;
		double camFollow = gL2RopeActive ? 12.0 : 6.0;
		gL2CameraY += (targetCam - gL2CameraY) * camFollow * dt;
	}

	if (gL2PlayerY >= (double)L2_SUMMIT_Y)
	{
		l2Win();
		return;
	}
	if (l2SecondsLeft() <= 0)
		l2Lose(L2_LOSE_TIME);
}

#endif
