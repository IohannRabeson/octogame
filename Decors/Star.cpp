#include "Star.hpp"
#include "SkyCycle.hpp"
#include "ABiome.hpp"
#include "Progress.hpp"

Star::Star() :
	m_animator(10.f, 10.f, 3.f, 0.3f),
	m_animation(1.f),
	m_detailPriority(0),
	m_cycle(nullptr)
{
}

Star::Star(SkyCycle * cycle) :
	Star()
{
	m_cycle = cycle;
}

bool Star::dieOutOfScreen(void)
{
	if (m_animator.getState() != DecorAnimator::State::Dead)
		m_animator.die();
	else
		return true;
	return false;
}

void Star::setup(ABiome& biome)
{
	m_size = biome.getStarSize();
	m_color = biome.getStarColor();
	m_sizeHeart = m_size / 50.f;

	m_glowSize = m_size / 2.5f;
	m_glowSizeCorner = m_glowSize / 2.f;

	m_animator.setup(sf::seconds(1000.f));
	m_animator.pause();
	m_detailPriority = biome.randomInt(-2, 0);
}

void Star::update(sf::Time frameTime, octo::VertexBuilder& builder, ABiome&)
{
	if (m_cycle && m_cycle->isDay())
		m_animator.die();

	m_animator.update(frameTime);
	if (m_animator.getState() == DecorAnimator::State::Dead)
	{
		m_animator.setup(sf::seconds(1000.f));
		m_animator.pause();
	}
	else if ((m_cycle && m_cycle->isNight()) || m_cycle == nullptr)
		m_animator.play();
	m_animation = m_animator.getAnimation();

	sf::Vector2f const & position = getPosition();
	int lod = Progress::getInstance().getLevelOfDetails();

	if (m_detailPriority <= lod)
	{
		sf::Color animationColor(m_color.r, m_color.g, m_color.b, m_color.a * (m_animation > 1.f ? 1.f : m_animation));
		//AShineBuilder::createStar(m_size * m_animation, m_sizeHeart * m_animation, position, animationColor, builder);
		AShineBuilder::createGlow(m_glowSize * m_animation, m_glowSizeCorner * m_animation, position, animationColor, builder);
	}
}

