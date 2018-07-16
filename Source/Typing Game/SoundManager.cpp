// Self Include:
#include "SoundManager.h"

// Local Includes:

// Library Includes:

// Implementation:
CSoundManager::CSoundManager()
{
	m_tMenuMusic.sFilename = "Data\\SoundAssets\\Music\\Menu_Loop.mp3";
	m_tMenuMusic.sName = "MENU_MUSIC";

	m_tGameMusic.sFilename = "Data\\SoundAssets\\Music\\Gameplay_Loop.mp3";
	m_tGameMusic.sName = "GAME_MUSIC";

	m_tCastleDamage.sFilename = "Data\\SoundAssets\\Misc\\Destruction_01.mp3";
	m_tCastleDamage.sName = "CASTLE_DAMAGE";

	m_tLoseGame.sFilename = "Data\\SoundAssets\\Misc\\Lose_Game.mp3";
	m_tLoseGame.sName = "LOSE_GAME";

	m_tWinGame.sFilename = "Data\\SoundAssets\\Misc\\Win_Game.mp3";
	m_tWinGame.sName = "WIN_GAME";

	m_tArrow.sFilename = "Data\\SoundAssets\\Misc\\Arrow.mp3";
	m_tArrow.sName = "ARROW";
}

CSoundManager::~CSoundManager()
{

}

void CSoundManager::Initialise()
{
	// Sound nicknames will be in the following format:
	// "TYPE_EVENT_TIER_NO
	// E.G.
	// HUMAN_DEATH_TIER1_1

	// Load Sounds
	LoadSound(m_tMenuMusic.sFilename, m_tMenuMusic.sName);
	LoadSound(m_tGameMusic.sFilename, m_tGameMusic.sName);
	LoadSound(m_tCastleDamage.sFilename, m_tCastleDamage.sName);
	LoadSound(m_tLoseGame.sFilename, m_tLoseGame.sName);
	LoadSound(m_tWinGame.sFilename, m_tWinGame.sName);
	LoadSound(m_tArrow.sFilename, m_tArrow.sName);

	ChangeVolume(m_tMenuMusic.sName, 200);
	ChangeVolume(m_tGameMusic.sName, 200);

	SetupHumanDeath();
	SetupMonsterDeath();

	SetupHumanSpawn();
	SetupMonsterSpawn();
	SetupConflict();
}

void CSoundManager::SetupHumanDeath()
{
	// Tier 1 Human Death Noise
	m_tHumanDeath[0][0].sFilename = "Data\\SoundAssets\\Humans\\01_Small\\sHuman_Death_01.mp3";
	m_tHumanDeath[0][0].sName = "HUMAN_DEATH_TIER1_1";
	m_tHumanDeath[0][1].sFilename = "Data\\SoundAssets\\Humans\\01_Small\\sHuman_Death_02.mp3";
	m_tHumanDeath[0][1].sName = "HUMAN_DEATH_TIER1_2";
	m_tHumanDeath[0][2].sFilename = "Data\\SoundAssets\\Humans\\01_Small\\sHuman_Death_03.mp3";
	m_tHumanDeath[0][2].sName = "HUMAN_DEATH_TIER1_3";

	// Tier 2 Human Death Noise
	m_tHumanDeath[1][0].sFilename = "Data\\SoundAssets\\Humans\\02_Medium\\mHuman_Death_01.mp3";
	m_tHumanDeath[1][0].sName = "HUMAN_DEATH_TIER2_1";
	m_tHumanDeath[1][1].sFilename = "Data\\SoundAssets\\Humans\\02_Medium\\mHuman_Death_02.mp3";
	m_tHumanDeath[1][1].sName = "HUMAN_DEATH_TIER2_2";
	m_tHumanDeath[1][2].sFilename = "Data\\SoundAssets\\Humans\\02_Medium\\mHuman_Death_03.mp3";
	m_tHumanDeath[1][2].sName = "HUMAN_DEATH_TIER2_3";

	// Tier 3 Human Death Noise
	m_tHumanDeath[2][0].sFilename = "Data\\SoundAssets\\Humans\\03_Large\\lHuman_Death_01.mp3";
	m_tHumanDeath[2][0].sName = "HUMAN_DEATH_TIER3_1";
	m_tHumanDeath[2][1].sFilename = "Data\\SoundAssets\\Humans\\03_Large\\lHuman_Death_02.mp3";
	m_tHumanDeath[2][1].sName = "HUMAN_DEATH_TIER3_2";
	m_tHumanDeath[2][2].sFilename = "Data\\SoundAssets\\Humans\\03_Large\\lHuman_Death_03.mp3";
	m_tHumanDeath[2][2].sName = "HUMAN_DEATH_TIER3_3";

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			LoadSound(m_tHumanDeath[i][j].sFilename, m_tHumanDeath[i][j].sName);
		}
	}
}

