#include "ShineEffect.hpp"
#include "ABiome.hpp"
#include <math.hpp>
#include <cmath>

ShineEffect::ShineEffect() :
	m_position(sf::Vector2f(0.f, 0.f)),
	m_animator(3.f, 3.f, 0.f, 0.1f, 0.f),
	m_animation(1.f),
	m_isShineEffect(true)
{
}

void ShineEffect::setup(ABiome& biome)
{
	m_size = biome.getShineEffectSize();
	m_color = biome.getShineEffectColor();
	m_angle = biome.getShineEffectRotateAngle();
	m_sizeHeart = m_size / 50.f;

	m_glowSize = m_size / 2.5f;
	m_glowSizeCorner = m_glowSize / 2.f;

	m_animator.setup(sf::Time::Zero);
}

void ShineEffect::update(sf::Time frameTime, octo::VertexBuilder& builder, ABiome& biome)
{
	m_isShineEffect = biome.canCreateShineEffect();
	if (m_isShineEffect)
	{
		m_animator.update(frameTime);
		m_animation = m_animator.getAnimation();

		sf::Vector2f const & position = getPosition();
		float angle = m_angle * m_animation * octo::Deg2Rad;
		float cosAngle = std::cos(angle);
		float sinAngle = std::sin(angle);

		sf::Color animationColor(m_color.r, m_color.g, m_color.b, m_color.a * (m_animation > 1.f ? 1.f : m_animation));
		ShineBuilder::createStar(m_size * m_animation, m_sizeHeart * m_animation, position, animationColor, builder, m_angle, cosAngle, sinAngle);
		ShineBuilder::createGlow(m_glowSize * m_animation, m_glowSizeCorner * m_animation, position, animationColor, builder, m_angle, cosAngle, sinAngle);
	}
}

DecorAnimator const & ShineEffect::getAnimator(void)
{
	return m_animator;
}
