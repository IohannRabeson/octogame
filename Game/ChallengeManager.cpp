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
	InputListener::removeInputListener();
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
	for (auto & it : m_challenges)
	{
		if (biome.getType() == it.second->getBiomeType())
		{
			if (!it.second->enable() && Progress::getInstance().isValidateChallenge(it.first))
				it.second->startGlitch(biome);
			it.second->update(frametime, biome, position);
		}
		/*
		else if (biome.getType() == ABiome::Type::Random && Progress::getInstance().isGameFinished())
		{
			if (!it.second->enable())
				it.second->startGlitch(biome);
			it.second->update(frametime, biome, position);
		}
		*/
	}
	udapteKey(biome, frametime);
}

ChallengeManager::AChallenge & ChallengeManager::getEffect(Effect effect)
{
	return (*m_challenges[effect]);
}

void ChallengeManager::udapteKey(ABiome & biome, sf::Time frametime)
{
	for (auto & it : m_challenges)
	{
		if (!it.second->enable() && m_keyPressed && !m_keyLock)
		{
			m_keyLock = true;
			it.second->startGlitchManual(biome);
			break;
		}
	/*
	for (auto & it : m_challenges)
	{
		switch (it.second->getBiomeType())
		{
			case ABiome::Type::Desert:
			{
				if (m_keyEntrance)
				{
					if (!it.second->enable())
						it.second->startGlitchManual(biome);
				}
				else if (it.second->enable())
					it.second->stopGlitchManual();
				break;
			}
			case ABiome::Type::Ice:
			{
				if (m_keyCapacity)
				{
					if (!it.second->enable())
						it.second->startGlitchManual(biome);
				}
				else if (it.second->enable())
					it.second->stopGlitchManual();
				break;
			}
			case ABiome::Type::Jungle:
			{
				if (m_keyElevator)
				{
					if (!it.second->enable())
						it.second->startGlitchManual(biome);
				}
				else if (it.second->enable())
					it.second->stopGlitchManual();
				break;
			}
			case ABiome::Type::Water:
			{
				if (m_keyJump)
				{
					if (!it.second->enable())
						it.second->startGlitchManual(biome);
				}
				else if (it.second->enable())
					it.second->stopGlitchManual();
				break;
			}
			default:
				break;
		}
		*/
		it.second->updateGlitchManual(frametime, biome);
	}
}

bool ChallengeManager::onInputPressed(InputListener::OctoKeys const & key)
{
	(void)key;
	m_keyPressed = true;
	/*
	switch (key)
	{
		case OctoKeys::Entrance:
			m_keyEntrance = true;
			break;
		case OctoKeys::Capacity:
			m_keyCapacity = true;
			break;
		case OctoKeys::Elevator:
			m_keyElevator = true;
			break;
		case OctoKeys::Jump:
			m_keyJump = true;
			break;
		default:
			break;
	}
	*/
	return true;
}

bool ChallengeManager::onInputReleased(InputListener::OctoKeys const & key)
{
	(void)key;
	// TODO : m_keyLock = false;
	m_keyLock = true;
	m_keyPressed = false;
	/*
	switch (key)
	{
		case OctoKeys::Entrance:
			m_keyEntrance = false;
			break;
		case OctoKeys::Capacity:
			m_keyCapacity = false;
			break;
		case OctoKeys::Elevator:
			m_keyElevator = false;
			break;
		case OctoKeys::Jump:
			m_keyJump = false;
			break;
		default:
			break;
	}
	*/
	return true;
}
