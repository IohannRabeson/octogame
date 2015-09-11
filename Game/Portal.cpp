#include "Portal.hpp"
#include "ResourceDefinitions.hpp"
#include "CircleShape.hpp"
#include "PhysicsEngine.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <ResourceManager.hpp>
#include <PostEffectManager.hpp>
#include <Camera.hpp>
#include <cassert>

Portal::Portal(void) :
	m_position(40.f, 0.f),
	m_shaderIndex(0u),
	m_state(State::Appear),
	m_radius(100.f),
	m_timer(0.f),
	m_timerMax(1.0f),
	m_box(PhysicsEngine::getShapeBuilder().createCircle())
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();
	octo::PostEffectManager & postEffect = octo::Application::getPostEffectManager();

	m_shader.loadFromMemory(resources.getText(VORTEX_FRAG), sf::Shader::Fragment);
	octo::PostEffect postEffectShader;
	postEffectShader.resetShader(&m_shader);
	m_shaderIndex = postEffect.addEffect(std::move(postEffectShader));
	m_shader.setParameter("time_max", m_timerMax);

	m_box->setGameObject(this);
	m_box->setApplyGravity(false);
	m_box->setType(AShape::Type::e_trigger);

	setRadius(m_radius);
	setPosition(m_position);

	static float const	Size = 8.f;
	PortalParticle::Prototype	prototype;

	prototype.emplace_back(sf::Vertex({-Size, Size}));
	prototype.emplace_back(sf::Vertex({Size, -Size}));
	prototype.emplace_back(sf::Vertex({-Size, -Size}));
	m_particles.reset(prototype, sf::Triangles, 1000);


	octo::SpriteAnimation::FrameList	frames;
	frames.emplace_back(sf::seconds(0.4), 0);
	frames.emplace_back(sf::seconds(0.4), 1);
	frames.emplace_back(sf::seconds(0.4), 2);
	frames.emplace_back(sf::seconds(0.4), 3);
	m_animation.setFrames(frames);
	m_animation.setLoop(octo::LoopMode::Loop);
	m_sprite.setSpriteSheet(resources.getSpriteSheet(OBJECT_PORTAL_OSS));
	m_sprite.setAnimation(m_animation);
	m_sprite.play();
}

Portal::~Portal(void)
{
	octo::Application::getPostEffectManager().enableEffect(m_shaderIndex, false);
}

void Portal::update(sf::Time frametime)
{
	m_particles.update(frametime);
	octo::PostEffectManager& postEffect = octo::Application::getPostEffectManager();
	postEffect.enableEffect(m_shaderIndex, false);

	switch (m_state)
	{
		case Appear:
			m_timer += frametime.asSeconds();
			if (m_timer >= m_timerMax)
				m_timer = m_timerMax;
			break;
		case Disappear:
			m_timer -= frametime.asSeconds();
			if (m_timer <= 0.f)
				m_timer = 0.f;
			break;
		default:
			break;
	}

	sf::FloatRect const & screen = octo::Application::getCamera().getRectangle();
	if (m_position.x + m_radius > screen.left && m_position.x - m_radius < screen.left + screen.width)
	{
		if (m_position.y + m_radius > screen.top && m_position.y - m_radius < screen.top + screen.height)
		{
			postEffect.enableEffect(m_shaderIndex, true);
			m_shader.setParameter("time", m_timer);
			m_shader.setParameter("resolution", octo::Application::getGraphicsManager().getVideoMode().width, octo::Application::getGraphicsManager().getVideoMode().height);
			m_shader.setParameter("center", m_position.x - screen.left, octo::Application::getGraphicsManager().getVideoMode().height - m_position.y + screen.top);
		}
	}

	m_sprite.setPosition(m_position + sf::Vector2f(-m_sprite.getGlobalBounds().width / 2.f, -m_sprite.getGlobalBounds().height / 2.f + 52.f));
	m_sprite.update(frametime);
}

void Portal::setPosition(sf::Vector2f const & position)
{
	m_position = position;
	m_particles.setEmitter(m_position);
	m_box->setPosition(sf::Vector2f(m_position.x - m_radius, m_position.y - m_radius));
}

void Portal::setRadius(float radius)
{
	m_radius = radius;
	m_shader.setParameter("radius", m_radius);
	m_particles.setRadius(m_radius);
	m_box->setRadius(m_radius);
}

void Portal::setBiome(ABiome & biome)
{
	m_particles.setBiome(biome);
}

void Portal::draw(sf::RenderTarget & render) const
{
	render.draw(m_sprite);
	m_particles.draw(render);
}

Portal::PortalParticle::PortalParticle(void) :
	m_maxParticle(20u),
	m_color(sf::Color(84.f, 12.f, 96.f)),
	m_radius(96.f),
	m_engine(std::time(0)),
	m_lifeTimeDistri(1.f, 2.f),
	m_directionDistri(0.f, octo::Pi2),
	m_distanceDistri(m_radius, m_radius * 2.f),
	m_biome(nullptr)
{}

void Portal::PortalParticle::update(sf::Time frameTime)
{
	assert(m_biome);
	ParticleSystem::update(frameTime);
	if (ParticleSystem::getCount() < m_maxParticle)
	{
		float direction = m_directionDistri(m_engine);
		float distance = m_distanceDistri(m_engine);

		m_color = m_biome->getParticleColorGround();
		emplace(m_color, m_emitter, sf::Vector2f(1.f, 1.f), direction * 180.f,
				sf::Time::Zero,
				sf::seconds(m_lifeTimeDistri(m_engine)),
				m_emitter + sf::Vector2f(std::sin(direction) * distance, std::cos(direction) * distance), m_emitter);
	}
}

void Portal::PortalParticle::setRadius(float radius)
{
	m_radius = radius;
	m_distanceDistri.param(std::uniform_real_distribution<float>::param_type(m_radius * 2.3f, m_radius * 2.8f));
}

void Portal::PortalParticle::setBiome(ABiome & biome)
{
	m_biome = &biome;
}

void Portal::PortalParticle::updateParticle(sf::Time frameTime, Particle& particle)
{
	static float const			AngularVelocity = 30.f;

	std::get<Component::Position>(particle) = octo::linearInterpolation(std::get<MyComponent::Start>(particle), std::get<MyComponent::End>(particle), std::get<MyComponent::Time>(particle) / std::get<MyComponent::Life>(particle));
	std::get<Component::Rotation>(particle) += AngularVelocity * frameTime.asSeconds();
	std::get<MyComponent::Time>(particle) += frameTime;
	std::get<Component::Color>(particle).a = 255 - 255 * std::max(0.f, (1.f - std::get<MyComponent::Time>(particle).asSeconds() / std::get<MyComponent::Life>(particle).asSeconds()));
}

bool Portal::PortalParticle::isDeadParticle(Particle const& particle)
{
	return (std::get<MyComponent::Time>(particle) >= std::get<MyComponent::Life>(particle));
}
