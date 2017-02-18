#include "InstanceDecor.hpp"
#include <Camera.hpp>
#include <Application.hpp>
#include <ResourceManager.hpp>

InstanceDecor::InstanceDecor(ResourceKey key)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();

	m_sprite.setSpriteSheet(resources.getSpriteSheet(key));
}

InstanceDecor::InstanceDecor(ResourceKey key, sf::Vector2f const & scale, sf::Vector2f const & position, std::size_t nbFrames, float frameDuration)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();

	m_sprite.setSpriteSheet(resources.getSpriteSheet(key));
	m_sprite.setScale(scale);
	m_sprite.setPosition(position);

	octo::SpriteAnimation::FrameList	frames;
	for (std::size_t i = 0u; i < nbFrames; i++)
		frames.emplace_back(sf::seconds(frameDuration), i);

	m_animation.setFrames(frames);
	m_animation.setLoop(octo::LoopMode::Loop);
	m_sprite.setAnimation(m_animation);
	m_sprite.play();
}

InstanceDecor::~InstanceDecor(void)
{}

void InstanceDecor::setupAnimation(std::initializer_list<octo::SpriteAnimation::Frame> list)
{
	m_animation.setFrames(list);
	m_animation.setLoop(octo::LoopMode::Loop);
	m_sprite.setAnimation(m_animation);
	m_sprite.play();
}

void InstanceDecor::addMapOffset(float x, float y)
{
	m_sprite.setPosition(m_sprite.getPosition().x + x, m_sprite.getPosition().y + y);
}

sf::Vector2f const & InstanceDecor::getPosition(void) const
{
	return m_sprite.getPosition();
}

void InstanceDecor::setPosition(sf::Vector2f const & position)
{
	m_sprite.setPosition(position.x - m_sprite.getLocalSize().x * m_sprite.getScale().x, position.y - m_sprite.getLocalSize().y * m_sprite.getScale().y);
}

octo::AnimatedSprite & InstanceDecor::getSprite(void)
{
	return m_sprite;
}

octo::SpriteAnimation & InstanceDecor::getAnimation(void)
{
	return m_animation;
}

void InstanceDecor::update(sf::Time frameTime)
{
	m_sprite.update(frameTime);
}

bool InstanceDecor::isInScreen(void)
{
	octo::Camera const & camera = octo::Application::getCamera();
	if (camera.getRectangle().intersects(m_sprite.getGlobalBounds()))
		return true;
	return false;
}

void InstanceDecor::draw(sf::RenderTarget& render, sf::RenderStates) const
{
	render.draw(m_sprite);
}
