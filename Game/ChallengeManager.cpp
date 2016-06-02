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
}

void ChallengeManager::update(ABiome & biome, sf::Vector2f const & position, sf::Time frametime)
{
	for (auto & it : m_challenges)
	{
		if (biome.getType() == it.second->getBiomeType() || biome.getType() == ABiome::Type::Random)
		{
			if (!it.second->enable() && Progress::getInstance().isValidateChallenge(it.first))
			{
				if (it.second->canStartGlitch())
				{
					it.second->setGlitch(true);
					it.second->setIntensity(biome.randomFloat(0.033f, 0.16f));
					it.second->setDuration(biome.randomFloat(0.25f, 0.75f));
					it.second->start();
				}
			}
			it.second->update(frametime, biome, position);
		}
	}
}

ChallengeManager::AChallenge & ChallengeManager::getEffect(Effect effect)
{
	return (*m_challenges[effect]);
}
