#include "ElevatorStream.hpp"
#include "ABiome.hpp"
#include "ResourceDefinitions.hpp"

#include <Application.hpp>
#include <ResourceManager.hpp>
#include "RectangleShape.hpp"
#include "PhysicsEngine.hpp"

#include <Math.hpp>
#include <Interpolations.hpp>

#include <random>
#include <ctime>

class ElevatorStream::BeamParticle : public octo::ParticleSystem<sf::Time, float, float>
{
	enum MyComponent
	{
		ElapsedTime = Component::User,
		UpSpeed,
		AngularVelocity
	};
public:
	BeamParticle() :
		m_cycleTime(sf::seconds(4)),
		m_speedUp(512.f),
		m_width(200.f),
		m_height(300.f),
		m_rotationFactor(0.f),
		m_emitInterval(sf::seconds(0.07f)),
		m_random(std::time(0)),
		m_distri(0.f, 1.f),
		m_biome(nullptr)
	{
		float const	size = 16.f;

		reset({sf::Vertex({0.f, 0.f}),
			   sf::Vertex({size, 0.f}),
			   sf::Vertex({size, size})},
			   sf::Triangles, 1000u);

	}

	void	setWidth(float width)
	{
		m_width = width;
	}

	void	setHeight(float height)
	{
		m_height = height;
	}

	void	setRotationFactor(float factor)
	{
		m_rotationFactor = factor;
	}

	void	setBiome(ABiome & biome)
	{
		m_biome = &biome;
	}

	float	getHeight(void) const
	{
		return m_height;
	}

	float	getWidth(void) const
	{
		return m_width;
	}

	void	updateParticle(sf::Time frameTime, Particle& particle)
	{
		sf::Vector2f	position = std::get<Component::Position>(particle);
		sf::Time		currentTime = std::get<MyComponent::ElapsedTime>(particle) + frameTime;
		sf::Color&		color = std::get<Component::Color>(particle);
		float			cycle = currentTime / m_cycleTime;
		float			heightPos = 0.f;

		position.y -= frameTime.asSeconds() * std::get<MyComponent::UpSpeed>(particle);
		heightPos = (position.y / m_height);
		position.x = std::cos(cycle * octo::Pi2) * m_width * 0.5f;
		color.a = heightPos * 255;
		if (currentTime >= m_cycleTime)
		{
			currentTime -= m_cycleTime;
		}
		std::get<MyComponent::ElapsedTime>(particle) = currentTime;
		std::get<Component::Position>(particle) = position;
		std::get<Component::Rotation>(particle) += m_rotationFactor * std::get<MyComponent::AngularVelocity>(particle);
		std::get<MyComponent::UpSpeed>(particle) *= 1.0001f;
	}

	void	update(sf::Time frameTime)
	{
		bool	emitParticle = false;

		m_emitTimer += frameTime;
		while (m_emitTimer > m_emitInterval)
		{
			emitParticle = true;
			m_emitTimer -= m_emitInterval;
		}
		if (emitParticle)
			createParticle();
		ParticleSystem::update(frameTime);
	}

	void	createParticle()
	{
		sf::Color color;
		if (m_biome)
			color = m_biome->getParticleColorGround();
		else
			color = sf::Color::White;
		emplace(color,
				sf::Vector2f(0, 0),
				sf::Vector2f(1.f, 1.f),
				m_distri(m_random) * 360.f,
				sf::seconds(m_distri(m_random) * m_cycleTime.asSeconds()),
				std::max(0.2f, m_distri(m_random)) * m_speedUp,
				m_distri(m_random) * 720 - 360
				);
	}
private:
	bool	isDeadParticle(Particle const& particle)
	{
		return (std::get<Component::Position>(particle).y < -m_height);
	}
private:
	typedef std::uniform_real_distribution<float>	Distri;

	sf::Time				m_cycleTime;
	float					m_speedUp;
	float					m_width;
	float					m_height;
	float					m_rotationFactor;
	sf::Time				m_emitTimer;
	sf::Time				m_emitInterval;
	std::mt19937			m_random;
	Distri					m_distri;
	ABiome *				m_biome;
};

