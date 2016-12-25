#ifndef STEAM_API_HPP
# define STEAM_API_HPP

#include "steam_api.h"
#include "Progress.hpp"

# define _ACH_ID( id, name ) { id, #id, name, "", 0, 0 }
struct Achievement_t
{
	int			m_eAchievementID;
	const char	*m_pchAchievementID;
	char		m_rgchName[128];
	char		m_rgchDescription[256];
	bool		m_bAchieved;
	int			m_iIconImage;
};

class SteamAPI
{
	// Defining our achievements
	enum EAchievements
	{
		ACH_RUN_THE_GAME = 0,
		ACH_TEST = 1,
		ACH_DEATH_1 = 2,
		ACH_TRAVEL_FAR_SINGLE = 3,
	};
	
	// Achievement array which will hold data about the achievements and their state
	Achievement_t g_Achievements[4] =
	{
		_ACH_ID( ACH_RUN_THE_GAME, "ACH_RUN_THE_GAME" ),
		_ACH_ID( ACH_TEST, "ACH_TEST" ),
		_ACH_ID( ACH_DEATH_1, "ACH_DEATH_1" ),
		_ACH_ID( ACH_TRAVEL_FAR_SINGLE, "Orbiter" ),
	};
	
public:
	SteamAPI(void);
	virtual ~SteamAPI(void);

	void update(Progress::data & data);
	void clearAllAchievements(void);
	void unlockAchievements(Progress::data & data);
	void updateStats(Progress::data & data);
	bool setAchievement(EAchievements achievement);

	bool requestStats();

private:
	int64 m_iAppID; // Our current AppID
	int m_iNumAchievements; // The number of Achievements
	bool m_bInitialized; // Have we called Request stats and received the callback?

public:
	STEAM_CALLBACK( SteamAPI, OnUserStatsReceived, UserStatsReceived_t, 
		m_CallbackUserStatsReceived );
	STEAM_CALLBACK( SteamAPI, OnUserStatsStored, UserStatsStored_t, 
		m_CallbackUserStatsStored );
	STEAM_CALLBACK( SteamAPI, OnAchievementStored, 
		UserAchievementStored_t, m_CallbackAchievementStored );
};

#endif
