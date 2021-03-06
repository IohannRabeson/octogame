#include "Crystal.hpp"
#include "ABiome.hpp"
#include <Interpolations.hpp>
#include <Math.hpp>

Crystal::Crystal() :
	m_partCount(0u),
	m_animator(1.f, 1.f, 3.f, 0.1f, 1.f),
	m_animation(0u),
	m_shineCrystalNumber(0u),
	m_shineTimer(sf::seconds(0.f)),
	m_shineTimerMax(sf::seconds(0.f))
{
}

bool Crystal::dieOutOfScreen(void)
{
	if (m_animator.getState() != DecorAnimator::State::Dead)
		m_animator.die();
	else
	{
		m_animator.setup(sf::seconds(10000000.f));
		return true;
	}
	return false;
}

void Crystal::createPolygon(sf::Vector2f const & size, sf::Vector2f const & origin, float const angle, sf::Color color, sf::Vector2f & up, sf::Vector2f & upLeft, octo::VertexBuilder & builder)
{
	up = sf::Vector2f(0.0f, -size.x - size.y);
	upLeft = sf::Vector2f(-size.x, -size.y);
	sf::Vector2f upMid(0.0f, -size.y);
	sf::Vector2f upRight(size.x, -size.y);

	sf::Vector2f down(0.0f, size.x);
	sf::Vector2f downLeft(-size.x, 0.f);
	sf::Vector2f downMid(0.0f, 0.f);
	sf::Vector2f downRight(size.x, 0.f);

	float radianAngle = angle * octo::Deg2Rad;
	float cosA = std::cos(radianAngle);
	float sinA = std::sin(radianAngle);

	octo::rotateVector(up, cosA, sinA);
	octo::rotateVector(upLeft, cosA, sinA);
	octo::rotateVector(upMid, cosA, sinA);
	octo::rotateVector(upRight, cosA, sinA);
	octo::rotateVector(down, cosA, sinA);
	octo::rotateVector(downLeft, cosA, sinA);
	octo::rotateVector(downMid, cosA, sinA);
	octo::rotateVector(downRight, cosA, sinA);

	upMid += origin;
	upRight += origin;
	down += origin;
	downLeft += origin;
	downMid += origin;
	downRight += origin;

	sf::Color tmpAddColor(5, 5, 5, 10);

	// Down right
	color += tmpAddColor;
	builder.createTriangle(down, downMid, downRight, color);
	// Mid right
	color += tmpAddColor;
	builder.createQuad(upRight, upMid, downMid, downRight, color);
	// Up right
	color += tmpAddColor;
	builder.createTriangle(up + origin, upMid, upRight, color);
	// Down left
	color += tmpAddColor;
	builder.createTriangle(down, downMid, downLeft, color);
	// Mid left
	color += tmpAddColor;
	builder.createQuad(upLeft + origin, upMid, downMid, downLeft, color);
	// Up left
	color += tmpAddColor;
	builder.createTriangle(up + origin, upMid, upLeft + origin, color);
}

void Crystal::createCrystal(std::vector<CrystalValue> const & values, sf::Vector2f const & origin, octo::VertexBuilder & builder)
{
	for (std::size_t i = 0; i < m_partCount; i++)
		createPolygon(values[i].size * m_animation, origin, values[i].angle, values[i].color, m_up[i], m_upLeft[i], builder);
}

void Crystal::setup(ABiome& biome)
{
	m_color = biome.getCrystalColor();
	m_partCount = biome.getCrystalPartCount();
	m_values.resize(m_partCount);

	m_up.resize(m_partCount);
	m_upLeft.resize(m_partCount);

	for (std::size_t i = 0; i < m_partCount; i++)
	{
		m_values[i].size = biome.getCrystalSize();
		m_values[i].angle = biome.randomFloat(-45.f + (i * 90.f / m_partCount), -45.f + ((i + 1) * 90.f / m_partCount));
		int deltaColor = biome.randomFloat(0.f, 80.f);
		m_values[i].color = m_color + sf::Color(deltaColor, deltaColor, deltaColor, deltaColor);
	}
	m_animator.setup(sf::seconds(10000000.f));
	m_shine.setSize(biome.getShineEffectSize());
	m_shine.setup(biome);
	m_shineTimerMax = sf::seconds(m_shine.getAnimator().getAnimationTime());
	m_shineCrystalNumber = biome.randomInt(0, m_partCount - 1);
}

void Crystal::update(sf::Time frameTime, octo::VertexBuilder& builder, ABiome& biome)
{
	sf::Vector2f const & position = getPosition();
	createCrystal(m_values, sf::Vector2f(position.x, position.y + m_size.x), builder);

	m_animator.update(frameTime);
	m_animation = m_animator.getAnimation();

	if (m_animation > 0.f)
	{
		float interpolateValue = 0.5f;
		if (m_shine.getAnimator().getState() == DecorAnimator::State::Grow)
			interpolateValue = 0.5f * m_shine.getAnimator().getAnimation();
		else if (m_shine.getAnimator().getState() == DecorAnimator::State::Die)
			interpolateValue = 0.5f + (0.5f * (1.f - m_shine.getAnimator().getAnimation()));
		else if (m_shine.getAnimator().getState() == DecorAnimator::State::Dead)
		{
			m_shineTimer -= m_shineTimerMax;
			m_shineCrystalNumber = biome.randomInt(0, m_partCount - 1);
		}

		sf::Vector2f shinePosition = octo::linearInterpolation(m_up[m_shineCrystalNumber], m_upLeft[m_shineCrystalNumber], interpolateValue);
		m_shine.setPosition(shinePosition + position);
		m_shine.update(frameTime, builder, biome);
	}
}

