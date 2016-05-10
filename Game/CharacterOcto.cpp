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
#include "RepairShipNanoRobot.hpp"
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
	m_timeRepairSpaceShipMax(sf::seconds(12.f)),
	m_spriteScale(0.6f),
	m_maxJumpWaterVelocity(-3000.f),
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
	m_keyDown(false),
	m_keyAction(false),
	m_keyPortal(false),
	m_keyElevator(false),
	m_keyE(false),
	m_collisionTile(false),
	m_collisionElevator(false),
	m_collisionPortal(false),
	m_collisionElevatorEvent(false),
	m_collisionSpaceShip(false),
	m_repairShip(false),
	m_inWater(false),
	m_isDeadlyWater(false)
{
	m_sound.reset(new OctoSound());
	m_cameraMovement.reset(new CameraMovement);

	InputListener::addInputListener();

	if (m_progress.canMoveMap())
		giveNanoRobot(new GroundTransformNanoRobot());
	if (m_progress.canRepair())
		giveRepairNanoRobot(new RepairNanoRobot());
	if (m_progress.canJump())
		giveNanoRobot(new JumpNanoRobot(sf::Vector2f(0.f, 0.f)));
	if (m_progress.canDoubleJump())
		giveNanoRobot(new DoubleJumpNanoRobot(sf::Vector2f(0.f, 0.f)));
	if (m_progress.canSlowFall())
		giveNanoRobot(new SlowFallNanoRobot(sf::Vector2f(0.f, 0.f)));
	if (m_progress.canUseWaterJump())
		giveNanoRobot(new WaterNanoRobot());
	if (m_progress.canRepairShip())
		giveNanoRobot(new RepairShipNanoRobot(sf::Vector2f(0.f, 0.f)));

	for (auto & robot : m_nanoRobots)
	{
		robot->setPosition(getPosition());
		robot->transfertToOcto(true);
		robot->setState(NanoRobot::State::FollowOcto);
	}
}

CharacterOcto::~CharacterOcto(void)
{
	InputListener::removeInputListener();
}

void	CharacterOcto::setup(ABiome & biome)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();

	m_waterLevel = biome.getWaterLevel();
	m_isDeadlyWater = biome.isDeadlyWater();
	m_box->setGameObject(this);
	m_box->setSize(sf::Vector2f(30.f, 85.f));
	m_box->setCollisionType(static_cast<std::size_t>(GameObjectType::Player));
	std::size_t mask = static_cast<std::size_t>(GameObjectType::Portal)
		| static_cast<std::size_t>(GameObjectType::GroundTransformNanoRobot)
		| static_cast<std::size_t>(GameObjectType::RepairNanoRobot)
		| static_cast<std::size_t>(GameObjectType::JumpNanoRobot)
		| static_cast<std::size_t>(GameObjectType::DoubleJumpNanoRobot)
		| static_cast<std::size_t>(GameObjectType::RepairShipNanoRobot)
		| static_cast<std::size_t>(GameObjectType::SlowFallNanoRobot)
		| static_cast<std::size_t>(GameObjectType::WaterNanoRobot)
		| static_cast<std::size_t>(GameObjectType::Elevator)
		| static_cast<std::size_t>(GameObjectType::Tent)
		| static_cast<std::size_t>(GameObjectType::Concert)
		| static_cast<std::size_t>(GameObjectType::CedricNpc)
		| static_cast<std::size_t>(GameObjectType::HouseFlatSnow)
		| static_cast<std::size_t>(GameObjectType::EngineSnow)
		| static_cast<std::size_t>(GameObjectType::WeirdHouseSnow)
		| static_cast<std::size_t>(GameObjectType::Bouibouik);
	m_box->setCollisionMask(mask);

	m_octoEvent.m_octo = this;
	m_eventBox->setGameObject(&m_octoEvent);
	m_eventBox->setRadius(400.f);
	m_eventBox->setCollisionType(static_cast<std::size_t>(GameObjectType::PlayerEvent));
	std::size_t maskEvent = static_cast<std::size_t>(GameObjectType::Portal)
