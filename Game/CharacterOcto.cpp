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
	m_pixelSecondJump(-1000.f),
	m_pixelSecondUmbrella(-150.f),
	m_pixelSecondWalk(320.f),
	m_pixelSecondAfterJump(-500.f),
	m_pixelSecondAfterFullJump(-400.f),
	m_pixelSecondElevator(-250.f),
	m_pixelSecondMultiplier(800.f),
	m_numberOfJump(1),
	m_originMove(false),
	m_onGround(false),
	m_onElevator(false),
	m_afterJump(false),
	m_keyLeft(false),
	m_keyRight(false),
	m_keySpace(false),
	m_keyUp(false)
{
	octo::ResourceManager&		resources = octo::Application::getResourceManager();

	m_box->setGameObject(this);
	m_sprite.setSpriteSheet(resources.getSpriteSheet(OCTO_COMPLETE_OSS));
	setupAnimation();
	setupMachine();
	m_sprite.restart();
	m_box->setSize(sf::Vector2f(80.f / 2.f,150.f));
}

void	CharacterOcto::setupAnimation()
{
	typedef octo::CharacterAnimation::Frame			Frame;

	m_idleAnimation.setFrames({
			Frame(sf::seconds(0.4f), {10, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {11, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {12, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {13, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {14, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {15, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
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
	m_jumpAnimation.setLoop(octo::LoopMode::NoLoop);

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
	m_umbrellaAnimation.setLoop(octo::LoopMode::NoLoop);

	m_elevatorAnimation.setFrames({
			Frame(sf::seconds(1.f), {23, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			});
	m_elevatorAnimation.setLoop(octo::LoopMode::Loop);

	m_deathAnimation.setFrames({
			Frame(sf::seconds(0.4f), {35, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {36, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {37, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {38, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			Frame(sf::seconds(1.4f), {39, sf::FloatRect(177 / 2, 0, 177, 152), sf::Vector2f()}),
			});
	m_deathAnimation.setLoop(octo::LoopMode::NoLoop);

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
	std::function<void()> clockDeadRestart = [this]{ m_clockDeath.restart();};
	std::function<void()> clockFallRestart = [this]{ m_clockFall.restart();};
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
	machine.addTransition(Left, state8, state1);
	machine.addTransition(Left, state10, state1);

	machine.addTransition(Right, state0, state2);
	machine.addTransition(Right, state1, state2);
	machine.addTransition(Right, state2, state2);
	machine.addTransition(Right, state3, state2);
	machine.addTransition(Right, state4, state2);
	machine.addTransition(Right, state5, state2);
	machine.addTransition(Right, state6, state2);
	machine.addTransition(Right, state7, state2);
	machine.addTransition(Right, state8, state2);
	machine.addTransition(Right, state10, state2);

	machine.addTransition(Jump, state0, state3);
	machine.addTransition(Jump, state1, state3);
	machine.addTransition(Jump, state2, state3);
	machine.addTransition(Jump, state6, state3);
	machine.addTransition(Jump, state8, state3);

	machine.addTransition(DoubleJump, state1, state4);
	machine.addTransition(DoubleJump, state2, state4);
	machine.addTransition(DoubleJump, state3, state4);
	machine.addTransition(DoubleJump, state5, state4);
	machine.addTransition(DoubleJump, state7, state4);
	machine.addTransition(DoubleJump, state8, state4);

	machine.addTransition(Fall, state0, state5, clockFallRestart);
	machine.addTransition(Fall, state1, state5, clockFallRestart);
	machine.addTransition(Fall, state2, state5, clockFallRestart);
	machine.addTransition(Fall, state3, state5, clockFallRestart);
	machine.addTransition(Fall, state4, state5, clockFallRestart);
	machine.addTransition(Fall, state5, state5, clockFallRestart);
	machine.addTransition(Fall, state6, state5, clockFallRestart);
	machine.addTransition(Fall, state7, state5, clockFallRestart);
	machine.addTransition(Fall, state8, state5, clockFallRestart);

	machine.addTransition(Dance, state0, state6);

	machine.addTransition(Umbrella, state1, state7);
	machine.addTransition(Umbrella, state2, state7);
	machine.addTransition(Umbrella, state3, state7);
	machine.addTransition(Umbrella, state4, state7);
	machine.addTransition(Umbrella, state5, state7);
	machine.addTransition(Umbrella, state7, state7);
	machine.addTransition(Umbrella, state8, state7);

	machine.addTransition(Elevator, state0, state8);
	machine.addTransition(Elevator, state1, state8);
	machine.addTransition(Elevator, state2, state8);
	machine.addTransition(Elevator, state3, state8);
	machine.addTransition(Elevator, state4, state8);
	machine.addTransition(Elevator, state5, state8);
	machine.addTransition(Elevator, state6, state8);
	machine.addTransition(Elevator, state7, state8);
	machine.addTransition(Elevator, state8, state8);

	machine.addTransition(Death, state0, state9, clockDeadRestart);
	machine.addTransition(Death, state1, state9, clockDeadRestart);
	machine.addTransition(Death, state2, state9, clockDeadRestart);
	machine.addTransition(Death, state3, state9, clockDeadRestart);
	machine.addTransition(Death, state4, state9, clockDeadRestart);
	machine.addTransition(Death, state5, state9, clockDeadRestart);
	machine.addTransition(Death, state6, state9, clockDeadRestart);
	machine.addTransition(Death, state7, state9, clockDeadRestart);
	machine.addTransition(Death, state8, state9, clockDeadRestart);
	machine.addTransition(Death, state10, state9, clockDeadRestart);

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
	machine.addTransition(Idle, state8, state0, clockAFKRestart);
	machine.addTransition(Idle, state9, state0, clockAFKRestart);
	machine.addTransition(Idle, state10, state0, clockAFKRestart);

	m_sprite.setMachine(machine);
}

void	CharacterOcto::update(sf::Time frameTime)
{
	m_sprite.update(frameTime);
	endDeath();
	dance();
	collisionTileUpdate(frameTime);
	collisionElevatorUpdate(frameTime);
	commitControlsToPhysics(frameTime);
	commitPhysicsToGraphics();
}

void	CharacterOcto::draw(sf::RenderTarget& render, sf::RenderStates states)const
{
	m_sprite.draw(render, states);
}

void	CharacterOcto::onCollision(GameObjectType type)
{
	switch(type){
		case GameObjectType::Tile:
			m_clockCollisionTile.restart();
			break;
		case GameObjectType::Elevator:
			m_clockCollisionElevator.restart();
			break;
		default:
			break;
	}
}

void	CharacterOcto::collisionTileUpdate(sf::Time frameTime)
{
	if (m_clockCollisionTile.getElapsedTime() > frameTime)
	{
		m_onGround = false;
		if (m_box->getGlobalBounds().top > m_previousTop
				&& m_sprite.getCurrentEvent() != Fall && !m_onElevator)
		{
			if (m_jumpVelocity != m_pixelSecondJump)
			{
				m_afterJump = true;
				m_afterJumpVelocity = m_pixelSecondAfterFullJump;
				if (m_sprite.getCurrentEvent() != Umbrella){
					m_sprite.setNextEvent(Fall);
				}
			}
		}
	}
	else
	{
		if (!m_onGround)
		{
			m_afterJump = false;
			m_onGround = true;
			m_numberOfJump = 1;
			if (dieFall())
				return;
			if (m_keyLeft)
				m_sprite.setNextEvent(Left);
			else if (m_keyRight)
				m_sprite.setNextEvent(Right);
			else
				m_sprite.setNextEvent(Idle);
		}
	}
}

void	CharacterOcto::collisionElevatorUpdate(sf::Time frameTime)
{
	sf::FloatRect const& bounds = m_box->getGlobalBounds();
	static bool maxHeight;
	float top = bounds.top + bounds.height;
	//TODO top pos elevator - 80.f
	float posElevator = -1400.f;
	if (m_clockCollisionElevator.getElapsedTime() < frameTime)
	{
		if (!m_onElevator)
		{
			m_onElevator = true;
			if (m_sprite.getCurrentEvent() != Umbrella)
				m_sprite.setNextEvent(Elevator);
			m_elevatorVelocity = m_pixelSecondElevator;
			m_numberOfJump = 3;
			maxHeight = false;
			m_box->setApplyGravity(false);
		}
		if (m_onElevator)
		{
			sf::Vector2f	velocity = m_box->getVelocity();
			if (top <= posElevator)
			{
				maxHeight = true;
			}
			if (top <= (posElevator + 200.f)
					&& maxHeight)
			{
				if (top <= posElevator
						&& m_sprite.getCurrentEvent() == Umbrella)
					m_sprite.setNextEvent(Elevator);
				else
					velocity.y = (-1.f * m_elevatorVelocity) * frameTime.asSeconds();
			}
			else
			{
				m_box->setApplyGravity(false);
				maxHeight = false;
				velocity.y = m_elevatorVelocity * frameTime.asSeconds();
			}
			m_box->setVelocity(velocity);
		}
	}
	else
	{
		if (m_onElevator)
		{
			if (m_sprite.getCurrentEvent() != Umbrella)
				m_sprite.setNextEvent(Fall);
			m_onElevator = false;
			m_box->setApplyGravity(true);
		}
	}
}

bool	CharacterOcto::dieFall()
{
	if (m_sprite.getCurrentEvent() == Fall
			&& m_clockFall.getElapsedTime() > sf::seconds(2.0f))
	{
		m_sprite.setNextEvent(Death);
		return true;
	}
	return false;
}

void	CharacterOcto::endDeath()
{
	if (m_sprite.getCurrentEvent() == Death
			&& m_clockDeath.getElapsedTime() > sf::seconds(3.0f))
	{
		m_sprite.setNextEvent(Idle);
	}
}

void	CharacterOcto::dance()
{
	if (m_sprite.getCurrentEvent() == Idle
			&& m_clockAFK.getElapsedTime() < sf::seconds(5.0f)
			&& m_clockAFK.getElapsedTime() > sf::seconds(4.0f))
	{
		m_sprite.setNextEvent(Dance);
	}
}

void	CharacterOcto::commitPhysicsToGraphics()
{
	sf::FloatRect const& bounds = m_box->getGlobalBounds();

	// TODO fix that
	m_sprite.setPosition(bounds.left - 65.f, bounds.top);
	m_previousTop = bounds.top;
}

void	CharacterOcto::commitControlsToPhysics(sf::Time frameTime)
{
	if (m_sprite.getCurrentEvent() == Death)
		return;

	sf::Vector2f	velocity = m_box->getVelocity();

	if (m_keyLeft)
	{
		velocity.x = (-1.f * m_pixelSecondWalk) * frameTime.asSeconds();
	}
	else if (m_keyRight)
	{
		velocity.x = m_pixelSecondWalk * frameTime.asSeconds();
	}

	if (m_keySpace
			&& (m_sprite.getCurrentEvent() == Jump || m_sprite.getCurrentEvent() == DoubleJump))
	{
		velocity.y = m_jumpVelocity * frameTime.asSeconds();
		m_jumpVelocity += (m_pixelSecondMultiplier * frameTime.asSeconds());
	}
	else if (m_afterJump && m_afterJumpVelocity < 0.f && !m_onElevator)
	{
		velocity.y = m_afterJumpVelocity * frameTime.asSeconds();
		m_afterJumpVelocity += (m_pixelSecondMultiplier * frameTime.asSeconds());
	}

	if (m_keyUp && m_sprite.getCurrentEvent() == Umbrella)
	{
		if (m_onElevator)
			velocity.y = (3.f * m_pixelSecondUmbrella) * frameTime.asSeconds();
		else
			velocity.y = m_pixelSecondUmbrella * frameTime.asSeconds();
	}
	m_box->setVelocity(velocity);
}

bool	CharacterOcto::onPressed(sf::Event::KeyEvent const& event)
{
	if (m_sprite.getCurrentEvent() == Death)
		return true;
	switch (event.code)
	{
		case sf::Keyboard::Left:
			caseLeft();
			break;
		case sf::Keyboard::Right:
			caseRight();
			break;
		case sf::Keyboard::Space:
			caseSpace();
			break;
		case sf::Keyboard::Up:
			caseUp();
			break;
		default:
			break;
	}
	return (true);
}

void	CharacterOcto::caseLeft()
{
	if (!m_keyLeft)
	{
		m_keyLeft = true;
		m_keyRight = false;
		if (m_onGround)
			m_sprite.setNextEvent(Left);
		if (!m_originMove)
		{
			m_sprite.setScale(-1, 1);
			m_sprite.setOrigin(m_sprite.getOrigin().x + 177.f, 0);
			m_originMove = true;
		}
	}
}

void	CharacterOcto::caseRight()
{
	if (!m_keyRight)
	{
		m_keyRight = true;
		m_keyLeft = false;
		if (m_onGround)
			m_sprite.setNextEvent(Right);
		if (m_originMove)
		{
			m_sprite.setScale(1, 1);
			m_sprite.setOrigin(m_sprite.getOrigin().x - 177.f, 0);
			m_originMove = false;
		}
	}
}

void	CharacterOcto::caseSpace()
{
	if (!m_keySpace)
	{
		m_keySpace = true;
		if (m_onGround)
		{
			m_sprite.setNextEvent(Jump);
			m_jumpVelocity = m_pixelSecondJump;
			m_numberOfJump = 1;
		}
		else if (m_numberOfJump == 1)
		{
			m_sprite.setNextEvent(DoubleJump);
			m_afterJump = false;
			m_jumpVelocity = m_pixelSecondJump;
			m_numberOfJump = 2;
		}
		else
			m_numberOfJump = 3;
	}
}

void CharacterOcto::caseUp()
{
	if (!m_keyUp && !m_onGround)
	{
		m_keyUp = true;
		m_sprite.setNextEvent(Umbrella);
	}
}

bool	CharacterOcto::onReleased(sf::Event::KeyEvent const& event)
{
	switch (event.code)
	{
		case sf::Keyboard::Left:
			m_keyLeft = false;
			break;
		case sf::Keyboard::Right:
			m_keyRight = false;
			break;
		case sf::Keyboard::Space:
			m_keySpace = false;
			if (!m_afterJump && !m_onGround){
				m_afterJump = true;
				m_afterJumpVelocity = m_pixelSecondAfterJump;
			}
			break;
		case sf::Keyboard::Up:
			m_keyUp = false;
		default:
			break;
	}
	if (m_sprite.getCurrentEvent() == Death)
		return true;
	if (m_onElevator)
	{
		m_sprite.setNextEvent(Elevator);
	}
	if (!m_onGround && !m_keyUp && !m_onElevator)
	{
		if (m_sprite.getCurrentEvent() != Fall)
		{
			m_sprite.setNextEvent(Fall);
		}
	}
	if (m_onGround && !m_keyLeft && !m_keyRight && !m_keyUp){
		m_sprite.restart();
		m_sprite.setNextEvent(Idle);
	}
	return (true);
}

sf::Vector2f	CharacterOcto::getPosition() const
{
	//TODO fix center
	return (m_box->getBaryCenter() + sf::Vector2f(0.f, -300.f));
}

