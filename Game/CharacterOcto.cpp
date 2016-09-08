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
#include "RectangleShape.hpp"
#include "CircleShape.hpp"
#include "TileShape.hpp"
#include "Progress.hpp"
#include "NanoRobot.hpp"
#include "GroundTransformNanoRobot.hpp"
#include "RepairNanoRobot.hpp"
#include "RepairShipNanoRobot.hpp"
#include "JumpNanoRobot.hpp"
#include "DoubleJumpNanoRobot.hpp"
#include "SlowFallNanoRobot.hpp"
#include "WaterNanoRobot.hpp"
#include "Portal.hpp"

CharacterOcto::CharacterOcto() :
	m_box(PhysicsEngine::getShapeBuilder().createRectangle(false)),
	m_eventBox(PhysicsEngine::getShapeBuilder().createCircle(false)),
	m_repairNanoRobot(nullptr),
	m_progress(Progress::getInstance()),
	m_timeEventIdleMax(sf::seconds(4.f)),
	m_timeRepairSpaceShipMax(sf::seconds(12.f)),
	m_timeSlowFallMax(sf::seconds(2.5f)),
	m_spriteScale(0.6f),
	m_maxJumpWaterVelocity(-3000.f),
	m_pixelSecondJump(-1300.f),
	m_pixelSecondSlowFall(-300.f),
	m_pixelSecondWalk(320.f),
	m_pixelSecondAfterJump(-400.f),
	m_pixelSecondAfterFullJump(-500.f),
	m_pixelSecondMultiplier(800.f),
	m_deltaPositionY(27.f),
	m_numberOfJump(0),
	m_originMove(false),
	m_onGround(false),
	m_onGroundDelayMax(sf::seconds(0.05f)),
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
	m_isDeadlyWater(false),
	m_meetNpc(false),
	m_replaceOcto(false),
	m_enableCutscene(false),
	m_isEndingInRocket(false),
	m_autoDisableCutscene(false),
	m_generator("random"),
	m_cutsceneTimerMax(sf::seconds(2.f)),
	m_cutsceneShader(PostEffectLayer::getInstance().getShader(CUTSCENE_FRAG))
{
	m_sound.reset(new OctoSound());

	m_cutsceneShader.setParameter("height", 0.15);

	if (!m_progress.isMenu())
		InputListener::addInputListener();
	else
		initAI();

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
	if (!m_progress.isMenu())
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
		| static_cast<std::size_t>(GameObjectType::FannyNpc)
		| static_cast<std::size_t>(GameObjectType::Snowman3Npc)
		| static_cast<std::size_t>(GameObjectType::HouseFlatSnow)
		| static_cast<std::size_t>(GameObjectType::EngineSnow)
		| static_cast<std::size_t>(GameObjectType::WeirdHouseSnow)
		| static_cast<std::size_t>(GameObjectType::Bouibouik)
		| static_cast<std::size_t>(GameObjectType::RocketDoor)
		| static_cast<std::size_t>(GameObjectType::CheckPoint);
	m_box->setCollisionMask(mask);

	m_octoEvent.m_octo = this;
	m_eventBox->setGameObject(&m_octoEvent);
	m_eventBox->setRadius(400.f);
	m_eventBox->setCollisionType(static_cast<std::size_t>(GameObjectType::PlayerEvent));
	std::size_t maskEvent = static_cast<std::size_t>(GameObjectType::Portal)
//Script AddNpc
		| static_cast<std::size_t>(GameObjectType::WindowGlitchNpc)
		| static_cast<std::size_t>(GameObjectType::FranGlitchNpc)
		| static_cast<std::size_t>(GameObjectType::JuGlitchNpc)
		| static_cast<std::size_t>(GameObjectType::LuGlitchNpc)
		| static_cast<std::size_t>(GameObjectType::LongChairNpc)
		| static_cast<std::size_t>(GameObjectType::Rocket)
		| static_cast<std::size_t>(GameObjectType::OctoDeathNpc)
		| static_cast<std::size_t>(GameObjectType::CedricEndNpc)
		| static_cast<std::size_t>(GameObjectType::TVScreen)
		| static_cast<std::size_t>(GameObjectType::FabienNpc)
		| static_cast<std::size_t>(GameObjectType::CheckPoint)
		| static_cast<std::size_t>(GameObjectType::OverCoolNpc)
		| static_cast<std::size_t>(GameObjectType::Pedestal)
		| static_cast<std::size_t>(GameObjectType::ForestSpirit2Npc)
		| static_cast<std::size_t>(GameObjectType::ForestSpirit1Npc)
		| static_cast<std::size_t>(GameObjectType::BirdBlueNpc)
		| static_cast<std::size_t>(GameObjectType::StrangerSnowNpc)
		| static_cast<std::size_t>(GameObjectType::StrangerGirlSnowNpc)
		| static_cast<std::size_t>(GameObjectType::SnowGirl2Npc)
		| static_cast<std::size_t>(GameObjectType::SnowGirl1Npc)
		| static_cast<std::size_t>(GameObjectType::Snowman3Npc)
		| static_cast<std::size_t>(GameObjectType::Snowman1Npc)
		| static_cast<std::size_t>(GameObjectType::Elevator)
		| static_cast<std::size_t>(GameObjectType::SpaceShip)
		| static_cast<std::size_t>(GameObjectType::CedricStartNpc)
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
	m_timeSlowFall = sf::Time::Zero;
	setupAnimation();
	setupMachine();
	m_sprite.setScale(m_spriteScale, m_spriteScale);
	m_sprite.restart();

	m_waterColor = biome.getWaterColor();
	m_secondWaterColor = biome.getSecondWaterColor();

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

	m_bubbleParticle.setup(sf::Vector2f(1.5f, 1.5f));
	m_bubbleParticle.setVelocity(sf::Vector2f(0.f, -70.f));
	m_bubbleParticle.setEmitTimeRange(0.2f, 0.3f);
	m_bubbleParticle.setGrowTimeRange(0.4f, 0.6f);
	m_bubbleParticle.setLifeTimeRange(1.f, 1.8f);
	m_bubbleParticle.setScaleFactor(10.f);
	m_bubbleParticle.setDispersion(80.f);
	m_bubbleParticle.setColor(sf::Color(255, 255, 255, 100));
	m_bubbleParticle.setCanEmit(false);
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

	m_fallAnimation.setFrames({
			Frame(sf::seconds(0.3f), {27, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {28, sf::FloatRect(), sf::Vector2f()}),
			});
	m_fallAnimation.setLoop(octo::LoopMode::Loop);

	m_dieFallAnimation.setFrames({
			Frame(sf::seconds(0.3f), {29, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {30, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {71, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {30, sf::FloatRect(), sf::Vector2f()}),
			});
	m_dieFallAnimation.setLoop(octo::LoopMode::Loop);

	//TODO define frame
	m_danceAnimation.setFrames({
			Frame(sf::seconds(0.4f), {10, sf::FloatRect(), sf::Vector2f()}),
			});
	m_danceAnimation.setLoop(octo::LoopMode::Loop);

	m_startSlowFallAnimation.setFrames({
			Frame(sf::seconds(0.2f), {49, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {50, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {51, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {52, sf::FloatRect(), sf::Vector2f()}),
			});
	m_startSlowFallAnimation.setLoop(octo::LoopMode::NoLoop);

	m_slowFallAnimation1.setFrames({
			Frame(sf::seconds(0.2f), {53, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {54, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {53, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {52, sf::FloatRect(), sf::Vector2f()}),
			});
	m_slowFallAnimation1.setLoop(octo::LoopMode::NoLoop);

	m_slowFallAnimation2.setFrames({
			Frame(sf::seconds(0.2f), {51, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {52, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {53, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {52, sf::FloatRect(), sf::Vector2f()}),
			});
	m_slowFallAnimation2.setLoop(octo::LoopMode::NoLoop);

	m_slowFallAnimation3.setFrames({
			Frame(sf::seconds(0.2f), {51, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {52, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {51, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {50, sf::FloatRect(), sf::Vector2f()}),
			});
	m_slowFallAnimation3.setLoop(octo::LoopMode::NoLoop);

	m_deathAnimation.setFrames({
			Frame(sf::seconds(0.2f), {35, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {36, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {37, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {38, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {39, sf::FloatRect(), sf::Vector2f()}),
			});
	m_deathAnimation.setLoop(octo::LoopMode::NoLoop);

	m_drinkAnimation.setFrames({
			Frame(sf::seconds(0.4f), {99, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {100, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {101, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {102, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {103, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {104, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {105, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {106, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {107, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {108, sf::FloatRect(), sf::Vector2f()}),
			});
	m_drinkAnimation.setLoop(octo::LoopMode::NoLoop);

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
	StatePtr					stateDoubleJump;
	StatePtr					stateFall;
	StatePtr					stateDieFall;
	StatePtr					stateDance;
	StatePtr					stateStartSlowFall;
	StatePtr					stateDeath;
	StatePtr					stateStartElevator;
	StatePtr					stateElevator;
	StatePtr					stateDanceMusic;
	StatePtr					stateStartJump;
	StatePtr					stateSlowFall1;
	StatePtr					stateSlowFall2;
	StatePtr					stateSlowFall3;
	StatePtr					stateSartWaterJump;
	StatePtr					stateWaterJump;
	StatePtr					statePortal;
	StatePtr					stateKonami;
	StatePtr					stateDrink;

	stateIdle = std::make_shared<State>("Idle", m_idleAnimation, m_sprite);
	stateLeft = std::make_shared<State>("Left", m_walkAnimation, m_sprite);
	stateRight = std::make_shared<State>("Right", m_walkAnimation, m_sprite);
	stateDoubleJump = std::make_shared<State>("DoubleJump", m_startJumpAnimation, m_sprite);
	stateFall = std::make_shared<State>("Fall", m_fallAnimation, m_sprite);
	stateDieFall = std::make_shared<State>("DieFall", m_dieFallAnimation, m_sprite);
	stateDance = std::make_shared<State>("Dance", m_danceAnimation, m_sprite);
	stateStartSlowFall = std::make_shared<State>("StartSlowFall", m_startSlowFallAnimation, m_sprite);
	stateDeath = std::make_shared<State>("Death", m_deathAnimation, m_sprite);
	stateStartElevator = std::make_shared<State>("StartElevator", m_startElevatorAnimation, m_sprite);
	stateElevator = std::make_shared<State>("Elevator", m_elevatorAnimation, m_sprite);
	stateDanceMusic = std::make_shared<State>("DanceWithMusic", m_danceWithMusicAnimation, m_sprite);
	stateStartJump = std::make_shared<State>("StartJump", m_startJumpAnimation, m_sprite);
	stateSlowFall1 = std::make_shared<State>("SlowFall1", m_slowFallAnimation1, m_sprite);
	stateSlowFall2 = std::make_shared<State>("SlowFall2", m_slowFallAnimation2, m_sprite);
	stateSlowFall3 = std::make_shared<State>("SlowFall3", m_slowFallAnimation3, m_sprite);
	stateSartWaterJump = std::make_shared<State>("StartWaterJump", m_startElevatorAnimation, m_sprite);
	stateWaterJump = std::make_shared<State>("WaterJump", m_elevatorAnimation, m_sprite);
	statePortal = std::make_shared<State>("PortalEvent", m_portalAnimation, m_sprite);
	stateKonami = std::make_shared<State>("KonamiCode", m_konamiCodeAnimation, m_sprite);
	stateDrink = std::make_shared<State>("Drink", m_drinkAnimation, m_sprite);

	machine.setStart(stateIdle);
	machine.addTransition(Left, stateIdle, stateLeft);
	machine.addTransition(Left, stateLeft, stateLeft);
	machine.addTransition(Left, stateRight, stateLeft);
	machine.addTransition(Left, stateDoubleJump, stateLeft);
	machine.addTransition(Left, stateFall, stateLeft);
	machine.addTransition(Left, stateDieFall, stateLeft);
	machine.addTransition(Left, stateDance, stateLeft);
	machine.addTransition(Left, stateStartSlowFall, stateLeft);
	machine.addTransition(Left, stateDeath, stateLeft);
	machine.addTransition(Left, stateStartElevator, stateLeft);
	machine.addTransition(Left, stateDanceMusic, stateLeft);
	machine.addTransition(Left, stateStartJump, stateLeft);
	machine.addTransition(Left, stateSlowFall1, stateLeft);
	machine.addTransition(Left, stateSlowFall2, stateLeft);
	machine.addTransition(Left, stateSlowFall3, stateLeft);
	machine.addTransition(Left, stateSartWaterJump, stateLeft);
	machine.addTransition(Left, stateWaterJump, stateLeft);
	machine.addTransition(Left, stateDrink, stateLeft);

	machine.addTransition(Right, stateIdle, stateRight);
	machine.addTransition(Right, stateLeft, stateRight);
	machine.addTransition(Right, stateRight, stateRight);
	machine.addTransition(Right, stateDoubleJump, stateRight);
	machine.addTransition(Right, stateFall, stateRight);
	machine.addTransition(Right, stateDieFall, stateRight);
	machine.addTransition(Right, stateDance, stateRight);
	machine.addTransition(Right, stateStartSlowFall, stateRight);
	machine.addTransition(Right, stateDeath, stateRight);
	machine.addTransition(Right, stateStartElevator, stateRight);
	machine.addTransition(Right, stateDanceMusic, stateRight);
	machine.addTransition(Right, stateStartJump, stateRight);
	machine.addTransition(Right, stateSlowFall1, stateLeft);
	machine.addTransition(Right, stateSlowFall2, stateLeft);
	machine.addTransition(Right, stateSlowFall3, stateLeft);
	machine.addTransition(Right, stateSartWaterJump, stateRight);
	machine.addTransition(Right, stateWaterJump, stateRight);
	machine.addTransition(Right, stateDrink, stateRight);

	machine.addTransition(StartJump, stateIdle, stateStartJump);
	machine.addTransition(StartJump, stateLeft, stateStartJump);
	machine.addTransition(StartJump, stateRight, stateStartJump);
	machine.addTransition(StartJump, stateDoubleJump, stateStartJump);
	machine.addTransition(StartJump, stateFall, stateStartJump);
	machine.addTransition(StartJump, stateDieFall, stateStartJump);
	machine.addTransition(StartJump, stateStartSlowFall, stateStartJump);
	machine.addTransition(StartJump, stateDeath, stateStartJump);
	machine.addTransition(StartJump, stateDanceMusic, stateStartJump);
	machine.addTransition(StartJump, stateStartJump, stateStartJump);
	machine.addTransition(StartJump, stateSlowFall1, stateStartJump);
	machine.addTransition(StartJump, stateSlowFall2, stateStartJump);
	machine.addTransition(StartJump, stateSlowFall3, stateStartJump);
	machine.addTransition(StartJump, stateSartWaterJump, stateStartJump);
	machine.addTransition(StartJump, stateWaterJump, stateStartJump);
	machine.addTransition(StartJump, statePortal, stateStartJump);
	machine.addTransition(StartJump, stateDrink, stateStartJump);

	machine.addTransition(DoubleJump, stateIdle, stateDoubleJump);
	machine.addTransition(DoubleJump, stateLeft, stateDoubleJump);
	machine.addTransition(DoubleJump, stateRight, stateDoubleJump);
	machine.addTransition(DoubleJump, stateDoubleJump, stateDoubleJump);
	machine.addTransition(DoubleJump, stateFall, stateDoubleJump);
	machine.addTransition(DoubleJump, stateDieFall, stateDoubleJump);
	machine.addTransition(DoubleJump, stateStartSlowFall, stateDoubleJump);
	machine.addTransition(DoubleJump, stateElevator, stateDoubleJump);
	machine.addTransition(DoubleJump, stateStartJump, stateDoubleJump);
	machine.addTransition(DoubleJump, stateSlowFall1, stateDoubleJump);
	machine.addTransition(DoubleJump, stateSlowFall2, stateDoubleJump);
	machine.addTransition(DoubleJump, stateSlowFall3, stateDoubleJump);
	machine.addTransition(DoubleJump, stateSartWaterJump, stateDoubleJump);
	machine.addTransition(DoubleJump, stateWaterJump, stateDoubleJump);
	machine.addTransition(DoubleJump, statePortal, stateDoubleJump);
	machine.addTransition(DoubleJump, stateDrink, stateDoubleJump);

	machine.addTransition(Fall, stateIdle, stateFall);
	machine.addTransition(Fall, stateLeft, stateFall);
	machine.addTransition(Fall, stateRight, stateFall);
	machine.addTransition(Fall, stateDoubleJump, stateFall);
	machine.addTransition(Fall, stateFall, stateFall);
	machine.addTransition(Fall, stateDieFall, stateFall);
	machine.addTransition(Fall, stateDance, stateFall);
	machine.addTransition(Fall, stateStartSlowFall, stateFall);
	machine.addTransition(Fall, stateStartElevator, stateFall);
	machine.addTransition(Fall, stateElevator, stateFall);
	machine.addTransition(Fall, stateDanceMusic, stateFall);
	machine.addTransition(Fall, stateStartJump, stateFall);
	machine.addTransition(Fall, stateSlowFall1, stateFall);
	machine.addTransition(Fall, stateSlowFall2, stateFall);
	machine.addTransition(Fall, stateSlowFall3, stateFall);
	machine.addTransition(Fall, stateSartWaterJump, stateFall);
	machine.addTransition(Fall, stateWaterJump, stateFall);
	machine.addTransition(Fall, statePortal, stateFall);
	machine.addTransition(Fall, stateKonami, stateFall);
	machine.addTransition(Fall, stateDrink, stateFall);

	machine.addTransition(DieFall, stateFall, stateDieFall);
	machine.addTransition(DieFall, stateIdle, stateDieFall);
	machine.addTransition(DieFall, stateLeft, stateDieFall);
	machine.addTransition(DieFall, stateRight, stateDieFall);
	machine.addTransition(DieFall, stateDoubleJump, stateDieFall);
	machine.addTransition(DieFall, stateDieFall, stateDieFall);
	machine.addTransition(DieFall, stateDance, stateDieFall);
	machine.addTransition(DieFall, stateStartSlowFall, stateDieFall);
	machine.addTransition(DieFall, stateStartElevator, stateDieFall);
	machine.addTransition(DieFall, stateElevator, stateDieFall);
	machine.addTransition(DieFall, stateDanceMusic, stateDieFall);
	machine.addTransition(DieFall, stateStartJump, stateDieFall);
	machine.addTransition(DieFall, stateSlowFall1, stateDieFall);
	machine.addTransition(DieFall, stateSlowFall2, stateDieFall);
	machine.addTransition(DieFall, stateSlowFall3, stateDieFall);
	machine.addTransition(DieFall, stateSartWaterJump, stateDieFall);
	machine.addTransition(DieFall, stateWaterJump, stateDieFall);
	machine.addTransition(DieFall, statePortal, stateDieFall);
	machine.addTransition(DieFall, stateKonami, stateDieFall);
	machine.addTransition(DieFall, stateDrink, stateDieFall);

	machine.addTransition(Dance, stateIdle, stateDance);

	machine.addTransition(DanceWithMusic, stateIdle, stateDanceMusic);

	machine.addTransition(StartSlowFall, stateIdle, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateLeft, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateRight, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateDoubleJump, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateFall, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateDieFall, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateDance, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateStartSlowFall, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateStartElevator, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateElevator, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateDanceMusic, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateStartJump, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateSlowFall1, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateSlowFall2, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateSlowFall3, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateSartWaterJump, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateWaterJump, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateDrink, stateStartSlowFall);

	machine.addTransition(SlowFall1, stateStartSlowFall, stateSlowFall1);
	machine.addTransition(SlowFall1, stateSlowFall1, stateSlowFall1);
	machine.addTransition(SlowFall1, stateSlowFall2, stateSlowFall1);
	machine.addTransition(SlowFall1, stateSlowFall3, stateSlowFall1);
	machine.addTransition(SlowFall2, stateStartSlowFall, stateSlowFall2);
	machine.addTransition(SlowFall2, stateSlowFall1, stateSlowFall2);
	machine.addTransition(SlowFall2, stateSlowFall2, stateSlowFall2);
	machine.addTransition(SlowFall2, stateSlowFall3, stateSlowFall2);
	machine.addTransition(SlowFall3, stateStartSlowFall, stateSlowFall3);
	machine.addTransition(SlowFall3, stateSlowFall1, stateSlowFall3);
	machine.addTransition(SlowFall3, stateSlowFall2, stateSlowFall3);
	machine.addTransition(SlowFall3, stateSlowFall3, stateSlowFall3);

	machine.addTransition(Death, stateIdle, stateDeath);
	machine.addTransition(Death, stateLeft, stateDeath);
	machine.addTransition(Death, stateRight, stateDeath);
	machine.addTransition(Death, stateDoubleJump, stateDeath);
	machine.addTransition(Death, stateFall, stateDeath);
	machine.addTransition(Death, stateDieFall, stateDeath);
	machine.addTransition(Death, stateDance, stateDeath);
	machine.addTransition(Death, stateStartSlowFall, stateDeath);
	machine.addTransition(Death, stateDanceMusic, stateDeath);
	machine.addTransition(Death, stateStartJump, stateDeath);
	machine.addTransition(Death, stateSlowFall1, stateDeath);
	machine.addTransition(Death, stateSlowFall2, stateDeath);
	machine.addTransition(Death, stateSlowFall3, stateDeath);
	machine.addTransition(Death, stateSartWaterJump, stateDeath);
	machine.addTransition(Death, stateWaterJump, stateDeath);

	machine.addTransition(StartElevator, stateIdle, stateStartElevator);
	machine.addTransition(StartElevator, stateLeft, stateStartElevator);
	machine.addTransition(StartElevator, stateRight, stateStartElevator);
	machine.addTransition(StartElevator, stateDoubleJump, stateStartElevator);
	machine.addTransition(StartElevator, stateFall, stateStartElevator);
	machine.addTransition(StartElevator, stateDieFall, stateStartElevator);
	machine.addTransition(StartElevator, stateDance, stateStartElevator);
	machine.addTransition(StartElevator, stateStartSlowFall, stateStartElevator);
	machine.addTransition(StartElevator, stateStartElevator, stateStartElevator);
	machine.addTransition(StartElevator, stateElevator, stateStartElevator);
	machine.addTransition(StartElevator, stateDanceMusic, stateStartElevator);
	machine.addTransition(StartElevator, stateStartJump, stateStartElevator);
	machine.addTransition(StartElevator, stateSlowFall1, stateStartElevator);
	machine.addTransition(StartElevator, stateSlowFall2, stateStartElevator);
	machine.addTransition(StartElevator, stateSlowFall3, stateStartElevator);
	machine.addTransition(StartElevator, stateSartWaterJump, stateStartElevator);
	machine.addTransition(StartElevator, stateWaterJump, stateStartElevator);

	machine.addTransition(Elevator, stateStartElevator, stateElevator);

	machine.addTransition(StartWaterJump, stateIdle, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateLeft, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateRight, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateDoubleJump, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateFall, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateDieFall, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateDance, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateStartSlowFall, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateDeath, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateStartElevator, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateElevator, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateDanceMusic, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateStartJump, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateSlowFall1, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateSlowFall2, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateSlowFall3, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateSartWaterJump, stateSartWaterJump);
	machine.addTransition(StartWaterJump, stateWaterJump, stateSartWaterJump);
	machine.addTransition(StartWaterJump, statePortal, stateSartWaterJump);

	machine.addTransition(WaterJump, stateSartWaterJump, stateWaterJump);
	machine.addTransition(WaterJump, stateStartJump, stateWaterJump);

	machine.addTransition(Idle, stateIdle, stateIdle);
	machine.addTransition(Idle, stateLeft, stateIdle);
	machine.addTransition(Idle, stateRight, stateIdle);
	machine.addTransition(Idle, stateDoubleJump, stateIdle);
	machine.addTransition(Idle, stateFall, stateIdle);
	machine.addTransition(Idle, stateDieFall, stateIdle);
	machine.addTransition(Idle, stateDance, stateIdle);
	machine.addTransition(Idle, stateStartSlowFall, stateIdle);
	machine.addTransition(Idle, stateDeath, stateIdle);
	machine.addTransition(Idle, stateStartElevator, stateIdle);
	machine.addTransition(Idle, stateDanceMusic, stateIdle);
	machine.addTransition(Idle, stateStartJump, stateIdle);
	machine.addTransition(Idle, stateSlowFall1, stateIdle);
	machine.addTransition(Idle, stateSlowFall2, stateIdle);
	machine.addTransition(Idle, stateSlowFall3, stateIdle);
	machine.addTransition(Idle, stateSartWaterJump, stateIdle);
	machine.addTransition(Idle, statePortal, stateIdle);
	machine.addTransition(Idle, stateKonami, stateIdle);
	machine.addTransition(Idle, stateDrink, stateIdle);

	machine.addTransition(PortalEvent, stateIdle, statePortal);
	machine.addTransition(PortalEvent, stateLeft, statePortal);
	machine.addTransition(PortalEvent, stateRight, statePortal);
	machine.addTransition(PortalEvent, stateDoubleJump, statePortal);
	machine.addTransition(PortalEvent, stateFall, statePortal);
	machine.addTransition(PortalEvent, stateDieFall, statePortal);
	machine.addTransition(PortalEvent, stateDance, statePortal);
	machine.addTransition(PortalEvent, stateStartSlowFall, statePortal);
	machine.addTransition(PortalEvent, stateDeath, statePortal);
	machine.addTransition(PortalEvent, stateStartElevator, statePortal);
	machine.addTransition(PortalEvent, stateDanceMusic, statePortal);
	machine.addTransition(PortalEvent, stateStartJump, statePortal);
	machine.addTransition(PortalEvent, stateSlowFall1, statePortal);
	machine.addTransition(PortalEvent, stateSlowFall2, statePortal);
	machine.addTransition(PortalEvent, stateSlowFall3, statePortal);
	machine.addTransition(PortalEvent, stateSartWaterJump, statePortal);
	machine.addTransition(PortalEvent, stateWaterJump, statePortal);

	machine.addTransition(KonamiCode, stateIdle, stateKonami);
	machine.addTransition(KonamiCode, stateLeft, stateKonami);
	machine.addTransition(KonamiCode, stateRight, stateKonami);
	machine.addTransition(KonamiCode, stateDoubleJump, stateKonami);
	machine.addTransition(KonamiCode, stateFall, stateKonami);
	machine.addTransition(KonamiCode, stateDieFall, stateKonami);
	machine.addTransition(KonamiCode, stateDance, stateKonami);
	machine.addTransition(KonamiCode, stateStartSlowFall, stateKonami);
	machine.addTransition(KonamiCode, stateDeath, stateKonami);
	machine.addTransition(KonamiCode, stateStartElevator, stateKonami);
	machine.addTransition(KonamiCode, stateDanceMusic, stateKonami);
	machine.addTransition(KonamiCode, stateStartJump, stateKonami);
	machine.addTransition(KonamiCode, stateSlowFall1, stateKonami);
	machine.addTransition(KonamiCode, stateSlowFall2, stateKonami);
	machine.addTransition(KonamiCode, stateSlowFall3, stateKonami);
	machine.addTransition(KonamiCode, stateSartWaterJump, stateKonami);
	machine.addTransition(KonamiCode, statePortal, stateKonami);

	machine.addTransition(Drink, stateIdle, stateDrink);
	machine.addTransition(Drink, stateLeft, stateDrink);
	machine.addTransition(Drink, stateRight, stateDrink);
	machine.addTransition(Drink, stateDoubleJump, stateDrink);
	machine.addTransition(Drink, stateFall, stateDrink);
	machine.addTransition(Drink, stateDieFall, stateDrink);
	machine.addTransition(Drink, stateDance, stateDrink);
	machine.addTransition(Drink, stateStartSlowFall, stateDrink);
	machine.addTransition(Drink, stateDeath, stateDrink);
	machine.addTransition(Drink, stateStartElevator, stateDrink);
	machine.addTransition(Drink, stateDanceMusic, stateDrink);
	machine.addTransition(Drink, stateStartJump, stateDrink);
	machine.addTransition(Drink, stateSlowFall1, stateDrink);
	machine.addTransition(Drink, stateSlowFall2, stateDrink);
	machine.addTransition(Drink, stateSlowFall3, stateDrink);
	machine.addTransition(Drink, stateSartWaterJump, stateDrink);
	machine.addTransition(Drink, statePortal, stateDrink);

	m_sprite.setMachine(machine);
}

void	CharacterOcto::update(sf::Time frameTime)
{
	Progress & progress = Progress::getInstance();
	if (progress.isMenu())
		updateAI(frameTime);

	if (progress.isResetDoubleJump())
		resetDoubleJump(true);

	dieGrass();

	if (m_onGround)
	{
		m_lastPositionOnGround = getPosition();
		m_onGroundDelay = m_onGroundDelayMax;
	}
	m_onGroundDelay -= frameTime;
	portalEvent();
	if (m_sprite.getCurrentEvent() != PortalEvent && m_sprite.getCurrentEvent() != KonamiCode && m_sprite.getCurrentEvent() != Drink)
		commitPhysicsToGraphics();
	m_sprite.update(frameTime);
	resetTimeEvent();
	timeEvent(frameTime);
	inWater();
	if (m_sprite.getCurrentEvent() != PortalEvent && m_sprite.getCurrentEvent() != KonamiCode && m_sprite.getCurrentEvent() != Drink && endDeath())
	{
		dance();
		dieFall();
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
	if ((m_sprite.getCurrentEvent() == Drink || m_sprite.getCurrentEvent() == KonamiCode) && m_sprite.isTerminated())
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

	m_ploufParticle.setEmitter(sf::Vector2f(m_box->getBaryCenter().x, m_waterLevel));
	m_ploufParticle.update(frameTime);
	m_waterParticle.setEmitter(m_box->getBaryCenter() + sf::Vector2f(-m_box->getSize().x / 2.f, m_box->getSize().y / 2.f));
	m_waterParticle.update(frameTime);
	if (!m_progress.canUseWaterJump())
	{
		m_ploufParticle.setColor(m_waterColor);
		m_waterParticle.setColor(m_waterColor);
	}
	else
	{
		m_ploufParticle.setColor(m_secondWaterColor);
		m_waterParticle.setColor(m_secondWaterColor);
	}
	m_inkParticle.update(frameTime);
	if (m_timeEventInk > sf::Time::Zero && m_timeEventInk < sf::seconds(0.07f))
	{
		m_inkParticle.setCanEmit(true);
		m_inkParticle.setPosition(m_box->getBaryCenter());
	}
	else
		m_inkParticle.setCanEmit(false);

	m_bubbleParticle.setPosition(m_box->getBaryCenter());
	m_bubbleParticle.update(frameTime);
	if (getPosition().y > m_waterLevel + 40.f && m_inWater)
		m_bubbleParticle.setCanEmit(true);
	else
		m_bubbleParticle.setCanEmit(false);

	for (auto & robot : m_nanoRobots)
	{
		robot->update(frameTime);
		robot->setPosition(m_box->getPosition() + sf::Vector2f(20.f, 0.f));
	}

	progress.setOctoPos(getPosition());

	if (m_replaceOcto && m_collidingTile.size())
	{
		m_replaceOcto = false;
		m_highestPosition.y = m_box->getPosition().y;
		for (std::size_t i = 0; i < m_collidingTile.size(); i += 2)
		{
			if (m_box->getGlobalBounds().left >= m_collidingTile[i].x && m_box->getGlobalBounds().left <= m_collidingTile[i + 1].x)
			{
				sf::Vector2f pos;
				pos.x = m_box->getGlobalBounds().left;
				pos.y = octo::linearInterpolation(m_collidingTile[i].y, m_collidingTile[i + 1].y, (m_box->getGlobalBounds().left - m_collidingTile[i].x) / Tile::TileSize);
				if (pos.y < m_highestPosition.y)
				{
					m_highestPosition.y = pos.y;
				}
			}
			if (m_box->getGlobalBounds().left + m_box->getGlobalBounds().width >= m_collidingTile[i].x && m_box->getGlobalBounds().left + m_box->getGlobalBounds().width <= m_collidingTile[i + 1].x)
			{
				sf::Vector2f pos;
				pos.x = m_box->getGlobalBounds().left + m_box->getGlobalBounds().width;
				pos.y = octo::linearInterpolation(m_collidingTile[i].y, m_collidingTile[i + 1].y, (m_box->getGlobalBounds().left + m_box->getGlobalBounds().width - m_collidingTile[i].x) / Tile::TileSize);
				if (pos.y < m_highestPosition.y)
				{
					m_highestPosition.y = pos.y;
				}
			}
			if (m_collidingTile[i].x >= m_box->getGlobalBounds().left && m_collidingTile[i + 1].x <= m_box->getGlobalBounds().left + m_box->getGlobalBounds().width)
			{
				if (m_collidingTile[i].y < m_highestPosition.y)
				{
					m_highestPosition.y = m_collidingTile[i].y;
				}
				if (m_collidingTile[i + 1].y < m_highestPosition.y)
				{
					m_highestPosition.y = m_collidingTile[i + 1].y;
				}
			}
		}
		m_box->setPosition(sf::Vector2f(m_box->getPosition().x - m_highestPosition.x, m_highestPosition.y - m_box->getGlobalBounds().height));
	}
	m_highestPosition.x = 0.f;

	if (m_enableCutscene)
	{
		m_cutsceneTimer += frameTime;
		if (m_cutsceneTimer > m_cutsceneTimerMax)
		{
			m_cutsceneTimer = m_cutsceneTimerMax;
			if (m_autoDisableCutscene)
				m_enableCutscene = false;
		}
		m_cutsceneShader.setParameter("time", m_cutsceneTimer / m_cutsceneTimerMax);
	}
	else
	{
		m_cutsceneTimer -= frameTime;
		m_cutsceneShader.setParameter("time", m_cutsceneTimer / m_cutsceneTimerMax);
		if (m_cutsceneTimer <= sf::Time::Zero)
			PostEffectLayer::getInstance().enableShader(CUTSCENE_FRAG, false);
	}
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
		case DieFall:
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
		case SlowFall1:
		case SlowFall2:
		case SlowFall3:
			m_timeSlowFall += frameTime;
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
	m_bubbleParticle.draw(render);
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

void	CharacterOcto::onCollision(TileShape * tileshape, GameObjectType type, sf::Vector2f const& collisionDirection)
{
	switch(type)
	{
		case GameObjectType::Tile:
			if (std::abs(collisionDirection.x) >= Tile::TileSize)
			{
				m_replaceOcto = true;
				m_collidingTile.push_back(tileshape->getVertex(0u));
				m_collidingTile.push_back(tileshape->getVertex(1u));
				m_highestPosition.x += collisionDirection.x;
			}
			if (collisionDirection.x == 0.f && collisionDirection.y != 0.f)
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

void	CharacterOcto::resetDoubleJump(bool reset)
{
	if (reset && m_numberOfJump > 1)
	{
		m_numberOfJump = 1;
		Progress::getInstance().resetDoubleJump(false);
	}
}

void	CharacterOcto::giveNanoRobot(NanoRobot * robot, bool firstTime)
{
	m_nanoRobots.push_back(std::unique_ptr<NanoRobot>(robot));
	if (robot->getEffectEnable())
		startKonamiCode(firstTime);
	if (firstTime)
		enableCutscene(true, true);
}

void	CharacterOcto::giveRepairNanoRobot(RepairNanoRobot * robot, bool firstTime)
{
	m_nanoRobots.push_back(std::unique_ptr<NanoRobot>(robot));
	if (robot->getEffectEnable())
		startKonamiCode(firstTime);
	m_repairNanoRobot = robot;
	if (firstTime)
		enableCutscene(true, true);
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
		m_progress.setOctoPosTransition(m_sprite.getPosition() + m_sprite.getGlobalSize() - cameraPos);
		m_progress.setReverseSprite(m_originMove);
		m_progress.setNextDestination(portal.getDestination());
		if (!m_progress.isMenu())
			m_progress.setRespawnType(Progress::RespawnType::Portal);
	}
}

void	CharacterOcto::startDrinkPotion(void)
{
	if (m_sprite.getCurrentEvent() != Drink && !Progress::getInstance().isMenu())
	{
		m_sprite.setNextEvent(Drink);
		m_box->setApplyGravity(false);
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
		if (m_numberOfJump == 0 && !m_inWater && !m_useElevator && m_onGroundDelay <= sf::Time::Zero)
			m_numberOfJump = 1;
	}
	else
	{
		if (!m_onGround)
		{
			m_numberOfJump = 0;
			m_onGround = true;
			m_afterJump = false;
			m_timeSlowFall = sf::Time::Zero;
			if (m_sprite.getCurrentEvent() == Death || m_sprite.getCurrentEvent() == DieFall)
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
	Progress & progress = Progress::getInstance();
	progress.setOctoDoubleJump(false);
	switch (event)
	{
		case WaterJump:
		case StartJump:
		case DoubleJump:
			progress.setOctoDoubleJump(true);
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
			{
				if (m_timeSlowFall <= m_timeSlowFallMax * (2.f / 4.f))
					m_sprite.setNextEvent(SlowFall1);
				else if (m_timeSlowFall <= m_timeSlowFallMax * (3.f / 4.f))
					m_sprite.setNextEvent(SlowFall2);
				else
					m_sprite.setNextEvent(SlowFall3);
			}
			break;
		case StartWaterJump:
			if (m_sprite.isTerminated())
				m_sprite.setNextEvent(WaterJump);
			break;
		case Fall:
		case DieFall:
			if (m_keyUp)
			{
				if (m_progress.canSlowFall() && m_timeSlowFall < m_timeSlowFallMax)
					m_sprite.setNextEvent(StartSlowFall);
			}
			if (m_keyElevator)
			{
				if (m_progress.canUseElevator() && m_collisionElevator)
					m_sprite.setNextEvent(StartElevator);
			}
			break;
		case SlowFall1:
		case SlowFall2:
		case SlowFall3:
			if (m_sprite.isTerminated())
			{
				if (m_timeSlowFall > m_timeSlowFallMax)
					m_sprite.setNextEvent(Fall);
				else if (m_timeSlowFall <= m_timeSlowFallMax * (2.f / 4.f))
						m_sprite.setNextEvent(SlowFall1);
				else if (m_timeSlowFall <= m_timeSlowFallMax * (3.f / 4.f))
					m_sprite.setNextEvent(SlowFall2);
				else
					m_sprite.setNextEvent(SlowFall3);
			}
			break;
		case StartElevator:
		case Elevator:
			if (!m_collisionElevator)
				m_sprite.setNextEvent(Fall);
			break;
		default:
			if (m_sprite.getCurrentEvent() != DieFall)
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
		m_timeSlowFall = sf::Time::Zero;
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

void	CharacterOcto::kill()
{
	if (m_sprite.getCurrentEvent() != Death)
	{
		Progress & progress = Progress::getInstance();

		m_sprite.setNextEvent(Death);
		m_helmetParticle.canEmit(true);
		m_helmetParticle.setPosition(getPosition() + sf::Vector2f(0.f, -25.f));
		progress.registerDeath(getPosition());
		progress.setKillOcto(false);
	}
}

bool	CharacterOcto::dieFall()
{
	if (m_timeEventFall > sf::seconds(2.3f) && m_sprite.getCurrentEvent() != DieFall)
		m_sprite.setNextEvent(DieFall);
	if (m_sprite.getCurrentEvent() == DieFall && m_onGround && !m_inWater && !Progress::getInstance().isMenu())
	{
		kill();
		return true;
	}
	return false;
}

bool	CharacterOcto::dieGrass()
{
	Progress & progress = Progress::getInstance();

	if (progress.getKillOcto() && !progress.isMenu())
	{
		kill();
		return true;
	}
	return false;
}

bool	CharacterOcto::endDeath()
{
	octo::Camera&				camera = octo::Application::getCamera();
	sf::Vector2f const&			cameraPos = sf::Vector2f(camera.getRectangle().left, camera.getRectangle().top);

	m_progress.setOctoPosTransition(m_sprite.getPosition() + m_sprite.getGlobalSize() - cameraPos);
	m_progress.setReverseSprite(m_originMove);
	if (m_sprite.getCurrentEvent() == Death)
	{
		if (m_sprite.isTerminated())
		{
			octo::StateManager &		states = octo::Application::getStateManager();
			states.change("octo_death");
		}
		return false;
	}
	return true;
}

void	CharacterOcto::dance()
{
	typedef octo::CharacterAnimation::Frame			Frame;
	Events	event = static_cast<Events>(m_sprite.getCurrentEvent());

	if (m_timeEventIdle > m_timeEventIdleMax && event == Idle)
	{
		m_timeEventIdleMax = sf::seconds(m_generator.randomFloat(3.f, 10.f));

		switch (m_generator.randomInt(0, 1))
		{
			case 0:
			{
				m_danceWithMusicAnimation.setFrames({
						Frame(sf::seconds(0.2f), {21, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.2f), {22, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.4f), {23, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.2f), {22, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.2f), {21, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.2f), {22, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.4f), {23, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.2f), {22, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.2f), {21, sf::FloatRect(), sf::Vector2f()}),
						});
				m_danceWithMusicAnimation.setLoop(octo::LoopMode::NoLoop);
				break;
			}
			case 1:
			{
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
				break;
			}
		}
		m_sprite.setNextEvent(DanceWithMusic);
	}

	if (event == DanceWithMusic && m_sprite.isTerminated())
		m_sprite.setNextEvent(Idle);
}

void	CharacterOcto::inWater()
{
	bool	emit = false;

	if (m_waterLevel != -1.f && m_box->getBaryCenter().y + m_box->getSize().y / 2.f > m_waterLevel)
	{
		Progress & progress = Progress::getInstance();

		if (!m_inWater)
		{
			m_numberOfJump = 0;
			emit = true;
			m_inWater = true;
		}

		if (m_isDeadlyWater && !progress.canUseWaterJump())
			kill();

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

	if (m_generator.randomBool(0.2f))
	{
		m_startJumpAnimation.setFrames({
				Frame(sf::seconds(0.2f), {77, sf::FloatRect(), sf::Vector2f()}),
				Frame(sf::seconds(0.2f), {78, sf::FloatRect(), sf::Vector2f()}),
				Frame(sf::seconds(0.2f), {79, sf::FloatRect(), sf::Vector2f()}),
				Frame(sf::seconds(0.2f), {80, sf::FloatRect(), sf::Vector2f()}),
				Frame(sf::seconds(0.2f), {81, sf::FloatRect(), sf::Vector2f()}),
				Frame(sf::seconds(0.2f), {82, sf::FloatRect(), sf::Vector2f()}),
				Frame(sf::seconds(0.2f), {84, sf::FloatRect(), sf::Vector2f()}),
				Frame(sf::seconds(0.2f), {28, sf::FloatRect(), sf::Vector2f()}),
				});
		m_startJumpAnimation.setLoop(octo::LoopMode::NoLoop);
	}
	else
	{
		m_startJumpAnimation.setFrames({
				Frame(sf::seconds(0.4f), {25, sf::FloatRect(), sf::Vector2f()}),
				Frame(sf::seconds(0.4f), {26, sf::FloatRect(), sf::Vector2f()}),
				Frame(sf::seconds(0.4f), {27, sf::FloatRect(), sf::Vector2f()}),
				Frame(sf::seconds(0.4f), {28, sf::FloatRect(), sf::Vector2f()}),
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
			(event == DoubleJump || event == StartJump))
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
		if ((event == StartSlowFall || event == SlowFall1 || event == SlowFall2 || event == SlowFall3))
		{
			if (event == StartSlowFall)
				velocity.x *= 1.3f;
			else
				velocity.x *= 1.6f;
			velocity.y = m_pixelSecondSlowFall;
		}
		if (event == StartWaterJump || event == WaterJump)
		{
			m_lastPositionOnGround = getPosition();
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
	if (m_collisionElevator && (event == Fall || event == DieFall))
	{
		if (event == DieFall)
			m_sprite.setNextEvent(Fall);
		velocity.y = m_pixelSecondSlowFall;
	}
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
			case DieFall:
				if (m_sprite.getCurrentEvent() != Fall)
					m_sprite.setNextEvent(Fall);
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
			case SlowFall1:
			case SlowFall2:
			case SlowFall3:
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
		if ((m_onGround || m_onGroundDelay >= sf::Time::Zero || m_inWater) && m_progress.canJump() && !m_numberOfJump)
		{
			m_sprite.setNextEvent(StartJump);
			m_jumpVelocity = m_pixelSecondJump;
			m_numberOfJump = 1;
			if (!m_progress.canDoubleJump() && m_inWater)
				m_numberOfJump = 0;
		}
		else if (m_numberOfJump == 1 && m_progress.canDoubleJump())
		{
			m_timeSlowFall = sf::Time::Zero;
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
		else if (!m_onGround && !m_inWater && m_progress.canSlowFall() && m_timeSlowFall < m_timeSlowFallMax)
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
			|| m_sprite.getCurrentEvent() == Drink
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
		case OctoKeys::Jump:
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

bool	CharacterOcto::isFalling(void)
{
	Events	state = static_cast<Events>(m_sprite.getCurrentEvent());
	if ((state == Fall || state == DieFall || state == SlowFall1 || state == SlowFall2 || state == SlowFall3) && m_lastPositionOnGround.y < getPosition().y)
		return true;
	return false;
}

bool	CharacterOcto::isRaising(void)
{
	Events	state = static_cast<Events>(m_sprite.getCurrentEvent());
	return state == WaterJump;
}

bool	CharacterOcto::isInAir(void) const
{
	return (!m_onGround);
}

bool	CharacterOcto::isMeetingNpc(void) const
{
	return m_meetNpc;
}

bool	CharacterOcto::isEndingInRocket(void) const
{
	return m_isEndingInRocket;
}

void	CharacterOcto::endInRocket(void)
{
	m_isEndingInRocket = true;
	m_box->setApplyGravity(false);
}

void	CharacterOcto::resetCollidingTileCount(void)
{
	m_collidingTile.clear();
}

bool	CharacterOcto::isOnGround(void) const
{
	return m_onGround;
}

void	CharacterOcto::meetNpc(bool meetNpc)
{
	m_meetNpc = meetNpc;
}

void	CharacterOcto::enableCutscene(bool enable, bool autoDisable)
{
	if (enable && !m_enableCutscene)
	{
		PostEffectLayer::getInstance().enableShader(CUTSCENE_FRAG, true);
		m_cutsceneTimer = sf::Time::Zero;
	}
	else
		m_cutsceneTimer = std::min(m_cutsceneTimer, m_cutsceneTimerMax);
	m_enableCutscene = enable;
	m_autoDisableCutscene = autoDisable;
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
		case OctoKeys::Jump:
			m_keySpace = false;
			if (state == DoubleJump || state == StartJump)
			{
				m_afterJump = true;
				m_afterJumpVelocity = m_pixelSecondAfterJump;
			}
			break;
		case OctoKeys::SlowFall:
			m_keyUp = false;
			if (state == WaterJump || state == SlowFall1 || state == SlowFall2 || state == SlowFall3)
			{
				m_afterJump = true;
				m_afterJumpVelocity = m_pixelSecondAfterJump;
			}
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
	if (state == Death || state == PortalEvent || state == KonamiCode || state == Drink || otherKeyReleased)
		return true;
	if (!m_onGround && !m_keyUp && !m_keyElevator)
	{
		if (state != Fall && state != DieFall)
		{
			if (state != StartJump && state != DoubleJump)
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

void	CharacterOcto::initAI(void)
{
	m_randomJumpTimer = sf::seconds(m_generator.randomFloat(1.f, 30.f));
	m_doubleJumpTimer = sf::seconds(m_generator.randomFloat(1.5f, 3.5f));
	m_directionTimer = sf::seconds(m_generator.randomFloat(30.f, 300.f));
	m_slowFallTimer = sf::seconds(m_generator.randomFloat(4.f, 10.f));
	m_portalTimer = sf::seconds(m_generator.randomFloat(45.f, 90.f));
	m_keyRight = true;
	m_keyLeft = false;
}

void	CharacterOcto::updateAI(sf::Time frameTime)
{
	//Jump if blocked
	m_jumpTimer -= frameTime;
	if (m_jumpTimer <= sf::Time::Zero && m_numberOfJump == 0)
	{
		m_keySpace = false;
		if (std::round(m_saveOctoPos.x) == std::round(getPosition().x))
		{
			m_doubleJumpTimer = sf::seconds(m_generator.randomFloat(1.5f, 3.5f));
			caseSpace();
		}
		m_saveOctoPos = getPosition();
	}

	//Random jump
	m_randomJumpTimer -= frameTime;
	if (m_randomJumpTimer <= sf::Time::Zero)
	{
		m_randomJumpTimer = sf::seconds(m_generator.randomFloat(1.f, 30.f));
		m_doubleJumpTimer = sf::seconds(m_generator.randomFloat(1.5f, 3.5f));
		caseSpace();
	}

	//Double jump
	if (m_numberOfJump == 1)
	{
		m_doubleJumpTimer -= frameTime;
		if (m_doubleJumpTimer <= sf::Time::Zero)
		{
			m_keySpace = false;
			caseSpace();
		}
	}

	//SlowFall
	m_slowFallTimer -= frameTime;
	if (m_slowFallTimer <= sf::Time::Zero)
	{
		m_slowFallTimer = sf::seconds(m_generator.randomFloat(4.f, 10.f));
		if (m_generator.randomBool(0.5f))
			caseUp();
		else
			m_keyUp = false;
	}

	//Portal
	m_portalTimer -= frameTime;
	if (m_portalTimer <= sf::Time::Zero)
	{
		caseAction();
		casePortal();
	}

	//Direction
	m_directionTimer -= frameTime;
	if (m_directionTimer <= sf::Time::Zero)
	{
		m_directionTimer = sf::seconds(m_generator.randomFloat(30.f, 300.f));
		if (m_keyRight)
		{
			m_keyRight = false;
			m_keyLeft = true;
		}
		else
		{
			m_keyRight = true;
			m_keyLeft = false;
		}
	}
}
