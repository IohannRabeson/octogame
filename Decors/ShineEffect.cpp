#include "ShineEffect.hpp"
#include "ABiome.hpp"
#include <Math.hpp>
#include <cmath>
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <AudioManager.hpp>
#include "ResourceDefinitions.hpp"

ShineEffect::ShineEffect() :
	m_angle(0.f),
	m_animator(3.f, 3.f, 0.f, 0.0f, 10.f),
	m_animation(1.f),
	m_isShineEffect(true),
	m_isSound(true),
	m_canPlaySound(true)
{
}

void ShineEffect::setSize(sf::Vector2f const & size)
{
	m_size = size;
}

void ShineEffect::setCanPlaySound(bool playSound)
{
	m_canPlaySound = playSound;
}

void ShineEffect::setup(ABiome& biome)
{
	m_color = biome.getShineEffectColor();
	m_angle = biome.getShineEffectRotateAngle();
	m_sizeHeart = m_size / 50.f;

	m_glowSize = m_size / 2.5f;
	m_glowSizeCorner = m_glowSize / 2.f;

	m_animator.setup(sf::Time::Zero);
}

void ShineEffect::playSound(ABiome & biome, sf::Vector2f const & position)
{
	if (m_canPlaySound)
	{
		if (m_animator.getState() == DecorAnimator::State::Grow && m_isSound == false)
			m_isSound = true;
		else if (m_animator.getState() == DecorAnimator::State::Die && m_isSound == true)
		{
			octo::AudioManager& audio = octo::Application::getAudioManager();
			octo::ResourceManager& resources = octo::Application::getResourceManager();
			audio.playSound(resources.getSound(CRYSTAL_OGG), 1.f, biome.randomFloat(0.2f, 1.f), sf::Vector3f(position.x, position.y, -150.f), 500.f, 50.f);
			m_isSound = false;
		}
	}
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

		playSound(biome, position);

		sf::Color animationColor(m_color.r, m_color.g, m_color.b, m_color.a * (m_animation > 1.f ? 1.f : m_animation));
		AShineBuilder::createStar(m_size * m_animation, m_sizeHeart * m_animation, position, animationColor, builder, m_angle, cosAngle, sinAngle);
		AShineBuilder::createGlow(m_glowSize * m_animation, m_glowSizeCorner * m_animation, position, animationColor, builder, m_angle, cosAngle, sinAngle);
	}
}

DecorAnimator const & ShineEffect::getAnimator(void)
{
	return m_animator;
}
