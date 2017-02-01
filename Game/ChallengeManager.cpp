#include "ChallengeManager.hpp"
#include "Progress.hpp"
#include "Challenges.hpp"

std::unique_ptr<ChallengeManager> ChallengeManager::m_instance = nullptr;

ChallengeManager::ChallengeManager(void) :
	m_launchManualGlitch(false),
	m_lockManualGlitch(false)
{
}

ChallengeManager::~ChallengeManager(void)
{
}

ChallengeManager & ChallengeManager::getInstance(void)
{
	if (m_instance == nullptr)
		m_instance.reset(new ChallengeManager());
	return *m_instance;
}

void ChallengeManager::reset(void)
{
	m_challenges.clear();
	m_challenges[Effect::Duplicate].reset(new ChallengeDuplicate());
	m_challenges[Effect::Persistence].reset(new ChallengePersistence());
	m_challenges[Effect::Pixelate].reset(new ChallengePixelate());
	m_challenges[Effect::Displacement].reset(new ChallengeDisplacement());
	m_challenges[Effect::Blur].reset(new ChallengeBlur());
}

void ChallengeManager::update(ABiome & biome, sf::Vector2f const & position, sf::Time frametime)
{
	if (Progress::getInstance().canUseBalle())
		updateManualGlitch(biome, frametime);
	else if (biome.getType() == ABiome::Type::Random && Progress::getInstance().isGameFinished())
		updateRandom(biome, position, frametime);
	else
		updateNormal(biome, position, frametime);
}

ChallengeManager::AChallenge & ChallengeManager::getEffect(Effect effect)
{
	return (*m_challenges[effect]);
}

void ChallengeManager::updateNormal(ABiome & biome, sf::Vector2f const & position, sf::Time frametime)
{
	for (auto & it : m_challenges)
	{
		if (biome.getType() == it.second->getBiomeType())
		{
			if (!it.second->enable() && Progress::getInstance().isValidateChallenge(it.first))
				it.second->startGlitch(biome);
			it.second->update(frametime, biome, position);
		}
	}
}

void ChallengeManager::updateRandom(ABiome & biome, sf::Vector2f const & position, sf::Time frametime)
{
	for (auto & it : m_challenges)
	{
		if (!it.second->enable())
			it.second->startGlitch(biome);
		it.second->update(frametime, biome, position);
	}
}

void ChallengeManager::updateManualGlitch(ABiome & biome, sf::Time frametime)
{
	if (m_launchManualGlitch && !m_lockManualGlitch)
	{
		for (auto & it : m_challenges)
		{
			if (!it.second->enable())
			{
				m_lockManualGlitch = true;
				it.second->startGlitchManual(biome);
				break;
			}
		}
	}

	for (auto & it : m_challenges)
		it.second->updateGlitchManual(frametime, biome);
}

bool ChallengeManager::launchManualGlitch(bool isLaunch)
{
	m_launchManualGlitch = isLaunch;
	if (isLaunch == false)
		m_lockManualGlitch = false;
	return m_lockManualGlitch;
}
