#pragma once

#ifndef UTILITY_H
#define UTILITY_H

#define WIN32_LEAN_AND_MEAN

// Includes
#include <Windows.h>
#include <WindowsX.h>
#include <d3dx10.h>
#include <d3d10.h>
#include <d3dx10math.h>
#include <dxerr.h>
#include <string>
#include <sstream>
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <vld.h>
#include <Mmsystem.h>

#pragma comment(lib, "winmm.lib")

// Definitions

// enumerations
enum QUESTIONSTATE
{
	NONE = 0,
	QUESTION = 1,
	ANSWER_RIGHT = 2,
	ANSWER_WRONG = 3
};

enum MONSTERSTATE
{
	MOVING = 0,
	ATTACKING = 1,
	DEAD = 2,
	STOPPED = 3,
	ARCHER_ATTACK = 4,
};

enum MONSTER
{
	SWORDSMANL1 = 0,
	SWORDSMANL2 = 1,
	SWORDSMANL3 = 2,
	MONSTERL1 = 3,
	MONSTERL2 = 4,
	MONSTERL3 = 5,
	ARCHER	= 6,
	MONSTERARCHER = 7
};

enum GAMESTATE
{
	MENU,
	HOWTOPLAY,
	GAME,
	GAMEOVER
};

// Structs
struct Vertex
{
	D3DXVECTOR3 vecPosition;
	D3DXVECTOR2 vecTexCoord;
};

struct SOUNDINFO
{
	std::string sFilename;
	std::string sName;
};

// Convenience macro for releasing COM objects.
#define ReleaseCOM(x) { if(x) { x->Release(); x = nullptr; } }
#define DeleteOBJ(x) {if(x) { delete x; x = nullptr; } }
#define RandomFloat() (rand() / (RAND_MAX + 1.0f))

#define MONSTER_TYPES 4
#define MONSTER_ATTACK_DISTANCE 2.0
#define MONSTER_ATTACK_CASTLE_DISTANCE 6
#define MONSTER_QUEUE_DISTANCE 2.7
#define ARCHER_SHOOT_DISTANCE 11
#define PLAYER_ID 0
#define AI_ID 1

//// GAME CONSTANTS ////
static const int g_kiCastleHealth = 50;
static const int g_kiMonsterCooldown[] = {4, 5, 8, 5};
static const int g_kiMonsterHealth[] = {4, 5, 10, 4};
static const int g_kiMonsterDamage[] = {1, 2, 4, 1};

#endif // UTILITY_H