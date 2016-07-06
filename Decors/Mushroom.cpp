#include "Mushroom.hpp"
#include "ABiome.hpp"
#include "ConvexShape.hpp"
#include "PhysicsEngine.hpp"
#include <Interpolations.hpp>
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <AudioManager.hpp>
#include "ResourceDefinitions.hpp"

Mushroom::Mushroom(void) :
	m_pointCount(5u),
	m_isPhysic(true),
	m_box(PhysicsEngine::getShapeBuilder().createConvex()),
	m_animator(0.5f, 4.f, 5.f, 0.2f),
	m_animation(1.f),
	m_bouncingTimer(sf::Time::Zero),
	m_bouncingTimerMax(sf::seconds(5.f)),
	m_bouncingBool(true),
	m_bouncingValue(0.f),
	m_sound(true)
{
	m_box->setGameObject(this);
	m_box->setCollisionType(static_cast<std::size_t>(GameObjectType::Mushroom));
	m_box->setCollisionMask(static_cast<std::size_t>(GameObjectType::Player));
	m_box->setType(AShape::Type::e_static);
	m_box->setApplyGravity(false);
	m_box->setVertexCount(4u);
}

void Mushroom::createMushroom(sf::Vector2f const & size, sf::Vector2f const & origin, sf::Color const & color, float bouncingValue, octo::VertexBuilder& builder)
{
	float unit = size.x / 6.f;
	m_leftFirst[0] = sf::Vector2f(0.f, -size.y);
	m_leftFirst[1] = sf::Vector2f(-unit, -size.y);
	m_leftFirst[2] = sf::Vector2f(-unit * 2.f, -size.y + unit);
	m_leftFirst[3] = sf::Vector2f(-unit * 2.f, -size.y + unit * 4.f);
	m_leftFirst[4] = sf::Vector2f(0.f, 0.f);

	m_leftSecond[0] = sf::Vector2f(0.f, -size.y);
	m_leftSecond[1] = sf::Vector2f(-unit * 4.f, -size.y);
	m_leftSecond[2] = sf::Vector2f(-unit * 5.f, -size.y + unit);
	m_leftSecond[3] = sf::Vector2f(-unit * 5.f, -size.y + unit);
	m_leftSecond[4] = sf::Vector2f(0.f, 0.f);

	for (std::size_t i = 0; i < m_pointCount; i++)
	{
		m_leftFinal[i].x = octo::linearInterpolation(m_leftFirst[i].x, m_leftSecond[i].x, bouncingValue) - unit / 2.f;
		m_leftFinal[i].y = octo::linearInterpolation(m_leftFirst[i].y, m_leftSecond[i].y, bouncingValue);
		m_rightFinal[i] = sf::Vector2f(-1 * m_leftFinal[i].x, m_leftFinal[i].y) + origin;
		m_leftFinal[i] += origin;
	}

	// Tige
	builder.createQuad(m_leftFinal[0], m_leftFinal[4], m_rightFinal[4], m_rightFinal[0], sf::Color(255, 255, 255));
	// Up part
	builder.createQuad(m_leftFinal[1], m_leftFinal[2], m_rightFinal[2], m_rightFinal[1], color);
	// Bottom part
	builder.createQuad(m_leftFinal[2], m_leftFinal[3], m_rightFinal[3], m_rightFinal[2], color);
	builder.createTriangle(m_leftFinal[4], m_rightFinal[4], sf::Vector2f(0.f, unit) + origin, sf::Color(255, 255, 255));

	if (m_isPhysic && size.y >= 200.f)
	{
		builder.createQuad(m_leftFinal[1], m_leftFinal[2], m_rightFinal[2], m_rightFinal[1], sf::Color(255, 255, 255, 100));
		builder.createQuad(m_leftFinal[2], m_leftFinal[3], m_rightFinal[3], m_rightFinal[2], sf::Color(255, 255, 255, 100));

		m_box->setVertex(2u, m_leftFinal[2]);
		m_box->setVertex(3u, m_leftFinal[1]);
		m_box->setVertex(0u, m_rightFinal[1]);
		m_box->setVertex(1u, m_rightFinal[2]);
		m_box->update();
	}
}

void Mushroom::setup(ABiome& biome)
{
	m_leftFirst.resize(m_pointCount);
	m_leftSecond.resize(m_pointCount);
	m_leftFinal.resize(m_pointCount);
	m_rightFinal.resize(m_pointCount);
	newMushroom(biome);
}

void Mushroom::newMushroom(ABiome & biome)
{
	m_size = biome.getMushroomSize();
	m_color = biome.getMushroomColor();
	m_animator.setup(biome.getMushroomLifeTime());
	m_sound = true;
}

float Mushroom::computeBouncingValue(sf::Time frameTime)
{
	if (m_bouncingBool == true)
	{
		m_bouncingTimer += frameTime;
		if (m_bouncingTimer >= m_bouncingTimerMax)
			m_bouncingBool = false;
	}
	else
	{
		m_bouncingTimer -= frameTime;
		if (m_bouncingTimer <= sf::Time::Zero)
			m_bouncingBool = true;
	}
	return m_bouncingTimer / m_bouncingTimerMax;
}

void Mushroom::playSound(ABiome & biome, sf::Vector2f const & position)
{
		if (m_animator.getState() == DecorAnimator::State::Grow && m_sound == false)
			m_sound = true;
		else if (m_animator.getState() == DecorAnimator::State::Life && m_sound == true)
		{
			octo::AudioManager& audio = octo::Application::getAudioManager();
			octo::ResourceManager& resources = octo::Application::getResourceManager();
			audio.playSound(resources.getSound(MUSHROOM_OGG), 1.f, biome.randomFloat(3.f, 4.f), sf::Vector3f(position.x, position.y, 0.f), 100.f, 0.8f);
			m_sound = false;
		}
}

void Mushroom::update(sf::Time frameTime, octo::VertexBuilder& builder, ABiome& biome)
{
	if (m_animator.update(frameTime))
		newMushroom(biome);
	m_animation = m_animator.getAnimation();
	sf::Vector2f const & position = getPosition();

	playSound(biome, position);

	createMushroom(m_size * m_animation, position, m_color, m_bouncingValue, builder);
	//one frame delay because of the phisics
	m_bouncingValue = computeBouncingValue(frameTime);
}
