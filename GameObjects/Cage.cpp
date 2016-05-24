#include "Cage.hpp"
#include "Progress.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>

Cage::Cage(sf::Vector2f const & scale, sf::Vector2f const & position) :
	InstanceDecor(CAGE_BACK_OSS, scale, position, 4u, 0.4f),
	m_isOpen(false)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();

	m_spriteFront.setSpriteSheet(resources.getSpriteSheet(CAGE_FRONT_OSS));
	m_spriteFront.setScale(scale);
	m_spriteFront.setPosition(position);

	octo::SpriteAnimation::FrameList	frames;
	frames.emplace_back(sf::seconds(0.4f), 0u);
	m_animationFront.setFrames(frames);
	m_animationFront.setLoop(octo::LoopMode::Loop);

	frames.clear();
	frames.emplace_back(sf::seconds(0.3f), 0u);
	frames.emplace_back(sf::seconds(0.3f), 1u);
	frames.emplace_back(sf::seconds(0.3f), 2u);
	frames.emplace_back(sf::seconds(0.3f), 3u);
	m_animationFrontOpen.setFrames(frames);
	m_animationFrontOpen.setLoop(octo::LoopMode::NoLoop);

	m_spriteFront.setAnimation(m_animationFront);
	m_spriteFront.play();

	//InstanceDecorSprite
	getAnimation().setLoop(octo::LoopMode::NoLoop);
	getSprite().pause();
}

void Cage::addMapOffset(float x, float y)
{
	InstanceDecor::addMapOffset(x, y);
	m_spriteFront.setPosition(m_spriteFront.getPosition().x + x, m_spriteFront.getPosition().y + y);
}

void Cage::setPosition(sf::Vector2f const & position)
{
	Cage::setPosition(position);
	m_spriteFront.setPosition(position);
}

void Cage::update(sf::Time frameTime)
{
	InstanceDecor::update(frameTime);
	m_spriteFront.update(frameTime);

	if (!m_isOpen) //TODO add the challenge validated from progress
	{
		m_spriteFront.setAnimation(m_animationFrontOpen);
		getSprite().play();
		m_isOpen = true;
	}
}

void Cage::drawFront(sf::RenderTarget& render, sf::RenderStates states) const
{
	m_spriteFront.draw(render, states);
}
