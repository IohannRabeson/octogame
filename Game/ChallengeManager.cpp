#include "ChallengeManager.hpp"
#include "Progress.hpp"
#include "Challenges.hpp"

std::unique_ptr<ChallengeManager> ChallengeManager::m_instance = nullptr;

ChallengeManager::ChallengeManager(void)
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
}

void ChallengeManager::update(ABiome & biome, sf::Vector2f const & position, sf::Time frametime)
{
	for (auto & it : m_challenges)
	{
		if (biome.getType() == it.second->getBiomeType() || biome.getType() == ABiome::Type::Random)
		{
			if (!it.second->enable() && Progress::getInstance().isValidateChallenge(it.first))
				it.second->startGlitch(biome);
			it.second->update(frametime, biome, position);
		}
	}
}

ChallengeManager::AChallenge & ChallengeManager::getEffect(Effect effect)
{
	return (*m_challenges[effect]);
}
