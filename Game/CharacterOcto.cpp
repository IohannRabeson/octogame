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
	m_doubleJump(false),
	m_onGround(true),
	m_afterJump(false)
{
	octo::ResourceManager&		resources = octo::Application::getResourceManager();

	m_box->setGameObject(this);
	m_sprite.setSpriteSheet(resources.getSpriteSheet(OCTO_COMPLETE_OSS));
	setupAnimation();
	setupMachine();
	m_sprite.restart();
	m_box->setSize(sf::Vector2f(177.f / 2.f,152.f));
	m_originMoove = false;
	m_sprite.setNextEvent(Idle);
	std::fill(m_controls.begin(), m_controls.end(), false);
}

void	CharacterOcto::setupAnimation()
{
	typedef octo::CharacterAnimation::Frame			Frame;

	m_idleAnimation.setFrames({
			Frame(sf::seconds(0.4f), {0, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			});
	m_idleAnimation.setLoop(octo::LoopMode::Loop);

	m_walkAnimation.setFrames({
			Frame(sf::seconds(0.2f), {0, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {1, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {2, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {3, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {4, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {5, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {6, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {7, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {8, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {9, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {10, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {11, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {12, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {13, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {14, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {15, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {16, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {17, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {18, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {19, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {20, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()})
	});
	m_walkAnimation.setLoop(octo::LoopMode::Loop);

	m_jumpAnimation.setFrames({
			Frame(sf::seconds(0.4f), {25, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {26, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()})
			});

	m_fallAnimation.setFrames({
			Frame(sf::seconds(0.2f), {27, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {28, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			});
	m_fallAnimation.setLoop(octo::LoopMode::Loop);

	m_danceAnimation.setFrames({
			Frame(sf::seconds(0.4f), {10, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {11, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {12, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {13, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {14, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {15, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			});
	m_danceAnimation.setLoop(octo::LoopMode::Loop);

	m_umbrellaAnimation.setFrames({
			Frame(sf::seconds(0.4f), {28, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			});
	m_umbrellaAnimation.setLoop(octo::LoopMode::Loop);

	m_elevatorAnimation.setFrames({
			Frame(sf::seconds(0.4f), {25, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			});
	m_elevatorAnimation.setLoop(octo::LoopMode::Loop);

	m_deathAnimation.setFrames({
			Frame(sf::seconds(0.4f), {35, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {36, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {37, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {38, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {39, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			});
	m_deathAnimation.setLoop(octo::LoopMode::Loop);

	m_drinkAnimation.setFrames({
			Frame(sf::seconds(0.4f), {25, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			});
	m_drinkAnimation.setLoop(octo::LoopMode::Loop);

}

void	CharacterOcto::setupMachine()
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	std::function<void()> clockAFKRestart = [this]{ m_clockAFK.restart();};
	octo::FiniteStateMachine	machine;
	StatePtr					state0;
	StatePtr					state1;
	StatePtr					state2;
	StatePtr					state3;
	StatePtr					state4;
	StatePtr					state5;
	StatePtr					state6;
	StatePtr					state7;
	StatePtr					state8;
	StatePtr					state9;
	StatePtr					state10;

	state0 = std::make_shared<State>("Idle", m_idleAnimation, m_sprite);
	state1 = std::make_shared<State>("Left", m_walkAnimation, m_sprite);
	state2 = std::make_shared<State>("Right", m_walkAnimation, m_sprite);
	state3 = std::make_shared<State>("Jump", m_jumpAnimation, m_sprite);
	state4 = std::make_shared<State>("DoubleJump", m_jumpAnimation, m_sprite);
	state5 = std::make_shared<State>("Fall", m_fallAnimation, m_sprite);
	state6 = std::make_shared<State>("Dance", m_danceAnimation, m_sprite);
	state7 = std::make_shared<State>("Umbrella", m_umbrellaAnimation, m_sprite);
	state8 = std::make_shared<State>("Elevator", m_elevatorAnimation, m_sprite);
	state9 = std::make_shared<State>("Death", m_deathAnimation, m_sprite);
	state10 = std::make_shared<State>("Drink", m_drinkAnimation, m_sprite);

	machine.setStart(state0);

	machine.addTransition(Left, state0, state1);
	machine.addTransition(Left, state1, state1);
	machine.addTransition(Left, state2, state1);
	machine.addTransition(Left, state3, state1);
	machine.addTransition(Left, state4, state1);
	machine.addTransition(Left, state5, state1);
	machine.addTransition(Left, state6, state1);
	machine.addTransition(Left, state7, state1);
	machine.addTransition(Left, state10, state1);

	machine.addTransition(Right, state0, state2);
	machine.addTransition(Right, state1, state2);
	machine.addTransition(Right, state2, state2);
	machine.addTransition(Right, state3, state2);
	machine.addTransition(Right, state4, state2);
	machine.addTransition(Right, state5, state2);
	machine.addTransition(Right, state6, state2);
	machine.addTransition(Right, state7, state2);
	machine.addTransition(Right, state10, state2);

	machine.addTransition(Jump, state0, state3);
	machine.addTransition(Jump, state1, state3);
	machine.addTransition(Jump, state2, state3);
	machine.addTransition(Jump, state6, state3);
	machine.addTransition(Jump, state8, state3);

	machine.addTransition(DoubleJump, state3, state4);
	machine.addTransition(DoubleJump, state5, state4);

	machine.addTransition(Fall, state0, state5);
	machine.addTransition(Fall, state1, state5);
	machine.addTransition(Fall, state2, state5);
	machine.addTransition(Fall, state3, state5);
	machine.addTransition(Fall, state4, state5);
	machine.addTransition(Fall, state5, state5);
	machine.addTransition(Fall, state7, state5);

	machine.addTransition(Dance, state0, state6);

	machine.addTransition(Umbrella, state3, state7);
	machine.addTransition(Umbrella, state4, state7);
	machine.addTransition(Umbrella, state5, state7);
	machine.addTransition(Umbrella, state7, state7);

	machine.addTransition(Elevator, state0, state8);
	machine.addTransition(Elevator, state1, state8);
	machine.addTransition(Elevator, state2, state8);
	machine.addTransition(Elevator, state5, state8);
	machine.addTransition(Elevator, state7, state8);

	machine.addTransition(Death, state0, state9);
	machine.addTransition(Death, state1, state9);
	machine.addTransition(Death, state2, state9);
	machine.addTransition(Death, state3, state9);
	machine.addTransition(Death, state4, state9);
	machine.addTransition(Death, state5, state9);
	machine.addTransition(Death, state6, state9);
	machine.addTransition(Death, state7, state9);
	machine.addTransition(Death, state8, state9);

	machine.addTransition(Drink, state0, state10);
	machine.addTransition(Drink, state1, state10);
	machine.addTransition(Drink, state2, state10);

	machine.addTransition(Idle, state0, state0);
	machine.addTransition(Idle, state1, state0, clockAFKRestart);
	machine.addTransition(Idle, state2, state0, clockAFKRestart);
	machine.addTransition(Idle, state3, state0, clockAFKRestart);
	machine.addTransition(Idle, state4, state0, clockAFKRestart);
	machine.addTransition(Idle, state5, state0, clockAFKRestart);
	machine.addTransition(Idle, state6, state0, clockAFKRestart);
	machine.addTransition(Idle, state7, state0, clockAFKRestart);
	machine.addTransition(Idle, state10, state0, clockAFKRestart);
	/*
	   std::ofstream ms;
	   ms.open("octoDot.dot");
	   machine.exportDot(ms, "octo");
	   ms.close();
	   */
	m_clockCollision.restart();
	m_sprite.setMachine(machine);
}

void	CharacterOcto::update(sf::Time frameTime)
{
	if (m_clockCollision.getElapsedTime() > frameTime){
		if (m_box->getGlobalBounds().top > m_prevY && m_sprite.getCurrentEvent() != Fall){
			if (!m_doubleJump && m_sprite.getCurrentEvent() != Umbrella){
				m_sprite.setNextEvent(Fall);
			}
			else
				m_doubleJump = false;
		}
		else {
		}
	}
	else{
		if (m_sprite.getCurrentEvent() == Fall || m_sprite.getCurrentEvent() == Umbrella){
			m_sprite.restart();
			m_clockAFK.restart();
			if (m_controls[0] == true)
				m_sprite.setNextEvent(Left);
			else if (m_controls[1] == true)
				m_sprite.setNextEvent(Right);
			else
				m_sprite.setNextEvent(Idle);
			m_onGround = true;
			m_canDoubleJump = false;
			m_afterJump = false;
		}
	}
	m_sprite.update(frameTime);
	PhysicsEngine::getInstance().update(frameTime.asSeconds());
	commitControlsToPhysics(frameTime);
	commitPhysicsToGraphics();
	dance();
}

void	CharacterOcto::draw(sf::RenderTarget& render, sf::RenderStates states)const
{
	m_sprite.draw(render, states);
}

void	CharacterOcto::onCollision(GameObjectType type)
{
	if (type == GameObjectType::Tile){
		m_clockCollision.restart();
	}
}

void	CharacterOcto::dance()
{
	if (m_sprite.getCurrentEvent() == Idle && m_clockAFK.getElapsedTime().asSeconds() > 2.4f)
		m_sprite.setNextEvent(Dance);

}

void	CharacterOcto::commitPhysicsToGraphics()
{
	sf::FloatRect const& bounds = m_box->getGlobalBounds();

	// TODO fix that
	m_sprite.setPosition(bounds.left - 50, bounds.top);
	m_prevY = bounds.top;
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
	else if (m_controls[2] && m_sprite.getCurrentEvent() == Umbrella){
		velocity.y = -5.f;
	}
	else if (m_afterJump && m_jumpVelocity < 0 )
		velocity.y = m_jumpVelocity++;
	m_box->setVelocity(velocity);
}

bool	CharacterOcto::onPressed(sf::Event::KeyEvent const& event)
{
	switch (event.code)
	{
		case sf::Keyboard::Left:
			if (m_controls[0] == false && m_sprite.canGetEvent(Left))
			{
				m_controls[0] = true;
				m_controls[1] = false;
				m_sprite.setNextEvent(Left);
				if (!m_originMoove){
					m_sprite.setScale(-1, 1);
					m_sprite.setOrigin(m_sprite.getOrigin().x + 177, 0);
					m_originMoove = true;
				}
			}
			break;
		case sf::Keyboard::Right:
			if (m_controls[1] == false && m_sprite.canGetEvent(Right))
			{
				m_controls[1] = true;
				m_controls[0] = false;
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
/*		std::cout << static_cast<int>(m_sprite.getCurrentEvent()) << "->";
		for ( auto d : m_sprite.getPossibleEvents()){
		std::cout << static_cast<int>(d) << ",";
		}
		std::cout << "(" << event.code << ")";
*/
				m_controls[2] = true;
				if (m_onGround && m_sprite.canGetEvent(Jump)){
					m_onGround = false;
					m_sprite.setNextEvent(Jump);
					m_canDoubleJump = true;
					m_jumpVelocity = -50.f;
				}
				else if (m_canDoubleJump && m_sprite.canGetEvent(DoubleJump)){
					m_doubleJump = true;
					m_sprite.setNextEvent(DoubleJump);
					m_canDoubleJump = false;
					m_jumpVelocity = -50.f;
				}
				else if (!m_onGround){
					m_sprite.setNextEvent(Umbrella);
				}
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
		case sf::Keyboard::Space:
			m_controls[2] = false;
			if (!m_afterJump){
			m_afterJump = true;
			m_jumpVelocity = -20.f;}
			m_sprite.setNextEvent(Fall);
		default:
			break;
	}
	if (m_sprite.canGetEvent(Idle) && m_onGround && std::find(m_controls.begin(), m_controls.end() - 1, true) == m_controls.end() -1)
	{
		m_sprite.setNextEvent(Idle);
	}
	return (true);
}

sf::Vector2f	CharacterOcto::getPosition()const
{
	return (m_box->getBaryCenter());
}

