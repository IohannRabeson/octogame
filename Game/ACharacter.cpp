/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACharacter.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/04 20:16:31 by irabeson          #+#    #+#             */
/*   Updated: 2015/07/15 20:51:03 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACharacter.hpp"
#include "PhysicsEngine.hpp"
#include "RectangleShape.hpp"

#include <cassert>

//
// class CharacterState
//
ACharacter::CharacterState::CharacterState(std::string const& name,
							   			   octo::SpriteAnimation const& animation,
							   			   ACharacter* character) :
	AState(name),
	m_sprite(character->m_sprite),
	m_animation(animation),
	m_box(character->m_box)
{
}

void	ACharacter::CharacterState::start()
{
	m_sprite.setAnimation(m_animation);
	m_sprite.restart();	
}

void	ACharacter::CharacterState::stop()
{
	m_sprite.stop();
}

void	ACharacter::CharacterState::update(sf::Time frameTime)
{
	m_sprite.update(frameTime);
}

octo::SpriteAnimation const&	ACharacter::CharacterState::getAnimation()
{
	return (m_animation);
}

octo::AnimatedSprite&	ACharacter::CharacterState::getSprite()
{
	return (m_sprite);
}

RectangleShape*	ACharacter::CharacterState::getBox()
{
	return (m_box);
}

//
// class ACharacter
//
ACharacter::~ACharacter()
{
}

void	ACharacter::setupSpriteSheet(octo::SpriteSheet const& spriteSheet)
{
	m_sprite.setSpriteSheet(spriteSheet);
}

void	ACharacter::setupPhysicsBox(sf::Vector2f const& boxSize)
{
	assert (m_box == nullptr);

	m_box = PhysicsEngine::getInstance().createRectangle();
	m_box->setSize(boxSize);
}

void	ACharacter::setupMachine(FiniteStateMachine const& machine)
{
	m_machine = machine;
}

bool	ACharacter::setEvent(EventId eventId)
{
	auto	state = m_machine.getCurrentState();

	// Check if eventId is an acceptable event
	for (auto transition : *state)
	{
		if (transition.second.getTriggerEvent() == eventId)
		{
			m_machine.setNextEvent(eventId);
			return (true);
		}
	}
	return (false);
}

void	ACharacter::setSleep(bool sleep)
{
	m_box->setSleep(sleep);
}

void	ACharacter::update(sf::Time frameTime)
{
	if (m_box->getSleep())
		return;
	m_machine.update(frameTime);
	updateCharacter(frameTime);
	commitPhysicsToGraphics();
}

void	ACharacter::updateCharacter(sf::Time)
{
	// Does nothing
}

void	ACharacter::draw(sf::RenderTarget& render, sf::RenderStates states)const
{
	if (m_box->getSleep())
		return;
	render.draw(m_sprite, states);
}

void	ACharacter::commitPhysicsToGraphics()
{
	m_sprite.setPosition(m_box->getPosition());
}

void	ACharacter::start()
{
	m_machine.start();
}
