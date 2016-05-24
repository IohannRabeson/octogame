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
	m_challenges[Effect::Duplicate].reset(new ChallengeDuplicate());
}

void ChallengeManager::update(ABiome & biome, sf::Vector2f const & position, sf::Time frametime)
{
	//TODO manage random biome
	for (auto & it : m_challenges)
	{
		//if biome.getType() == it.second->getBiomeType()
		//if it.second.enable == false
		//if progress.isChallengeDone(biome.getType())
		//generate random to do glitch
		it.second->validateArea(biome, position);
		it.second->update(frametime);
	}
}

ChallengeManager::Challenge & ChallengeManager::getEffect(Effect effect)
{
	return (*m_challenges[effect]);
}

ChallengeManager::Challenge::Challenge(ResourceKey key, float challengeDuration, float glitchDuration, sf::FloatRect const & area) :
	m_delayMax(sf::seconds(4.f)),
	m_duration(sf::seconds(challengeDuration)),
	m_challengeDuration(sf::seconds(challengeDuration)),
	m_glitchDuration(sf::seconds(glitchDuration)),
	m_area(area),
	m_validArea(false)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();
	octo::PostEffectManager & postEffect = octo::Application::getPostEffectManager();

	m_shader.loadFromMemory(resources.getText(key), sf::Shader::Fragment);
	octo::PostEffect postEffectShader;
	postEffectShader.resetShader(&m_shader);
	m_index = postEffect.addEffect(std::move(postEffectShader));
}

void ChallengeDuplicate::Challenge::update(sf::Time frametime)
{
	if (!enable())
		return;
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
			//Progress::getInstance().endChallenge();
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

sf::Time ChallengeManager::Challenge::getDuration(void) const
{
	return (m_duration);
}

void ChallengeManager::Challenge::setDuration(float duration)
{
	m_duration = sf::seconds(duration);
}

// Duplicate
ChallengeDuplicate::ChallengeDuplicate(void) :
	Challenge(VISION_TROUBLE_FRAG, 6.f, 1.f, sf::FloatRect(sf::Vector2f(55.f * 16.f, -3400.f), sf::Vector2f(420.f * 16.f, 2200.f)))
{}

void ChallengeDuplicate::updateShader(sf::Time )
{
	float length = octo::linearInterpolation(0.f, 60.f, std::min(m_timer, m_duration) / m_duration);
	sf::FloatRect const & rect = octo::Application::getCamera().getRectangle();
	float rotation = m_timer.asSeconds() / 5.f;
	float x = std::cos(rotation * octo::Pi2 * 1.5f) * length / rect.width;
	float y = std::sin(rotation * octo::Pi2 * 2.f) * length / rect.height;
	float z = std::sin(rotation * octo::Pi2) * length / rect.height;
	m_shader.setParameter("offset", x, y, z);
}

// Pixelate
