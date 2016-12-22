#include "Steamworks.hpp"
#include <steam_api.h>

#include <iostream>

std::unique_ptr<Steamworks> Steamworks::m_instance = nullptr;

Steamworks::Steamworks(void)
{
	if (SteamAPI_Init())
	{
		std::cout << "Init " << SteamFriends()->GetPersonaName() << " steam success" << std::endl;
		clearAchievement();
		setAchievement("TEST");

		if (SteamUserStats()->UploadLeaderboardScore(SteamUserStats()->FindLeaderboard("STAT"),
												k_ELeaderboardUploadScoreMethodKeepBest,
												10, NULL, 0))
		{
			std::cout << "Leaderboard success" << std::endl;
		}
		else
			std::cout << "Leaderboard failed" << std::endl;
	}
	else
		std::cout << "Init steam fail" << std::endl;
}

void Steamworks::initAPI(void)
{
}

void Steamworks::clearAchievement(void)
{
	if (SteamUserStats()->ClearAchievement("TEST"))
		std::cout << "Clear success" << std::endl;
	else
		std::cout << "Clear failed" << std::endl;
}

void Steamworks::setAchievement(std::string name)
{
	if (SteamUserStats()->SetAchievement(name.c_str()))
		std::cout << "Set " << name << " success." << std::endl;
	else
		std::cout << "Set " << name << " failed." << std::endl;
}

Steamworks & Steamworks::getInstance()
{
	if (m_instance == nullptr)
	{
		m_instance.reset(new Steamworks());
	}
	return *m_instance;
}

