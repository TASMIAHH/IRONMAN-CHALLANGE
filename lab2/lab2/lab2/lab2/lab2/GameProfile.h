//======================================================================
//  GameProfile.h
//  Iron Man Challenge: The Ultimate Training
//
//  Persistent Gamer Profile & High Score Tracking System.
//  Maintains individual high scores, medals, and completion status
//  for each level per gamer, with automatic disk persistence across
//  sessions.
//======================================================================
#ifndef GAME_PROFILE_H
#define GAME_PROFILE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_GAMER_NAME     24
#define MAX_SAVED_PROFILES 16
#define PROFILE_SAVE_FILE  "gamer_profiles.dat"
#define PROFILE_BACKUP_FILE "Assets/gamer_profiles.dat"

//----------------------------------------------------------------------
// Profile data structure
//----------------------------------------------------------------------
struct GamerProfile
{
	char name[MAX_GAMER_NAME];
	long levelHighScore[TOTAL_LEVELS + 1];  // 1-based: [1] to [TOTAL_LEVELS]
	int  levelMedal[TOTAL_LEVELS + 1];      // 0=None, 1=Bronze, 2=Silver, 3=Gold
	int  levelCompleted[TOTAL_LEVELS + 1];  // 0 or 1
	long totalScore;                        // sum of high scores across all levels
	int  gamesPlayed;                       // total challenge runs
	unsigned long lastActiveTime;           // unix timestamp
};

// Global profile storage
struct GamerProfile gProfiles[MAX_SAVED_PROFILES];
int gProfileCount = 0;
int gActiveProfileIndex = -1;
int gNewHighScoreAchieved = 0; // flag for in-game celebration

//----------------------------------------------------------------------
// Rank & Title Computation based on Total Score
//----------------------------------------------------------------------
const char* profileGetRankTitle(long totalScore)
{
	if (totalScore >= 16000) return "LEGENDARY IRON MAN";
	if (totalScore >= 12000) return "AVENGER TITAN";
	if (totalScore >= 8000)  return "STARK SPECIAL AGENT";
	if (totalScore >= 5000)  return "TACTICAL CADET";
	if (totalScore >= 2000)  return "RESERVE ATHLETE";
	if (totalScore > 0)      return "INITIATE RECRUIT";
	return "UNRANKED RECRUIT";
}

const char* profileGetMedalName(int medal)
{
	if (medal == 3) return "GOLD";
	if (medal == 2) return "SILVER";
	if (medal == 1) return "BRONZE";
	return "NONE";
}

//----------------------------------------------------------------------
// File Path Resolver
//----------------------------------------------------------------------
static const char* profileGetSavePath()
{
	// Check if working directory allows writing, otherwise fallback
	FILE *f = fopen(PROFILE_SAVE_FILE, "a+");
	if (f)
	{
		fclose(f);
		return PROFILE_SAVE_FILE;
	}
	return PROFILE_BACKUP_FILE;
}

//----------------------------------------------------------------------
// Forward declaration
//----------------------------------------------------------------------
void profileSaveAll();

//----------------------------------------------------------------------
// Initialization
//----------------------------------------------------------------------
void profileInit()
{
	gProfileCount = 0;
	gActiveProfileIndex = -1;
	gNewHighScoreAchieved = 0;
	memset(gProfiles, 0, sizeof(gProfiles));
}