void CSoundManager::SetupMonsterDeath()
{
	// Monster Death Noise
	m_tMonsterDeath[0].sFilename = "Data\\SoundAssets\\Monsters\\smMonster_Death_01.mp3";
	m_tMonsterDeath[0].sName = "MONSTER_DEATH_TIER1_1";
	m_tMonsterDeath[1].sFilename = "Data\\SoundAssets\\Monsters\\smMonster_Death_01.mp3";
	m_tMonsterDeath[1].sName = "MONSTER_DEATH_TIER2_1";
	m_tMonsterDeath[2].sFilename = "Data\\SoundAssets\\Monsters\\03_Large\\lMonster_Death_01.mp3";
	m_tMonsterDeath[2].sName = "MONSTER_DEATH_TIER3_1";

	for (int i = 0; i < 3; ++i)
	{
		LoadSound(m_tMonsterDeath[i].sFilename, m_tMonsterDeath[i].sName);
	}
}

void CSoundManager::SetupHumanSpawn()
{
	m_tHumanSpawn[0][0].sFilename = "Data\\SoundAssets\\Humans\\01_Small\\sHuman_Spawn_01.mp3";
	m_tHumanSpawn[0][0].sName = "HUMAN_SPAWN_TIER1_1";
	m_tHumanSpawn[0][1].sFilename = "Data\\SoundAssets\\Humans\\01_Small\\sHuman_Spawn_02.mp3";
	m_tHumanSpawn[0][1].sName = "HUMAN_SPAWN_TIER1_2";

	m_tHumanSpawn[1][0].sFilename = "Data\\SoundAssets\\Humans\\02_Medium\\mHuman_Spawn_01.mp3";
	m_tHumanSpawn[1][0].sName = "HUMAN_SPAWN_TIER2_1";
	m_tHumanSpawn[1][1].sFilename = "Data\\SoundAssets\\Humans\\02_Medium\\mHuman_Spawn_02.mp3";
	m_tHumanSpawn[1][1].sName = "HUMAN_SPAWN_TIER2_2";

	m_tHumanSpawn[2][0].sFilename = "Data\\SoundAssets\\Humans\\03_Large\\lHuman_Spawn_01.mp3";
	m_tHumanSpawn[2][0].sName = "HUMAN_SPAWN_TIER3_1";										
	m_tHumanSpawn[2][1].sFilename = "Data\\SoundAssets\\Humans\\03_Large\\lHuman_Spawn_02.mp3";
	m_tHumanSpawn[2][1].sName = "HUMAN_SPAWN_TIER3_2";

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			LoadSound(m_tHumanSpawn[i][j].sFilename, m_tHumanSpawn[i][j].sName);
		}
	}
}

void CSoundManager::SetupMonsterSpawn()
{
	m_tMonsterSpawn[0].sFilename = "Data\\SoundAssets\\Monsters\\01_Small\\sMonster_Spawn_01.mp3";
	m_tMonsterSpawn[0].sName = "MONSTER_SPAWN_TIER1_1";
	m_tMonsterSpawn[1].sFilename = "Data\\SoundAssets\\Monsters\\02_Medium\\mMonster_Spawn_01.mp3";
	m_tMonsterSpawn[1].sName = "MONSTER_SPAWN_TIER2_1";
	m_tMonsterSpawn[2].sFilename = "Data\\SoundAssets\\Monsters\\03_Large\\lMonster_Spawn_01.mp3";
	m_tMonsterSpawn[2].sName = "MONSTER_SPAWN_TIER3_1";

	for (int i = 0; i < 3; ++i)
	{
		LoadSound(m_tMonsterSpawn[i].sFilename, m_tMonsterSpawn[i].sName);
	}
}

void CSoundManager::SetupConflict()
{
	m_tConflict[0].sFilename = "Data\\SoundAssets\\Misc\\Conflict_01.mp3";
	m_tConflict[0].sName = "CONFLICT_1";
	m_tConflict[1].sFilename = "Data\\SoundAssets\\Misc\\Conflict_02.mp3";
	m_tConflict[1].sName = "CONFLICT_2";
	m_tConflict[2].sFilename = "Data\\SoundAssets\\Misc\\Conflict_03.mp3";
	m_tConflict[2].sName = "CONFLICT_3";
	m_tConflict[3].sFilename = "Data\\SoundAssets\\Misc\\Conflict_04.mp3";
	m_tConflict[3].sName = "CONFLICT_4";
	m_tConflict[4].sFilename = "Data\\SoundAssets\\Misc\\Conflict_05.mp3";
	m_tConflict[4].sName = "CONFLICT_5";

	for (int i = 0; i < 5; ++i)
	{
		LoadSound(m_tConflict[i].sFilename, m_tConflict[i].sName);
	}
}