ElevatorStream::ElevatorStream() :
	m_particles(new BeamParticle()),
	m_waveCycleDuration(sf::seconds(0.5)),
	m_box(PhysicsEngine::getShapeBuilder().createRectangle(false)),
	m_topY(0.f),
	m_rayCountVertex(12),
	m_ray(new sf::Vertex[m_rayCountVertex]),
	m_borderColor(255, 255, 255, 150),
	m_centerColor(255, 255, 255, 50),
	m_upColor(255, 255, 255, 0),
	m_state(Disappear),
	m_timer(sf::Time::Zero),
	m_timerMax(sf::seconds(1.5f))

{
	octo::ResourceManager&	resources = octo::Application::getResourceManager();

	m_box->setGameObject(this);
	m_box->setType(AShape::Type::e_trigger);
	m_box->setApplyGravity(false);
	m_box->setCollisionType(static_cast<std::size_t>(GameObjectType::Elevator));
	m_box->setCollisionMask(static_cast<std::size_t>(GameObjectType::Player) | static_cast<std::size_t>(GameObjectType::PlayerEvent));
	m_box->setSize(150.f, 0.f);
	m_particles->setWidth(150.f);
	m_shader.loadFromMemory(resources.getText(ELEVATOR_VERT), sf::Shader::Vertex);
	m_shader.setParameter("wave_amplitude", 5.f);

	float height = getHeight();
	float unit = getWidth() / 6.f;

	m_ray[0] = sf::Vertex(sf::Vector2f(-unit * 2.f, -height), m_upColor);
	m_ray[1] = sf::Vertex(sf::Vector2f(-unit, -height), m_upColor);
	m_ray[2] = sf::Vertex(sf::Vector2f(-unit, 0), m_centerColor);
	m_ray[3] = sf::Vertex(sf::Vector2f(-unit * 2.f, 0), m_borderColor);

	m_ray[4] = sf::Vertex(sf::Vector2f(-unit, -height), m_upColor);
	m_ray[5] = sf::Vertex(sf::Vector2f(unit, -height), m_upColor);
	m_ray[6] = sf::Vertex(sf::Vector2f(unit, 0), m_centerColor);
	m_ray[7] = sf::Vertex(sf::Vector2f(-unit, 0), m_centerColor);

	m_ray[8] = sf::Vertex(sf::Vector2f(unit * 2.f, -height), m_upColor);
	m_ray[9] = sf::Vertex(sf::Vector2f(unit, -height), m_upColor);
	m_ray[10] = sf::Vertex(sf::Vector2f(unit, 0), m_centerColor);
	m_ray[11] = sf::Vertex(sf::Vector2f(unit * 2.f, 0), m_borderColor);

	setupSprite();

	m_smoke.setup(sf::Vector2f(4.f, 4.f));
	m_smoke.setVelocity(sf::Vector2f(0.f, -80.f));
	m_smoke.setEmitTimeRange(0.2f, 0.3f);
	m_smoke.setGrowTimeRange(0.4f, 0.6f);
	m_smoke.setLifeTimeRange(0.6f, 0.8f);
	m_smoke.setScaleFactor(10.f);
	m_smoke.setDispersion(80.f);
	m_smoke.setColor(sf::Color(205, 205, 205, 200));
}