//----------------------------------------------------------------------
// Load All Profiles from Disk
//----------------------------------------------------------------------
void profileLoadAll()
{
	profileInit();

	const char *path = PROFILE_SAVE_FILE;
	FILE *f = fopen(path, "r");
	if (!f)
	{
		path = PROFILE_BACKUP_FILE;
		f = fopen(path, "r");
	}

	if (!f)
	{
		// No save file yet, start fresh
		return;
	}

	char line[256];
	struct GamerProfile temp;
	memset(&temp, 0, sizeof(temp));

	while (fgets(line, sizeof(line), f))
	{
		// Format: NAME|L1_SCORE|L1_MEDAL|L1_COMP|L2_SCORE|L2_MEDAL|L2_COMP|L3_SCORE|L3_MEDAL|L3_COMP|L4_SCORE|L4_MEDAL|L4_COMP|TOTAL|RUNS|TIMESTAMP
		if (line[0] == '#' || line[0] == '\n' || line[0] == '\r')
			continue;

		char name[MAX_GAMER_NAME] = {0};
		long s1 = 0, s2 = 0, s3 = 0, s4 = 0, tot = 0;
		int m1 = 0, m2 = 0, m3 = 0, m4 = 0;
		int c1 = 0, c2 = 0, c3 = 0, c4 = 0;
		int runs = 0;
		unsigned long ts = 0;

		int parsed = sscanf(line, "%23[^|]|%ld|%d|%d|%ld|%d|%d|%ld|%d|%d|%ld|%d|%d|%ld|%d|%lu",
			name,
			&s1, &m1, &c1,
			&s2, &m2, &c2,
			&s3, &m3, &c3,
			&s4, &m4, &c4,
			&tot, &runs, &ts);

		if (parsed >= 1)
		{
			if (gProfileCount < MAX_SAVED_PROFILES)
			{
				struct GamerProfile *p = &gProfiles[gProfileCount];
				strncpy(p->name, name, MAX_GAMER_NAME - 1);
				p->name[MAX_GAMER_NAME - 1] = '\0';

				p->levelHighScore[1] = s1; p->levelMedal[1] = m1; p->levelCompleted[1] = c1;
				p->levelHighScore[2] = s2; p->levelMedal[2] = m2; p->levelCompleted[2] = c2;
				p->levelHighScore[3] = s3; p->levelMedal[3] = m3; p->levelCompleted[3] = c3;
				p->levelHighScore[4] = s4; p->levelMedal[4] = m4; p->levelCompleted[4] = c4;

				p->totalScore = s1 + s2 + s3 + s4;
				p->gamesPlayed = runs;
				p->lastActiveTime = ts;

				gProfileCount++;
			}
		}
	}

	fclose(f);

	// Select first profile if any exist
	if (gProfileCount > 0)
	{
		gActiveProfileIndex = 0;
		// Sync active profile's completion status to gLevelCompleted
		int lvl;
		for (lvl = 1; lvl <= TOTAL_LEVELS; lvl++)
		{
			gLevelCompleted[lvl] = gProfiles[0].levelCompleted[lvl];
		}
	}
}

//----------------------------------------------------------------------
// Save All Profiles to Disk
//----------------------------------------------------------------------
void profileSaveAll()
{
	const char *path = profileGetSavePath();
	FILE *f = fopen(path, "w");
	if (!f) return;

	fprintf(f, "# Iron Man Challenge Gamer Profiles Database\n");
	fprintf(f, "# Format: NAME|L1_SCORE|L1_MEDAL|L1_COMP|L2_SCORE|L2_MEDAL|L2_COMP|L3_SCORE|L3_MEDAL|L3_COMP|L4_SCORE|L4_MEDAL|L4_COMP|TOTAL|RUNS|TIMESTAMP\n");

	int i;
	for (i = 0; i < gProfileCount; i++)
	{
		struct GamerProfile *p = &gProfiles[i];
		p->totalScore = p->levelHighScore[1] + p->levelHighScore[2] + p->levelHighScore[3] + p->levelHighScore[4];

		fprintf(f, "%s|%ld|%d|%d|%ld|%d|%d|%ld|%d|%d|%ld|%d|%d|%ld|%d|%lu\n",
			p->name,
			p->levelHighScore[1], p->levelMedal[1], p->levelCompleted[1],
			p->levelHighScore[2], p->levelMedal[2], p->levelCompleted[2],
			p->levelHighScore[3], p->levelMedal[3], p->levelCompleted[3],
			p->levelHighScore[4], p->levelMedal[4], p->levelCompleted[4],
			p->totalScore,
			p->gamesPlayed,
			p->lastActiveTime);
	}

	fflush(f);
	fclose(f);
}

//----------------------------------------------------------------------
// Lookup or Register Profile
//----------------------------------------------------------------------
int profileFindIndex(const char *name)
{
	if (!name || !name[0]) return -1;
	int i;
	for (i = 0; i < gProfileCount; i++)
	{
		if (stricmp(gProfiles[i].name, name) == 0)
			return i;
	}
	return -1;
}