void CSoundManager::SetupMonsterAmbiance()
{
	// Tier 1 Monster Monster Noise
	m_tMonsterAmbiance[0][0].sFilename = "Data\\SoundAssets\\Monsters\\01_Small\\sMonster_Ambiance_01.mp3";
	m_tMonsterAmbiance[0][0].sName = "MONSTER_AMBIANCE_TIER1_1";
	m_tMonsterAmbiance[0][1].sFilename = "Data\\SoundAssets\\Monsters\\01_Small\\sMonster_Ambiance_02.mp3";
	m_tMonsterAmbiance[0][1].sName = "MONSTER_AMBIANCE_TIER1_1";
	m_tMonsterAmbiance[0][2].sFilename = "Data\\SoundAssets\\Monsters\\01_Small\\sMonster_Ambiance_03.mp3";
	m_tMonsterAmbiance[0][2].sName = "MONSTER_AMBIANCE_TIER1_1";

	// Tier 2 Monster Monster Noise
	m_tMonsterAmbiance[1][0].sFilename = "Data\\SoundAssets\\Monsters\\02_Medium\\sMonster_Ambiance_01.mp3";
	m_tMonsterAmbiance[1][0].sName = "MONSTER_AMBIANCE_TIER2_1";
	m_tMonsterAmbiance[1][1].sFilename = "Data\\SoundAssets\\Monsters\\01_Small\\sMonster_Ambiance_02.mp3";
	m_tMonsterAmbiance[1][1].sName = "MONSTER_AMBIANCE_TIER2_1";
	m_tMonsterAmbiance[1][2].sFilename = "Data\\SoundAssets\\Monsters\\01_Small\\sMonster_Ambiance_03.mp3";
	m_tMonsterAmbiance[1][2].sName = "MONSTER_AMBIANCE_TIER2_1";

	// Tier 3 Monster Monster Noise
	m_tMonsterAmbiance[2][0].sFilename = "Data\\SoundAssets\\Monsters\\01_Small\\sMonster_Ambiance_01.mp3";
	m_tMonsterAmbiance[2][0].sName = "MONSTER_AMBIANCE_TIER3_1";
	m_tMonsterAmbiance[2][1].sFilename = "Data\\SoundAssets\\Monsters\\01_Small\\sMonster_Ambiance_02.mp3";
	m_tMonsterAmbiance[2][1].sName = "MONSTER_AMBIANCE_TIER3_1";
	m_tMonsterAmbiance[2][2].sFilename = "Data\\SoundAssets\\Monsters\\01_Small\\sMonster_Ambiance_03.mp3";
	m_tMonsterAmbiance[2][2].sName = "MONSTER_AMBIANCE_TIER3_1";

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			LoadSound(m_tMonsterAmbiance[i][j].sFilename, m_tMonsterAmbiance[i][j].sName);
		}
	}
}

void CSoundManager::SetupHumanAmbiance()
{
	// Tier 1 Monster Monster Noise
	m_tHumanAmbiance[0][0].sFilename = "Data\\SoundAssets\\Monsters\\01_Small\\sMonster_Ambiance_01.mp3";
	m_tHumanAmbiance[0][0].sName = "MONSTER_AMBIANCE_TIER1_1";
	m_tHumanAmbiance[0][1].sFilename = "Data\\SoundAssets\\Monsters\\01_Small\\sMonster_Ambiance_02.mp3";
	m_tHumanAmbiance[0][1].sName = "MONSTER_AMBIANCE_TIER1_1";

	// Tier 2 Monster Monster Noise
	m_tHumanAmbiance[1][0].sFilename = "Data\\SoundAssets\\Monsters\\02_Medium\\sMonster_Ambiance_01.mp3";
	m_tHumanAmbiance[1][0].sName = "MONSTER_AMBIANCE_TIER2_1";
	m_tHumanAmbiance[1][1].sFilename = "Data\\SoundAssets\\Monsters\\01_Small\\sMonster_Ambiance_02.mp3";
	m_tHumanAmbiance[1][1].sName = "MONSTER_AMBIANCE_TIER2_1";

	// Tier 3 Monster Monster Noise
	m_tHumanAmbiance[2][0].sFilename = "Data\\SoundAssets\\Monsters\\01_Small\\sMonster_Ambiance_01.mp3";
	m_tHumanAmbiance[2][0].sName = "MONSTER_AMBIANCE_TIER3_1";
	m_tHumanAmbiance[2][1].sFilename = "Data\\SoundAssets\\Monsters\\01_Small\\sMonster_Ambiance_02.mp3";
	m_tHumanAmbiance[2][1].sName = "MONSTER_AMBIANCE_TIER3_1";

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			LoadSound(m_tHumanAmbiance[i][j].sFilename, m_tHumanAmbiance[i][j].sName);
		}
	}
}

