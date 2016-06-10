#include "Portal.hpp"
#include "ResourceDefinitions.hpp"
#include "CircleShape.hpp"
#include "PhysicsEngine.hpp"
#include "Progress.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <ResourceManager.hpp>
#include <PostEffectManager.hpp>
#include <Camera.hpp>
#include <cassert>

Portal::Portal(Level destination) :
	m_destination(destination),
	m_position(40.f, 0.f),
	m_shaderIndex(0u),
	m_maxParticle(40u),
	m_state(State::Disappear),
	m_radius(100.f),
	m_timer(0.f),
	m_timerMax(1.0f),
	m_box(PhysicsEngine::getShapeBuilder().createCircle())
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();
	octo::PostEffectManager & postEffect = octo::Application::getPostEffectManager();
	Progress & progress = Progress::getInstance();
	progress.registerPortal(destination);

	m_shader.loadFromMemory(resources.getText(VORTEX_FRAG), sf::Shader::Fragment);
	octo::PostEffect postEffectShader;
	postEffectShader.resetShader(&m_shader);
	m_shaderIndex = postEffect.addEffect(std::move(postEffectShader));
	m_shader.setParameter("time_max", m_timerMax);

	m_box->setGameObject(this);
	m_box->setApplyGravity(false);
	m_box->setType(AShape::Type::e_trigger);
	m_box->setCollisionType(static_cast<std::size_t>(GameObjectType::Portal));
	m_box->setCollisionMask(static_cast<std::size_t>(GameObjectType::Player) | static_cast<std::size_t>(GameObjectType::PlayerEvent));

	setRadius(m_radius);
	setPosition(m_position);

	static float const	Size = 8.f;
	PortalParticle::Prototype	prototype;

	prototype.emplace_back(sf::Vertex({-Size, Size}));
	prototype.emplace_back(sf::Vertex({Size, -Size}));
	prototype.emplace_back(sf::Vertex({-Size, -Size}));
	m_particles.reset(prototype, sf::Triangles, 1000);

	//Setup Sprite
	typedef octo::CharacterAnimation::Frame Frame;

	m_sprite.setSpriteSheet(resources.getSpriteSheet(OBJECT_PORTAL_OSS));

	m_animationClosed.setFrames({
		Frame(sf::seconds(0.4f), {10u, sf::FloatRect(), sf::Vector2f()}),
		Frame(sf::seconds(0.4f), {9u, sf::FloatRect(), sf::Vector2f()}),
	});
	m_animationClosed.setLoop(octo::LoopMode::Loop);

	m_animationOpening.setFrames({
		Frame(sf::seconds(0.4f), {8u, sf::FloatRect(), sf::Vector2f()}),
		Frame(sf::seconds(0.4f), {7u, sf::FloatRect(), sf::Vector2f()}),
		Frame(sf::seconds(0.4f), {6u, sf::FloatRect(), sf::Vector2f()}),
		Frame(sf::seconds(0.4f), {5u, sf::FloatRect(), sf::Vector2f()}),
		Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
		Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
	});
	m_animationOpening.setLoop(octo::LoopMode::NoLoop);

	m_animationOpened.setFrames({
		Frame(sf::seconds(0.8f), {0u, sf::FloatRect(), sf::Vector2f()}),
		Frame(sf::seconds(0.8f), {1u, sf::FloatRect(), sf::Vector2f()}),
		Frame(sf::seconds(0.8f), {2u, sf::FloatRect(), sf::Vector2f()}),
		Frame(sf::seconds(0.8f), {3u, sf::FloatRect(), sf::Vector2f()}),
	});
	m_animationOpened.setLoop(octo::LoopMode::Loop);

	//Setup state machine

	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					closed;
	StatePtr					opening;
	StatePtr					opened;

	closed = std::make_shared<State>("0", m_animationClosed, m_sprite);
	opening = std::make_shared<State>("1", m_animationOpening, m_sprite);
	opened = std::make_shared<State>("2", m_animationOpened, m_sprite);

	machine.setStart(closed);
	machine.addTransition(Closed, closed, closed);
	machine.addTransition(Closed, opening, closed);

	machine.addTransition(Opening, opening, opening);
	machine.addTransition(Opening, closed, opening);
	machine.addTransition(Opening, opened, opening);

	machine.addTransition(Opened, opened, opened);
	machine.addTransition(Opened, opening, opened);
	machine.addTransition(Opened, closed, opened);

	m_sprite.setMachine(machine);
	m_sprite.restart();

	if (!progress.isMetPortal(m_destination))
		m_sprite.setNextEvent(Closed);
	else
		m_sprite.setNextEvent(Opened);

	m_state = Disappear;
}