//Script AddNpc
		| static_cast<std::size_t>(GameObjectType::BirdBlueNpc)
		| static_cast<std::size_t>(GameObjectType::StrangerSnowNpc)
		| static_cast<std::size_t>(GameObjectType::StrangerGirlSnowNpc)
		| static_cast<std::size_t>(GameObjectType::SnowGirl2Npc)
		| static_cast<std::size_t>(GameObjectType::SnowGirl1Npc)
		| static_cast<std::size_t>(GameObjectType::Snowman3Npc)
		| static_cast<std::size_t>(GameObjectType::Snowman1Npc)
		| static_cast<std::size_t>(GameObjectType::Elevator)
		| static_cast<std::size_t>(GameObjectType::SpaceShip)
		| static_cast<std::size_t>(GameObjectType::CedricNpc)
		| static_cast<std::size_t>(GameObjectType::FannyNpc)
		| static_cast<std::size_t>(GameObjectType::FranfranNpc)
		| static_cast<std::size_t>(GameObjectType::IohannNpc)
		| static_cast<std::size_t>(GameObjectType::PunkNpc)
		| static_cast<std::size_t>(GameObjectType::FatNpc)
		| static_cast<std::size_t>(GameObjectType::GuiNpc)
		| static_cast<std::size_t>(GameObjectType::JuNpc)
		| static_cast<std::size_t>(GameObjectType::LucienNpc)
		| static_cast<std::size_t>(GameObjectType::OldDesertStaticNpc)
		| static_cast<std::size_t>(GameObjectType::VinceNpc)
		| static_cast<std::size_t>(GameObjectType::AmandineNpc)
		| static_cast<std::size_t>(GameObjectType::FaustNpc)
		| static_cast<std::size_t>(GameObjectType::WolfNpc)
		| static_cast<std::size_t>(GameObjectType::ConstanceNpc)
		| static_cast<std::size_t>(GameObjectType::JeffMouffyNpc)
		| static_cast<std::size_t>(GameObjectType::JellyfishNpc)
		| static_cast<std::size_t>(GameObjectType::BirdRedNpc)
		| static_cast<std::size_t>(GameObjectType::BrayouNpc)
		| static_cast<std::size_t>(GameObjectType::ClementineNpc)
		| static_cast<std::size_t>(GameObjectType::CanouilleNpc)
		| static_cast<std::size_t>(GameObjectType::WellKeeperNpc)
		| static_cast<std::size_t>(GameObjectType::TurbanNpc)
		| static_cast<std::size_t>(GameObjectType::Snowman2Npc);
	m_eventBox->setCollisionMask(maskEvent);
	m_eventBox->setApplyGravity(false);
	m_eventBox->setType(AShape::Type::e_trigger);

	m_sprite.setSpriteSheet(resources.getSpriteSheet(OCTO_OSS));

	m_timeEventFall = sf::Time::Zero;
	m_timeEventIdle = sf::Time::Zero;
	m_timeEventDeath = sf::Time::Zero;
	m_timeEventInk = sf::Time::Zero;
	m_timeRepairSpaceShip = sf::Time::Zero;
	setupAnimation();
	setupMachine();
	m_sprite.setScale(m_spriteScale, m_spriteScale);
	m_sprite.restart();

	m_ploufParticle.canEmit(false);
	m_ploufParticle.setColor(biome.getWaterColor());

	m_waterParticle.canEmit(false);
	m_waterParticle.setWidth(m_box->getSize().x);
	m_waterParticle.setColor(biome.getWaterColor());

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
			Frame(sf::seconds(0.2f), {0, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {1, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {2, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {3, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {4, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {5, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {6, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {7, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {8, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {9, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {10, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {11, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {12, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {13, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {14, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {15, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {16, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {17, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {18, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {19, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {20, sf::FloatRect(), sf::Vector2f()}),
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
			Frame(sf::seconds(0.2f), {53, sf::FloatRect(), sf::Vector2f()}),
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

	m_konamiCodeAnimation.setFrames({
			Frame(sf::seconds(0.1f), {56, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {57, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {58, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {59, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {60, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(6.f), {61, sf::FloatRect(), sf::Vector2f()}),
			});
	m_konamiCodeAnimation.setLoop(octo::LoopMode::NoLoop);
}

void	CharacterOcto::setupMachine()
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					stateIdle;
	StatePtr					stateLeft;
	StatePtr					stateRight;
	StatePtr					stateJump;
	StatePtr					stateDoubleJump;
	StatePtr					stateFall;
	StatePtr					stateDance;
	StatePtr					stateStartSlowFall;
	StatePtr					stateDeath;
	StatePtr					stateStartElevator;
	StatePtr					stateElevator;
	StatePtr					stateDanceMusic;
	StatePtr					stateStartJump;
	StatePtr					stateSlowFall;
	StatePtr					stateSartWaterJump;
	StatePtr					stateWaterJump;
	StatePtr					statePortal;
	StatePtr					stateKonami;

	stateIdle = std::make_shared<State>("Idle", m_idleAnimation, m_sprite);
	stateLeft = std::make_shared<State>("Left", m_walkAnimation, m_sprite);
	stateRight = std::make_shared<State>("Right", m_walkAnimation, m_sprite);
	stateJump = std::make_shared<State>("Jump", m_jumpAnimation, m_sprite);
	stateDoubleJump = std::make_shared<State>("DoubleJump", m_jumpAnimation, m_sprite);
	stateFall = std::make_shared<State>("Fall", m_fallAnimation, m_sprite);
	stateDance = std::make_shared<State>("Dance", m_danceAnimation, m_sprite);
	stateStartSlowFall = std::make_shared<State>("StartSlowFall", m_startSlowFallAnimation, m_sprite);
	stateDeath = std::make_shared<State>("Death", m_deathAnimation, m_sprite);
	stateStartElevator = std::make_shared<State>("StartElevator", m_startElevatorAnimation, m_sprite);
	stateElevator = std::make_shared<State>("Elevator", m_elevatorAnimation, m_sprite);
	stateDanceMusic = std::make_shared<State>("DanceWithMusic", m_danceWithMusicAnimation, m_sprite);
	stateStartJump = std::make_shared<State>("StartJump", m_startJumpAnimation, m_sprite);
	stateSlowFall = std::make_shared<State>("SlowFall", m_slowFallAnimation, m_sprite);
	stateSartWaterJump = std::make_shared<State>("StartWaterJump", m_startElevatorAnimation, m_sprite);
	stateWaterJump = std::make_shared<State>("WaterJump", m_elevatorAnimation, m_sprite);
	statePortal = std::make_shared<State>("PortalEvent", m_portalAnimation, m_sprite);
	stateKonami = std::make_shared<State>("KonamiCode", m_konamiCodeAnimation, m_sprite);

	machine.setStart(stateIdle);
	machine.addTransition(Left, stateIdle, stateLeft);
	machine.addTransition(Left, stateLeft, stateLeft);
	machine.addTransition(Left, stateRight, stateLeft);
	machine.addTransition(Left, stateJump, stateLeft);
	machine.addTransition(Left, stateDoubleJump, stateLeft);
	machine.addTransition(Left, stateFall, stateLeft);
	machine.addTransition(Left, stateDance, stateLeft);
	machine.addTransition(Left, stateStartSlowFall, stateLeft);
	machine.addTransition(Left, stateDeath, stateLeft);
	machine.addTransition(Left, stateStartElevator, stateLeft);
	machine.addTransition(Left, stateDanceMusic, stateLeft);
	machine.addTransition(Left, stateStartJump, stateLeft);
	machine.addTransition(Left, stateSlowFall, stateLeft);
	machine.addTransition(Left, stateSartWaterJump, stateLeft);
	machine.addTransition(Left, stateWaterJump, stateLeft);

	machine.addTransition(Right, stateIdle, stateRight);
	machine.addTransition(Right, stateLeft, stateRight);
	machine.addTransition(Right, stateRight, stateRight);
	machine.addTransition(Right, stateJump, stateRight);
	machine.addTransition(Right, stateDoubleJump, stateRight);
	machine.addTransition(Right, stateFall, stateRight);
	machine.addTransition(Right, stateDance, stateRight);
	machine.addTransition(Right, stateStartSlowFall, stateRight);
	machine.addTransition(Right, stateDeath, stateRight);
	machine.addTransition(Right, stateStartElevator, stateRight);
	machine.addTransition(Right, stateDanceMusic, stateRight);
	machine.addTransition(Right, stateStartJump, stateRight);
	machine.addTransition(Right, stateSlowFall, stateRight);
	machine.addTransition(Right, stateSartWaterJump, stateRight);
	machine.addTransition(Right, stateWaterJump, stateRight);

	machine.addTransition(StartJump, stateIdle, stateStartJump);
	machine.addTransition(StartJump, stateLeft, stateStartJump);
	machine.addTransition(StartJump, stateRight, stateStartJump);
	machine.addTransition(StartJump, stateJump, stateStartJump);
	machine.addTransition(StartJump, stateDoubleJump, stateStartJump);
	machine.addTransition(StartJump, stateFall, stateStartJump);
	machine.addTransition(StartJump, stateStartSlowFall, stateStartJump);
	machine.addTransition(StartJump, stateDeath, stateStartJump);
	machine.addTransition(StartJump, stateDanceMusic, stateStartJump);
	machine.addTransition(StartJump, stateStartJump, stateStartJump);
	machine.addTransition(StartJump, stateSlowFall, stateStartJump);
	machine.addTransition(StartJump, stateSartWaterJump, stateStartJump);
	machine.addTransition(StartJump, stateWaterJump, stateStartJump);
	machine.addTransition(StartJump, statePortal, stateStartJump);

	machine.addTransition(Jump, stateStartJump, stateJump);

	machine.addTransition(DoubleJump, stateIdle, stateDoubleJump);
	machine.addTransition(DoubleJump, stateLeft, stateDoubleJump);
	machine.addTransition(DoubleJump, stateRight, stateDoubleJump);
	machine.addTransition(DoubleJump, stateJump, stateDoubleJump);
	machine.addTransition(DoubleJump, stateFall, stateDoubleJump);
	machine.addTransition(DoubleJump, stateStartSlowFall, stateDoubleJump);
	machine.addTransition(DoubleJump, stateElevator, stateDoubleJump);
	machine.addTransition(DoubleJump, stateStartJump, stateDoubleJump);
	machine.addTransition(DoubleJump, stateSlowFall, stateDoubleJump);
	machine.addTransition(DoubleJump, stateSartWaterJump, stateDoubleJump);
	machine.addTransition(DoubleJump, stateWaterJump, stateDoubleJump);
	machine.addTransition(DoubleJump, statePortal, stateDoubleJump);

	machine.addTransition(Fall, stateIdle, stateFall);
	machine.addTransition(Fall, stateLeft, stateFall);
	machine.addTransition(Fall, stateRight, stateFall);
	machine.addTransition(Fall, stateJump, stateFall);
	machine.addTransition(Fall, stateDoubleJump, stateFall);
	machine.addTransition(Fall, stateFall, stateFall);
	machine.addTransition(Fall, stateDance, stateFall);
	machine.addTransition(Fall, stateStartSlowFall, stateFall);
	machine.addTransition(Fall, stateStartElevator, stateFall);
	machine.addTransition(Fall, stateElevator, stateFall);
	machine.addTransition(Fall, stateDanceMusic, stateFall);
	machine.addTransition(Fall, stateStartJump, stateFall);
	machine.addTransition(Fall, stateSlowFall, stateFall);
	machine.addTransition(Fall, stateSartWaterJump, stateFall);
	machine.addTransition(Fall, stateWaterJump, stateFall);
	machine.addTransition(Fall, statePortal, stateFall);
	machine.addTransition(Fall, stateKonami, stateFall);

	machine.addTransition(Dance, stateIdle, stateDance);

	machine.addTransition(DanceWithMusic, stateIdle, stateDanceMusic);

	machine.addTransition(StartSlowFall, stateIdle, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateLeft, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateRight, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateJump, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateDoubleJump, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateFall, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateDance, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateStartSlowFall, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateStartElevator, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateElevator, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateDanceMusic, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateStartJump, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateSlowFall, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateSartWaterJump, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateWaterJump, stateStartSlowFall);

	machine.addTransition(SlowFall, stateStartSlowFall, stateSlowFall);

	machine.addTransition(Death, stateIdle, stateDeath);
	machine.addTransition(Death, stateLeft, stateDeath);
	machine.addTransition(Death, stateRight, stateDeath);
	machine.addTransition(Death, stateJump, stateDeath);
	machine.addTransition(Death, stateDoubleJump, stateDeath);
	machine.addTransition(Death, stateFall, stateDeath);
	machine.addTransition(Death, stateDance, stateDeath);
	machine.addTransition(Death, stateStartSlowFall, stateDeath);
	machine.addTransition(Death, stateDanceMusic, stateDeath);
	machine.addTransition(Death, stateStartJump, stateDeath);
	machine.addTransition(Death, stateSlowFall, stateDeath);
	machine.addTransition(Death, stateSartWaterJump, stateDeath);
	machine.addTransition(Death, stateWaterJump, stateDeath);

	machine.addTransition(StartElevator, stateIdle, stateStartElevator);
	machine.addTransition(StartElevator, stateLeft, stateStartElevator);
	machine.addTransition(StartElevator, stateRight, stateStartElevator);
	machine.addTransition(StartElevator, stateJump, stateStartElevator);
	machine.addTransition(StartElevator, stateDoubleJump, stateStartElevator);
	machine.addTransition(StartElevator, stateFall, stateStartElevator);
	machine.addTransition(StartElevator, stateDance, stateStartElevator);
	machine.addTransition(StartElevator, stateStartSlowFall, stateStartElevator);
	machine.addTransition(StartElevator, stateStartElevator, stateStartElevator);
	machine.addTransition(StartElevator, stateElevator, stateStartElevator);
	machine.addTransition(StartElevator, stateDanceMusic, stateStartElevator);
	machine.addTransition(StartElevator, stateStartJump, stateStartElevator);
	machine.addTransition(StartElevator, stateSlowFall, stateStartElevator);
	machine.addTransition(StartElevator, stateSartWaterJump, stateStartElevator);
	machine.addTransition(StartElevator, stateWaterJump, stateStartElevator);

	machine.addTransition(Elevator, stateStartElevator, stateElevator);

	machine.addTransition(StartWaterJump, stateIdle, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateLeft, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateRight, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateJump, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateDoubleJump, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateFall, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateDance, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateStartSlowFall, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateDeath, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateStartElevator, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateElevator, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateDanceMusic, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateStartJump, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateSlowFall, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateSartWaterJump, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateWaterJump, stateSartWaterJump);
	machine.addTransition(StartWaterJump, statePortal, stateSartWaterJump);

	machine.addTransition(WaterJump, stateSartWaterJump, stateWaterJump);

	machine.addTransition(Idle, stateIdle, stateIdle);
	machine.addTransition(Idle, stateLeft, stateIdle);
	machine.addTransition(Idle, stateRight, stateIdle);
	machine.addTransition(Idle, stateJump, stateIdle);
	machine.addTransition(Idle, stateDoubleJump, stateIdle);
	machine.addTransition(Idle, stateFall, stateIdle);
	machine.addTransition(Idle, stateDance, stateIdle);
	machine.addTransition(Idle, stateStartSlowFall, stateIdle);
	machine.addTransition(Idle, stateDeath, stateIdle);
	machine.addTransition(Idle, stateStartElevator, stateIdle);
	machine.addTransition(Idle, stateDanceMusic, stateIdle);
	machine.addTransition(Idle, stateStartJump, stateIdle);
	machine.addTransition(Idle, stateSlowFall, stateIdle);
	machine.addTransition(Idle, stateSartWaterJump, stateIdle);
	machine.addTransition(Idle, statePortal, stateIdle);
	machine.addTransition(Idle, stateKonami, stateIdle);

	machine.addTransition(PortalEvent, stateIdle, statePortal);
	machine.addTransition(PortalEvent, stateLeft, statePortal);
	machine.addTransition(PortalEvent, stateRight, statePortal);
	machine.addTransition(PortalEvent, stateJump, statePortal);
	machine.addTransition(PortalEvent, stateDoubleJump, statePortal);
	machine.addTransition(PortalEvent, stateFall, statePortal);
	machine.addTransition(PortalEvent, stateDance, statePortal);
	machine.addTransition(PortalEvent, stateStartSlowFall, statePortal);
	machine.addTransition(PortalEvent, stateDeath, statePortal);
	machine.addTransition(PortalEvent, stateStartElevator, statePortal);
	machine.addTransition(PortalEvent, stateDanceMusic, statePortal);
	machine.addTransition(PortalEvent, stateStartJump, statePortal);
	machine.addTransition(PortalEvent, stateSlowFall, statePortal);
	machine.addTransition(PortalEvent, stateSartWaterJump, statePortal);
	machine.addTransition(PortalEvent, stateWaterJump, statePortal);

	machine.addTransition(KonamiCode, stateIdle, stateKonami);
	machine.addTransition(KonamiCode, stateLeft, stateKonami);
	machine.addTransition(KonamiCode, stateRight, stateKonami);
	machine.addTransition(KonamiCode, stateJump, stateKonami);
	machine.addTransition(KonamiCode, stateDoubleJump, stateKonami);
	machine.addTransition(KonamiCode, stateFall, stateKonami);
	machine.addTransition(KonamiCode, stateDance, stateKonami);
	machine.addTransition(KonamiCode, stateStartSlowFall, stateKonami);
	machine.addTransition(KonamiCode, stateDeath, stateKonami);
	machine.addTransition(KonamiCode, stateStartElevator, stateKonami);
	machine.addTransition(KonamiCode, stateDanceMusic, stateKonami);
	machine.addTransition(KonamiCode, stateStartJump, stateKonami);
	machine.addTransition(KonamiCode, stateSlowFall, stateKonami);
	machine.addTransition(KonamiCode, stateSartWaterJump, stateKonami);
	machine.addTransition(KonamiCode, statePortal, stateKonami);

	m_sprite.setMachine(machine);
}

void	CharacterOcto::update(sf::Time frameTime)
{
	portalEvent();
	if (m_sprite.getCurrentEvent() != PortalEvent && m_sprite.getCurrentEvent() != KonamiCode)
		commitPhysicsToGraphics();
	m_sprite.update(frameTime);
	resetTimeEvent();
	timeEvent(frameTime);
	inWater();
	if (m_sprite.getCurrentEvent() != PortalEvent && m_sprite.getCurrentEvent() != KonamiCode && endDeath())
	{
		dance();
		collisionElevatorUpdate();
		collisionTileUpdate();
		commitControlsToPhysics(frameTime.asSeconds());
		commitEnvironmentToPhysics();
		commitEventToGraphics();
		if (m_collisionPortal && m_keyPortal)
		{
			m_box->setApplyGravity(false);
			m_sprite.setNextEvent(PortalEvent);
		}
	}
	else
		m_helmetParticle.update(frameTime);
	m_sound->update(frameTime, static_cast<Events>(m_sprite.getCurrentEvent()),
			m_inWater, m_onGround);
	if (m_sprite.getCurrentEvent() == KonamiCode && m_sprite.isTerminated())
	{
		m_box->setApplyGravity(true);
		if (m_onGround)
			m_sprite.setNextEvent(Idle);
		else
			m_sprite.setNextEvent(Fall);
	}
	if (!m_collisionSpaceShip && !m_collisionElevatorEvent && m_progress.canRepair()
			&& m_repairNanoRobot && m_repairNanoRobot->getState() == NanoRobot::State::Repair)
		m_repairNanoRobot->setState(NanoRobot::State::FollowOcto);

	if (!m_collisionSpaceShip && !m_collisionElevatorEvent && m_progress.canRepairShip())
	{
		for (auto & robot : m_nanoRobots)
		{
			if (robot->getState() != NanoRobot::State::Speak)
				robot->setState(NanoRobot::State::FollowOcto);
		}
	}

	if (m_repairShip)
	{
		m_timeRepairSpaceShip += frameTime;
		if (m_timeRepairSpaceShip > m_timeRepairSpaceShipMax)
		{
			octo::StateManager & states = octo::Application::getStateManager();
			m_progress.spaceShipRepair(true);
			states.change("zero");
		}
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
	m_waterParticle.setEmitter(m_box->getBaryCenter()
			+ sf::Vector2f(-m_box->getSize().x / 2.f, m_box->getSize().y / 2.f));
	m_waterParticle.update(frameTime);
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
	if (!m_onGround && !m_inWater && !m_onElevator)
		m_cameraMovement->setFall(true);
	else
		m_cameraMovement->setFall(false);

	m_cameraMovement->follow(frameTime, m_box->getPosition());
}

void	CharacterOcto::portalEvent()
{
	if (!m_collisionPortal && m_sprite.getCurrentEvent() == PortalEvent)
	{
		m_box->setApplyGravity(true);
		if (m_onGround)
			m_sprite.setNextEvent(Idle);
		else
			m_sprite.setNextEvent(Fall);
	}
}

void	CharacterOcto::timeEvent(sf::Time frameTime)
{
	switch (m_sprite.getCurrentEvent())
	{
		case Fall:
			m_timeEventFall += frameTime;
			m_cameraMovement->setEventFallTimer(m_timeEventFall);
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
	m_waterParticle.draw(render);
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

void	CharacterOcto::setStartPosition(sf::Vector2f const & position)
{
	m_box->setPosition(position);
	m_box->update();
	m_eventBox->setPosition(position);
	m_eventBox->update();
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
	if (m_progress.canRepair() && m_keyAction)
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
			if (robot->getState() == NanoRobot::State::GoingToRepairShip || robot->getState() == NanoRobot::State::RepairShip)
				continue;
			robot->setState(NanoRobot::State::GoingToRepairShip);
			robot->setTarget(spaceShip->getPosition());
		}
		m_repairShip = true;
	}
	m_collisionSpaceShip = true;
}

void	CharacterOcto::usePortal(Portal & portal)
{
	octo::Camera&				camera = octo::Application::getCamera();
	sf::Vector2f const&			cameraPos = sf::Vector2f(camera.getRectangle().left, camera.getRectangle().top);

	m_collisionPortal = true;
	if (m_sprite.getCurrentEvent() == PortalEvent && m_sprite.isTerminated())
	{
		m_progress.setOctoPos(m_sprite.getPosition() + m_sprite.getGlobalSize() - cameraPos);
		m_progress.setReverseSprite(m_originMove);
		m_progress.setNextDestination(portal.getDestination());
	}
}

void	CharacterOcto::startKonamiCode(bool canStart)
{
	if (canStart && m_sprite.getCurrentEvent() != KonamiCode)
	{
		m_sprite.setNextEvent(KonamiCode);
		m_box->setApplyGravity(false);
	}
}

void	CharacterOcto::collisionTileUpdate()
{
	if (!m_collisionTile)
	{
		m_onGround = false;
		onSky(static_cast<Events>(m_sprite.getCurrentEvent()));
		if (m_numberOfJump == 0 && !m_inWater && !m_useElevator)
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
		case Fall:
			if (m_keyUp)
			{
				if (m_progress.canSlowFall())
					m_sprite.setNextEvent(StartSlowFall);
			}
			if (m_keyElevator)
			{
				if (m_progress.canUseElevator() && m_collisionElevator)
					m_sprite.setNextEvent(StartElevator);
			}
			break;
		case SlowFall:
			break;
		case StartElevator:
		case Elevator:
			if (!m_collisionElevator)
				m_sprite.setNextEvent(Fall);
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
		else if (!m_keyElevator)
		{
			m_useElevator = false;
			m_box->setApplyGravity(true);
		}
		if (top <= m_topElevator)
			m_onTopElevator = true;
		if (m_keyAction && !m_keyElevator && m_progress.canUseElevator())
		{
			m_keyElevator = true;
			m_sprite.setNextEvent(StartElevator);
		}
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
	if (m_timeEventFall > sf::seconds(3.0f) && !m_inWater)
	{
		m_sprite.setNextEvent(Death);
		m_helmetParticle.canEmit(true);
		m_helmetParticle.setPosition(getPosition() + sf::Vector2f(0.f, -25.f));
		Progress::getInstance().registerDeath(getPosition().x);
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
			states.change("octo_death");
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
			if (m_isDeadlyWater)
			{
				m_sprite.setNextEvent(Death);
				m_helmetParticle.canEmit(true);
				m_helmetParticle.setPosition(getPosition() + sf::Vector2f(0.f, -25.f));
				Progress::getInstance().registerDeath(getPosition().x);
			}
		}
		m_waterParticle.clear();
	}
	else if (m_inWater)
	{
		emit = true;
		m_inWater = false;
		m_waterParticle.canEmit(true);
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
			velocity.x *= 0.4f;
			velocity.y = m_jumpVelocity;
			if (!m_inWater)
				m_jumpVelocity += m_pixelSecondMultiplier * frametime;
			else if (m_jumpVelocity > m_maxJumpWaterVelocity)
				m_jumpVelocity -= m_pixelSecondMultiplier * frametime * 2.3f;
		}
	}
	if (!m_onTopElevator && m_keyElevator)
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
				if (!m_keyDown)
				{
					velocity.x *= 1.2f;
					velocity.y = m_pixelSecondSlowFall;
				}
				break;
			case StartWaterJump:
				velocity.y *= 0.9f;
				break;
			case StartSlowFall:
			case SlowFall:
			case WaterJump:
				break;
			default:
				velocity.x *= 1.2f;
				velocity.y *= 1.2f;
				break;
		}
		m_box->setVelocity(velocity);
	}
}

void	CharacterOcto::caseLeft()
{
	if (!m_keyLeft)
	{
		m_keyLeft = true;
		m_keyRight = false;
		if (m_onGround && m_progress.canWalk() && m_numberOfJump != 1)
			m_sprite.setNextEvent(Left);
	}
}

void	CharacterOcto::caseRight()
{
	if (!m_keyRight)
	{
		m_keyRight = true;
		m_keyLeft = false;
		if (m_onGround && m_progress.canWalk() && m_numberOfJump != 1)
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
		else if (m_onElevator && m_progress.canUseElevator() && !m_keyElevator)
		{
			m_keyElevator = true;
			m_sprite.setNextEvent(StartElevator);
		}
		else if (!m_onGround && !m_inWater && m_progress.canSlowFall())
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

bool	CharacterOcto::onInputPressed(InputListener::OctoKeys const & key)
{
	if (m_sprite.getCurrentEvent() == Death
			|| m_sprite.getCurrentEvent() == KonamiCode
			|| m_sprite.getCurrentEvent() == PortalEvent)
		return true;
	switch (key)
	{
		case OctoKeys::Left:
			caseLeft();
			m_progress.walk();
			break;
		case OctoKeys::Right:
			caseRight();
			m_progress.walk();
			break;
		case OctoKeys::Space:
			caseSpace();
			break;
		case OctoKeys::SlowFall:
			caseUp();
			break;
		case OctoKeys::Down:
			m_keyDown = true;
			break;
		case OctoKeys::Use:
			m_keyE = true;
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

bool	CharacterOcto::onInputReleased(InputListener::OctoKeys const & key)
{
	Events	state = static_cast<Events>(m_sprite.getCurrentEvent());
	bool otherKeyReleased = false;

	switch (key)
	{
		case OctoKeys::Left:
			m_keyLeft = false;
			break;
		case OctoKeys::Right:
			m_keyRight = false;
			break;
		case OctoKeys::Space:
			m_keySpace = false;
			if (state == Jump || state == DoubleJump || state == StartJump)
			{
				m_afterJump = true;
				m_afterJumpVelocity = m_pixelSecondAfterJump;
			}
			break;
		case OctoKeys::Up:
			m_keyUp = false;
			if (!m_keyE)
				m_keyElevator = false;
			break;
		case OctoKeys::Down:
			m_keyDown = false;
			break;
		case OctoKeys::Use:
			m_keyE = false;
			m_keyAction = false;
			m_keyPortal = false;
			if (!m_keyUp)
				m_keyElevator = false;
			break;
		default:
			otherKeyReleased = true;
			break;
	}
	if (state == Death || state == PortalEvent || state == KonamiCode || otherKeyReleased)
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

bool	CharacterOcto::getDoubleJump()
{
	if (m_sprite.getCurrentEvent() == DoubleJump)
		return true;
	return false;
}

float	CharacterOcto::getWaterLevel() const
{
	return m_waterLevel;
}


void			CharacterOcto::collideZoomEvent(sf::Vector2f const & position)
{
	m_cameraMovement->collideZoomEvent(position);
}
