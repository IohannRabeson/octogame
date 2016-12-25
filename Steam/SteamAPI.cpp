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
	bool bRet = SteamAPI_Init();
	if (bRet)
	{
		m_iAppID = SteamUtils()->GetAppID();
		requestStats();
		int32 deathCount = 0;
		SteamUserStats()->GetStat("STAT_DEATH", &deathCount);
		std::cout << "Deaths : " << deathCount << std::endl;
		clearAllAchievements();
	}
}

SteamAPI::~SteamAPI(void)
{
	// Shutdown Steam TODO: Causing segfault ?
	//SteamAPI_Shutdown();
}

void SteamAPI::update(Progress::data & data)
{
	(void)data;
	SteamAPI_RunCallbacks();
	updateStats(data);
	unlockAchievements(data);
}

void SteamAPI::clearAllAchievements(void)
{
	for (std::size_t i = 0; i < 4; i++)
		SteamUserStats()->ClearAchievement(g_Achievements[i].m_pchAchievementID);
}

void SteamAPI::unlockAchievements(Progress::data & data)
{
	setAchievement(ACH_RUN_THE_GAME);

	std::cout << data.deathCount << std::endl;
	if (data.deathCount >= 13)
		setAchievement(ACH_DEATH_1);
}

void SteamAPI::updateStats(Progress::data & data)
{
	SteamUserStats()->SetStat("STAT_DEATH", static_cast<int32>(data.deathCount));
	SteamUserStats()->StoreStats();

}

bool SteamAPI::setAchievement(EAchievements achievement)
{
	// Have we received a call back from Steam yet and is the achievement false ?
	if (m_bInitialized && !g_Achievements[achievement].m_bAchieved)
	{
		if (SteamUserStats()->SetAchievement(g_Achievements[achievement].m_pchAchievementID))
		{
			std::cout << g_Achievements[achievement].m_pchAchievementID << std::endl;
			std::cout << g_Achievements[achievement].m_rgchName << std::endl;
			std::cout << g_Achievements[achievement].m_rgchDescription << std::endl;
			g_Achievements[achievement].m_bAchieved = true;
		}
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
			std::cout << buffer << std::endl;
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
			std::cout << "Stored stats for Steam\n" << std::endl;
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
		std::cout << "Stored Achievement " << pCallback->m_rgchAchievementName << " for Steam\n" << std::endl;
	}
}

