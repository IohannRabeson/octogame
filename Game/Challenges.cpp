#include "Challenges.hpp"
#include "PostEffectLayer.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <PostEffectManager.hpp>
#include <Interpolations.hpp>
#include <Camera.hpp>

ChallengeManager::AChallenge::AChallenge(ResourceKey key, float challengeDuration, float delay, sf::FloatRect const & area, ABiome::Type biomeType, std::pair<float, float> glitchIntensityRange, std::pair<float, float> glitchDurationRange) :
	m_shader(PostEffectLayer::getInstance().getShader(key)),
	m_index(PostEffectLayer::getInstance().getShaderIndex(key)),
	m_delayMax(sf::seconds(delay)),
	m_duration(sf::seconds(challengeDuration)),
	m_area(area),
	m_biomeType(biomeType),
	m_intensity(1.f),
	m_validArea(false),
	m_isGlitch(false),
	m_glitchIntensityRange(glitchIntensityRange),
	m_glitchDurationRange(glitchDurationRange)
{
}

ChallengeManager::AChallenge::~AChallenge(void)
{
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
		m_timer -= frametime;
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

void ChallengeManager::AChallenge::startGlitch(ABiome & biome)
{
	if (m_glitchTimer <= sf::Time::Zero)
	{
		setGlitch(true);
		setIntensity(biome.randomFloat(m_glitchIntensityRange.first, m_glitchIntensityRange.second));
		setDuration(biome.randomFloat(m_glitchDurationRange.first, m_glitchDurationRange.second));
		start();
	}
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
	AChallenge(VISION_TROUBLE_FRAG, 6.f, 4.f, sf::FloatRect(sf::Vector2f(45.f * 16.f, -2400.f), sf::Vector2f(420.f * 16.f, 2200.f)), ABiome::Type::Jungle, std::pair<float, float>(0.033f, 0.16f), std::pair<float, float>(0.25f, 0.75f)),
	m_rotation(0.f)
{}

void ChallengeDuplicate::updateShader(sf::Time frametime)
{
	float length = m_intensity * octo::cosinusInterpolation(0.f, 60.f, std::min(m_timer, m_duration) / m_duration);
	sf::FloatRect const & rect = octo::Application::getCamera().getRectangle();
	m_rotation += frametime.asSeconds() / 5.f;
	float x = std::cos(m_rotation * octo::Pi2 * 1.5f) * length / rect.width;
	float y = std::sin(m_rotation * octo::Pi2 * 2.f) * length / rect.height;
	float z = std::sin(m_rotation * octo::Pi2) * length / rect.height;
	m_shader.setParameter("offset", x, y, z);
}

// Persistence
ChallengePersistence::ChallengePersistence(void) :
	AChallenge(PERSISTENCE_FRAG, 6.f, 1.f, sf::FloatRect(sf::Vector2f(90.f * 16.f, -150.f * 16.f), sf::Vector2f(250.f * 16.f, 750.f * 16.f)), ABiome::Type::Desert, std::pair<float, float>(0.7f, 0.9f), std::pair<float, float>(1.f, 2.f))
{
	m_shader.setParameter("intensity", 1.f);
}

void ChallengePersistence::updateShader(sf::Time)
{
	m_shader.setParameter("intensity", octo::linearInterpolation(1.f, 1.02f - m_intensity, std::min(m_timer, m_duration) / m_duration));
}

// Pixelate
ChallengePixelate::ChallengePixelate(void) :
	AChallenge(PIXELATE_FRAG, 6.f, 4.f, sf::FloatRect(sf::Vector2f(50.f * 16.f, -210.f * 16.f), sf::Vector2f(120.f * 16.f, 155.f * 16.f)), ABiome::Type::Random, std::pair<float, float>(0.033f, 0.16f), std::pair<float, float>(0.25f, 0.75f))
{}

void ChallengePixelate::updateShader(sf::Time)
{
	m_shader.setParameter("pixel_threshold", m_intensity * octo::linearInterpolation(0.f, 0.02f, std::min(m_timer, m_duration) / m_duration));
}

// Displacement
ChallengeDisplacement::ChallengeDisplacement(void) :
	AChallenge(DISPLACEMENT_FRAG, 6.f, 4.f, sf::FloatRect(sf::Vector2f(45.f * 16.f, -2400.f), sf::Vector2f(420.f * 16.f, 2200.f)), ABiome::Type::Jungle, std::pair<float, float>(0.05f, 0.15f), std::pair<float, float>(0.75f, 1.75f))
{
	sf::FloatRect const & rect = octo::Application::getCamera().getRectangle();
	m_shader.setParameter("resolution", rect.width, rect.height);
}

void ChallengeDisplacement::updateShader(sf::Time)
{
	m_shader.setParameter("intensity", m_intensity * octo::linearInterpolation(0.f, 1.f, std::min(m_timer, m_duration) / m_duration));
}

// Blur
ChallengeBlur::ChallengeBlur(void) :
	AChallenge(KERNEL_POST_EFFECT_FRAG, 4.f, 1.f, sf::FloatRect(sf::Vector2f(50.f * 16.f, -210.f * 16.f), sf::Vector2f(120.f * 16.f, 155.f * 16.f)), ABiome::Type::Ice, std::pair<float, float>(0.05f, 0.15f), std::pair<float, float>(0.75f, 1.75f))
{
	m_shader.setParameter("offset", 1.f / 150.f);
	m_shader.setParameter("intensity", 0.f);
	sf::Transform kernel(
		1.f / 16.f, 2.f / 16.f, 1.f / 16.f,
		2.f / 16.f, 4.f / 16.f, 2.f / 16.f,
		1.f / 16.f, 2.f / 16.f, 1.f / 16.f
		);
	m_shader.setParameter("kernel", kernel);
}

void ChallengeBlur::updateShader(sf::Time)
{
	m_shader.setParameter("intensity", octo::linearInterpolation(0.f, m_intensity, std::min(m_timer, m_duration) / m_duration));
}