void	ElevatorStream::setupSprite(void)
{
	octo::ResourceManager&				resources = octo::Application::getResourceManager();
	octo::SpriteAnimation::FrameList	frames;
	frames.emplace_back(sf::seconds(0.2), 0);
	frames.emplace_back(sf::seconds(0.2), 1);
	m_brokenAnimation.setFrames(frames);
	m_brokenAnimation.setLoop(octo::LoopMode::Loop);

	frames.clear();
	frames.emplace_back(sf::seconds(0.2), 2);
	frames.emplace_back(sf::seconds(0.2), 3);
	frames.emplace_back(sf::seconds(0.2), 4);
	frames.emplace_back(sf::seconds(0.2), 5);
	m_animation.setFrames(frames);
	m_animation.setLoop(octo::LoopMode::Loop);

	m_spriteBottomFront.setSpriteSheet(resources.getSpriteSheet(OBJECT_ELEVATOR_BOTTOM_FRONT_OSS));
	m_spriteBottomFront.setAnimation(m_brokenAnimation);
	m_spriteBottomFront.setScale(sf::Vector2f(0.8f, 0.8f));
	m_spriteBottomFront.play();
	m_spriteBottomBack.setSpriteSheet(resources.getSpriteSheet(OBJECT_ELEVATOR_BOTTOM_BACK_OSS));
	m_spriteBottomBack.setAnimation(m_brokenAnimation);
	m_spriteBottomBack.setScale(sf::Vector2f(0.8f, 0.8f));
	m_spriteBottomBack.play();
	m_spriteTopFront.setSpriteSheet(resources.getSpriteSheet(OBJECT_ELEVATOR_TOP_FRONT_OSS));
	m_spriteTopFront.setAnimation(m_brokenAnimation);
	m_spriteTopFront.setScale(sf::Vector2f(0.8f, 0.8f));
	m_spriteTopFront.play();
	m_spriteTopBack.setSpriteSheet(resources.getSpriteSheet(OBJECT_ELEVATOR_TOP_BACK_OSS));
	m_spriteTopBack.setAnimation(m_brokenAnimation);
	m_spriteTopBack.setScale(sf::Vector2f(0.8f, 0.8f));
	m_spriteTopBack.play();
}

void	ElevatorStream::setHeight(float height)
{
	sf::Vector2f const &	sizeBox = m_box->getSize();

	m_box->setSize(sizeBox.x, height);
	m_particles->setHeight(height);
}

void	ElevatorStream::setTopY(float topY)
{
	sf::Vector2f const &	posBox = m_box->getPosition();

	m_box->setPosition(posBox.x, topY);
	m_topY = topY;
}

void	ElevatorStream::setWidth(float width)
{
	sf::Vector2f const &	sizeBox = m_box->getSize();

	m_box->setSize(width, sizeBox.y);
	m_particles->setWidth(width);
}

/*!	Set the rotation factor (0.f to 1.f) */
void	ElevatorStream::setRotationFactor(float factor)
{
	m_particles->setRotationFactor(factor);
}

void	ElevatorStream::setBiome(ABiome & biome)
{
	m_particles->setBiome(biome);
}

void	ElevatorStream::setPosition(sf::Vector2f const & position)
{
	m_position = position;
	m_position.x += getWidth() / 2.f + Tile::TileSize;
	m_position.y -= Tile::TileSize - Map::OffsetY;

	sf::Vector2f const &	posBox = m_box->getPosition();

	m_box->setPosition(m_position.x - (getWidth() / 2.f), posBox.y);
	m_spriteBottomFront.setPosition(m_position + sf::Vector2f(-m_spriteBottomFront.getGlobalBounds().width / 2.f, -m_spriteBottomFront.getGlobalBounds().height / 2.f - 30.f));
	m_spriteBottomBack.setPosition(m_position + sf::Vector2f(-m_spriteBottomBack.getGlobalBounds().width / 2.f, -m_spriteBottomBack.getGlobalBounds().height / 2.f - 30.f));
	m_spriteTopFront.setPosition(sf::Vector2f(-m_spriteTopFront.getGlobalBounds().width / 2.f + m_position.x, -m_spriteTopFront.getGlobalBounds().height / 2.f - 30.f + getTopY()));
	m_spriteTopBack.setPosition(sf::Vector2f(-m_spriteTopBack.getGlobalBounds().width / 2.f + m_position.x, -m_spriteTopBack.getGlobalBounds().height / 2.f - 30.f + getTopY()));

	m_smoke.setPosition(m_position + sf::Vector2f(-getWidth() / 2.f, -50.f));
	m_particles->setPosition(m_position + sf::Vector2f(0.f, -100.f));
	setHeight(m_position.y - getTopY() - 100.f);
}