int profileSelectOrAdd(const char *name)
{
	if (!name || !name[0]) return -1;

	// Clean name: trim leading and trailing spaces
	char cleanName[MAX_GAMER_NAME];
	strncpy(cleanName, name, MAX_GAMER_NAME - 1);
	cleanName[MAX_GAMER_NAME - 1] = '\0';

	// Trim leading
	char *start = cleanName;
	while (*start == ' ' || *start == '\t') start++;

	// Trim trailing
	int len = (int)strlen(start);
	while (len > 0 && (start[len - 1] == ' ' || start[len - 1] == '\t' || start[len - 1] == '\r' || start[len - 1] == '\n'))
	{
		start[len - 1] = '\0';
		len--;
	}

	if (len == 0) return -1;

	int idx = profileFindIndex(start);
	if (idx >= 0)
	{
		// Existing profile found
		gActiveProfileIndex = idx;
		gProfiles[idx].lastActiveTime = (unsigned long)time(NULL);

		// Sync level completions to game state
		int lvl;
		for (lvl = 1; lvl <= TOTAL_LEVELS; lvl++)
			gLevelCompleted[lvl] = gProfiles[idx].levelCompleted[lvl];

		profileSaveAll();
		return idx;
	}

	// Create new profile if space available
	if (gProfileCount < MAX_SAVED_PROFILES)
	{
		idx = gProfileCount++;
		struct GamerProfile *p = &gProfiles[idx];
		memset(p, 0, sizeof(struct GamerProfile));
		strncpy(p->name, start, MAX_GAMER_NAME - 1);
		p->name[MAX_GAMER_NAME - 1] = '\0';
		p->lastActiveTime = (unsigned long)time(NULL);

		gActiveProfileIndex = idx;

		int lvl;
		for (lvl = 1; lvl <= TOTAL_LEVELS; lvl++)
			gLevelCompleted[lvl] = 0;

		profileSaveAll();
		return idx;
	}

	// Overwrite oldest profile if full
	int oldestIdx = 0;
	unsigned long oldestTime = gProfiles[0].lastActiveTime;
	int i;
	for (i = 1; i < gProfileCount; i++)
	{
		if (gProfiles[i].lastActiveTime < oldestTime)
		{
			oldestTime = gProfiles[i].lastActiveTime;
			oldestIdx = i;
		}
	}

	struct GamerProfile *p = &gProfiles[oldestIdx];
	memset(p, 0, sizeof(struct GamerProfile));
	strncpy(p->name, start, MAX_GAMER_NAME - 1);
	p->name[MAX_GAMER_NAME - 1] = '\0';
	p->lastActiveTime = (unsigned long)time(NULL);

	gActiveProfileIndex = oldestIdx;

	int lvl;
	for (lvl = 1; lvl <= TOTAL_LEVELS; lvl++)
		gLevelCompleted[lvl] = 0;

	profileSaveAll();
	return oldestIdx;
}

//----------------------------------------------------------------------
// Get Active Profile Pointer
//----------------------------------------------------------------------
struct GamerProfile* profileGetActive()
{
	if (gActiveProfileIndex >= 0 && gActiveProfileIndex < gProfileCount)
		return &gProfiles[gActiveProfileIndex];
	return NULL;
}

const char* profileGetActiveName()
{
	struct GamerProfile *p = profileGetActive();
	return p ? p->name : "ANONYMOUS";
}

long profileGetLevelHighScore(int level)
{
	struct GamerProfile *p = profileGetActive();
	if (p && level >= 1 && level <= TOTAL_LEVELS)
		return p->levelHighScore[level];
	return 0;
}

int profileGetLevelMedal(int level)
{
	struct GamerProfile *p = profileGetActive();
	if (p && level >= 1 && level <= TOTAL_LEVELS)
		return p->levelMedal[level];
	return 0;
}

//----------------------------------------------------------------------
// Update Level Score & Medals
//----------------------------------------------------------------------
void profileUpdateLevelScore(int level, long score, int medal, int completed)
{
	struct GamerProfile *p = profileGetActive();
	if (!p || level < 1 || level > TOTAL_LEVELS) return;

	p->gamesPlayed++;
	p->lastActiveTime = (unsigned long)time(NULL);
	gNewHighScoreAchieved = 0;

	if (score > p->levelHighScore[level])
	{
		p->levelHighScore[level] = score;
		gNewHighScoreAchieved = 1;
	}

	if (medal > p->levelMedal[level])
	{
		p->levelMedal[level] = medal;
	}

	if (completed)
	{
		p->levelCompleted[level] = 1;
		gLevelCompleted[level] = 1;
	}

	p->totalScore = p->levelHighScore[1] + p->levelHighScore[2] + p->levelHighScore[3] + p->levelHighScore[4];

	// Immediate persistent save
	profileSaveAll();
}

#endif
