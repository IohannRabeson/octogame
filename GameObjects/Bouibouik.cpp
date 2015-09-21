#include "Bouibouik.hpp"
#include "PhysicsEngine.hpp"
#include "CircleShape.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <PostEffectManager.hpp>
#include <Interpolations.hpp>

Bouibouik::Bouibouik(void) :
	SimpleObject(BOUIBOUIK_OSS),
	m_box(PhysicsEngine::getShapeBuilder().createCircle(false)),
	m_shaderIndex(0u),
	m_timer(sf::Time::Zero),
	m_effectDuration(sf::seconds(20.f)),
	m_startBalle(false),
	m_collideWithOcto(false)
{
	typedef octo::SpriteAnimation::Frame	Frame;
	setupAnimation({Frame(sf::seconds(0.2f), 0u),
					Frame(sf::seconds(0.2f), 1u),
					Frame(sf::seconds(0.2f), 2u),
					Frame(sf::seconds(0.2f), 1u)}, octo::LoopMode::Loop);
	setupSmoke();

	m_box->setGameObject(this);
	m_box->setRadius(200.f);
	m_box->setCollisionType(static_cast<std::uint32_t>(GameObjectType::Bouibouik));
	m_box->setCollisionMask(static_cast<std::uint32_t>(GameObjectType::Player));
	m_box->setApplyGravity(false);
	m_box->setType(AShape::Type::e_trigger);

	octo::ResourceManager & resources = octo::Application::getResourceManager();
	octo::PostEffectManager & postEffect = octo::Application::getPostEffectManager();

	m_shader.loadFromMemory(resources.getText(PERSISTENCE_FRAG), sf::Shader::Fragment);
	octo::PostEffect postEffectShader;
	postEffectShader.resetShader(&m_shader);
	m_shaderIndex = postEffect.addEffect(std::move(postEffectShader));
}

Bouibouik::~Bouibouik(void)
{
	octo::Application::getPostEffectManager().enableEffect(m_shaderIndex, false);
}

void Bouibouik::setupSmoke(void)
{
	m_smoke.setup(sf::Vector2f(5.f, 5.f));
	m_smoke.setVelocity(sf::Vector2f(0.f, -100.f));
	m_smoke.setEmitTimeRange(0.2f, 0.3f);
	m_smoke.setGrowTimeRange(0.4f, 0.6f);
	m_smoke.setLifeTimeRange(0.6f, 0.8f);
	m_smoke.setScaleFactor(10.f);
	m_smoke.setDispersion(80.f);
	m_smoke.setColor(sf::Color(18, 14, 66, 150));
}

void Bouibouik::startBalle(void)
{
	octo::PostEffectManager& postEffect = octo::Application::getPostEffectManager();
	postEffect.enableEffect(m_shaderIndex, true);
	m_startBalle = true;
	m_collideWithOcto = true;
	m_smoke.setVelocity(sf::Vector2f(0.f, -200.f));
	m_smoke.setEmitTimeRange(0.05f, 0.1f);
	m_smoke.setScaleFactor(15.f);
	m_smoke.setDispersion(150.f);
}

void Bouibouik::setPosition(sf::Vector2f const & position)
{
	SimpleObject::setPosition(position - sf::Vector2f(0.f, 370.f));
	m_smoke.setPosition(position - sf::Vector2f(-55.f, 350.f));
	octo::AnimatedSprite const & sprite = getSprite();
	m_box->setPosition(position.x - 50.f, sprite.getPosition().y);
	m_box->update();
}

void Bouibouik::update(sf::Time frameTime)
{
	SimpleObject::update(frameTime);
	m_smoke.update(frameTime);

	if (m_startBalle)
	{
		m_timer += frameTime;
		float intensity;
		sf::Time step = m_effectDuration / 6.f;
		if (m_timer < step)
			intensity = octo::linearInterpolation(1.f, 0.1f, m_timer / step);
		else if (m_timer < 3.f * step)
			intensity = octo::linearInterpolation(0.1f, 0.02f, (m_timer - step) / (3.f * step));
		else if (m_timer < 5.f * step)
			intensity = octo::linearInterpolation(0.02f, 0.1f, (m_timer - 3.f * step) / (5.f * step));
		else
			intensity = octo::linearInterpolation(0.1f, 1.f, (m_timer - 5.f * step) / (m_effectDuration));
		m_shader.setParameter("intensity", intensity);
		if (m_timer > m_effectDuration)
		{
			m_timer = sf::Time::Zero;
			octo::PostEffectManager& postEffect = octo::Application::getPostEffectManager();
			postEffect.enableEffect(m_shaderIndex, false);
			m_startBalle = false;
		}
	}
	if (m_collideWithOcto)
	{
		m_smoke.setVelocity(sf::Vector2f(0.f, -100.f));
		m_smoke.setEmitTimeRange(0.2f, 0.3f);
		m_smoke.setScaleFactor(10.f);
		m_smoke.setDispersion(80.f);
	}
	m_collideWithOcto = false;
}

void Bouibouik::draw(sf::RenderTarget& render, sf::RenderStates states) const
{
	SimpleObject::draw(render, states);
	m_smoke.draw(render);
}
