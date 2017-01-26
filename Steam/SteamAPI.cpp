#include "SteamAPI.hpp"
#include <iostream>

SteamAPI::SteamAPI(void) :
	m_iAppID(0),
	m_iNumAchievements(4),
	m_bInitialized(false),
	m_CallbackUserStatsReceived(this, &SteamAPI::OnUserStatsReceived),
	m_CallbackUserStatsStored(this, &SteamAPI::OnUserStatsStored),
	m_CallbackAchievementStored(this, &SteamAPI::OnAchievementStored)
{
	m_initRet = SteamAPI_Init();
	if (m_initRet)
	{
		m_iAppID = SteamUtils()->GetAppID();
		requestStats();
		reset();
	}
}

SteamAPI::~SteamAPI(void)
{
	// Shutdown Steam TODO: Causing segfault ?
	//SteamAPI_Shutdown();
}

void SteamAPI::reset(void)
{
	SteamUserStats()->SetStat("STAT_TIME_PLAYED", static_cast<int32>(0u));
	SteamUserStats()->SetStat("STAT_COUNT_LAUNCH", static_cast<int32>(0u));
	SteamUserStats()->SetStat("STAT_COUNT_RANDOM_PORTAL", static_cast<int32>(0u));
	SteamUserStats()->SetStat("STAT_COUNT_NPC", static_cast<int32>(0u));
	SteamUserStats()->SetStat("STAT_COUNT_JUMP", static_cast<int32>(0u));
	SteamUserStats()->SetStat("STAT_COUNT_DEATH", static_cast<int32>(0u));
	SteamUserStats()->SetStat("STAT_COUNT_NANOROBOT", static_cast<int32>(0u));
	SteamUserStats()->SetStat("STAT_COUNT_SPIRIT", static_cast<int32>(0u));
	SteamUserStats()->SetStat("STAT_END_BLUE", static_cast<int32>(0u));
	SteamUserStats()->SetStat("STAT_END_RED", static_cast<int32>(0u));
	//SteamUserStats()->SetStat("STAT_FAT_NPC", static_cast<int32>(data.isFatNpc));
	SteamUserStats()->SetStat("STAT_SPACESHIP_REPAIR", static_cast<int32>(0u));
	SteamUserStats()->SetStat("STAT_LONG_INTRO", static_cast<int32>(0u));
	SteamUserStats()->SetStat("STAT_TRY_TO_ESCAPE", static_cast<int32>(0u));
	SteamUserStats()->SetStat("STAT_DOORS", static_cast<int32>(0u));
	SteamUserStats()->SetStat("STAT_FINISH_HARD", static_cast<int32>(0u));
	SteamUserStats()->SetStat("STAT_FINISH_ZERO_DEATH", static_cast<int32>(0u));
	SteamUserStats()->StoreStats();

	for (std::size_t i = 0; i < EAchievements::COUNT_ACH; i++)
		SteamUserStats()->ClearAchievement(g_Achievements[i].m_pchAchievementID);

	SteamAPI_RunCallbacks();
}

void SteamAPI::update(Progress::data & data)
{
	if (m_initRet)
	{
		SteamAPI_RunCallbacks();
		updateStats(data);
		unlockAchievements(data);
	}
}

void SteamAPI::unlockAchievements(Progress::data & data)
{
	(void)data;
//	setAchievement(ACH_RUN_THE_GAME);
//	if (data.deathCount >= 13)
//		setAchievement(ACH_DEATH_1);
}