void CSoundManager::SendCommand(std::string _sCommand)
{
	// Convert command to wide string
	std::wstring sWideSoundCommand;
	sWideSoundCommand.assign(_sCommand.begin(), _sCommand.end());

	// Send the command
	mciSendString(sWideSoundCommand.c_str(), NULL, 0, NULL);
}

void CSoundManager::LoadSound(std::string _sFilename, std::string _sName)
{
	std::string sSoundCommand = "Open " + _sFilename + "\ alias " + _sName;

	SendCommand(sSoundCommand);
}

void CSoundManager::PlaySound(std::string _sSound, bool _bRepeat)
{
	std::string sCommand;
	sCommand = "Play " + _sSound + " from 0";

	if (_bRepeat)
	{
		sCommand.append(" repeat");
	}

	// Play sound
    SendCommand(sCommand);
}

void CSoundManager::CloseSound(std::string _sSound)
{
	std::string sCommand;
	sCommand = "Close " + _sSound;

	SendCommand(sCommand);
}

void CSoundManager::StopSound(std::string _sSound)
{
	std::string sCommand;
	sCommand = "Stop " + _sSound;

	SendCommand(sCommand);
}

void CSoundManager::ChangeVolume(std::string _sSound, int _iVolume)
{
	std::string sCommand;
	sCommand = "setaudio " + _sSound + " volume to ";

	std::ostringstream oss;
	oss << _iVolume;
	sCommand.append(oss.str());

	SendCommand(sCommand);
}

std::string CSoundManager::CreateNickName(bool _bHuman, int _iTier, std::string _sEvent, int _iBot, int _iTop)
{
	int iSound = RandomInt(_iBot, _iTop);

	std::ostringstream oss;
	
	if (_bHuman)
	{
		oss << "HUMAN_";
	}
	else
	{
		oss << "MONSTER_";
	}

	oss << _sEvent;
	oss << "_";

	std::ostringstream iss;
	iss << _iTier;

	oss << "TIER";
	oss << iss.str();
	oss << "_";

	iss.str("");
	iss.clear();
	iss << iSound;

	oss << iss.str();

	return (oss.str());
}

void CSoundManager::PlayDeathNoise(bool _bHuman, int _iTier)
{
	int iTop, iBot;

	if (_bHuman)
	{
		iBot = 1;
		iTop = 3;
	}
	else
	{
		iBot = 1;
		iTop = 1;
	}

	std::string sName = CreateNickName(_bHuman, _iTier, "DEATH", iBot, iTop);

	StopSound(sName);
	PlaySound(sName, false);

	ChangeVolume(sName, 200);
}

void CSoundManager::PlaySpawnNoise(bool _bHuman, int _iTier)
{
	int iTop, iBot;

	if (_bHuman)
	{
		iBot = 1;
		iTop = 2;
	}
	else
	{
		iBot = 1;
		iTop = 1;
	}

	std::string sName = CreateNickName(_bHuman, _iTier, "SPAWN", iBot, iTop);

	StopSound(sName);
	PlaySound(sName, false);

	ChangeVolume(sName, 180);
}

void CSoundManager::PlayCastleDamage()
{
	StopSound(m_tCastleDamage.sName);
	PlaySound(m_tCastleDamage.sName, true);

	ChangeVolume(m_tCastleDamage.sName, 250);
}

void CSoundManager::StartConflict()
{
	int iConflict = RandomInt(0, 4);

	for (int i = 0; i < 5; ++i)
	{
		StopSound(m_tConflict[i].sName);
	}

	PlaySound(m_tConflict[iConflict].sName, true);

	ChangeVolume(m_tConflict[iConflict].sName, 75);
}

void CSoundManager::EndConflict()
{
	for (int i = 0; i < 5; ++i)
	{
		StopSound(m_tConflict[i].sName);
	}
}

void CSoundManager::FinishGame()
{
	StopSound("CASTLE_DAMAGE");
	StopSound("GAME_MUSIC");
	PlaySound("MENU_MUSIC", true);
}

void CSoundManager::PlayWin()
{
	StopSound("WIN_GAME");
	PlaySound("WIN_GAME", false);
}

void CSoundManager::PlayLose()
{
	StopSound("LOSE_GAME");
	PlaySound("LOSE_GAME", false);
}

void CSoundManager::PlayAmbiance(bool _bHuman, int _iTier)
{
	int iTop, iBot;
	iBot = 1;

	if (_bHuman)
	{
		iTop = 2;
	}
	else
	{
		iTop = 3;
	}

	std::string sName = CreateNickName(_bHuman, _iTier, "AMBIANCE", iBot, iTop);

	StopSound(sName);
	PlaySound(sName, false);

	ChangeVolume(sName, 200);
}

void CSoundManager::PlayArrow()
{
	StopSound("ARROW");
	PlaySound("ARROW", false);
}