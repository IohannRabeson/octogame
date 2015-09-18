#include "CharacterOcto.hpp"
#include "ResourceDefinitions.hpp"
#include "PhysicsEngine.hpp"
#include "ElevatorStream.hpp"
#include "GroundTransformNanoRobot.hpp"
#include "RepairNanoRobot.hpp"
#include "JumpNanoRobot.hpp"
#include "DoubleJumpNanoRobot.hpp"
#include "SlowFallNanoRobot.hpp"
#include <Application.hpp>
#include <AudioManager.hpp>
#include <ResourceManager.hpp>
#include <GraphicsManager.hpp>

CharacterOcto::CharacterOcto() :
	m_box(PhysicsEngine::getShapeBuilder().createRectangle(false)),
	m_eventBox(PhysicsEngine::getShapeBuilder().createCircle(false)),
	m_repairNanoRobot(nullptr),
	m_progress(Progress::getInstance()),
	m_engine(std::time(0)),
	m_jumpDistribution(0, 4),
	m_danceDistribution(2.f, 6.f),
	m_timeEventIdleMax(sf::seconds(4.f)),
	m_spriteScale(0.6f),
	m_pixelSecondJump(-1300.f),
	m_pixelSecondSlowFall(-300.f),
	m_pixelSecondWalk(320.f),
	m_pixelSecondAfterJump(-500.f),
	m_pixelSecondAfterFullJump(-400.f),
	m_pixelSecondMultiplier(800.f),
	m_deltaPositionY(27.f),
	m_numberOfJump(1),
	m_originMove(false),
	m_onGround(false),
	m_onElevator(false),
	m_useElevator(false),
	m_afterJump(false),
	m_keyLeft(false),
	m_keyRight(false),
	m_keySpace(false),
	m_keyUp(false),
	m_keyAction(false),
	m_collisionTile(false),
	m_collisionElevator(false),
	m_collisionElevatorEvent(false),
	m_doScale(false)
{
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	graphics.addKeyboardListener(this);

	if (m_progress.canMoveMap())
		giveNanoRobot(new GroundTransformNanoRobot());
	if (m_progress.canRepair())
		giveRepairNanoRobot(new RepairNanoRobot());
	if (m_progress.canJump())
		giveNanoRobot(new JumpNanoRobot());
	if (m_progress.canDoubleJump())
		giveNanoRobot(new DoubleJumpNanoRobot());
	if (m_progress.canSlowFall())
		giveNanoRobot(new SlowFallNanoRobot());

	for (auto & robot : m_nanoRobots)
	{
		robot->setPosition(getPosition());
		robot->transfertToOcto();
		m_progress.removeNanoRobot();
		robot->setState(NanoRobot::State::FollowOcto);
	}
}

CharacterOcto::~CharacterOcto(void)
{
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	graphics.removeKeyboardListener(this);
}

void	CharacterOcto::setup(void)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();

	m_box->setGameObject(this);
	m_box->setSize(sf::Vector2f(30.f, 85.f));
	m_box->setCollisionType(static_cast<std::uint32_t>(GameObjectType::Player));
	std::uint32_t mask = static_cast<std::uint32_t>(GameObjectType::Portal)
		| static_cast<std::uint32_t>(GameObjectType::GroundTransformNanoRobot)
		| static_cast<std::uint32_t>(GameObjectType::RepairNanoRobot)
		| static_cast<std::uint32_t>(GameObjectType::JumpNanoRobot)
		| static_cast<std::uint32_t>(GameObjectType::Elevator);
	m_box->setCollisionMask(mask);

	m_octoEvent.m_octo = this;
	m_eventBox->setGameObject(&m_octoEvent);
	m_eventBox->setRadius(400.f);
	m_eventBox->setCollisionType(static_cast<std::uint32_t>(GameObjectType::PlayerEvent));
	std::uint32_t maskEvent = static_cast<std::uint32_t>(GameObjectType::Portal)
		| static_cast<std::uint32_t>(GameObjectType::Elevator)
		| static_cast<std::uint32_t>(GameObjectType::CedricNpc)
		| static_cast<std::uint32_t>(GameObjectType::FannyNpc)
		| static_cast<std::uint32_t>(GameObjectType::JuNpc)
		| static_cast<std::uint32_t>(GameObjectType::TurbanNpc);
	m_eventBox->setCollisionMask(maskEvent);
	m_eventBox->setApplyGravity(false);
	m_eventBox->setType(AShape::Type::e_trigger);

	m_sprite.setSpriteSheet(resources.getSpriteSheet(NEW_OCTO_OSS));
	m_timeEventFall = sf::Time::Zero;
	m_timeEventIdle = sf::Time::Zero;
	m_timeEventDeath = sf::Time::Zero;
	m_timeEventInk = sf::Time::Zero;
	setupAnimation();
	setupMachine();
	m_sprite.setScale(m_spriteScale, m_spriteScale);
	m_sprite.restart();

	m_ink.setCanEmit(false);
	m_ink.setup(sf::Vector2f(3.f, 3.f));
	m_ink.setVelocity(sf::Vector2f(0.f, 100.f));
	m_ink.setEmitTimeRange(0.005f, 0.01f);
	m_ink.setGrowTimeRange(0.1f, 0.2f);
	m_ink.setLifeTimeRange(0.4f, 0.5f);
	m_ink.setScaleFactor(30.f);
	m_ink.setColor(sf::Color(0, 0, 0));
}

