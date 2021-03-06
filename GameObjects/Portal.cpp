#include "Portal.hpp"
#include "CircleShape.hpp"
#include "PhysicsEngine.hpp"
#include "Progress.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>
#include <cassert>

Portal::Portal(Level destination, ResourceKey const & key, ResourceKey shader, sf::Color centerColor) :
	m_generator("random"),
	m_shaderName(shader),
	m_destination(destination),
	m_position(40.f, 0.f),
	m_shader(PostEffectLayer::getInstance().getShader(m_shaderName)),
	m_maxParticle(40u),
	m_key(key),
	m_state(State::Disappear),
	m_radius(100.f),
	m_timerActivate(0.f),
	m_timerActivateMax(1.0f),
	m_box(PhysicsEngine::getShapeBuilder().createCircle()),
	m_soundVolume(1.f)
{
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	octo::ResourceManager & resources = octo::Application::getResourceManager();
	Progress & progress = Progress::getInstance();
	progress.registerPortal(destination);

	m_shader.setUniform("time_max", m_timerActivateMax);
	if (progress.getCurrentDestination() == Level::Portal)
		m_shader.setUniform("center_color", sf::Glsl::Vec4(sf::Color::White));
	else
	{
		if (progress.isGameFinished() && (destination != Level::Red && destination != Level::Blue && destination != Level::Random && destination != Level::Rewards && destination != Level::EndTimeLapse && destination != Level::EndRocket))
		{
			m_destination = Level::Portal;
			m_shader.setUniform("center_color", sf::Glsl::Vec4(centerColor));
		}
		else
			m_shader.setUniform("center_color", sf::Glsl::Vec4(centerColor));
	}

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

	m_sprite.setSpriteSheet(resources.getSpriteSheet(key));

	if (strcmp(m_key, OBJECT_PORTAL_RANDOM_OSS) == 0)
	{
		m_state = Disappear;
		m_animationClosed.setFrames({
			Frame(sf::seconds(0.4f), {12u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {13u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {14u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {15u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {16u, sf::FloatRect(), sf::Vector2f()}),
		});
		m_animationClosed.setLoop(octo::LoopMode::NoLoop);

		m_animationOpening.setFrames({
			Frame(sf::seconds(0.4f), {16u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {15u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {14u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {13u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {12u, sf::FloatRect(), sf::Vector2f()}),
		});
		m_animationOpening.setLoop(octo::LoopMode::NoLoop);

		m_animationOpened.setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {9u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {11u, sf::FloatRect(), sf::Vector2f()}),
		});
		m_animationOpened.setLoop(octo::LoopMode::Loop);
	}
	else if (strcmp(m_key, OBJECT_PORTAL_BLUE_OSS) == 0 || strcmp(m_key, OBJECT_PORTAL_RED_OSS) == 0)
	{
		m_state = AlwaysOpen;
		m_animationClosed.setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {5u, sf::FloatRect(), sf::Vector2f()}),
		});
		m_animationClosed.setLoop(octo::LoopMode::Loop);

		m_animationOpening.setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {5u, sf::FloatRect(), sf::Vector2f()}),
		});
		m_animationOpening.setLoop(octo::LoopMode::NoLoop);

		m_animationOpened.setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {5u, sf::FloatRect(), sf::Vector2f()}),
		});
		m_animationOpened.setLoop(octo::LoopMode::Loop);
	}
	else
	{
		m_state = Disappear;
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
	}

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

	if (!progress.isMetPortal(m_destination) || (progress.isMetPortal(m_destination) && m_destination == Level::Random)
		|| (progress.getCurrentDestination() == Level::Portal && progress.isMetRandom(destination)))
		m_sprite.setNextEvent(Closed);
	else
		m_sprite.setNextEvent(Opened);

	//TODO : To change to the good sound
	m_sound = audio.playSound(resources.getSound(OBJECT_PORTAL_START_OGG), 0.f, m_generator.randomFloat(0.9f, 1.1f), sf::Vector3f(m_box->getBaryCenter().x, m_box->getBaryCenter().y, -150.f) , 800.f, 200.f);
	m_sound->setLoop(true);

	//TODO : not clean, only for last level
	if (destination == progress.getCurrentDestination())
		m_sprite.setNextEvent(Opened);
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
	Progress & progress = Progress::getInstance();
	progress.setPortalPosition(m_destination, m_position);
	m_particles.update(frametime);

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
			{
				m_sprite.setNextEvent(Opened);
				if (m_destination != Level::Random)
					Progress::getInstance().meetPortal(m_destination);
			}
			else if (m_sprite.getCurrentEvent() == Events::Opened)
			{
				m_particles.setMaxParticle(m_timerActivate / m_timerActivateMax * static_cast<float>(m_maxParticle));
				m_timerActivate += frametime.asSeconds();
				m_isActive = true;
				if (m_timerActivate >= m_timerActivateMax)
				{
					m_state = Activated;
					m_timerActivate = m_timerActivateMax;
				}
			}
			break;
		}
		case Activated:
			m_particles.setMaxParticle(m_maxParticle);
			break;
		case Disappear:
			m_particles.setTransparency(std::min(1.f, (m_timerActivate / m_timerActivateMax)));
			m_timerActivate -= frametime.asSeconds();
			m_isActive = false;
			if (m_timerActivate <= 0.f)
			{
				m_timerActivate = 0.f;
				m_particles.setMaxParticle(0u);
				m_particles.clear();
			}
			break;
		case AlwaysOpen:
			m_isActive = true;
			m_particles.setMaxParticle(m_maxParticle);
			m_timerActivate = m_timerActivateMax;
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
			PostEffectLayer::getInstance().enableShader(m_shaderName, true);
			m_shader.setUniform("time", m_timerActivate);
			m_shader.setUniform("radius", m_radius * zoomFactor);
			m_shader.setUniform("resolution", sf::Vector2f(octo::Application::getGraphicsManager().getVideoMode().width, octo::Application::getGraphicsManager().getVideoMode().height));
			m_shader.setUniform("center", sf::Vector2f((m_position.x - screen.left) * zoomFactor, octo::Application::getGraphicsManager().getVideoMode().height + (-m_position.y + screen.top) * zoomFactor));
		}
		else if (Progress::getInstance().getCurrentDestination() == Level::Portal)
			PostEffectLayer::getInstance().enableShader(m_shaderName, false);
	}
	else if (Progress::getInstance().getCurrentDestination() == Level::Portal)
		PostEffectLayer::getInstance().enableShader(m_shaderName, false);

	updateSound();
	m_sprite.update(frametime);
	if (m_state != AlwaysOpen)
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
	m_shader.setUniform("radius", m_radius);
	m_particles.setRadius(m_radius);
	m_box->setRadius(m_radius);
}

