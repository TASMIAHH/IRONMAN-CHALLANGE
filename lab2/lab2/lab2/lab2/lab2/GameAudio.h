//======================================================================
//  GameAudio.h
//  All sound is played through the Windows MCI interface (mciSendString),
//  exactly like the original iGraphics lab template. winmm.lib is linked
//  automatically by the #pragma inside glut.h
//======================================================================
#ifndef GAME_AUDIO_H
#define GAME_AUDIO_H

//----------------------------------------------------------------------
// MCI aliases
//----------------------------------------------------------------------
#define SND_INTRO      "introsong"
#define SND_BACKGROUND "bgsong"
#define SND_GAMEOVER   "ggsong"
#define SND_LEVEL01    "lvl01song"
#define SND_COLLISION  "hitsfx"

// Which track is supposed to be looping right now.
// 0 = none, 1 = intro, 2 = menu background, 3 = level 01
int gLoopingTrack = 0;

// CHANGED: mute state for the whole game
bool gAudioMuted = false;

//----------------------------------------------------------------------
// Helpers
//----------------------------------------------------------------------
void mciOpen(const char *file, const char *alias)
{
	char cmd[256];
	sprintf(cmd, "open \"%s\" type mpegvideo alias %s", file, alias);
	mciSendString(cmd, NULL, 0, NULL);
}

void mciClose(const char *alias)
{
	char cmd[128];
	sprintf(cmd, "close %s", alias);
	mciSendString(cmd, NULL, 0, NULL);
}

void mciPlayLoop(const char *alias)
{
	char cmd[128];
	sprintf(cmd, "play %s from 0 repeat", alias);
	mciSendString(cmd, NULL, 0, NULL);
}

void mciPlayOnce(const char *alias)
{
	char cmd[128];
	sprintf(cmd, "play %s from 0", alias);
	mciSendString(cmd, NULL, 0, NULL);
}

void mciStop(const char *alias)
{
	char cmd[128];
	sprintf(cmd, "stop %s", alias);
	mciSendString(cmd, NULL, 0, NULL);
	sprintf(cmd, "seek %s to start", alias);
	mciSendString(cmd, NULL, 0, NULL);
}

// Returns 1 while the given alias is actually producing sound.
int mciIsPlaying(const char *alias)
{
	char cmd[128];
	char reply[64];

	sprintf(cmd, "status %s mode", alias);
	reply[0] = '\0';
	mciSendString(cmd, reply, sizeof(reply), NULL);

	return (strcmp(reply, "playing") == 0);
}

// CHANGED: sets the playback volume of one alias (0 = silent, 1000 = full)
void mciSetVolume(const char *alias, int volume)
{
	char cmd[128];
	sprintf(cmd, "setaudio %s volume to %d", alias, volume);
	mciSendString(cmd, NULL, 0, NULL);
}

//----------------------------------------------------------------------
// Public audio API
//----------------------------------------------------------------------
void audioInit()
{
	mciOpen(PATH_INTRO_SOUND, SND_INTRO);
	mciOpen(PATH_BG_SOUND, SND_BACKGROUND);
	mciOpen(PATH_GAMEOVER_SOUND, SND_GAMEOVER);
	mciOpen(PATH_LEVEL01_MUSIC, SND_LEVEL01);
	mciOpen(PATH_COLLISION_SOUND, SND_COLLISION);
}

void audioShutdown()
{
	mciClose(SND_INTRO);
	mciClose(SND_BACKGROUND);
	mciClose(SND_GAMEOVER);
	mciClose(SND_LEVEL01);
	mciClose(SND_COLLISION);
	gLoopingTrack = 0;
}

// Loops the intro theme. Used by the main menu and its sub-screens.
void audioPlayIntroTheme()
{
	if (gLoopingTrack == 1) return;              // already running

	mciStop(SND_BACKGROUND);
	mciPlayLoop(SND_INTRO);
	gLoopingTrack = 1;
}

// Loops the in-game background music. Starts when "NEW GAME" is chosen.
void audioPlayBackgroundTheme()
{
	if (gLoopingTrack == 2) return;

	mciStop(SND_INTRO);
	mciStop(SND_LEVEL01);
	mciPlayLoop(SND_BACKGROUND);
	gLoopingTrack = 2;
}

// Loops the Level 01 theme. Runs for the whole stage and is stopped
// only when the player wins or loses.
void audioPlayLevel01Theme()
{
	if (gLoopingTrack == 3) return;

	mciStop(SND_INTRO);
	mciStop(SND_BACKGROUND);
	mciPlayLoop(SND_LEVEL01);
	gLoopingTrack = 3;
}

// Loops the Level 03 theme.
void audioPlayLevel03Theme()
{
	if (gLoopingTrack == 2) return;

	mciStop(SND_INTRO);
	mciStop(SND_LEVEL01);
	mciPlayLoop(SND_BACKGROUND);
	gLoopingTrack = 2;
}

// Loops the Level 04 theme.
void audioPlayLevel04Theme()
{
	if (gLoopingTrack == 3) return;

	mciStop(SND_INTRO);
	mciStop(SND_BACKGROUND);
	mciPlayLoop(SND_LEVEL01);
	gLoopingTrack = 3;
}

void audioStopAllMusic()
{
	mciStop(SND_INTRO);
	mciStop(SND_BACKGROUND);
	mciStop(SND_LEVEL01);
	gLoopingTrack = 0;
}

void audioPlayGameOver()
{
	mciPlayOnce(SND_GAMEOVER);
}

// Short impact sting, played on every obstacle collision.
void audioPlayCollision()
{
	mciPlayOnce(SND_COLLISION);
}

// CHANGED: applies the current mute state to every audio alias
void audioApplyMuteState()
{
	int vol = gAudioMuted ? 0 : 1000;
	mciSetVolume(SND_INTRO, vol);
	mciSetVolume(SND_BACKGROUND, vol);
	mciSetVolume(SND_GAMEOVER, vol);
	mciSetVolume(SND_LEVEL01, vol);
	mciSetVolume(SND_COLLISION, vol);
}

// CHANGED: flips mute on/off and immediately applies it
void audioToggleMute()
{
	gAudioMuted = !gAudioMuted;
	audioApplyMuteState();
}

//----------------------------------------------------------------------
// Watchdog.
// "play ... repeat" is handled by the MPEG driver, but on a few Windows
// builds it drops the loop when a track ends. This timer re-starts the
// track that is supposed to be looping if it has fallen silent, so the
// menu music never dies. Registered with iSetTimer() from main().
//----------------------------------------------------------------------
void audioLoopWatchdog()
{
	if (gLoopingTrack == 1 && !mciIsPlaying(SND_INTRO))
		mciPlayLoop(SND_INTRO);
	else if (gLoopingTrack == 2 && !mciIsPlaying(SND_BACKGROUND))
		mciPlayLoop(SND_BACKGROUND);
	else if (gLoopingTrack == 3 && !mciIsPlaying(SND_LEVEL01))
		mciPlayLoop(SND_LEVEL01);
}

#endif