void	CharacterOcto::setupAnimation()
{
	typedef octo::CharacterAnimation::Frame			Frame;

	randomJumpAnimation();
	m_idleAnimation.setFrames({
			Frame(sf::seconds(0.4f), {10, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {11, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {12, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {13, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {14, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {15, sf::FloatRect(), sf::Vector2f()}),
			});
	m_idleAnimation.setLoop(octo::LoopMode::Loop);

	m_walkAnimation.setFrames({
			Frame(sf::seconds(0.3f), {0, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {1, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {2, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {3, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {4, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {5, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {6, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {7, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {8, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {9, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {10, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {11, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {12, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {13, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {14, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {15, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {16, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {17, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {18, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {19, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {20, sf::FloatRect(), sf::Vector2f()})
	});
	m_walkAnimation.setLoop(octo::LoopMode::Loop);

	m_jumpAnimation.setFrames({
			Frame(sf::seconds(0.3f), {26, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {27, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {28, sf::FloatRect(), sf::Vector2f()}),
			});
	m_jumpAnimation.setLoop(octo::LoopMode::NoLoop);

	m_fallAnimation.setFrames({
			Frame(sf::seconds(0.3f), {27, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {28, sf::FloatRect(), sf::Vector2f()}),
			});
	m_fallAnimation.setLoop(octo::LoopMode::Loop);

	//TODO define frame
	m_danceAnimation.setFrames({
			Frame(sf::seconds(0.4f), {10, sf::FloatRect(), sf::Vector2f()}),
			});
	m_danceAnimation.setLoop(octo::LoopMode::Loop);

	m_danceWithMusicAnimation.setFrames({
			Frame(sf::seconds(0.35f), {77, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.35f), {78, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.35f), {79, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.35f), {80, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {81, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.35f), {82, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.35f), {83, sf::FloatRect(), sf::Vector2f()}),
			});
	m_danceWithMusicAnimation.setLoop(octo::LoopMode::NoLoop);

	m_slowFallAnimation.setFrames({
			Frame(sf::seconds(0.2f), {49, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {50, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {51, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {52, sf::FloatRect(), sf::Vector2f()}),
			});
	m_slowFallAnimation.setLoop(octo::LoopMode::NoLoop);

	m_deathAnimation.setFrames({
			Frame(sf::seconds(0.4f), {35, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {36, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {37, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {38, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(1.4f), {39, sf::FloatRect(), sf::Vector2f()}),
			});
	m_deathAnimation.setLoop(octo::LoopMode::NoLoop);

	m_drinkAnimation.setFrames({
			Frame(sf::seconds(0.4f), {25, sf::FloatRect(), sf::Vector2f()}),
			});
	m_drinkAnimation.setLoop(octo::LoopMode::Loop);

	m_answerWolfAnimation.setFrames({
			Frame(sf::seconds(0.4f), {56, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {57, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {58, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {59, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {60, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(1.5f), {61, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {62, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {63, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {64, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {65, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {66, sf::FloatRect(), sf::Vector2f()}),
			});
	m_answerWolfAnimation.setLoop(octo::LoopMode::Loop);

	m_startElevatorAnimation.setFrames({
			Frame(sf::seconds(0.1f), {70, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {71, sf::FloatRect(), sf::Vector2f()})
			});
	m_startElevatorAnimation.setLoop(octo::LoopMode::NoLoop);

	m_elevatorAnimation.setFrames({
			Frame(sf::seconds(0.4f), {72, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {73, sf::FloatRect(), sf::Vector2f()}),
			});
	m_elevatorAnimation.setLoop(octo::LoopMode::Loop);
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
	StatePtr					state6;
	StatePtr					state7;
	StatePtr					state8;
	StatePtr					state9;
	StatePtr					state10;
	StatePtr					state11;
	StatePtr					state12;
	StatePtr					state13;

	state0 = std::make_shared<State>("Idle", m_idleAnimation, m_sprite);
	state1 = std::make_shared<State>("Left", m_walkAnimation, m_sprite);
	state2 = std::make_shared<State>("Right", m_walkAnimation, m_sprite);
	state3 = std::make_shared<State>("Jump", m_jumpAnimation, m_sprite);
	state4 = std::make_shared<State>("DoubleJump", m_jumpAnimation, m_sprite);
	state5 = std::make_shared<State>("Fall", m_fallAnimation, m_sprite);
	state6 = std::make_shared<State>("Dance", m_danceAnimation, m_sprite);
	state7 = std::make_shared<State>("SlowFall", m_slowFallAnimation, m_sprite);
	state8 = std::make_shared<State>("Death", m_deathAnimation, m_sprite);
	state9 = std::make_shared<State>("Drink", m_drinkAnimation, m_sprite);
	state10 = std::make_shared<State>("StartElevator", m_startElevatorAnimation, m_sprite);
	state11 = std::make_shared<State>("Elevator", m_elevatorAnimation, m_sprite);
	state12 = std::make_shared<State>("DanceWithMusic", m_danceWithMusicAnimation, m_sprite);
	state13 = std::make_shared<State>("StartJump", m_startJumpAnimation, m_sprite);

	machine.setStart(state0);

	machine.addTransition(Left, state0, state1);
	machine.addTransition(Left, state1, state1);
	machine.addTransition(Left, state2, state1);
	machine.addTransition(Left, state3, state1);
	machine.addTransition(Left, state4, state1);
	machine.addTransition(Left, state5, state1);
	machine.addTransition(Left, state6, state1);
	machine.addTransition(Left, state12, state1);
	machine.addTransition(Left, state13, state1);
	machine.addTransition(Left, state7, state1);
	machine.addTransition(Left, state8, state1);
	machine.addTransition(Left, state9, state1);
	machine.addTransition(Left, state10, state1);

	machine.addTransition(Right, state0, state2);
	machine.addTransition(Right, state1, state2);
	machine.addTransition(Right, state2, state2);
	machine.addTransition(Right, state3, state2);
	machine.addTransition(Right, state4, state2);
	machine.addTransition(Right, state5, state2);
	machine.addTransition(Right, state6, state2);
	machine.addTransition(Right, state12, state2);
	machine.addTransition(Right, state13, state2);
	machine.addTransition(Right, state7, state2);
	machine.addTransition(Right, state8, state2);
	machine.addTransition(Right, state9, state2);
	machine.addTransition(Right, state10, state2);

	machine.addTransition(StartJump, state0, state13);
	machine.addTransition(StartJump, state1, state13);
	machine.addTransition(StartJump, state2, state13);
	machine.addTransition(StartJump, state6, state13);
	machine.addTransition(StartJump, state12, state13);

	machine.addTransition(Jump, state13, state3);

	machine.addTransition(DoubleJump, state1, state4);
	machine.addTransition(DoubleJump, state2, state4);
	machine.addTransition(DoubleJump, state3, state4);
	machine.addTransition(DoubleJump, state13, state4);
	machine.addTransition(DoubleJump, state5, state4);
	machine.addTransition(DoubleJump, state7, state4);

	machine.addTransition(Fall, state0, state5);
	machine.addTransition(Fall, state1, state5);
	machine.addTransition(Fall, state2, state5);
	machine.addTransition(Fall, state3, state5);
	machine.addTransition(Fall, state4, state5);
	machine.addTransition(Fall, state5, state5);
	machine.addTransition(Fall, state6, state5);
	machine.addTransition(Fall, state7, state5);
	machine.addTransition(Fall, state10, state5);
	machine.addTransition(Fall, state11, state5);
	machine.addTransition(Fall, state12, state5);
	machine.addTransition(Fall, state13, state5);

	machine.addTransition(Dance, state0, state6);

	machine.addTransition(DanceWithMusic, state0, state12);

	machine.addTransition(SlowFall, state0, state7);
	machine.addTransition(SlowFall, state1, state7);
	machine.addTransition(SlowFall, state2, state7);
	machine.addTransition(SlowFall, state3, state7);
	machine.addTransition(SlowFall, state4, state7);
	machine.addTransition(SlowFall, state5, state7);
	machine.addTransition(SlowFall, state6, state7);
	machine.addTransition(SlowFall, state7, state7);
	machine.addTransition(SlowFall, state10, state7);
	machine.addTransition(SlowFall, state11, state7);
	machine.addTransition(SlowFall, state12, state7);
	machine.addTransition(SlowFall, state13, state7);

	machine.addTransition(Death, state0, state8);
	machine.addTransition(Death, state1, state8);
	machine.addTransition(Death, state2, state8);
	machine.addTransition(Death, state3, state8);
	machine.addTransition(Death, state13, state8);
	machine.addTransition(Death, state4, state8);
	machine.addTransition(Death, state5, state8);
	machine.addTransition(Death, state6, state8);
	machine.addTransition(Death, state12, state8);
	machine.addTransition(Death, state7, state8);
	machine.addTransition(Death, state9, state8);

	machine.addTransition(Drink, state0, state9);
	machine.addTransition(Drink, state1, state9);
	machine.addTransition(Drink, state2, state9);

	machine.addTransition(StartElevator, state0, state10);
	machine.addTransition(StartElevator, state1, state10);
	machine.addTransition(StartElevator, state2, state10);
	machine.addTransition(StartElevator, state3, state10);
	machine.addTransition(StartElevator, state4, state10);
	machine.addTransition(StartElevator, state5, state10);
	machine.addTransition(StartElevator, state6, state10);
	machine.addTransition(StartElevator, state7, state10);
	machine.addTransition(StartElevator, state10, state10);
	machine.addTransition(StartElevator, state12, state10);
	machine.addTransition(StartElevator, state13, state10);

	machine.addTransition(Elevator, state10, state11);

	machine.addTransition(Idle, state0, state0);
	machine.addTransition(Idle, state1, state0);
	machine.addTransition(Idle, state2, state0);
	machine.addTransition(Idle, state3, state0);
	machine.addTransition(Idle, state13, state0);
	machine.addTransition(Idle, state4, state0);
	machine.addTransition(Idle, state5, state0);
	machine.addTransition(Idle, state6, state0);
	machine.addTransition(Idle, state12, state0);
	machine.addTransition(Idle, state7, state0);
	machine.addTransition(Idle, state8, state0);
	machine.addTransition(Idle, state9, state0);

	m_sprite.setMachine(machine);
}

void	CharacterOcto::update(sf::Time frameTime)
{
	timeEvent(frameTime);
	if (endDeath())
	{
		dance();
		collisionElevatorUpdate();
		collisionTileUpdate();
		commitPhysicsToGraphics();
		commitEventToGraphics();
		m_sprite.update(frameTime);
		commitControlsToPhysics(frameTime.asSeconds());
	}
	else
	{
		commitPhysicsToGraphics();
		m_sprite.update(frameTime);
	}
	m_previousTop = m_box->getGlobalBounds().top;

	if (!m_collisionElevatorEvent && m_progress.canRepair())
		m_repairNanoRobot->setState(RepairNanoRobot::State::None);

	m_collisionTile = false;
	m_collisionElevator = false;
	m_collisionElevatorEvent = false;

	m_ink.update(frameTime);
	if (m_timeEventInk > sf::Time::Zero && m_timeEventInk < sf::seconds(0.07f))
	{
		m_ink.setCanEmit(true);
		m_ink.setPosition(m_box->getBaryCenter());
	}
	else
		m_ink.setCanEmit(false);

	for (auto & robot : m_nanoRobots)
	{
		robot->update(frameTime);
		robot->setPosition(m_box->getPosition() + sf::Vector2f(20.f, 0.f));
	}
}

void	CharacterOcto::timeEvent(sf::Time frameTime)
{
	switch (m_sprite.getCurrentEvent())
	{
		case Fall:
			m_timeEventFall += frameTime;
			break;
		case Idle:
			m_timeEventIdle += frameTime;
			break;
		case Death:
			m_timeEventDeath += frameTime;
			break;
		case DoubleJump:
			m_timeEventInk += frameTime;
			break;
		default:
			m_timeEventFall = sf::Time::Zero;
			m_timeEventIdle = sf::Time::Zero;
			m_timeEventDeath = sf::Time::Zero;
			m_timeEventInk = sf::Time::Zero;
			break;
	}
}

void	CharacterOcto::draw(sf::RenderTarget& render, sf::RenderStates states)const
{
	m_ink.draw(render);
	m_sprite.draw(render, states);
}

void	CharacterOcto::drawNanoRobot(sf::RenderTarget& render, sf::RenderStates states = sf::RenderStates())const
{
	for (auto & robot : m_nanoRobots)
		robot->draw(render, states);
}

void	CharacterOcto::onCollision(GameObjectType type, sf::Vector2f const& collisionDirection)
{
	switch(type)
	{
		case GameObjectType::Tile:
			// TODO
			if (collisionDirection.x == 0 && collisionDirection.y < 0)
				m_collisionTile = true;
			break;
		case GameObjectType::Elevator:
			m_collisionElevator = true;
			break;
		default:
			break;
	}
}

void	CharacterOcto::setTopElevator(float top)
{
	m_topElevator = top;
}

void	CharacterOcto::setPosition(sf::Vector2f const & position)
{
	m_box->setPosition(position);
}

void	CharacterOcto::giveNanoRobot(NanoRobot * robot)
{
	m_nanoRobots.push_back(std::unique_ptr<NanoRobot>(robot));
}

void	CharacterOcto::giveRepairNanoRobot(RepairNanoRobot * robot)
{
	m_nanoRobots.push_back(std::unique_ptr<NanoRobot>(robot));
	m_repairNanoRobot = robot;
}

void	CharacterOcto::repairElevator(ElevatorStream & elevator)
{
	if (m_progress.canRepair() && m_keyAction && getPosition().y > 100.f)
	{
		if (!elevator.isActivated())
		{
			elevator.activate();
			m_repairNanoRobot->setState(RepairNanoRobot::State::Repair);
			sf::Vector2f target = elevator.getPosition();
			target.x -= elevator.getWidth() / 2.f - octo::linearInterpolation(0.f, elevator.getWidth(), elevator.getRepairAdvancement());
			target.y -= 50.f;
			m_repairNanoRobot->setTarget(target);
		}
		else
			m_repairNanoRobot->setState(RepairNanoRobot::State::Done);
	}
	else if (m_progress.canRepair())
		m_repairNanoRobot->setState(RepairNanoRobot::State::None);
	m_collisionElevatorEvent = true;
}

void	CharacterOcto::usePortal(Portal & portal)
{
	if (m_keyUp)
	{
		m_progress.setNextDestination(portal.getDestination());
	}
}

void	CharacterOcto::collisionTileUpdate()
{
	if (!m_collisionTile)
	{
		m_onGround = false;
		onSky(static_cast<Events>(m_sprite.getCurrentEvent()));
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

void	CharacterOcto::onSky(Events event)
{
	switch (event)
	{
		case StartJump:
			if (m_sprite.isTerminated())
				m_sprite.setNextEvent(Jump);
			break;
		case Jump:
		case DoubleJump:
			if (m_box->getGlobalBounds().top > m_previousTop
					&& m_jumpVelocity != m_pixelSecondJump)
			{
				m_afterJump = true;
				m_afterJumpVelocity = m_pixelSecondAfterFullJump;
				m_sprite.setNextEvent(Fall);
			}
			break;
		case SlowFall:
		case Fall:
		case StartElevator:
		case Elevator:
			break;
		default:
			m_sprite.setNextEvent(Fall);
			break;
	}
}

void	CharacterOcto::collisionElevatorUpdate()
{
	sf::FloatRect const&	bounds = m_box->getGlobalBounds();
	float					top = bounds.top + (bounds.height / 2.f);

	if (m_collisionElevator)
	{
		m_onElevator = true;
		if (m_sprite.getCurrentEvent() == StartElevator)
		{
			if (!m_useElevator)
			{
				m_onTopElevator = false;
				m_useElevator = true;
				m_numberOfJump = 3;
				m_box->setApplyGravity(false);
			}
			if (m_sprite.isTerminated())
				m_sprite.setNextEvent(Elevator);
		}
		if (!m_keyUp)
		{
			m_useElevator = false;
			m_box->setApplyGravity(true);
		}
		if (top <= m_topElevator)
			m_onTopElevator = true;
	}
	else
	{
		m_onElevator = false;
		m_onTopElevator = false;
		if (m_useElevator)
		{
			m_useElevator = false;
			m_numberOfJump = 1;
			m_box->setApplyGravity(true);
			if (m_keyUp)
				m_sprite.setNextEvent(Fall);
		}
	}
}

bool	CharacterOcto::dieFall()
{
	if (m_timeEventFall > sf::seconds(2.0f))
	{
		m_sprite.setNextEvent(Death);
		return true;
	}
	return false;
}

bool	CharacterOcto::endDeath()
{
	if (m_sprite.getCurrentEvent() == Death)
	{
		if (m_timeEventDeath > sf::seconds(3.0f))
		{
			if (m_keyLeft)
				m_sprite.setNextEvent(Left);
			else if (m_keyRight)
				m_sprite.setNextEvent(Right);
			else
				m_sprite.setNextEvent(Idle);
		}
		else
			return false;
	}
	return true;
}

void	CharacterOcto::dance()
{
	if (m_timeEventIdle > m_timeEventIdleMax)
	{
		m_timeEventIdleMax = sf::seconds(m_danceDistribution(m_engine));
		if (octo::Application::getAudioManager().getMusicVolume() > 0.f)
			m_sprite.setNextEvent(DanceWithMusic);
	}
	if (m_sprite.getCurrentEvent() == DanceWithMusic && m_sprite.isTerminated())
		m_sprite.setNextEvent(Idle);
}

void	CharacterOcto::randomJumpAnimation()
{
	typedef octo::CharacterAnimation::Frame			Frame;

	if(!m_jumpDistribution(m_engine))
	{
		m_startJumpAnimation.setFrames({
				Frame(sf::seconds(0.2f), {80, sf::FloatRect(), sf::Vector2f()}),
				Frame(sf::seconds(0.2f), {81, sf::FloatRect(), sf::Vector2f()}),
				Frame(sf::seconds(0.2f), {82, sf::FloatRect(), sf::Vector2f()}),
				Frame(sf::seconds(0.2f), {83, sf::FloatRect(), sf::Vector2f()}),
				Frame(sf::seconds(0.2f), {81, sf::FloatRect(), sf::Vector2f()}),
				Frame(sf::seconds(0.2f), {80, sf::FloatRect(), sf::Vector2f()}),
				});
		m_startJumpAnimation.setLoop(octo::LoopMode::NoLoop);
	}
	else
	{
		m_startJumpAnimation.setFrames({
				Frame(sf::seconds(0.3f), {25, sf::FloatRect(), sf::Vector2f()}),
				Frame(sf::seconds(0.3f), {26, sf::FloatRect(), sf::Vector2f()})
				});
		m_startJumpAnimation.setLoop(octo::LoopMode::NoLoop);
	}
}

void	CharacterOcto::commitPhysicsToGraphics()
{

	sf::Vector2f const&	pos = m_box->getRenderCenter();
	float				xPos = pos.x - ((m_sprite.getLocalSize().x  * m_spriteScale) / 2.f);
	float				yPos =  pos.y - ((m_sprite.getLocalSize().y * m_spriteScale) - (m_box->getSize().y / 2.f));

	m_sprite.setPosition(sf::Vector2f(xPos, yPos + m_deltaPositionY));
	m_eventBox->setPosition(pos.x - m_eventBox->getRadius(), pos.y - m_eventBox->getRadius());
}

void	CharacterOcto::commitEventToGraphics()
{
	if (!m_doScale || m_box->getSleep())
		return;
	if (m_keyLeft && !m_originMove)
	{
		m_sprite.setScale(-1.f * m_spriteScale, 1.f * m_spriteScale);
		m_sprite.setOrigin(m_sprite.getOrigin().x + m_sprite.getLocalSize().x, 0.f);
		m_originMove = true;
	}
	else if (m_keyRight && m_originMove)
	{
		m_sprite.setScale(1.f * m_spriteScale, 1.f * m_spriteScale);
		m_sprite.setOrigin(m_sprite.getOrigin().x - m_sprite.getLocalSize().x, 0.f);
		m_originMove = false;
	}
	m_doScale = false;
}


void	CharacterOcto::commitControlsToPhysics(float frametime)
{
	sf::Vector2f	velocity = m_box->getVelocity();

	if (m_progress.canWalk())
	{
		if (m_keyLeft)
		{
			velocity.x = (-1.f * m_pixelSecondWalk);
		}
		else if (m_keyRight)
		{
			velocity.x = m_pixelSecondWalk;
		}
	}

	if (m_keySpace && (m_sprite.getCurrentEvent() == Jump || m_sprite.getCurrentEvent() == DoubleJump || m_sprite.getCurrentEvent() == StartJump))
	{
		velocity.y = m_jumpVelocity;
		m_jumpVelocity += m_pixelSecondMultiplier * frametime;
	}
	else if (m_afterJump && m_afterJumpVelocity < 0.f && !m_useElevator)
	{
		velocity.y = m_afterJumpVelocity;
		m_afterJumpVelocity += m_pixelSecondMultiplier * frametime;
	}

	if (m_keyUp)
	{
		if (m_sprite.getCurrentEvent() == SlowFall)
		{
			velocity.x *= 1.6f; // TODO fix, not frametime dependant
			velocity.y = m_pixelSecondSlowFall;
		}
		if (!m_onTopElevator)
		{
			if (m_sprite.getCurrentEvent() == StartElevator)
				velocity.y = (1.2f * m_pixelSecondSlowFall);
			if (m_sprite.getCurrentEvent() == Elevator)
				velocity.y = (2.5f * m_pixelSecondSlowFall);
		}
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
		case sf::Keyboard::Q:
			caseAction();
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
		m_doScale = true;
		if (m_onGround && m_progress.canWalk())
		{
			m_sprite.setNextEvent(Left);
		}
	}
}

void	CharacterOcto::caseRight()
{
	if (!m_keyRight)
	{
		m_keyRight = true;
		m_keyLeft = false;
		m_doScale = true;
		if (m_onGround && m_progress.canWalk())
		{
			m_sprite.setNextEvent(Right);
		}
	}
}

void	CharacterOcto::caseSpace()
{
	if (!m_keySpace)
	{
		randomJumpAnimation();
		m_keySpace = true;
		if (m_onGround && m_progress.canJump())
		{
			m_sprite.setNextEvent(StartJump);
			m_jumpVelocity = m_pixelSecondJump;
			m_numberOfJump = 1;
		}
		else if (m_numberOfJump == 1 && m_progress.canDoubleJump())
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
	if (!m_keyUp)
	{
		m_keyUp = true;
		if (!m_onGround  && m_progress.canSlowFall())
			m_sprite.setNextEvent(SlowFall);
		if (m_onElevator && m_progress.canUseElevator())
			m_sprite.setNextEvent(StartElevator);
	}
}

void CharacterOcto::caseAction()
{
	if (!m_keyAction)
	{
		m_keyAction = true;
	}
}

bool	CharacterOcto::onReleased(sf::Event::KeyEvent const& event)
{
	Events	state = static_cast<Events>(m_sprite.getCurrentEvent());
	bool otherKeyReleased = false;

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
			if (state == Jump || state == DoubleJump || state == StartJump)
			{
				m_afterJump = true;
				m_afterJumpVelocity = m_pixelSecondAfterJump;
			}
			break;
		case sf::Keyboard::Up:
			m_keyUp = false;
			break;
		case sf::Keyboard::Q:
			m_keyAction = false;
			break;
		default:
			otherKeyReleased = true;
			break;
	}
	if (state == Death || otherKeyReleased)
		return true;
	if (!m_onGround && !m_keyUp)
	{
		if (state != Fall)
		{
			if (state != StartJump)
				m_sprite.setNextEvent(Fall);
		}
	}
	if (m_onGround && !m_keyLeft && !m_keyRight && !m_keyUp)
	{
		if (state != Dance || state != DanceWithMusic)
		{
			m_sprite.setNextEvent(Idle);
		}
	}
	return true;
}

sf::Vector2f const &	CharacterOcto::getPhysicsPosition() const
{
	return m_box->getPosition();
}

sf::Vector2f const &	CharacterOcto::getPosition() const
{
	return m_box->getRenderCenter();
}

sf::Vector2f	CharacterOcto::getBubblePosition() const
{
	return (m_box->getBaryCenter() + sf::Vector2f(-40.f, -80.f));
}

