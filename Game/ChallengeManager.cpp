#include "ChallengeManager.hpp"
#include "Progress.hpp"
#include "ABiome.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <PostEffectManager.hpp>

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
	m_challenges[Effect::Duplicate].reset(new ChallengePersistence());
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
					it.second->setIntensity(biome.randomFloat(2.f, 10.f));
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

ChallengeManager::AChallenge::AChallenge(ResourceKey key, float challengeDuration, float intensity, sf::FloatRect const & area, ABiome::Type biomeType) :
	m_delayMax(sf::seconds(4.f)),
	m_duration(sf::seconds(challengeDuration)),
	m_area(area),
	m_biomeType(biomeType),
	m_intensity(intensity),
	m_validArea(false),
	m_isGlitch(false)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();
	octo::PostEffectManager & postEffect = octo::Application::getPostEffectManager();

	m_shader.loadFromMemory(resources.getText(key), sf::Shader::Fragment);
	octo::PostEffect postEffectShader;
	postEffectShader.resetShader(&m_shader);
	m_index = postEffect.addEffect(std::move(postEffectShader));
}

ChallengeManager::AChallenge::~AChallenge(void)
{
	octo::Application::getPostEffectManager().removeEffect(m_index);
}

void ChallengeManager::AChallenge::update(sf::Time frametime, ABiome & biome, sf::Vector2f const & position)
{
	if (enable())
	{
		if (isGlitch())
		{
			updateGlitch(frametime, biome);
		}
		else
		{
			validateArea(biome, position);
			updateChallenge(frametime);
		}
	}
	else
		m_glitchTimer -= frametime;
}

void ChallengeManager::AChallenge::updateGlitch(sf::Time frametime, ABiome & biome)
{
	m_delay += frametime;
	if (m_delay <= m_duration)
		m_timer += frametime;
	else
		m_timer -= frametime;

	if (m_timer < sf::Time::Zero)
	{
		m_timer = sf::Time::Zero;
		m_delay = sf::Time::Zero;
		stop();
		setGlitch(false);
		m_glitchTimer = sf::seconds(biome.randomFloat(10.f, 30.f));
	}
	updateShader(frametime);
}

void ChallengeManager::AChallenge::updateChallenge(sf::Time frametime)
{
	m_delay += frametime;
	if (m_delay < m_delayMax)
		return;

	// We are in the area and the challenge has started
	if (m_validArea)
	{
		m_timer += frametime;
	}
	else // We leaved the area so the challenge is stopping
	{
		m_timer = std::min(m_timer, m_duration);
		m_timer -= frametime * 10.f;
		if (m_timer < sf::Time::Zero)
		{
			m_timer = sf::Time::Zero;
			m_delay = sf::Time::Zero;
			stop();
		}
	}
	updateShader(frametime);
}

void ChallengeManager::AChallenge::validateArea(ABiome & biome, sf::Vector2f const & position)
{
	sf::Vector2f pos = position;
	while (pos.x < 0.f)
		pos.x += biome.getMapSizeFloat().x;
	while (pos.x > biome.getMapSizeFloat().x)
		pos.x -= biome.getMapSizeFloat().x;
	m_validArea = m_area.contains(pos);
}

void ChallengeManager::AChallenge::start(void)
{
	octo::PostEffectManager & postEffect = octo::Application::getPostEffectManager();
	postEffect.enableEffect(m_index, true);
}

void ChallengeManager::AChallenge::stop(void)
{
	octo::PostEffectManager & postEffect = octo::Application::getPostEffectManager();
	postEffect.enableEffect(m_index, false);
}

bool ChallengeManager::AChallenge::enable(void) const
{
	octo::PostEffectManager & postEffect = octo::Application::getPostEffectManager();
	return postEffect.getEffect(m_index).isEnabled();
}

bool ChallengeManager::AChallenge::isFinished(void) const
{
	return (m_timer >= m_duration);
}

bool ChallengeManager::AChallenge::isGlitch(void) const
{
	return m_isGlitch;
}

bool ChallengeManager::AChallenge::canStartGlitch(void) const
{
	return (m_glitchTimer <= sf::Time::Zero);
}

sf::Time ChallengeManager::AChallenge::getDuration(void) const
{
	return (m_duration);
}

ABiome::Type ChallengeManager::AChallenge::getBiomeType(void) const
{
	return m_biomeType;
}

void ChallengeManager::AChallenge::setDuration(float duration)
{
	m_duration = sf::seconds(duration);
}

void ChallengeManager::AChallenge::setIntensity(float intensity)
{
	m_intensity = intensity;
}

void ChallengeManager::AChallenge::setGlitch(bool isGlitch)
{
	m_isGlitch = isGlitch;
}

// Duplicate
ChallengeDuplicate::ChallengeDuplicate(void) :
	AChallenge(VISION_TROUBLE_FRAG, 6.f, 60.f, sf::FloatRect(sf::Vector2f(45.f * 16.f, -2400.f), sf::Vector2f(420.f * 16.f, 2200.f)), ABiome::Type::Jungle),
	m_rotation(0.f)
{}

void ChallengeDuplicate::updateShader(sf::Time frametime)
{
	float length = octo::cosinusInterpolation(0.f, m_intensity, std::min(m_timer, m_duration) / m_duration);
	sf::FloatRect const & rect = octo::Application::getCamera().getRectangle();
	m_rotation += frametime.asSeconds() / 5.f;
	float x = std::cos(m_rotation * octo::Pi2 * 1.5f) * length / rect.width;
	float y = std::sin(m_rotation * octo::Pi2 * 2.f) * length / rect.height;
	float z = std::sin(m_rotation * octo::Pi2) * length / rect.height;
	m_shader.setParameter("offset", x, y, z);
}

// Persistence
ChallengePersistence::ChallengePersistence(void) :
	AChallenge(PERSISTENCE_FRAG, 6.f, 60.f, sf::FloatRect(sf::Vector2f(45.f * 16.f, -2400.f), sf::Vector2f(420.f * 16.f, 2200.f)), ABiome::Type::Jungle)
{
	m_shader.setParameter("intensity", 1.f);
}

void ChallengePersistence::updateShader(sf::Time)
{
	m_shader.setParameter("intensity", octo::linearInterpolation(1.f, 0.02f, std::min(m_timer, m_duration) / m_duration));
}

// Pixelate