Portal::~Portal(void)
{
}

void Portal::addMapOffset(float x, float y)
{
	m_position = m_position + sf::Vector2f(x, y);
	m_particles.setEmitter(m_position);
	m_sprite.setPosition(m_sprite.getPosition().x + x, m_sprite.getPosition().y + y);
	m_box->setPosition(m_box->getPosition().x + x, m_box->getPosition().y + y);
	m_box->update();
}

sf::Vector2f const & Portal::getPosition(void) const
{
	return m_sprite.getPosition();
}

void Portal::update(sf::Time frametime)
{
	m_particles.update(frametime);
	octo::PostEffectManager& postEffect = octo::Application::getPostEffectManager();
	postEffect.enableEffect(m_shaderIndex, false);

	if (m_timer >= m_timerMax)
		m_isActive = true;
	else
		m_isActive = false;

	switch (m_state)
	{
		case FirstAppear:
		{
			if (m_sprite.getCurrentEvent() == Events::Closed)
				m_sprite.setNextEvent(Opening);
			break;
		}
		case Appear:
		{
			if (m_sprite.getCurrentEvent() == Events::Opening && m_sprite.isTerminated())
				m_sprite.setNextEvent(Opened);
			else if (m_sprite.getCurrentEvent() == Events::Opened)
			{
			m_particles.setMaxParticle(m_timer / m_timerMax * static_cast<float>(m_maxParticle));
			m_timer += frametime.asSeconds();
			if (m_timer >= m_timerMax)
			{
				Progress::getInstance().meetPortal(m_destination);
				m_state = Activated;
				m_timer = m_timerMax;
			}
			}
			break;
		}
		case Activated:
			m_particles.setMaxParticle(m_maxParticle);
			break;
		case Disappear:
			m_particles.setMaxParticle(0u);
			m_particles.clear();
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
			float zoomFactor = octo::Application::getGraphicsManager().getVideoMode().height / screen.height;
			postEffect.enableEffect(m_shaderIndex, true);
			m_shader.setParameter("time", m_timer);
			m_shader.setParameter("radius", m_radius * zoomFactor);
			m_shader.setParameter("resolution", octo::Application::getGraphicsManager().getVideoMode().width, octo::Application::getGraphicsManager().getVideoMode().height);
			m_shader.setParameter("center", (m_position.x - screen.left) * zoomFactor, octo::Application::getGraphicsManager().getVideoMode().height + (-m_position.y + screen.top) * zoomFactor);
		}
	}

	m_sprite.update(frametime);
	m_state = Disappear;
}

void Portal::setPosition(sf::Vector2f const & position)
{
	m_position = position;
	m_position.x += getRadius();
	m_position.y -= getRadius() + Tile::TripleTileSize;
	m_particles.setEmitter(m_position);
	m_box->setPosition(sf::Vector2f(m_position.x - m_radius, m_position.y - m_radius));
	m_box->update();
	//TODO : verify with Julien because I dont really understand what am I doing!
	m_sprite.setPosition(m_position.x - m_radius * 2.f, m_position.y - m_radius * 2.f);
}

void Portal::setRadius(float radius)
{
	m_radius = radius;
	m_shader.setParameter("radius", m_radius);
	m_particles.setRadius(m_radius);
	m_box->setRadius(m_radius);
}

void Portal::appear(void)
{
	if (m_state == Activated || isLock())
		return;
	if (m_sprite.getCurrentEvent() == Events::Closed)
		m_state = State::FirstAppear;
	else
		m_state = State::Appear;
}

bool Portal::isLock(void)
{
//	if (m_destination == Level::WaterA && Progress::getInstance().getNanoRobotCount() < 4)
//		return true;
	return false;
}

void Portal::setBiome(ABiome & biome)
{
	m_particles.setBiome(biome);
}

void Portal::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	m_sprite.draw(render, states);
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
