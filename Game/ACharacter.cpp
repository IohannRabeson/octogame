/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACharacter.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/04 20:16:31 by irabeson          #+#    #+#             */
/*   Updated: 2015/07/14 20:34:19 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACharacter.hpp"
#include "PhysicsEngine.hpp"
#include "RectangleShape.hpp"

void	ACharacter::setup(octo::SpriteSheet const& spriteSheet,
						  FiniteStateMachine&& machine,
				  		  sf::Vector2f const& boxSize)
{
	m_box = PhysicsEngine::getInstance().createRectangle();
	m_box->setSize(boxSize);
	m_sprite.setSpriteSheet(spriteSheet);
	m_machine = std::forward<FiniteStateMachine>(machine);
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
	commitPhysicsToGraphics();
}

void	ACharacter::draw(sf::RenderTarget& render, sf::RenderStates states)const
{
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
