#include "SimpleObject.hpp"
#include "PhysicsEngine.hpp"
#include "CircleShape.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <PostEffectManager.hpp>

SimpleObject::SimpleObject(ResourceKey key, ResourceKey shader, float duration) :
	m_box(PhysicsEngine::getShapeBuilder().createCircle(false)),
	m_shaderIndex(0u),
	m_timer(sf::Time::Zero),
	m_effectDuration(sf::seconds(duration)),
	m_startBalle(false),
	m_collideWithOcto(false)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();
	octo::PostEffectManager & postEffect = octo::Application::getPostEffectManager();

	m_sprite.setSpriteSheet(resources.getSpriteSheet(key));
	m_sprite.setScale(1.f, 1.f);

	m_shader.loadFromMemory(resources.getText(shader), sf::Shader::Fragment);
	octo::PostEffect postEffectShader;
	postEffectShader.resetShader(&m_shader);
	m_shaderIndex = postEffect.addEffect(std::move(postEffectShader));
}

SimpleObject::~SimpleObject(void)
{
	octo::Application::getPostEffectManager().enableEffect(m_shaderIndex, false);
}

void SimpleObject::startBalle(void)
{
	octo::PostEffectManager& postEffect = octo::Application::getPostEffectManager();
	postEffect.enableEffect(m_shaderIndex, true);
	m_startBalle = true;
	m_collideWithOcto = true;
}

void SimpleObject::setupAnimation(std::initializer_list<octo::SpriteAnimation::Frame> list, octo::LoopMode loop)
{
	m_animation.setFrames(list);
	m_animation.setLoop(loop);

	m_sprite.setAnimation(m_animation);
	m_sprite.play();
}

void SimpleObject::setPosition(sf::Vector2f const & position)
{
	m_sprite.setPosition(position.x, position.y);
	m_box->setPosition(m_offset + m_sprite.getPosition() + m_sprite.getGlobalSize() / 2.f - sf::Vector2f(m_box->getRadius(), m_box->getRadius()));
	m_box->update();
}

sf::Shader & SimpleObject::getShader(void)
{
	return m_shader;
}

octo::AnimatedSprite const & SimpleObject::getSprite(void) const
{
	return m_sprite;
}

bool SimpleObject::getStartBalle(void) const
{
	return m_startBalle;
}

bool SimpleObject::getCollideWithOcto(void) const
{
	return m_collideWithOcto;
}

sf::Time SimpleObject::getTimer(void) const
{
	return m_timer;
}

sf::Time SimpleObject::getEffectDuration(void) const
{
	return m_effectDuration;
}

void SimpleObject::adjustBox(sf::Vector2f const & offset, float radius)
{
	m_offset = offset;
	m_box->setRadius(radius);
}

void SimpleObject::setupBox(AGameObjectBase * gameObject, std::size_t type, std::size_t mask)
{
	m_box->setGameObject(gameObject);
	m_box->setCollisionType(type);
	m_box->setCollisionMask(mask);
	m_box->setRadius(m_sprite.getGlobalSize().x / 2.f);
	m_box->setApplyGravity(false);
	m_box->setType(AShape::Type::e_trigger);
}

void SimpleObject::update(sf::Time frameTime)
{
	if (m_startBalle)
	{
		m_timer += frameTime;
		if (m_timer > m_effectDuration)
		{
			m_timer = sf::Time::Zero;
			octo::PostEffectManager& postEffect = octo::Application::getPostEffectManager();
			postEffect.enableEffect(m_shaderIndex, false);
			m_startBalle = false;
		}
	}
	m_sprite.update(frameTime);
	m_collideWithOcto = false;
}

void SimpleObject::draw(sf::RenderTarget& render, sf::RenderStates) const
{
	render.draw(m_sprite);
}