void SteamAPI::updateStats(Progress::data & data)
{
	SteamUserStats()->SetStat("STAT_TIME_PLAYED", static_cast<int32>(data.timePlayed));
	SteamUserStats()->SetStat("STAT_COUNT_LAUNCH", static_cast<int32>(data.launchCount));
	SteamUserStats()->SetStat("STAT_COUNT_RANDOM_PORTAL", static_cast<int32>(data.activatedMonolith));
	SteamUserStats()->SetStat("STAT_COUNT_NPC", static_cast<int32>(data.npcCount));
	SteamUserStats()->SetStat("STAT_COUNT_JUMP", static_cast<int32>(data.jumpCount));
	SteamUserStats()->SetStat("STAT_COUNT_DEATH", static_cast<int32>(data.deathCount));
	SteamUserStats()->SetStat("STAT_COUNT_NANOROBOT", static_cast<int32>(data.nanoRobotCount));
	SteamUserStats()->SetStat("STAT_COUNT_SPIRIT", static_cast<int32>(data.spiritCount));
	SteamUserStats()->SetStat("STAT_END_BLUE", static_cast<int32>(data.isBlueEnd));
	SteamUserStats()->SetStat("STAT_END_RED", static_cast<int32>(data.isRedEnd));
	//SteamUserStats()->SetStat("STAT_FAT_NPC", static_cast<int32>(data.isFatNpc));
	SteamUserStats()->SetStat("STAT_SPACESHIP_REPAIR", static_cast<int32>(data.spaceShipRepair));
	SteamUserStats()->SetStat("STAT_LONG_INTRO", static_cast<int32>(data.longIntro));
	SteamUserStats()->SetStat("STAT_TRY_TO_ESCAPE", static_cast<int32>(data.tryToEscape));
	SteamUserStats()->SetStat("STAT_DOORS", static_cast<int32>(data.doorFound));
	SteamUserStats()->SetStat("STAT_FINISH_HARD", static_cast<int32>(data.isGameFinishedHard));
	SteamUserStats()->SetStat("STAT_FINISH_ZERO_DEATH", static_cast<int32>(data.isGameFinishedZeroDeath));

	SteamUserStats()->StoreStats();
}

bool SteamAPI::setAchievement(EAchievements achievement)
{
	// Have we received a call back from Steam yet and is the achievement false ?
	if (m_bInitialized && !g_Achievements[achievement].m_bAchieved)
	{
		if (SteamUserStats()->SetAchievement(g_Achievements[achievement].m_pchAchievementID))
			g_Achievements[achievement].m_bAchieved = true;
		return SteamUserStats()->StoreStats();
	}
	// If not then we can't set achievements yet
	return false;
}

bool SteamAPI::requestStats()
{
	if (NULL == SteamUserStats() || NULL == SteamUser())
		return false;
	if (!SteamUser()->BLoggedOn())
		return false;
	return SteamUserStats()->RequestCurrentStats();
}

/*
** CALLBACKS
*/

void SteamAPI::OnUserStatsReceived( UserStatsReceived_t *pCallback )
{
	// we may get callbacks for other games' stats arriving, ignore them
	if (m_iAppID == static_cast<int64>(pCallback->m_nGameID))
	{
		if (k_EResultOK == pCallback->m_eResult)
		{
			std::cout << "Received stats and achievements from Steam\n" << std::endl;
			m_bInitialized = true;

			// load achievements
			for (int iAch = 0; iAch < m_iNumAchievements; ++iAch)
			{
				Achievement_t &ach = g_Achievements[iAch];

				SteamUserStats()->GetAchievement(ach.m_pchAchievementID, &ach.m_bAchieved);
				_snprintf(ach.m_rgchName, sizeof(ach.m_rgchName), "%s", SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID, "name"));
				_snprintf(ach.m_rgchDescription, sizeof(ach.m_rgchDescription), "%s", SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID, "desc"));
			}
		}
		else
		{
			char buffer[128];
			_snprintf(buffer, 128, "requestStats - failed, %d\n", pCallback->m_eResult);
			//std::cout << buffer << std::endl;
		}
	}
}

void SteamAPI::OnUserStatsStored(UserStatsStored_t *pCallback)
{
	// we may get callbacks for other games' stats arriving, ignore them
	if (m_iAppID == static_cast<int64>(pCallback->m_nGameID))
	{
		if (k_EResultOK == pCallback->m_eResult)
		{
			//std::cout << "Stored stats for Steam\n" << std::endl;
		}
		else
		{
			char buffer[128];
			_snprintf( buffer, 128, "StatsStored - failed, %d\n", pCallback->m_eResult );
			std::cout << buffer << std::endl;
		}
	}
}

void SteamAPI::OnAchievementStored( UserAchievementStored_t *pCallback )
{
	// we may get callbacks for other games' stats arriving, ignore them
	if (m_iAppID == static_cast<int64>(pCallback->m_nGameID))
	{
		//std::cout << "Stored Achievement " << pCallback->m_rgchAchievementName << " for Steam\n" << std::endl;
	}
}

