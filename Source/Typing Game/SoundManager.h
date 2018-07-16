#pragma once

#ifndef _CSOUNDMANAGER_H_
#define _CSOUNDMANAGER_H_

// Library Includes:

// Local Includes:
#include "Utility.h"

// Class Declaration:
class CSoundManager
{
	// class functions
	public:
		CSoundManager();
		~CSoundManager();

		int RandomInt(unsigned int _iMin, unsigned int _iMax)
		{
			return ((rand() % ((_iMax + 1) - _iMin)) + _iMin);
		}

		void Initialise();
		void SetupHumanDeath();
		void SetupMonsterDeath();
		void SetupHumanSpawn();
		void SetupMonsterSpawn();
		void SetupConflict();
		void SetupMonsterAmbiance();
		void SetupHumanAmbiance();

		void FinishGame();

		void SendCommand(std::string _sCommand);

		void LoadSound(std::string _sFilename, std::string _sName);
		void PlaySound(std::string _sSound, bool _bRepeat);
		void StopSound(std::string _sSound);
		void CSoundManager::CloseSound(std::string _sSound);
		void ChangeVolume(std::string _sSound, int _iVolume);
		std::string CreateNickName(bool _bHuman, int _iTier, std::string _sEvent, int _iBot, int _iTop);

		void PlayDeathNoise(bool _bHuman, int _iTier);
		void PlaySpawnNoise(bool _bHuman, int _iTier);
		void PlayAmbiance(bool _bHuman, int _iTier);

		void PlayArrow();
		void PlayCastleDamage();
		void PlayWin();
		void PlayLose();

		void StartConflict();
		void EndConflict();

	protected:

	private:

	// class members
	public:

	protected:

	private:
		SOUNDINFO m_tMenuMusic;
		SOUNDINFO m_tGameMusic;
		SOUNDINFO m_tCastleDamage;
		SOUNDINFO m_tLoseGame;
		SOUNDINFO m_tWinGame;
		SOUNDINFO m_tArrow;

		SOUNDINFO m_tConflict[5];

		SOUNDINFO m_tHumanDeath[3][3];
		SOUNDINFO m_tMonsterDeath[3];

		SOUNDINFO m_tHumanSpawn[3][2];
		SOUNDINFO m_tMonsterSpawn[3];

		SOUNDINFO m_tHumanAmbiance[3][2];
		SOUNDINFO m_tMonsterAmbiance[3][3];
};

#endif // _CSOUNDMANAGER_H_