float	ElevatorStream::getHeight(void) const
{
	return m_particles->getHeight();
}

float	ElevatorStream::getWidth(void) const
{
	return m_particles->getWidth();
}

float	ElevatorStream::getPosY(void) const
{
	return m_particles->getPosition().y;
}

float	ElevatorStream::getTopY(void) const
{
	return m_topY;
}

sf::Vector2f const & ElevatorStream::getPosition(void) const
{
	return m_position;
}

float ElevatorStream::getRepairAdvancement(void) const
{
	return m_timer.asSeconds() / m_timerMax.asSeconds();
}

bool ElevatorStream::isActivated(void) const
{
	return (m_state == Activated);
}

void	ElevatorStream::activate(void)
{
	if (m_state != Activated)
		m_state = Appear;
}

void	ElevatorStream::createRay()
{
	static const float unit = getWidth() / 6.f;
	float const height = getHeight();

	float ratio = m_timer.asSeconds() / m_timerMax.asSeconds();

	m_ray[0].position = sf::Vector2f(-unit * 2.f * ratio, -height);
	m_ray[1].position = sf::Vector2f(-unit * ratio, -height);
	m_ray[2].position = sf::Vector2f(-unit * ratio, 0.f);
	m_ray[3].position = sf::Vector2f(-unit * 2.f * ratio, 0.f);

	m_ray[4].position = sf::Vector2f(-unit * ratio, -height);
	m_ray[5].position = sf::Vector2f(unit * ratio, -height);
	m_ray[6].position = sf::Vector2f(unit * ratio, 0.f);
	m_ray[7].position = sf::Vector2f(-unit * ratio, 0.f);

	m_ray[8].position = sf::Vector2f(unit * 2.f * ratio, -height);
	m_ray[9].position = sf::Vector2f(unit * ratio, -height);
	m_ray[10].position = sf::Vector2f(unit * ratio, 0.f);
	m_ray[11].position = sf::Vector2f(unit * 2.f * ratio, 0.f);

	for (std::size_t i = 0; i < m_rayCountVertex; i++)
		m_ray[i].position += m_particles->getPosition();
}

void	ElevatorStream::update(sf::Time frameTime)
{
	switch (m_state)
	{
		case Appear:
			m_smoke.setCanEmit(true);
			m_timer += frameTime;
			if (m_timer >= m_timerMax)
			{
				m_timer = m_timerMax;
				m_state = Activated;
				m_spriteTopBack.setAnimation(m_animation);
				m_spriteTopFront.setAnimation(m_animation);
				m_spriteBottomBack.setAnimation(m_animation);
				m_spriteBottomFront.setAnimation(m_animation);
			}
			break;
		case Activated:
			m_smoke.setCanEmit(false);
			m_particles->update(frameTime);
			m_waveCycle += frameTime;
			m_shader.setParameter("wave_phase", m_waveCycle.asSeconds());
			break;
		case Disappear:
			m_smoke.setCanEmit(true);
			m_timer -= frameTime;
			if (m_timer < sf::Time::Zero)
				m_timer = sf::Time::Zero;
			break;
		default:
			break;
	}

	createRay();

	m_spriteBottomFront.update(frameTime);
	m_spriteBottomBack.update(frameTime);
	m_spriteTopFront.update(frameTime);
	m_spriteTopBack.update(frameTime);

	m_smoke.update(frameTime);

	if (m_state != Activated)
		m_state = Disappear;
}

void	ElevatorStream::draw(sf::RenderTarget& render, sf::RenderStates) const
{
	sf::RenderStates	states;

	render.draw(m_spriteBottomBack);
	render.draw(m_spriteTopBack);
	states.shader = &m_shader;
	if (m_state == Activated)
		m_particles->draw(render, states);
	render.draw(m_ray.get(), m_rayCountVertex, sf::Quads);
}

void	ElevatorStream::drawFront(sf::RenderTarget& render, sf::RenderStates) const
{
	render.draw(m_spriteBottomFront);
	render.draw(m_spriteTopFront);
	m_smoke.draw(render);
}
