#include "ChallengeManager.hpp"
#include "Progress.hpp"
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

void ChallengeManager::update(sf::Time frametime)
{
	for (auto & it : m_challenges)
		it.second->update(frametime);
}

ChallengeManager::Challenge & ChallengeManager::getEffect(Effect effect)
{
	return (*m_challenges[effect]);
}

ChallengeManager::Challenge::Challenge(ResourceKey key, float duration) :
	m_duration(sf::seconds(duration))
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();
	octo::PostEffectManager & postEffect = octo::Application::getPostEffectManager();

	m_shader.loadFromMemory(resources.getText(key), sf::Shader::Fragment);
	octo::PostEffect postEffectShader;
	postEffectShader.resetShader(&m_shader);
	m_index = postEffect.addEffect(std::move(postEffectShader));
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

ChallengeDuplicate::ChallengeDuplicate(void) :
	Challenge(VISION_TROUBLE_FRAG, 32.f)
{}

void ChallengeDuplicate::update(sf::Time frametime)
{
	if (!enable())
		return;
	m_delay += frametime;
	if (m_delay < sf::seconds(4.f))
		return;
	if (Progress::getInstance().canOpenDoubleJump() && !Progress::getInstance().canDoubleJump())
		m_timer += frametime * 10.f;
	else
		m_timer += frametime;
	float length;
	if (m_timer < m_duration / 2.f)
		length = octo::linearInterpolation(0.f, 2.f, m_timer / (m_duration / 2.f));
	else
		length = octo::linearInterpolation(2.f, 0.f, (m_timer - m_duration / 2.f) / (m_duration / 2.f));
	sf::FloatRect const & rect = octo::Application::getCamera().getRectangle();
	length *= 40.f;
	float rotation = m_timer.asSeconds() / 5.f;
	float x = std::cos(rotation * octo::Pi2 * 1.5f) * length / rect.width;
	float y = std::sin(rotation * octo::Pi2 * 2.f) * length / rect.height;
	float z = std::sin(rotation * octo::Pi2) * length / rect.height;
	m_shader.setParameter("offset", x, y, z);
	if (m_timer > m_duration)
	{
		m_timer = sf::Time::Zero;
		m_delay = sf::Time::Zero;
		stop();
		Progress::getInstance().endChallenge();
	}
}
