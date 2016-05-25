#include "ChallengeManager.hpp"
#include "Progress.hpp"
#include "ABiome.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <PostEffectManager.hpp>

std::unique_ptr<ChallengeManager> ChallengeManager::m_instance = nullptr;

ChallengeManager::ChallengeManager(void)
{
	m_generator.setSeed("random");
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
}

void ChallengeManager::update(ABiome & biome, sf::Vector2f const & position, sf::Time frametime)
{
	for (auto & it : m_challenges)
	{
		if (biome.getType() == it.second->getBiomeType() || biome.getType() == ABiome::Type::Random)
		{
			if (!it.second->enable() && Progress::getInstance().isValidateChallenge(it.first))
			{
				if (m_generator.randomBool(0.0005f))
				{
					it.second->setGlitch(true);
					it.second->start();
				}
			}
			if (it.second->enable())
			{
				if (it.second->isGlitch())
				{
					it.second->updateGlitch(frametime);
				}
				else
				{
					it.second->validateArea(biome, position);
					it.second->updateChallenge(frametime);
				}
			}
		}
	}
}

ChallengeManager::Challenge & ChallengeManager::getEffect(Effect effect)
{
	return (*m_challenges[effect]);
}

ChallengeManager::Challenge::Challenge(ResourceKey key, float challengeDuration, float glitchDuration, sf::FloatRect const & area, ABiome::Type biomeType) :
	m_delayMax(sf::seconds(4.f)),
	m_duration(sf::seconds(challengeDuration)),
	m_challengeDuration(sf::seconds(challengeDuration)),
	m_glitchDuration(sf::seconds(glitchDuration)),
	m_area(area),
	m_biomeType(biomeType),
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

void ChallengeDuplicate::Challenge::updateGlitch(sf::Time frametime)
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
	}
	updateShader(frametime);
}

void ChallengeDuplicate::Challenge::updateChallenge(sf::Time frametime)
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

void ChallengeManager::Challenge::validateArea(ABiome & biome, sf::Vector2f const & position)
{
	sf::Vector2f pos = position;
	while (pos.x < 0.f)
		pos.x += biome.getMapSizeFloat().x;
	while (pos.x > biome.getMapSizeFloat().x)
		pos.x -= biome.getMapSizeFloat().x;
	m_validArea = m_area.contains(pos);
}

void ChallengeManager::Challenge::start(void)
{
	octo::PostEffectManager & postEffect = octo::Application::getPostEffectManager();
	postEffect.enableEffect(m_index, true);
}

void ChallengeManager::Challenge::stop(void)
{
	octo::PostEffectManager & postEffect = octo::Application::getPostEffectManager();
	postEffect.enableEffect(m_index, false);
}

bool ChallengeManager::Challenge::enable(void) const
{
	octo::PostEffectManager & postEffect = octo::Application::getPostEffectManager();
	return postEffect.getEffect(m_index).isEnabled();
}

bool ChallengeManager::Challenge::isFinished(void) const
{
	return (m_timer >= m_duration);
}

bool ChallengeManager::Challenge::isGlitch(void) const
{
	return m_isGlitch;
}

sf::Time ChallengeManager::Challenge::getDuration(void) const
{
	return (m_duration);
}

ABiome::Type ChallengeManager::Challenge::getBiomeType(void) const
{
	return m_biomeType;
}

void ChallengeManager::Challenge::setDuration(float duration)
{
	m_duration = sf::seconds(duration);
}

void ChallengeManager::Challenge::setGlitch(bool isGlitch)
{
	if (isGlitch)
		m_duration = m_glitchDuration;
	else
		m_duration = m_challengeDuration;
	m_isGlitch = isGlitch;
}

// Duplicate
ChallengeDuplicate::ChallengeDuplicate(void) :
	Challenge(VISION_TROUBLE_FRAG, 6.f, 0.2f, sf::FloatRect(sf::Vector2f(55.f * 16.f, -3400.f), sf::Vector2f(420.f * 16.f, 2200.f)), ABiome::Type::Jungle),
	m_rotation(0.f),
	m_intensity(60.f)
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

void ChallengeDuplicate::setGlitch(bool isGlitch)
{
	Challenge::setGlitch(isGlitch);
	if (isGlitch)
		m_intensity = 10.f;
	else
		m_intensity = 60.f;
}

// Pixelate
