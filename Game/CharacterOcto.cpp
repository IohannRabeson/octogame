#include <Application.hpp>
#include <AudioManager.hpp>
#include <PostEffectManager.hpp>
#include <ResourceManager.hpp>
#include <GraphicsManager.hpp>

#include "CharacterOcto.hpp"
#include "OctoSound.hpp"
#include "ResourceDefinitions.hpp"
#include "PhysicsEngine.hpp"
#include "ElevatorStream.hpp"
#include "SpaceShip.hpp"
#include "GroundTransformNanoRobot.hpp"
#include "RepairNanoRobot.hpp"
#include "JumpNanoRobot.hpp"
#include "DoubleJumpNanoRobot.hpp"
#include "SlowFallNanoRobot.hpp"
#include "WaterNanoRobot.hpp"

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
	m_numberOfJump(0),
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
	m_keyPortal(false),
	m_keyElevator(false),
	m_collisionTile(false),
	m_collisionElevator(false),
	m_collisionPortal(false),
	m_collisionElevatorEvent(false),
	m_collisionSpaceShip(false),
	m_inWater(false)
{
	m_sound.reset(new OctoSound());
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
	if (m_progress.canUseWaterJump())
		giveNanoRobot(new WaterNanoRobot());

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

void	CharacterOcto::setup(ABiome & biome)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();

	m_waterLevel = biome.getWaterLevel();
	m_box->setGameObject(this);
	m_box->setSize(sf::Vector2f(30.f, 85.f));
	m_box->setCollisionType(static_cast<std::size_t>(GameObjectType::Player));
	std::size_t mask = static_cast<std::size_t>(GameObjectType::Portal)
		| static_cast<std::size_t>(GameObjectType::GroundTransformNanoRobot)
		| static_cast<std::size_t>(GameObjectType::RepairNanoRobot)
		| static_cast<std::size_t>(GameObjectType::JumpNanoRobot)
		| static_cast<std::size_t>(GameObjectType::DoubleJumpNanoRobot)
		| static_cast<std::size_t>(GameObjectType::Elevator)
		| static_cast<std::size_t>(GameObjectType::Tent)
		| static_cast<std::size_t>(GameObjectType::Concert)
		| static_cast<std::size_t>(GameObjectType::CedricNpc)
		| static_cast<std::size_t>(GameObjectType::Bouibouik);
	m_box->setCollisionMask(mask);

	m_octoEvent.m_octo = this;
	m_eventBox->setGameObject(&m_octoEvent);
	m_eventBox->setRadius(400.f);
	m_eventBox->setCollisionType(static_cast<std::size_t>(GameObjectType::PlayerEvent));
	std::size_t maskEvent = static_cast<std::size_t>(GameObjectType::Portal)
		| static_cast<std::size_t>(GameObjectType::Elevator)
		| static_cast<std::size_t>(GameObjectType::SpaceShip)
		| static_cast<std::size_t>(GameObjectType::CedricNpc)
		| static_cast<std::size_t>(GameObjectType::FannyNpc)
		| static_cast<std::size_t>(GameObjectType::FranfranNpc)
		| static_cast<std::size_t>(GameObjectType::IohannNpc)
		| static_cast<std::size_t>(GameObjectType::PunkNpc)
		| static_cast<std::size_t>(GameObjectType::GuiNpc)
		| static_cast<std::size_t>(GameObjectType::JuNpc)
		| static_cast<std::size_t>(GameObjectType::LucienNpc)
		| static_cast<std::size_t>(GameObjectType::OldDesertStaticNpc)
		| static_cast<std::size_t>(GameObjectType::VinceNpc)
		| static_cast<std::size_t>(GameObjectType::TurbanNpc);
	m_eventBox->setCollisionMask(maskEvent);
	m_eventBox->setApplyGravity(false);
	m_eventBox->setType(AShape::Type::e_trigger);

	m_sprite.setSpriteSheet(resources.getSpriteSheet(OCTO_OSS));

	m_timeEventFall = sf::Time::Zero;
	m_timeEventIdle = sf::Time::Zero;
	m_timeEventDeath = sf::Time::Zero;
	m_timeEventInk = sf::Time::Zero;
	setupAnimation();
	setupMachine();
	m_sprite.setScale(m_spriteScale, m_spriteScale);
	m_sprite.restart();

	m_ploufParticle.canEmit(false);
	m_ploufParticle.setColor(biome.getWaterColor());

	m_inkParticle.setCanEmit(false);
	m_inkParticle.setup(sf::Vector2f(3.f, 3.f));
	m_inkParticle.setVelocity(sf::Vector2f(0.f, 100.f));
	m_inkParticle.setEmitTimeRange(0.005f, 0.01f);
	m_inkParticle.setGrowTimeRange(0.1f, 0.2f);
	m_inkParticle.setLifeTimeRange(0.4f, 0.5f);
	m_inkParticle.setScaleFactor(30.f);
	m_inkParticle.setColor(sf::Color(0, 0, 0));
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
			Frame(sf::seconds(0.2f), {77, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {78, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {79, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {80, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {81, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {82, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {84, sf::FloatRect(), sf::Vector2f()}),
			});
	m_danceWithMusicAnimation.setLoop(octo::LoopMode::NoLoop);

	m_startSlowFallAnimation.setFrames({
			Frame(sf::seconds(0.2f), {49, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {50, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {51, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {52, sf::FloatRect(), sf::Vector2f()}),
			});
	m_startSlowFallAnimation.setLoop(octo::LoopMode::NoLoop);

	m_slowFallAnimation.setFrames({
			Frame(sf::seconds(0.2f), {52, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {53, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {54, sf::FloatRect(), sf::Vector2f()}),
			});
	m_slowFallAnimation.setLoop(octo::LoopMode::Loop);

	m_deathAnimation.setFrames({
			Frame(sf::seconds(0.2f), {35, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {36, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {37, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {38, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {39, sf::FloatRect(), sf::Vector2f()}),
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

	m_portalAnimation.setFrames({
			Frame(sf::seconds(0.3f), {91, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {92, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {93, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.05f), {94, sf::FloatRect(), sf::Vector2f()}),
			});
	m_portalAnimation.setLoop(octo::LoopMode::NoLoop);

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
	StatePtr					state14;
	StatePtr					state15;
	StatePtr					state16;
	StatePtr					state17;

	state0 = std::make_shared<State>("Idle", m_idleAnimation, m_sprite);
	state1 = std::make_shared<State>("Left", m_walkAnimation, m_sprite);
	state2 = std::make_shared<State>("Right", m_walkAnimation, m_sprite);
	state3 = std::make_shared<State>("Jump", m_jumpAnimation, m_sprite);
	state4 = std::make_shared<State>("DoubleJump", m_jumpAnimation, m_sprite);
	state5 = std::make_shared<State>("Fall", m_fallAnimation, m_sprite);
	state6 = std::make_shared<State>("Dance", m_danceAnimation, m_sprite);
	state7 = std::make_shared<State>("StartSlowFall", m_startSlowFallAnimation, m_sprite);
	state8 = std::make_shared<State>("Death", m_deathAnimation, m_sprite);
	state9 = std::make_shared<State>("Drink", m_drinkAnimation, m_sprite);
	state10 = std::make_shared<State>("StartElevator", m_startElevatorAnimation, m_sprite);
	state11 = std::make_shared<State>("Elevator", m_elevatorAnimation, m_sprite);
	state12 = std::make_shared<State>("DanceWithMusic", m_danceWithMusicAnimation, m_sprite);
	state13 = std::make_shared<State>("StartJump", m_startJumpAnimation, m_sprite);
	state14 = std::make_shared<State>("SlowFall", m_slowFallAnimation, m_sprite);
	state15 = std::make_shared<State>("StartWaterJump", m_startElevatorAnimation, m_sprite);
	state16 = std::make_shared<State>("WaterJump", m_elevatorAnimation, m_sprite);
	state17 = std::make_shared<State>("PortalEvent", m_portalAnimation, m_sprite);

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
	machine.addTransition(Left, state9, state1);
	machine.addTransition(Left, state10, state1);
	machine.addTransition(Left, state12, state1);
	machine.addTransition(Left, state13, state1);
	machine.addTransition(Left, state14, state1);
	machine.addTransition(Left, state15, state1);
	machine.addTransition(Left, state16, state1);

	machine.addTransition(Right, state0, state2);
	machine.addTransition(Right, state1, state2);
	machine.addTransition(Right, state2, state2);
	machine.addTransition(Right, state3, state2);
	machine.addTransition(Right, state4, state2);
	machine.addTransition(Right, state5, state2);
	machine.addTransition(Right, state6, state2);
	machine.addTransition(Right, state7, state2);
	machine.addTransition(Right, state8, state2);
	machine.addTransition(Right, state9, state2);
	machine.addTransition(Right, state10, state2);
	machine.addTransition(Right, state12, state2);
	machine.addTransition(Right, state13, state2);
	machine.addTransition(Right, state14, state2);
	machine.addTransition(Right, state15, state2);
	machine.addTransition(Right, state16, state2);

	machine.addTransition(StartJump, state0, state13);
	machine.addTransition(StartJump, state1, state13);
	machine.addTransition(StartJump, state2, state13);
	machine.addTransition(StartJump, state3, state13);
	machine.addTransition(StartJump, state4, state13);
	machine.addTransition(StartJump, state5, state13);
	machine.addTransition(StartJump, state7, state13);
	machine.addTransition(StartJump, state8, state13);
	machine.addTransition(StartJump, state9, state13);
	machine.addTransition(StartJump, state12, state13);
	machine.addTransition(StartJump, state13, state13);
	machine.addTransition(StartJump, state14, state13);
	machine.addTransition(StartJump, state15, state13);
	machine.addTransition(StartJump, state16, state13);

	machine.addTransition(Jump, state13, state3);

	machine.addTransition(DoubleJump, state0, state4);
	machine.addTransition(DoubleJump, state1, state4);
	machine.addTransition(DoubleJump, state2, state4);
	machine.addTransition(DoubleJump, state3, state4);
	machine.addTransition(DoubleJump, state5, state4);
	machine.addTransition(DoubleJump, state7, state4);
	machine.addTransition(DoubleJump, state13, state4);
	machine.addTransition(DoubleJump, state14, state4);
	machine.addTransition(DoubleJump, state15, state4);
	machine.addTransition(DoubleJump, state16, state4);

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
	machine.addTransition(Fall, state14, state5);
	machine.addTransition(Fall, state15, state5);
	machine.addTransition(Fall, state16, state5);
	machine.addTransition(Fall, state17, state5);

	machine.addTransition(Dance, state0, state6);

	machine.addTransition(DanceWithMusic, state0, state12);

	machine.addTransition(StartSlowFall, state0, state7);
	machine.addTransition(StartSlowFall, state1, state7);
	machine.addTransition(StartSlowFall, state2, state7);
	machine.addTransition(StartSlowFall, state3, state7);
	machine.addTransition(StartSlowFall, state4, state7);
	machine.addTransition(StartSlowFall, state5, state7);
	machine.addTransition(StartSlowFall, state6, state7);
	machine.addTransition(StartSlowFall, state7, state7);
	machine.addTransition(StartSlowFall, state10, state7);
	machine.addTransition(StartSlowFall, state11, state7);
	machine.addTransition(StartSlowFall, state12, state7);
	machine.addTransition(StartSlowFall, state13, state7);
	machine.addTransition(StartSlowFall, state14, state7);
	machine.addTransition(StartSlowFall, state15, state7);
	machine.addTransition(StartSlowFall, state16, state7);

	machine.addTransition(SlowFall, state7, state14);

	machine.addTransition(Death, state0, state8);
	machine.addTransition(Death, state1, state8);
	machine.addTransition(Death, state2, state8);
	machine.addTransition(Death, state3, state8);
	machine.addTransition(Death, state4, state8);
	machine.addTransition(Death, state5, state8);
	machine.addTransition(Death, state6, state8);
	machine.addTransition(Death, state7, state8);
	machine.addTransition(Death, state9, state8);
	machine.addTransition(Death, state12, state8);
	machine.addTransition(Death, state13, state8);
	machine.addTransition(Death, state14, state8);
	machine.addTransition(Death, state15, state8);
	machine.addTransition(Death, state16, state8);

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
	machine.addTransition(StartElevator, state11, state10);
	machine.addTransition(StartElevator, state12, state10);
	machine.addTransition(StartElevator, state13, state10);
	machine.addTransition(StartElevator, state14, state10);
	machine.addTransition(StartElevator, state15, state10);
	machine.addTransition(StartElevator, state16, state10);

	machine.addTransition(Elevator, state10, state11);

	machine.addTransition(StartWaterJump, state0, state15);
	machine.addTransition(StartWaterJump, state1, state15);
	machine.addTransition(StartWaterJump, state2, state15);
	machine.addTransition(StartWaterJump, state3, state15);
	machine.addTransition(StartWaterJump, state4, state15);
	machine.addTransition(StartWaterJump, state5, state15);
	machine.addTransition(StartWaterJump, state6, state15);
	machine.addTransition(StartWaterJump, state7, state15);
	machine.addTransition(StartWaterJump, state10, state15);
	machine.addTransition(StartWaterJump, state11, state15);
	machine.addTransition(StartWaterJump, state12, state15);
	machine.addTransition(StartWaterJump, state13, state15);
	machine.addTransition(StartWaterJump, state14, state15);
	machine.addTransition(StartWaterJump, state15, state15);
	machine.addTransition(StartWaterJump, state16, state15);

	machine.addTransition(WaterJump, state15, state16);

	machine.addTransition(Idle, state0, state0);
	machine.addTransition(Idle, state1, state0);
	machine.addTransition(Idle, state2, state0);
	machine.addTransition(Idle, state3, state0);
	machine.addTransition(Idle, state4, state0);
	machine.addTransition(Idle, state5, state0);
	machine.addTransition(Idle, state6, state0);
	machine.addTransition(Idle, state7, state0);
	machine.addTransition(Idle, state8, state0);
	machine.addTransition(Idle, state9, state0);
	machine.addTransition(Idle, state10, state0);
	machine.addTransition(Idle, state12, state0);
	machine.addTransition(Idle, state13, state0);
	machine.addTransition(Idle, state14, state0);
	machine.addTransition(Idle, state15, state0);
	machine.addTransition(Idle, state17, state0);

	machine.addTransition(PortalEvent, state0, state17);
	machine.addTransition(PortalEvent, state1, state17);
	machine.addTransition(PortalEvent, state2, state17);
	machine.addTransition(PortalEvent, state3, state17);
	machine.addTransition(PortalEvent, state4, state17);
	machine.addTransition(PortalEvent, state5, state17);
	machine.addTransition(PortalEvent, state6, state17);
	machine.addTransition(PortalEvent, state7, state17);
	machine.addTransition(PortalEvent, state8, state17);
	machine.addTransition(PortalEvent, state9, state17);
	machine.addTransition(PortalEvent, state10, state17);
	machine.addTransition(PortalEvent, state12, state17);
	machine.addTransition(PortalEvent, state13, state17);
	machine.addTransition(PortalEvent, state14, state17);
	machine.addTransition(PortalEvent, state15, state17);
	machine.addTransition(PortalEvent, state16, state17);


	m_sprite.setMachine(machine);
}

void	CharacterOcto::update(sf::Time frameTime)
{
	if (!m_collisionPortal && m_sprite.getCurrentEvent() == PortalEvent)
	{
		if (m_onGround)
			m_sprite.setNextEvent(Idle);
		else
			m_sprite.setNextEvent(Fall);
	}
	if (m_sprite.getCurrentEvent() != PortalEvent)
		commitPhysicsToGraphics();
	m_sprite.update(frameTime);
	resetTimeEvent();
	timeEvent(frameTime);
	inWater();
	if (m_sprite.getCurrentEvent() != PortalEvent && endDeath())
	{
		dance();
		collisionElevatorUpdate();
		collisionTileUpdate();
		commitControlsToPhysics(frameTime.asSeconds());
		commitEnvironmentToPhysics();
		commitEventToGraphics();
		if (m_collisionPortal && m_keyPortal)
			m_sprite.setNextEvent(PortalEvent);
	}
	else
		m_helmetParticle.update(frameTime);
	m_sound->update(frameTime, static_cast<Events>(m_sprite.getCurrentEvent()),
			m_inWater, m_onGround);

	if (!m_collisionSpaceShip && !m_collisionElevatorEvent && m_progress.canRepair() && m_repairNanoRobot->getState() == NanoRobot::State::Repair)
		m_repairNanoRobot->setState(NanoRobot::State::FollowOcto);

	if (!m_collisionSpaceShip && !m_collisionElevatorEvent && m_progress.canRepairShip())
	{
		for (auto & robot : m_nanoRobots)
			robot->setState(NanoRobot::State::FollowOcto);
	}

	m_collisionTile = false;
	m_collisionElevator = false;
	m_collisionPortal = false;
	m_collisionElevatorEvent = false;
	m_collisionSpaceShip = false;
	m_previousTop = m_box->getGlobalBounds().top;
	m_prevEvent = static_cast<Events>(m_sprite.getCurrentEvent());

	m_ploufParticle.setEmitter(m_box->getBaryCenter());
	m_ploufParticle.update(frameTime);
	m_inkParticle.update(frameTime);
	if (m_timeEventInk > sf::Time::Zero && m_timeEventInk < sf::seconds(0.07f))
	{
		m_inkParticle.setCanEmit(true);
		m_inkParticle.setPosition(m_box->getBaryCenter());
	}
	else
		m_inkParticle.setCanEmit(false);

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
			break;
	}
}

void	CharacterOcto::resetTimeEvent()
{
	if (m_prevEvent != m_sprite.getCurrentEvent())
	{
		m_timeEventFall = sf::Time::Zero;
		m_timeEventIdle = sf::Time::Zero;
		m_timeEventDeath = sf::Time::Zero;
		m_timeEventInk = sf::Time::Zero;
	}
}

void	CharacterOcto::draw(sf::RenderTarget& render, sf::RenderStates states)const
{
	m_inkParticle.draw(render);
	m_sprite.draw(render, states);
	m_helmetParticle.draw(render);
	m_ploufParticle.draw(render);
}

void	CharacterOcto::drawNanoRobot(sf::RenderTarget& render, sf::RenderStates states = sf::RenderStates())const
{
	for (auto & robot : m_nanoRobots)
		robot->draw(render, states);
}

void	CharacterOcto::drawText(sf::RenderTarget& render, sf::RenderStates states = sf::RenderStates())const
{
	for (auto & robot : m_nanoRobots)
		robot->drawText(render, states);
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
			m_repairNanoRobot->setState(NanoRobot::State::Repair);
			sf::Vector2f target = elevator.getPosition();
			target.x -= elevator.getWidth() / 2.f - octo::linearInterpolation(0.f, elevator.getWidth(), elevator.getRepairAdvancement());
			target.y -= 50.f;
			m_repairNanoRobot->setTarget(target);
		}
		else
			m_repairNanoRobot->setState(NanoRobot::State::FollowOcto);
	}
	else if (m_progress.canRepair())
		m_repairNanoRobot->setState(NanoRobot::State::FollowOcto);
	m_collisionElevatorEvent = true;
}

void	CharacterOcto::collideSpaceShip(SpaceShip * spaceShip)
{
	if (m_progress.canRepairShip() && m_keyAction)
	{
		for (auto & robot : m_nanoRobots)
		{
			robot->setState(NanoRobot::State::RepairShip);
			robot->setTarget(spaceShip->getPosition());
		}
	}
	m_collisionSpaceShip = true;
}

void	CharacterOcto::usePortal(Portal & portal)
{
	m_collisionPortal = true;
	if (m_sprite.isTerminated() && m_sprite.getCurrentEvent() == PortalEvent)
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
		if (m_numberOfJump == 0)
			m_numberOfJump = 1;
	}
	else
	{
		if (!m_onGround)
		{
			m_numberOfJump = 0;
			m_onGround = true;
			m_afterJump = false;
			if (dieFall())
				return;
			if (m_keyLeft)
				m_sprite.setNextEvent(Left);
			else if (m_keyRight)
				m_sprite.setNextEvent(Right);
			else
				m_sprite.setNextEvent(Idle);
		}
		if (m_numberOfJump == 3)
			m_numberOfJump = 0;
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
		case WaterJump:
		case Jump:
		case DoubleJump:
			if (m_box->getGlobalBounds().top > m_previousTop
					&& m_jumpVelocity != m_pixelSecondJump)
			{
				m_afterJump = true;
				m_afterJumpVelocity = m_pixelSecondAfterFullJump;
				if (m_keyUp && m_progress.canSlowFall())
						m_sprite.setNextEvent(StartSlowFall);
				else
					m_sprite.setNextEvent(Fall);
			}
			break;
		case StartSlowFall:
			if (m_sprite.isTerminated())
				m_sprite.setNextEvent(SlowFall);
			break;
		case StartWaterJump:
			if (m_sprite.isTerminated())
				m_sprite.setNextEvent(WaterJump);
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
		m_timeEventFall = sf::Time::Zero;
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
		if (!m_keyElevator)
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
			m_numberOfJump = 0;
			m_box->setApplyGravity(true);
			if (m_keyElevator)
				m_sprite.setNextEvent(Fall);
		}
	}
}

bool	CharacterOcto::dieFall()
{
	if (m_timeEventFall > sf::seconds(2.0f) && !m_inWater)
	{
		m_sprite.setNextEvent(Death);
		m_helmetParticle.canEmit(true);
		m_helmetParticle.setPosition(getPosition() + sf::Vector2f(0.f, -25.f));
		return true;
	}
	return false;
}

bool	CharacterOcto::endDeath()
{
	octo::Camera&				camera = octo::Application::getCamera();
	sf::Vector2f const&			cameraPos = sf::Vector2f(camera.getRectangle().left, camera.getRectangle().top);

	m_progress.setOctoPos(m_sprite.getPosition() + m_sprite.getGlobalSize() - cameraPos);
	m_progress.setReverseSprite(m_originMove);
	if (m_sprite.getCurrentEvent() == Death)
	{
		if (m_sprite.isTerminated())
		{
			octo::StateManager &		states = octo::Application::getStateManager();
			Progress::getInstance().endChallenge();
			states.push("octo_death");
		}
		return false;
	}
	return true;
}

void	CharacterOcto::dance()
{
	Events	event = static_cast<Events>(m_sprite.getCurrentEvent());

	if (m_timeEventIdle > m_timeEventIdleMax && event == Idle)
	{
		m_timeEventIdleMax = sf::seconds(m_danceDistribution(m_engine));
		if (octo::Application::getAudioManager().getMusicVolume() > 0.f)
			m_sprite.setNextEvent(DanceWithMusic);
	}
	if (event == DanceWithMusic && m_sprite.isTerminated())
		m_sprite.setNextEvent(Idle);
}

void	CharacterOcto::inWater()
{
	bool	emit = false;

	if (m_waterLevel != -1.f && m_box->getBaryCenter().y > m_waterLevel)
	{
		if (!m_inWater)
		{
			m_numberOfJump = 0;
			emit = true;
			m_inWater = true;
		}
	}
	else if (m_inWater)
	{
		emit = true;
		m_inWater = false;
	}
	if (emit)
		m_ploufParticle.canEmit(true);
}

void	CharacterOcto::randomJumpAnimation()
{
	typedef octo::CharacterAnimation::Frame			Frame;

	if(!m_jumpDistribution(m_engine))
	{
		m_startJumpAnimation.setFrames({
				Frame(sf::seconds(0.2f), {77, sf::FloatRect(), sf::Vector2f()}),
				Frame(sf::seconds(0.2f), {78, sf::FloatRect(), sf::Vector2f()}),
				Frame(sf::seconds(0.2f), {79, sf::FloatRect(), sf::Vector2f()}),
				Frame(sf::seconds(0.2f), {80, sf::FloatRect(), sf::Vector2f()}),
				Frame(sf::seconds(0.2f), {81, sf::FloatRect(), sf::Vector2f()}),
				Frame(sf::seconds(0.2f), {82, sf::FloatRect(), sf::Vector2f()}),
				Frame(sf::seconds(0.2f), {84, sf::FloatRect(), sf::Vector2f()}),
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
	sf::Vector2f const&	velocity = m_box->getVelocity();

	if (velocity.x < 0 && !m_originMove)
	{
		m_sprite.setScale(-1.f * m_spriteScale, 1.f * m_spriteScale);
		m_sprite.setOrigin(m_sprite.getLocalSize().x, 0.f);
		m_originMove = true;
	}
	else if (velocity.x > 0 && m_originMove)
	{
		m_sprite.setScale(1.f * m_spriteScale, 1.f * m_spriteScale);
		m_sprite.setOrigin(0.f , 0.f);
		m_originMove = false;
	}
}


void	CharacterOcto::commitControlsToPhysics(float frametime)
{
	sf::Vector2f	velocity = m_box->getVelocity();
	Events			event = static_cast<Events>(m_sprite.getCurrentEvent());

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
	if (m_keySpace && m_numberOfJump < 3 &&
			(event == Jump || event == DoubleJump || event == StartJump))
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
		if (event == StartSlowFall || event == SlowFall)
		{
			if (event == StartSlowFall)
				velocity.x *= 1.3f;
			else
				velocity.x *= 1.6f;
			velocity.y = m_pixelSecondSlowFall;
		}
		if (event == StartWaterJump || event == WaterJump)
		{
			velocity.x = 0.f;
			velocity.y = m_jumpVelocity;
			if (!m_inWater)
				m_jumpVelocity += m_pixelSecondMultiplier * frametime;
			else
				m_jumpVelocity -= m_pixelSecondMultiplier * frametime;
		}
	}
	if (!m_onTopElevator)
	{
		if (event == StartElevator)
			velocity.y = (1.2f * m_pixelSecondSlowFall);
		else if (event == Elevator)
			velocity.y = (2.5f * m_pixelSecondSlowFall);
	}
	if (m_collisionElevator && event == Fall)
		velocity.y = m_pixelSecondSlowFall;
	m_box->setVelocity(velocity);
}

void	CharacterOcto::commitEnvironmentToPhysics()
{
	sf::Vector2f	velocity = m_box->getVelocity();
	Events			state = static_cast<Events>(m_sprite.getCurrentEvent());

	if (m_inWater)
	{
		switch (state)
		{
			case Fall:
				velocity.x *= 0.7f;
				velocity.y = m_pixelSecondSlowFall;
				break;
			case StartWaterJump:
				velocity.y *= 0.9f;
				break;
			case StartSlowFall:
			case SlowFall:
			case WaterJump:
				break;
			default:
				velocity.x *= 0.8f;
				velocity.y *= 0.8f;
				break;
		}
	}
	m_box->setVelocity(velocity);
}

void	CharacterOcto::caseLeft()
{
	if (!m_keyLeft)
	{
		m_keyLeft = true;
		m_keyRight = false;
		if (m_onGround && m_progress.canWalk())
			m_sprite.setNextEvent(Left);
	}
}

void	CharacterOcto::caseRight()
{
	if (!m_keyRight)
	{
		m_keyRight = true;
		m_keyLeft = false;
		if (m_onGround && m_progress.canWalk())
			m_sprite.setNextEvent(Right);
	}
}

void	CharacterOcto::caseSpace()
{
	if (!m_keySpace)
	{
		randomJumpAnimation();
		m_keySpace = true;
		if ((m_onGround || m_inWater) && m_progress.canJump() && !m_numberOfJump)
		{
			m_sprite.setNextEvent(StartJump);
			m_jumpVelocity = m_pixelSecondJump;
			m_numberOfJump = 1;
			if (!m_progress.canDoubleJump() && m_inWater)
				m_numberOfJump = 0;
		}
		else if (m_numberOfJump == 1 && m_progress.canDoubleJump())
		{
			m_sprite.setNextEvent(DoubleJump);
			m_afterJump = false;
			m_jumpVelocity = m_pixelSecondJump;
			m_numberOfJump = 2;
			if (m_inWater)
				m_numberOfJump = 0;
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
		if (m_inWater && m_progress.canUseWaterJump())
		{
			m_jumpVelocity = m_pixelSecondJump * 0.9f;
			m_sprite.setNextEvent(StartWaterJump);
		}
		else if (m_onElevator && m_progress.canUseElevator())
		{
			m_keyElevator = true;
			m_sprite.setNextEvent(StartElevator);
		}
		else if (!m_onGround && m_progress.canSlowFall())
			m_sprite.setNextEvent(StartSlowFall);
	}
}

void CharacterOcto::caseAction()
{
	if (!m_keyAction)
	{
		m_keyAction = true;
	}
}

void CharacterOcto::casePortal()
{
	if (!m_keyPortal)
	{
		m_keyPortal = true;
	}
}

bool	CharacterOcto::onPressed(sf::Event::KeyEvent const& event)
{
	if (m_sprite.getCurrentEvent() == Death)
		return true;
	switch (event.code)
	{
		case sf::Keyboard::Left:
			caseLeft();
			m_progress.walk();
			break;
		case sf::Keyboard::Right:
			caseRight();
			m_progress.walk();
			break;
		case sf::Keyboard::Space:
			caseSpace();
			break;
		case sf::Keyboard::Up:
			caseUp();
			break;
		case sf::Keyboard::E:
			if (m_onElevator && m_progress.canUseElevator() && !m_keyElevator)
			{
				m_sprite.setNextEvent(StartElevator);
				m_keyElevator = true;
			}
			else
			{
				caseAction();
				casePortal();
			}
			break;
		default:
			break;
	}
	return (true);
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
			m_keyElevator = false;
			break;
		case sf::Keyboard::E:
			m_keyAction = false;
			m_keyPortal = false;
			m_keyElevator = false;
			break;
		default:
			otherKeyReleased = true;
			break;
	}
	if (state == Death || otherKeyReleased || state == PortalEvent)
		return true;
	if (!m_onGround && !m_keyUp && !m_keyElevator)
	{
		if (state != Fall)
		{
			if (state != StartJump && state != Jump && state != DoubleJump)
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

