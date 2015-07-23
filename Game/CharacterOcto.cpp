/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CharacterOcto.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/23 00:33:57 by irabeson          #+#    #+#             */
/*   Updated: 2015/07/23 13:19:58 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CharacterOcto.hpp"
#include "ResourceDefinitions.hpp"
#include "PhysicsEngine.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>

CharacterOcto::CharacterOcto() :
	m_box(PhysicsEngine::getShapeBuilder().createRectangle(false))
{
	typedef octo::CharacterAnimation::Frame			Frame;
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::ResourceManager&		resources = octo::Application::getResourceManager();
	octo::FiniteStateMachine	machine;
	StatePtr					state0;
	StatePtr					state1;
	StatePtr					state2;

	m_box->setApplyGravity(true);
	m_idleAnimation.setFrames({Frame(sf::seconds(0.2f), {0, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()})});
	m_idleAnimation.setLoop(octo::LoopMode::Loop);
	m_walkAnimation.setFrames({Frame(sf::seconds(0.2f), {0, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
							   Frame(sf::seconds(0.2f), {1, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
							   Frame(sf::seconds(0.2f), {2, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
							   Frame(sf::seconds(0.2f), {3, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
							   Frame(sf::seconds(0.2f), {4, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
							   Frame(sf::seconds(0.2f), {5, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
							   Frame(sf::seconds(0.2f), {6, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()})});
	m_walkAnimation.setLoop(octo::LoopMode::Loop);
	state0 = std::make_shared<State>("0", m_idleAnimation, m_sprite);
	state1 = std::make_shared<State>("1", m_walkAnimation, m_sprite);
	state2 = std::make_shared<State>("2", m_walkAnimation, m_sprite);
	machine.setStart(state0);
	machine.addTransition(Left, state0, state1);
	machine.addTransition(Left, state2, state1);
	machine.addTransition(Right, state0, state2);
	machine.addTransition(Right, state1, state2);
	machine.addTransition(Idle, state0, state0);
	machine.addTransition(Idle, state1, state0);
	machine.addTransition(Idle, state2, state0);
	m_sprite.setSpriteSheet(resources.getSpriteSheet(OCTO_COMPLETE_OSS));
	m_sprite.setMachine(machine);
	m_sprite.restart();
	//m_sprite.setOrigin(177 / 2, 0);
	std::fill(m_controls.begin(), m_controls.end(), false);
}

void	CharacterOcto::update(sf::Time frameTime)
{
	sf::FloatRect const&	rect = m_sprite.getBoundingBox();

	m_box->setSize(sf::Vector2f(rect.width, rect.height));
	m_sprite.update(frameTime);
	commitControlsToPhysics();
	// TODO: put this line in Game
	PhysicsEngine::getInstance().update(frameTime.asSeconds());
	commitPhysicsToGraphics();
}

void	CharacterOcto::draw(sf::RenderTarget& render, sf::RenderStates states)const
{
	// TODO: put this line in Game
	PhysicsEngine::getInstance().debugDraw(render);
	m_sprite.draw(render, states);
}

void	CharacterOcto::commitPhysicsToGraphics()
{
	sf::FloatRect const& bounds = m_box->getGlobalBounds();

	m_sprite.setPosition(bounds.left, bounds.top);	
}

void	CharacterOcto::commitControlsToPhysics()
{
	sf::Vector2f	velocity = m_box->getVelocity();

	if (m_controls[0])
	{
		velocity.x = -16.f;
	}
	else if (m_controls[1])
	{
		velocity.x = 16.f;
	}
	m_box->setVelocity(velocity);
}

bool	CharacterOcto::onPressed(sf::Event::KeyEvent const& event)
{
	switch (event.code)
	{
		case sf::Keyboard::Left:
			if (m_controls[0] == false)
			{
				m_controls[0] = true;
				m_sprite.setNextEvent(Left);
				m_sprite.setScale(-1, 1);
			}
			break;
		case sf::Keyboard::Right:
			if (m_controls[1] == false)
			{
				m_controls[1] = true;
				m_sprite.setNextEvent(Right);
				m_sprite.setScale(1, 1);
			}
			break;
		default:
			break;
	}
	return (true);
}

bool	CharacterOcto::onReleased(sf::Event::KeyEvent const& event)
{
	switch (event.code)
	{
		case sf::Keyboard::Left:
			m_controls[0] = false;
			break;
		case sf::Keyboard::Right:
			m_controls[1] = false;
			break;
		default:
			break;
	}
	if (std::find(m_controls.begin(), m_controls.end(), true) == m_controls.end())
	{
		m_sprite.setNextEvent(Idle);
	}
	return (true);
}

sf::Vector2f	CharacterOcto::getPosition()const
{
	return (m_box->getBaryCenter());
}
