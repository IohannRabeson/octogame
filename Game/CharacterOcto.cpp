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
	m_box(PhysicsEngine::getShapeBuilder().createRectangle(false)),
	m_pixelSecond(320.f),
	m_canDoubleJump(false),
	m_onGround(true)
	{
	octo::ResourceManager&		resources = octo::Application::getResourceManager();
	
	m_sprite.setSpriteSheet(resources.getSpriteSheet(OCTO_COMPLETE_OSS));
	setupAnimation();
	setupMachine();
	m_sprite.restart();
	m_box->setSize(sf::Vector2f(177.f / 2.f,152.f));
	m_originMoove = false;
	std::fill(m_controls.begin(), m_controls.end(), false);
}

void	CharacterOcto::setupAnimation()
{
	typedef octo::CharacterAnimation::Frame			Frame;
	m_idleAnimation.setFrames({Frame(sf::seconds(0.4f), {0, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			});

	m_idleAnimation.setLoop(octo::LoopMode::Loop);
	m_walkAnimation.setFrames({Frame(sf::seconds(0.2f), {0, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {1, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {2, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {3, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {4, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {5, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {6, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()})});
	m_walkAnimation.setLoop(octo::LoopMode::Loop);

	m_jumpAnimation.setFrames({
			Frame(sf::seconds(0.4f), {25, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {26, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()})
			});
	m_jumpAnimation.setLoop(octo::LoopMode::Loop);

	m_fallAnimation.setFrames({
			Frame(sf::seconds(0.4f), {29, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {30, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			});
	m_fallAnimation.setLoop(octo::LoopMode::Loop);
}

void	CharacterOcto::setupMachine()
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					state0;
	StatePtr					state1;
	StatePtr					state2;
	StatePtr					state3;
	StatePtr					state4;
	StatePtr					state5;

	state0 = std::make_shared<State>("0", m_idleAnimation, m_sprite);
	state1 = std::make_shared<State>("1", m_walkAnimation, m_sprite);
	state2 = std::make_shared<State>("2", m_walkAnimation, m_sprite);
	state3 = std::make_shared<State>("3", m_jumpAnimation, m_sprite);
	state4 = std::make_shared<State>("4", m_jumpAnimation, m_sprite);
	state5 = std::make_shared<State>("5", m_fallAnimation, m_sprite);
	
	machine.setStart(state0);
	machine.addTransition(Left, state0, state1);
	machine.addTransition(Left, state2, state1);
	machine.addTransition(Left, state3, state1);
	machine.addTransition(Left, state4, state1);
	machine.addTransition(Left, state5, state1);

	machine.addTransition(Right, state0, state2);
	machine.addTransition(Right, state1, state2);
	machine.addTransition(Right, state3, state2);
	machine.addTransition(Right, state4, state2);
	machine.addTransition(Right, state5, state2);

	machine.addTransition(Jump, state0, state3);
	machine.addTransition(Jump, state1, state3);
	machine.addTransition(Jump, state2, state3);
	machine.addTransition(Jump, state4, state3);

	machine.addTransition(DoubleJump, state3, state4);
	machine.addTransition(DoubleJump, state5, state4);
	machine.addTransition(DoubleJump, state0, state4);

	machine.addTransition(Fall, state0, state5);
	machine.addTransition(Fall, state1, state5);
	machine.addTransition(Fall, state2, state5);
	machine.addTransition(Fall, state3, state5);
	machine.addTransition(Fall, state4, state5);

	machine.addTransition(Idle, state0, state0);
	machine.addTransition(Idle, state1, state0);
	machine.addTransition(Idle, state2, state0);
	machine.addTransition(Idle, state3, state0);
	machine.addTransition(Idle, state4, state0);
	machine.addTransition(Idle, state5, state0);

	m_sprite.setMachine(machine);
}

void	CharacterOcto::update(sf::Time frameTime)
{
	m_sprite.update(frameTime);
	PhysicsEngine::getInstance().update(frameTime.asSeconds());
	commitControlsToPhysics(frameTime);
	commitPhysicsToGraphics();
}

void	CharacterOcto::draw(sf::RenderTarget& render, sf::RenderStates states)const
{
	m_sprite.draw(render, states);
}

void	CharacterOcto::commitPhysicsToGraphics()
{
	sf::FloatRect const& bounds = m_box->getGlobalBounds();

	m_sprite.setPosition(bounds.left, bounds.top);
}

void	CharacterOcto::commitControlsToPhysics(sf::Time frameTime)
{
	sf::Vector2f	velocity = m_box->getVelocity();
	if (m_controls[0])
	{
		velocity.x = (-1 * m_pixelSecond) * frameTime.asSeconds();
	}
	else if (m_controls[1])
	{
		velocity.x = m_pixelSecond * frameTime.asSeconds();
	}
	if (m_controls[2] && (m_jumpVelocity * frameTime.asSeconds() < 0)
			&& (m_sprite.getCurrentEvent() == Jump || m_sprite.getCurrentEvent() == DoubleJump))
	{
		velocity.y = m_jumpVelocity++;
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
				if (!m_originMoove){
					m_sprite.setScale(-1, 1);
					m_sprite.setOrigin(m_sprite.getOrigin().x + 177, 0);
					m_originMoove = true;
				}
			}
			break;
		case sf::Keyboard::Right:
			if (m_controls[1] == false)
			{
				m_controls[1] = true;
				m_sprite.setNextEvent(Right);
				if (m_originMoove){
					m_sprite.setScale(1, 1);
					m_sprite.setOrigin(m_sprite.getOrigin().x - 177, 0);
					m_originMoove = false;
				}
			}
			break;
		case sf::Keyboard::Space:
			if (m_controls[2] == false)
			{
				m_controls[2] = true;
				m_jumpVelocity = -40.f;
				if (m_canDoubleJump && m_sprite.canGetEvent(DoubleJump)){
					m_sprite.setNextEvent(DoubleJump);
					m_canDoubleJump = false;
					m_onGround = false;
				}
				else if (m_sprite.canGetEvent(Jump) && m_onGround){
					m_sprite.setNextEvent(Jump);
					m_canDoubleJump = true;
					m_onGround = false;
				}
			}
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
		case sf::Keyboard::Space:
			m_controls[2] = false;
		default:
			break;
	}
	if (std::find(m_controls.begin(), m_controls.end(), true) == m_controls.end())
	{
		if (m_sprite.getCurrentEvent() == Jump || m_sprite.getCurrentEvent() == DoubleJump)
			m_sprite.setNextEvent(Fall);
		else{
			m_onGround = true;
			m_sprite.setNextEvent(Idle);
		}
	}
	return (true);
}

sf::Vector2f	CharacterOcto::getPosition()const
{
	return (m_box->getBaryCenter());
}

