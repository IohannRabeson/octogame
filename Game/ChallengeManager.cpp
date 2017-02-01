#include "ChallengeManager.hpp"
#include "Progress.hpp"
#include "Challenges.hpp"

std::unique_ptr<ChallengeManager> ChallengeManager::m_instance = nullptr;

ChallengeManager::ChallengeManager(void) :
	m_keyEntrance(false),
	m_keyCapacity(false),
	m_keyElevator(false),
	m_keyJump(false),
	m_keyPressed(false),
	m_keyLock(false)
{
	InputListener::addInputListener();
}

ChallengeManager::~ChallengeManager(void)
{
	//InputListener::removeInputListener();
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
/*	if (1)
		updateAquarium(biome, frametime);
	else*/ if (biome.getType() == ABiome::Type::Random && Progress::getInstance().isGameFinished())
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

void ChallengeManager::updateAquarium(ABiome & biome, sf::Time frametime)
{
	if (m_keyPressed && !m_keyLock)
	{
		for (auto & it : m_challenges)
		{
			if (!it.second->enable() && biome.randomBool(0.1f))
			{
				m_keyLock = true;
				it.second->startGlitchManual(biome);
				break;
			}
		}
	}

	for (auto & it : m_challenges)
		it.second->updateGlitchManual(frametime, biome);
}

bool ChallengeManager::onInputPressed(InputListener::OctoKeys const & key)
{
	switch (key)
	{
		case OctoKeys::Entrance:
		case OctoKeys::Capacity:
		case OctoKeys::Elevator:
		case OctoKeys::Jump:
		case OctoKeys::GroundLeft:
		case OctoKeys::GroundRight:
			m_keyPressed = true;
			break;
		default:
			break;
	}
	return true;
}

bool ChallengeManager::onInputReleased(InputListener::OctoKeys const & key)
{
	switch (key)
	{
		case OctoKeys::Entrance:
		case OctoKeys::Capacity:
		case OctoKeys::Elevator:
		case OctoKeys::Jump:
		case OctoKeys::GroundLeft:
		case OctoKeys::GroundRight:
			m_keyLock = false;
			m_keyPressed = false;
			break;
		default:
			break;
	}
	return true;
}