bool Portal::zoom(void) const
{
	if (m_destination != Progress::getInstance().getLastDestination() && m_destination != Progress::getInstance().getCurrentDestination())
		return true;
	return false;
}

void Portal::appear(void)
{
	if (m_state == Activated || isLock() || m_state == AlwaysOpen)
		return;
	if (m_sprite.getCurrentEvent() == Events::Closed)
		m_state = State::FirstAppear;
	else
	{
		m_particles.setTransparency(1.f);
		m_state = State::Appear;
	}
}

void Portal::updateSound(void)
{
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	float						volume = 0.f;

	volume = m_soundVolume * (m_timerActivate / m_timerActivateMax);
	m_sound->setVolume(volume * audio.getSoundVolume());
	m_sound->setPosition(sf::Vector3f(m_box->getBaryCenter().x, m_box->getBaryCenter().y, -150.f));
}

bool Portal::isLock(void)
{
	Progress & progress = Progress::getInstance();
	if (progress.getCurrentDestination() == Level::EndTimeLapse)
		return true;
	if (progress.isMetPortal(m_destination) && m_destination == Level::Random)
		return true;
	if (progress.getCurrentDestination() == Level::Portal && progress.isMetRandom(m_destination))
		return true;
	return false;
}

void Portal::setBiome(ABiome & biome)
{
	m_particles.setBiome(biome);
}

bool Portal::isInScreen(void)
{
	octo::Camera const & camera = octo::Application::getCamera();
	if (camera.getRectangle().intersects(m_sprite.getGlobalBounds()))
		return true;
	return false;
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
	m_biome(nullptr),
	m_transparency(1.f)
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
	std::get<Component::Color>(particle).a = m_transparency * (255 - 255 * std::max(0.f, (1.f - std::get<MyComponent::Time>(particle).asSeconds() / std::get<MyComponent::Life>(particle).asSeconds())));
}

bool Portal::PortalParticle::isDeadParticle(Particle const& particle)
{
	return (std::get<MyComponent::Time>(particle) >= std::get<MyComponent::Life>(particle));
}
