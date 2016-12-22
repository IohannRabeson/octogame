#include "SteamAPI.hpp"

SteamAPI::SteamAPI(void)
{
	// Initialize Steam
	bool bRet = SteamAPI_Init();
	// Create the SteamAchievements object if Steam was successfully initialized
	if (bRet)
	{
		g_SteamAchievements = new CSteamAchievements(g_Achievements, 4);
	}
}

SteamAPI::~SteamAPI(void)
{
	// Shutdown Steam
	SteamAPI_Shutdown();
	// Delete the SteamAchievements object
	if (g_SteamAchievements)
		delete g_SteamAchievements;
}

void SteamAPI::update(void)
{
	
}

void SteamAPI::triggerAchievement(EAchievements achievementDone)
{
	(void)achievementDone;
	if (g_SteamAchievements)
		g_SteamAchievements->SetAchievement("TEST");
}
