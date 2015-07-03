#include "Star.hpp"
#include "ABiome.hpp"

Star::Star() :
	m_animator(5.f, 3.f, 3.f, 0.3f),
	m_animation(1.f)
{
}

Star::Star(float * dayNightClock) :
	Star()
{
	m_dayNightClock = dayNightClock;
}

void Star::setup(ABiome& biome)
{
	m_size = biome.getStarSize();
	m_color = biome.getStarColor();
	m_sizeHeart = m_size / 50.f;

	m_glowSize = m_size / 2.5f;
	m_glowSizeCorner = m_glowSize / 2.f;

	m_animator.setup(biome.getStarLifeTime());
	m_animator.pause();
}

void Star::update(sf::Time frameTime, octo::VertexBuilder& builder, ABiome&)
{
	if (*m_dayNightClock < 0.5f)
		m_animator.die();

	if (m_animator.update(frameTime))
		m_animator.pause();
	else if (*m_dayNightClock > 0.5f)
		m_animator.play();
	m_animation = m_animator.getAnimation();

	sf::Vector2f const & position = getPosition();

	sf::Color animationColor(m_color.r, m_color.g, m_color.b, m_color.a * (m_animation > 1.f ? 1.f : m_animation));
	createStar(m_size * m_animation, m_sizeHeart * m_animation, position, animationColor, builder);
	createGlow(m_glowSize * m_animation, m_glowSizeCorner * m_animation, position, animationColor, builder);
}

