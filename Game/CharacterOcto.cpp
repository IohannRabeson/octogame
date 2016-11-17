#include <Application.hpp>
#include <AudioManager.hpp>
#include <PostEffectManager.hpp>
#include <ResourceManager.hpp>
#include <GraphicsManager.hpp>

#include "CharacterOcto.hpp"
#include "OctoSound.hpp"
#include "MusicManager.hpp"
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
	m_boxSize(sf::Vector2f(30.f, 85.f)),
	m_eventBox(PhysicsEngine::getShapeBuilder().createCircle(false)),
	m_repairNanoRobot(nullptr),
	m_progress(Progress::getInstance()),
	m_timeEventDieFallMax(sf::seconds(2.3f)),
	m_timeEventIdleMax(sf::seconds(4.f)),
	m_timeRepairSpaceShipMax(sf::seconds(10.f)),
	m_timeSlowFallMax(sf::seconds(1.5f)),
	m_timeStopVelocity(sf::seconds(0.06f)),
	m_timeStopVelocityMax(sf::seconds(0.06f)),
	m_timerStartUseDoor(sf::seconds(1.f)),
	m_timerStartUseDoorMax(sf::seconds(0.2f)),
	m_soundUseDoor(nullptr),
	m_factorDirectionVelocityX(1.f),
	m_spriteScale(0.6f),
	m_maxJumpWaterVelocity(-3000.f),
	m_pixelSecondJump(-1300.f),
	m_pixelSecondSlowFall(-400.f),
	m_pixelSecondWalk(320.f),
	m_pixelSecondAfterJump(-400.f),
	m_pixelSecondAfterFullJump(-500.f),
	m_pixelSecondMultiplier(800.f),
	m_deltaPositionY(27.f),
	m_numberOfJump(0),
	m_originMove(false),
	m_onGround(false),
	m_onGroundDelayMax(sf::seconds(0.05f)),
	m_soundGeneration(nullptr),
	m_groundVolume(0.7f),
	m_groundSoundTime(sf::Time::Zero),
	m_groundSoundTimeMax(sf::seconds(0.6f)),
	m_onElevator(false),
	m_useElevator(false),
	m_afterJump(false),
	m_keyLeft(false),
	m_keyRight(false),
	m_keyGroundRight(false),
	m_keyGroundLeft(false),
	m_keyJump(false),
	m_keyCapacity(false),
	m_keyDown(false),
	m_keyEntrance(false),
	m_keyPortal(false),
	m_keyElevator(false),
	m_keyZoomIn(false),
	m_collisionTile(false),
	m_collisionElevator(false),
	m_collisionPortal(false),
	m_collisionElevatorEvent(false),
	m_collisionSpaceShip(false),
	m_repairShip(false),
	m_inWater(false),
	m_isDeadlyWater(false),
	m_meetNpc(false),
	m_collidePortalEvent(false),
	m_replaceOcto(false),
	m_enableCutscene(false),
	m_stopFollowCamera(false),
	m_autoDisableCutscene(false),
	m_doorAction(false),
	m_isRocketEnd(false),
	m_generator("random"),
	m_cutsceneTimerMax(sf::seconds(2.f)),
	m_cutscenePauseTimerMax(sf::seconds(4.f)),
	m_adaptBoxTimerMax(sf::seconds(0.5f)),
	m_cutsceneShader(PostEffectLayer::getInstance().getShader(CUTSCENE_FRAG))
{
	m_sound.reset(new OctoSound());
	m_cutsceneShader.setParameter("height", 0.15);

	InputListener::addInputListener();

	initAI();
	initAIEnd();

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

	Level level = Progress::getInstance().getCurrentDestination();
	if (level == Level::Red || level == Level::Blue)
		enableCutscene(true, false);

	octo::AudioManager&			audio = octo::Application::getAudioManager();
	octo::ResourceManager &		resources = octo::Application::getResourceManager();

	m_soundUseDoor = audio.playSound(resources.getSound(OBJECT_TIMELAPSE_OGG), 0.f);
	m_soundUseDoor->setLoop(true);
	m_soundGeneration = audio.playSound(resources.getSound(OCTO_SOUND_GROUND_OGG), 0.f);
	m_soundGeneration->setLoop(true);
}

CharacterOcto::~CharacterOcto(void)
{
	if (m_soundUseDoor != nullptr)
		m_soundUseDoor->stop();
	if (m_soundGeneration != nullptr)
		m_soundGeneration->stop();
	if (!m_progress.isMenu())
		InputListener::removeInputListener();
}

void	CharacterOcto::setup(ABiome & biome)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();

	m_waterLevel = biome.getWaterLevel();
	m_sound->setWaterLevel(m_waterLevel);
	m_isDeadlyWater = biome.isDeadlyWater();
	m_timeEventDieVoidMax = biome.getTimeDieVoid();
	m_box->setGameObject(this);
	m_box->setSize(m_boxSize);
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
		| static_cast<std::size_t>(GameObjectType::HouseFlatSnow)
		| static_cast<std::size_t>(GameObjectType::EngineSnow)
		| static_cast<std::size_t>(GameObjectType::WeirdHouseSnow)
		| static_cast<std::size_t>(GameObjectType::Bouibouik)
		| static_cast<std::size_t>(GameObjectType::RocketDoor)
		| static_cast<std::size_t>(GameObjectType::CheckPoint)
		| static_cast<std::size_t>(GameObjectType::Door)
		| static_cast<std::size_t>(GameObjectType::Monolith)
		| static_cast<std::size_t>(GameObjectType::Pyramid);
	m_box->setCollisionMask(mask);

	m_octoEvent.m_octo = this;
	m_eventBox->setGameObject(&m_octoEvent);
	m_eventBox->setRadius(400.f);
	m_eventBox->setCollisionType(static_cast<std::size_t>(GameObjectType::PlayerEvent));
	std::size_t maskEvent = static_cast<std::size_t>(GameObjectType::Portal)
		| static_cast<std::size_t>(GameObjectType::SmokeInstance)
//Script AddNpc
		| static_cast<std::size_t>(GameObjectType::FlorentNpc)
		| static_cast<std::size_t>(GameObjectType::AnthemJungle)
		| static_cast<std::size_t>(GameObjectType::ScientistCedric)
		| static_cast<std::size_t>(GameObjectType::ScientistLu)
		| static_cast<std::size_t>(GameObjectType::ScientistFran)
		| static_cast<std::size_t>(GameObjectType::ScientistJu)
		| static_cast<std::size_t>(GameObjectType::ScientistJu)
		| static_cast<std::size_t>(GameObjectType::Rocket)
		| static_cast<std::size_t>(GameObjectType::OctoDeathNpc)
		| static_cast<std::size_t>(GameObjectType::TVScreen)
		| static_cast<std::size_t>(GameObjectType::CheckPoint)
		| static_cast<std::size_t>(GameObjectType::Pedestal)
		| static_cast<std::size_t>(GameObjectType::Elevator)
		| static_cast<std::size_t>(GameObjectType::SpaceShip)
		| static_cast<std::size_t>(GameObjectType::Monolith)
		| static_cast<std::size_t>(GameObjectType::WolfNpc)
		| static_cast<std::size_t>(GameObjectType::JellyfishNpc)
		| static_cast<std::size_t>(GameObjectType::Npc)
		| static_cast<std::size_t>(GameObjectType::IdleNpc)
		| static_cast<std::size_t>(GameObjectType::SpecialNpc)
		| static_cast<std::size_t>(GameObjectType::DisappearNpc)
		| static_cast<std::size_t>(GameObjectType::WalkNpc)
		| static_cast<std::size_t>(GameObjectType::FlyNpc);
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

	Progress const & progress = Progress::getInstance();
	if (!progress.isMenu() && progress.getNextDestination() == Level::Rewards)
		caseRight();

	//Force check joystick
	if (sf::Joystick::getAxisPosition(0, sf::Joystick::X) > 50 || sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) > 50)
		caseRight();
	if (sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -50 || sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) < -50)
		caseLeft();
}

void	CharacterOcto::setupAnimation()
{
	typedef octo::CharacterAnimation::Frame			Frame;

	randomJumpAnimation();
	m_idleAnimation.setFrames({
			Frame(sf::seconds(0.4f), {0, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {5, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {6, sf::FloatRect(), sf::Vector2f()}),
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
			Frame(sf::seconds(0.2f), {98, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {99, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {100, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {101, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {102, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {103, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {104, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {105, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {106, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {107, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {108, sf::FloatRect(), sf::Vector2f()}),
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
	StatePtr					stateStartSlowFall;
	StatePtr					stateDeath;
	StatePtr					stateStartElevator;
	StatePtr					stateElevator;
	StatePtr					stateWait;
	StatePtr					stateStartJump;
	StatePtr					stateSlowFall1;
	StatePtr					stateSlowFall2;
	StatePtr					stateSlowFall3;
	StatePtr					stateStartWaterJump;
	StatePtr					stateWaterJump;
	StatePtr					statePortalEvent;
	StatePtr					stateKonamiCode;
	StatePtr					stateDrink;

	stateIdle = std::make_shared<State>("Idle", m_idleAnimation, m_sprite);
	stateLeft = std::make_shared<State>("Left", m_walkAnimation, m_sprite);
	stateRight = std::make_shared<State>("Right", m_walkAnimation, m_sprite);
	stateDoubleJump = std::make_shared<State>("DoubleJump", m_startJumpAnimation, m_sprite);
	stateFall = std::make_shared<State>("Fall", m_fallAnimation, m_sprite);
	stateDieFall = std::make_shared<State>("DieFall", m_dieFallAnimation, m_sprite);
	stateWait = std::make_shared<State>("Wait", m_waitAnimation, m_sprite);
	stateStartSlowFall = std::make_shared<State>("StartSlowFall", m_startSlowFallAnimation, m_sprite);
	stateDeath = std::make_shared<State>("Death", m_deathAnimation, m_sprite);
	stateStartElevator = std::make_shared<State>("StartElevator", m_startElevatorAnimation, m_sprite);
	stateElevator = std::make_shared<State>("Elevator", m_elevatorAnimation, m_sprite);
	stateStartJump = std::make_shared<State>("StartJump", m_startJumpAnimation, m_sprite);
	stateSlowFall1 = std::make_shared<State>("SlowFall1", m_slowFallAnimation1, m_sprite);
	stateSlowFall2 = std::make_shared<State>("SlowFall2", m_slowFallAnimation2, m_sprite);
	stateSlowFall3 = std::make_shared<State>("SlowFall3", m_slowFallAnimation3, m_sprite);
	stateStartWaterJump = std::make_shared<State>("StartWaterJump", m_startElevatorAnimation, m_sprite);
	stateWaterJump = std::make_shared<State>("WaterJump", m_elevatorAnimation, m_sprite);
	statePortalEvent = std::make_shared<State>("PortalEvent", m_portalAnimation, m_sprite);
	stateKonamiCode = std::make_shared<State>("KonamiCode", m_konamiCodeAnimation, m_sprite);
	stateDrink = std::make_shared<State>("Drink", m_drinkAnimation, m_sprite);

	machine.setStart(stateIdle);

	machine.addTransition(Idle, stateIdle, stateIdle);
	machine.addTransition(Idle, stateRight, stateIdle);
	machine.addTransition(Idle, stateLeft, stateIdle);
	machine.addTransition(Idle, stateStartJump, stateIdle);
	machine.addTransition(Idle, stateDoubleJump, stateIdle);
	machine.addTransition(Idle, stateFall, stateIdle);
	machine.addTransition(Idle, stateDieFall, stateIdle);
	machine.addTransition(Idle, stateWait, stateIdle);
	machine.addTransition(Idle, stateStartSlowFall, stateIdle);
	machine.addTransition(Idle, stateDeath, stateIdle);
	machine.addTransition(Idle, stateDrink, stateIdle);
	machine.addTransition(Idle, stateStartElevator, stateIdle);
	machine.addTransition(Idle, stateElevator, stateIdle);
	machine.addTransition(Idle, stateSlowFall1, stateIdle);
	machine.addTransition(Idle, stateSlowFall2, stateIdle);
	machine.addTransition(Idle, stateSlowFall3, stateIdle);
	machine.addTransition(Idle, stateStartWaterJump, stateIdle);
	machine.addTransition(Idle, stateWaterJump, stateIdle);
	machine.addTransition(Idle, statePortalEvent, stateIdle);
	machine.addTransition(Idle, stateKonamiCode, stateIdle);

	machine.addTransition(Right, stateIdle, stateRight);
	machine.addTransition(Right, stateRight, stateRight);
	machine.addTransition(Right, stateLeft, stateRight);
	machine.addTransition(Right, stateStartJump, stateRight);
	machine.addTransition(Right, stateDoubleJump, stateRight);
	machine.addTransition(Right, stateFall, stateRight);
	machine.addTransition(Right, stateDieFall, stateRight);
	machine.addTransition(Right, stateWait, stateRight);
	machine.addTransition(Right, stateStartSlowFall, stateRight);
	machine.addTransition(Right, stateDeath, stateRight);
	machine.addTransition(Right, stateDrink, stateRight);
	machine.addTransition(Right, stateStartElevator, stateRight);
	machine.addTransition(Right, stateElevator, stateRight);
	machine.addTransition(Right, stateSlowFall1, stateRight);
	machine.addTransition(Right, stateSlowFall2, stateRight);
	machine.addTransition(Right, stateSlowFall3, stateRight);
	machine.addTransition(Right, stateStartWaterJump, stateRight);
	machine.addTransition(Right, stateWaterJump, stateRight);
	machine.addTransition(Right, statePortalEvent, stateRight);
	machine.addTransition(Right, stateKonamiCode, stateRight);

	machine.addTransition(Left, stateIdle, stateLeft);
	machine.addTransition(Left, stateRight, stateLeft);
	machine.addTransition(Left, stateLeft, stateLeft);
	machine.addTransition(Left, stateStartJump, stateLeft);
	machine.addTransition(Left, stateDoubleJump, stateLeft);
	machine.addTransition(Left, stateFall, stateLeft);
	machine.addTransition(Left, stateDieFall, stateLeft);
	machine.addTransition(Left, stateWait, stateLeft);
	machine.addTransition(Left, stateStartSlowFall, stateLeft);
	machine.addTransition(Left, stateDeath, stateLeft);
	machine.addTransition(Left, stateDrink, stateLeft);
	machine.addTransition(Left, stateStartElevator, stateLeft);
	machine.addTransition(Left, stateElevator, stateLeft);
	machine.addTransition(Left, stateSlowFall1, stateLeft);
	machine.addTransition(Left, stateSlowFall2, stateLeft);
	machine.addTransition(Left, stateSlowFall3, stateLeft);
	machine.addTransition(Left, stateStartWaterJump, stateLeft);
	machine.addTransition(Left, stateWaterJump, stateLeft);
	machine.addTransition(Left, statePortalEvent, stateLeft);
	machine.addTransition(Left, stateKonamiCode, stateLeft);

	machine.addTransition(StartJump, stateIdle, stateStartJump);
	machine.addTransition(StartJump, stateRight, stateStartJump);
	machine.addTransition(StartJump, stateLeft, stateStartJump);
	machine.addTransition(StartJump, stateStartJump, stateStartJump);
	machine.addTransition(StartJump, stateDoubleJump, stateStartJump);
	machine.addTransition(StartJump, stateFall, stateStartJump);
	machine.addTransition(StartJump, stateDieFall, stateStartJump);
	machine.addTransition(StartJump, stateWait, stateStartJump);
	machine.addTransition(StartJump, stateStartSlowFall, stateStartJump);
	machine.addTransition(StartJump, stateDeath, stateStartJump);
	machine.addTransition(StartJump, stateDrink, stateStartJump);
	machine.addTransition(StartJump, stateStartElevator, stateStartJump);
	machine.addTransition(StartJump, stateElevator, stateStartJump);
	machine.addTransition(StartJump, stateSlowFall1, stateStartJump);
	machine.addTransition(StartJump, stateSlowFall2, stateStartJump);
	machine.addTransition(StartJump, stateSlowFall3, stateStartJump);
	machine.addTransition(StartJump, stateStartWaterJump, stateStartJump);
	machine.addTransition(StartJump, stateWaterJump, stateStartJump);
	machine.addTransition(StartJump, statePortalEvent, stateStartJump);
	machine.addTransition(StartJump, stateKonamiCode, stateStartJump);

	machine.addTransition(DoubleJump, stateIdle, stateDoubleJump);
	machine.addTransition(DoubleJump, stateRight, stateDoubleJump);
	machine.addTransition(DoubleJump, stateLeft, stateDoubleJump);
	machine.addTransition(DoubleJump, stateStartJump, stateDoubleJump);
	machine.addTransition(DoubleJump, stateDoubleJump, stateDoubleJump);
	machine.addTransition(DoubleJump, stateFall, stateDoubleJump);
	machine.addTransition(DoubleJump, stateDieFall, stateDoubleJump);
	machine.addTransition(DoubleJump, stateWait, stateDoubleJump);
	machine.addTransition(DoubleJump, stateStartSlowFall, stateDoubleJump);
	machine.addTransition(DoubleJump, stateDeath, stateDoubleJump);
	machine.addTransition(DoubleJump, stateDrink, stateDoubleJump);
	machine.addTransition(DoubleJump, stateStartElevator, stateDoubleJump);
	machine.addTransition(DoubleJump, stateElevator, stateDoubleJump);
	machine.addTransition(DoubleJump, stateSlowFall1, stateDoubleJump);
	machine.addTransition(DoubleJump, stateSlowFall2, stateDoubleJump);
	machine.addTransition(DoubleJump, stateSlowFall3, stateDoubleJump);
	machine.addTransition(DoubleJump, stateStartWaterJump, stateDoubleJump);
	machine.addTransition(DoubleJump, stateWaterJump, stateDoubleJump);
	machine.addTransition(DoubleJump, statePortalEvent, stateDoubleJump);
	machine.addTransition(DoubleJump, stateKonamiCode, stateDoubleJump);

	machine.addTransition(Fall, stateIdle, stateFall);
	machine.addTransition(Fall, stateRight, stateFall);
	machine.addTransition(Fall, stateLeft, stateFall);
	machine.addTransition(Fall, stateStartJump, stateFall);
	machine.addTransition(Fall, stateDoubleJump, stateFall);
	machine.addTransition(Fall, stateFall, stateFall);
	machine.addTransition(Fall, stateDieFall, stateFall);
	machine.addTransition(Fall, stateWait, stateFall);
	machine.addTransition(Fall, stateStartSlowFall, stateFall);
	machine.addTransition(Fall, stateDeath, stateFall);
	machine.addTransition(Fall, stateDrink, stateFall);
	machine.addTransition(Fall, stateStartElevator, stateFall);
	machine.addTransition(Fall, stateElevator, stateFall);
	machine.addTransition(Fall, stateSlowFall1, stateFall);
	machine.addTransition(Fall, stateSlowFall2, stateFall);
	machine.addTransition(Fall, stateSlowFall3, stateFall);
	machine.addTransition(Fall, stateStartWaterJump, stateFall);
	machine.addTransition(Fall, stateWaterJump, stateFall);
	machine.addTransition(Fall, statePortalEvent, stateFall);
	machine.addTransition(Fall, stateKonamiCode, stateFall);

	machine.addTransition(DieFall, stateIdle, stateDieFall);
	machine.addTransition(DieFall, stateRight, stateDieFall);
	machine.addTransition(DieFall, stateLeft, stateDieFall);
	machine.addTransition(DieFall, stateStartJump, stateDieFall);
	machine.addTransition(DieFall, stateDoubleJump, stateDieFall);
	machine.addTransition(DieFall, stateFall, stateDieFall);
	machine.addTransition(DieFall, stateDieFall, stateDieFall);
	machine.addTransition(DieFall, stateWait, stateDieFall);
	machine.addTransition(DieFall, stateStartSlowFall, stateDieFall);
	machine.addTransition(DieFall, stateDeath, stateDieFall);
	machine.addTransition(DieFall, stateDrink, stateDieFall);
	machine.addTransition(DieFall, stateStartElevator, stateDieFall);
	machine.addTransition(DieFall, stateElevator, stateDieFall);
	machine.addTransition(DieFall, stateSlowFall1, stateDieFall);
	machine.addTransition(DieFall, stateSlowFall2, stateDieFall);
	machine.addTransition(DieFall, stateSlowFall3, stateDieFall);
	machine.addTransition(DieFall, stateStartWaterJump, stateDieFall);
	machine.addTransition(DieFall, stateWaterJump, stateDieFall);
	machine.addTransition(DieFall, statePortalEvent, stateDieFall);
	machine.addTransition(DieFall, stateKonamiCode, stateDieFall);

	machine.addTransition(Wait, stateIdle, stateWait);
	machine.addTransition(Wait, stateRight, stateWait);
	machine.addTransition(Wait, stateLeft, stateWait);
	machine.addTransition(Wait, stateStartJump, stateWait);
	machine.addTransition(Wait, stateDoubleJump, stateWait);
	machine.addTransition(Wait, stateFall, stateWait);
	machine.addTransition(Wait, stateDieFall, stateWait);
	machine.addTransition(Wait, stateWait, stateWait);
	machine.addTransition(Wait, stateStartSlowFall, stateWait);
	machine.addTransition(Wait, stateDeath, stateWait);
	machine.addTransition(Wait, stateDrink, stateWait);
	machine.addTransition(Wait, stateStartElevator, stateWait);
	machine.addTransition(Wait, stateElevator, stateWait);
	machine.addTransition(Wait, stateSlowFall1, stateWait);
	machine.addTransition(Wait, stateSlowFall2, stateWait);
	machine.addTransition(Wait, stateSlowFall3, stateWait);
	machine.addTransition(Wait, stateStartWaterJump, stateWait);
	machine.addTransition(Wait, stateWaterJump, stateWait);
	machine.addTransition(Wait, statePortalEvent, stateWait);
	machine.addTransition(Wait, stateKonamiCode, stateWait);

	machine.addTransition(StartSlowFall, stateIdle, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateRight, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateLeft, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateStartJump, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateDoubleJump, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateFall, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateDieFall, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateWait, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateStartSlowFall, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateDeath, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateDrink, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateStartElevator, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateElevator, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateSlowFall1, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateSlowFall2, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateSlowFall3, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateStartWaterJump, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateWaterJump, stateStartSlowFall);
	machine.addTransition(StartSlowFall, statePortalEvent, stateStartSlowFall);
	machine.addTransition(StartSlowFall, stateKonamiCode, stateStartSlowFall);

	machine.addTransition(Death, stateIdle, stateDeath);
	machine.addTransition(Death, stateRight, stateDeath);
	machine.addTransition(Death, stateLeft, stateDeath);
	machine.addTransition(Death, stateStartJump, stateDeath);
	machine.addTransition(Death, stateDoubleJump, stateDeath);
	machine.addTransition(Death, stateFall, stateDeath);
	machine.addTransition(Death, stateDieFall, stateDeath);
	machine.addTransition(Death, stateWait, stateDeath);
	machine.addTransition(Death, stateStartSlowFall, stateDeath);
	machine.addTransition(Death, stateDeath, stateDeath);
	machine.addTransition(Death, stateDrink, stateDeath);
	machine.addTransition(Death, stateStartElevator, stateDeath);
	machine.addTransition(Death, stateElevator, stateDeath);
	machine.addTransition(Death, stateSlowFall1, stateDeath);
	machine.addTransition(Death, stateSlowFall2, stateDeath);
	machine.addTransition(Death, stateSlowFall3, stateDeath);
	machine.addTransition(Death, stateStartWaterJump, stateDeath);
	machine.addTransition(Death, stateWaterJump, stateDeath);
	machine.addTransition(Death, statePortalEvent, stateDeath);
	machine.addTransition(Death, stateKonamiCode, stateDeath);

	machine.addTransition(Drink, stateIdle, stateDrink);
	machine.addTransition(Drink, stateRight, stateDrink);
	machine.addTransition(Drink, stateLeft, stateDrink);
	machine.addTransition(Drink, stateStartJump, stateDrink);
	machine.addTransition(Drink, stateDoubleJump, stateDrink);
	machine.addTransition(Drink, stateFall, stateDrink);
	machine.addTransition(Drink, stateDieFall, stateDrink);
	machine.addTransition(Drink, stateWait, stateDrink);
	machine.addTransition(Drink, stateStartSlowFall, stateDrink);
	machine.addTransition(Drink, stateDeath, stateDrink);
	machine.addTransition(Drink, stateDrink, stateDrink);
	machine.addTransition(Drink, stateStartElevator, stateDrink);
	machine.addTransition(Drink, stateElevator, stateDrink);
	machine.addTransition(Drink, stateSlowFall1, stateDrink);
	machine.addTransition(Drink, stateSlowFall2, stateDrink);
	machine.addTransition(Drink, stateSlowFall3, stateDrink);
	machine.addTransition(Drink, stateStartWaterJump, stateDrink);
	machine.addTransition(Drink, stateWaterJump, stateDrink);
	machine.addTransition(Drink, statePortalEvent, stateDrink);
	machine.addTransition(Drink, stateKonamiCode, stateDrink);

	machine.addTransition(StartElevator, stateIdle, stateStartElevator);
	machine.addTransition(StartElevator, stateRight, stateStartElevator);
	machine.addTransition(StartElevator, stateLeft, stateStartElevator);
	machine.addTransition(StartElevator, stateStartJump, stateStartElevator);
	machine.addTransition(StartElevator, stateDoubleJump, stateStartElevator);
	machine.addTransition(StartElevator, stateFall, stateStartElevator);
	machine.addTransition(StartElevator, stateDieFall, stateStartElevator);
	machine.addTransition(StartElevator, stateWait, stateStartElevator);
	machine.addTransition(StartElevator, stateStartSlowFall, stateStartElevator);
	machine.addTransition(StartElevator, stateDeath, stateStartElevator);
	machine.addTransition(StartElevator, stateDrink, stateStartElevator);
	machine.addTransition(StartElevator, stateStartElevator, stateStartElevator);
	machine.addTransition(StartElevator, stateElevator, stateStartElevator);
	machine.addTransition(StartElevator, stateSlowFall1, stateStartElevator);
	machine.addTransition(StartElevator, stateSlowFall2, stateStartElevator);
	machine.addTransition(StartElevator, stateSlowFall3, stateStartElevator);
	machine.addTransition(StartElevator, stateStartWaterJump, stateStartElevator);
	machine.addTransition(StartElevator, stateWaterJump, stateStartElevator);
	machine.addTransition(StartElevator, statePortalEvent, stateStartElevator);
	machine.addTransition(StartElevator, stateKonamiCode, stateStartElevator);

	machine.addTransition(Elevator, stateIdle, stateElevator);
	machine.addTransition(Elevator, stateRight, stateElevator);
	machine.addTransition(Elevator, stateLeft, stateElevator);
	machine.addTransition(Elevator, stateStartJump, stateElevator);
	machine.addTransition(Elevator, stateDoubleJump, stateElevator);
	machine.addTransition(Elevator, stateFall, stateElevator);
	machine.addTransition(Elevator, stateDieFall, stateElevator);
	machine.addTransition(Elevator, stateWait, stateElevator);
	machine.addTransition(Elevator, stateStartSlowFall, stateElevator);
	machine.addTransition(Elevator, stateDeath, stateElevator);
	machine.addTransition(Elevator, stateDrink, stateElevator);
	machine.addTransition(Elevator, stateStartElevator, stateElevator);
	machine.addTransition(Elevator, stateElevator, stateElevator);
	machine.addTransition(Elevator, stateSlowFall1, stateElevator);
	machine.addTransition(Elevator, stateSlowFall2, stateElevator);
	machine.addTransition(Elevator, stateSlowFall3, stateElevator);
	machine.addTransition(Elevator, stateStartWaterJump, stateElevator);
	machine.addTransition(Elevator, stateWaterJump, stateElevator);
	machine.addTransition(Elevator, statePortalEvent, stateElevator);
	machine.addTransition(Elevator, stateKonamiCode, stateElevator);

	machine.addTransition(SlowFall1, stateIdle, stateSlowFall1);
	machine.addTransition(SlowFall1, stateRight, stateSlowFall1);
	machine.addTransition(SlowFall1, stateLeft, stateSlowFall1);
	machine.addTransition(SlowFall1, stateStartJump, stateSlowFall1);
	machine.addTransition(SlowFall1, stateDoubleJump, stateSlowFall1);
	machine.addTransition(SlowFall1, stateFall, stateSlowFall1);
	machine.addTransition(SlowFall1, stateDieFall, stateSlowFall1);
	machine.addTransition(SlowFall1, stateWait, stateSlowFall1);
	machine.addTransition(SlowFall1, stateStartSlowFall, stateSlowFall1);
	machine.addTransition(SlowFall1, stateDeath, stateSlowFall1);
	machine.addTransition(SlowFall1, stateDrink, stateSlowFall1);
	machine.addTransition(SlowFall1, stateStartElevator, stateSlowFall1);
	machine.addTransition(SlowFall1, stateElevator, stateSlowFall1);
	machine.addTransition(SlowFall1, stateSlowFall1, stateSlowFall1);
	machine.addTransition(SlowFall1, stateSlowFall2, stateSlowFall1);
	machine.addTransition(SlowFall1, stateSlowFall3, stateSlowFall1);
	machine.addTransition(SlowFall1, stateStartWaterJump, stateSlowFall1);
	machine.addTransition(SlowFall1, stateWaterJump, stateSlowFall1);
	machine.addTransition(SlowFall1, statePortalEvent, stateSlowFall1);
	machine.addTransition(SlowFall1, stateKonamiCode, stateSlowFall1);

	machine.addTransition(SlowFall2, stateIdle, stateSlowFall2);
	machine.addTransition(SlowFall2, stateRight, stateSlowFall2);
	machine.addTransition(SlowFall2, stateLeft, stateSlowFall2);
	machine.addTransition(SlowFall2, stateStartJump, stateSlowFall2);
	machine.addTransition(SlowFall2, stateDoubleJump, stateSlowFall2);
	machine.addTransition(SlowFall2, stateFall, stateSlowFall2);
	machine.addTransition(SlowFall2, stateDieFall, stateSlowFall2);
	machine.addTransition(SlowFall2, stateWait, stateSlowFall2);
	machine.addTransition(SlowFall2, stateStartSlowFall, stateSlowFall2);
	machine.addTransition(SlowFall2, stateDeath, stateSlowFall2);
	machine.addTransition(SlowFall2, stateDrink, stateSlowFall2);
	machine.addTransition(SlowFall2, stateStartElevator, stateSlowFall2);
	machine.addTransition(SlowFall2, stateElevator, stateSlowFall2);
	machine.addTransition(SlowFall2, stateSlowFall1, stateSlowFall2);
	machine.addTransition(SlowFall2, stateSlowFall2, stateSlowFall2);
	machine.addTransition(SlowFall2, stateSlowFall3, stateSlowFall2);
	machine.addTransition(SlowFall2, stateStartWaterJump, stateSlowFall2);
	machine.addTransition(SlowFall2, stateWaterJump, stateSlowFall2);
	machine.addTransition(SlowFall2, statePortalEvent, stateSlowFall2);
	machine.addTransition(SlowFall2, stateKonamiCode, stateSlowFall2);

	machine.addTransition(SlowFall3, stateIdle, stateSlowFall3);
	machine.addTransition(SlowFall3, stateRight, stateSlowFall3);
	machine.addTransition(SlowFall3, stateLeft, stateSlowFall3);
	machine.addTransition(SlowFall3, stateStartJump, stateSlowFall3);
	machine.addTransition(SlowFall3, stateDoubleJump, stateSlowFall3);
	machine.addTransition(SlowFall3, stateFall, stateSlowFall3);
	machine.addTransition(SlowFall3, stateDieFall, stateSlowFall3);
	machine.addTransition(SlowFall3, stateWait, stateSlowFall3);
	machine.addTransition(SlowFall3, stateStartSlowFall, stateSlowFall3);
	machine.addTransition(SlowFall3, stateDeath, stateSlowFall3);
	machine.addTransition(SlowFall3, stateDrink, stateSlowFall3);
	machine.addTransition(SlowFall3, stateStartElevator, stateSlowFall3);
	machine.addTransition(SlowFall3, stateElevator, stateSlowFall3);
	machine.addTransition(SlowFall3, stateSlowFall1, stateSlowFall3);
	machine.addTransition(SlowFall3, stateSlowFall2, stateSlowFall3);
	machine.addTransition(SlowFall3, stateSlowFall3, stateSlowFall3);
	machine.addTransition(SlowFall3, stateStartWaterJump, stateSlowFall3);
	machine.addTransition(SlowFall3, stateWaterJump, stateSlowFall3);
	machine.addTransition(SlowFall3, statePortalEvent, stateSlowFall3);
	machine.addTransition(SlowFall3, stateKonamiCode, stateSlowFall3);

	machine.addTransition(StartWaterJump, stateIdle, stateStartWaterJump);
	machine.addTransition(StartWaterJump, stateRight, stateStartWaterJump);
	machine.addTransition(StartWaterJump, stateLeft, stateStartWaterJump);
	machine.addTransition(StartWaterJump, stateStartJump, stateStartWaterJump);
	machine.addTransition(StartWaterJump, stateDoubleJump, stateStartWaterJump);
	machine.addTransition(StartWaterJump, stateFall, stateStartWaterJump);
	machine.addTransition(StartWaterJump, stateDieFall, stateStartWaterJump);
	machine.addTransition(StartWaterJump, stateWait, stateStartWaterJump);
	machine.addTransition(StartWaterJump, stateStartSlowFall, stateStartWaterJump);
	machine.addTransition(StartWaterJump, stateDeath, stateStartWaterJump);
	machine.addTransition(StartWaterJump, stateDrink, stateStartWaterJump);
	machine.addTransition(StartWaterJump, stateStartElevator, stateStartWaterJump);
	machine.addTransition(StartWaterJump, stateElevator, stateStartWaterJump);
	machine.addTransition(StartWaterJump, stateSlowFall1, stateStartWaterJump);
	machine.addTransition(StartWaterJump, stateSlowFall2, stateStartWaterJump);
	machine.addTransition(StartWaterJump, stateSlowFall3, stateStartWaterJump);
	machine.addTransition(StartWaterJump, stateStartWaterJump, stateStartWaterJump);
	machine.addTransition(StartWaterJump, stateWaterJump, stateStartWaterJump);
	machine.addTransition(StartWaterJump, statePortalEvent, stateStartWaterJump);
	machine.addTransition(StartWaterJump, stateKonamiCode, stateStartWaterJump);

	machine.addTransition(WaterJump, stateIdle, stateWaterJump);
	machine.addTransition(WaterJump, stateRight, stateWaterJump);
	machine.addTransition(WaterJump, stateLeft, stateWaterJump);
	machine.addTransition(WaterJump, stateStartJump, stateWaterJump);
	machine.addTransition(WaterJump, stateDoubleJump, stateWaterJump);
	machine.addTransition(WaterJump, stateFall, stateWaterJump);
	machine.addTransition(WaterJump, stateDieFall, stateWaterJump);
	machine.addTransition(WaterJump, stateWait, stateWaterJump);
	machine.addTransition(WaterJump, stateStartSlowFall, stateWaterJump);
	machine.addTransition(WaterJump, stateDeath, stateWaterJump);
	machine.addTransition(WaterJump, stateDrink, stateWaterJump);
	machine.addTransition(WaterJump, stateStartElevator, stateWaterJump);
	machine.addTransition(WaterJump, stateElevator, stateWaterJump);
	machine.addTransition(WaterJump, stateSlowFall1, stateWaterJump);
	machine.addTransition(WaterJump, stateSlowFall2, stateWaterJump);
	machine.addTransition(WaterJump, stateSlowFall3, stateWaterJump);
	machine.addTransition(WaterJump, stateStartWaterJump, stateWaterJump);
	machine.addTransition(WaterJump, stateWaterJump, stateWaterJump);
	machine.addTransition(WaterJump, statePortalEvent, stateWaterJump);
	machine.addTransition(WaterJump, stateKonamiCode, stateWaterJump);

	machine.addTransition(PortalEvent, stateIdle, statePortalEvent);
	machine.addTransition(PortalEvent, stateRight, statePortalEvent);
	machine.addTransition(PortalEvent, stateLeft, statePortalEvent);
	machine.addTransition(PortalEvent, stateStartJump, statePortalEvent);
	machine.addTransition(PortalEvent, stateDoubleJump, statePortalEvent);
	machine.addTransition(PortalEvent, stateFall, statePortalEvent);
	machine.addTransition(PortalEvent, stateDieFall, statePortalEvent);
	machine.addTransition(PortalEvent, stateWait, statePortalEvent);
	machine.addTransition(PortalEvent, stateStartSlowFall, statePortalEvent);
	machine.addTransition(PortalEvent, stateDeath, statePortalEvent);
	machine.addTransition(PortalEvent, stateDrink, statePortalEvent);
	machine.addTransition(PortalEvent, stateStartElevator, statePortalEvent);
	machine.addTransition(PortalEvent, stateElevator, statePortalEvent);
	machine.addTransition(PortalEvent, stateSlowFall1, statePortalEvent);
	machine.addTransition(PortalEvent, stateSlowFall2, statePortalEvent);
	machine.addTransition(PortalEvent, stateSlowFall3, statePortalEvent);
	machine.addTransition(PortalEvent, stateStartWaterJump, statePortalEvent);
	machine.addTransition(PortalEvent, stateWaterJump, statePortalEvent);
	machine.addTransition(PortalEvent, statePortalEvent, statePortalEvent);
	machine.addTransition(PortalEvent, stateKonamiCode, statePortalEvent);

	machine.addTransition(KonamiCode, stateIdle, stateKonamiCode);
	machine.addTransition(KonamiCode, stateRight, stateKonamiCode);
	machine.addTransition(KonamiCode, stateLeft, stateKonamiCode);
	machine.addTransition(KonamiCode, stateStartJump, stateKonamiCode);
	machine.addTransition(KonamiCode, stateDoubleJump, stateKonamiCode);
	machine.addTransition(KonamiCode, stateFall, stateKonamiCode);
	machine.addTransition(KonamiCode, stateDieFall, stateKonamiCode);
	machine.addTransition(KonamiCode, stateWait, stateKonamiCode);
	machine.addTransition(KonamiCode, stateStartSlowFall, stateKonamiCode);
	machine.addTransition(KonamiCode, stateDeath, stateKonamiCode);
	machine.addTransition(KonamiCode, stateDrink, stateKonamiCode);
	machine.addTransition(KonamiCode, stateStartElevator, stateKonamiCode);
	machine.addTransition(KonamiCode, stateElevator, stateKonamiCode);
	machine.addTransition(KonamiCode, stateSlowFall1, stateKonamiCode);
	machine.addTransition(KonamiCode, stateSlowFall2, stateKonamiCode);
	machine.addTransition(KonamiCode, stateSlowFall3, stateKonamiCode);
	machine.addTransition(KonamiCode, stateStartWaterJump, stateKonamiCode);
	machine.addTransition(KonamiCode, stateWaterJump, stateKonamiCode);
	machine.addTransition(KonamiCode, statePortalEvent, stateKonamiCode);
	machine.addTransition(KonamiCode, stateKonamiCode, stateKonamiCode);

	m_sprite.setMachine(machine);
}

void	CharacterOcto::update(sf::Time frameTime, sf::Time realFrameTime)
{
	Progress & progress = Progress::getInstance();

	if (progress.getCurrentDestination() == Level::EndRocket)
		updateAIEnd(frameTime);
	else if (progress.isMenu())
		updateAI(frameTime);

	updateBox(frameTime);
	dieGrass();
	updateGroundDelay(frameTime);
	portalEvent();
	commitPhysicsToGraphics();
	m_sprite.update(frameTime);
	resetTimeEvent();
	timeEvent(frameTime);
	inWater();
	if (m_sprite.getCurrentEvent() != PortalEvent && m_sprite.getCurrentEvent() != KonamiCode && m_sprite.getCurrentEvent() != Drink && endDeath())
	{
		wait();
		dieFall();
		collisionElevatorUpdate();
		collisionTileUpdate();
		commitControlsToPhysics(frameTime.asSeconds());
		commitEnvironmentToPhysics();
		commitEventToGraphics();
	}
	else
		m_helmetParticle.update(frameTime);
	m_sound->update(frameTime, static_cast<Events>(m_sprite.getCurrentEvent()), m_inWater, m_onGround);
	if ((m_sprite.getCurrentEvent() == Drink || m_sprite.getCurrentEvent() == KonamiCode) && m_sprite.isTerminated())
	{
		m_box->setApplyGravity(true);
		if (m_onGround)
			m_sprite.setNextEvent(Idle);
		else
			m_sprite.setNextEvent(Fall);
	}

	updateNanoRobots(frameTime);
	updateDoorAction(frameTime);
	updateParticles(frameTime);
	resetColisionBolean();
	replaceOcto();
	updateCutscene(realFrameTime);

	m_previousTop = m_box->getGlobalBounds().top;
	m_prevEvent = static_cast<Events>(m_sprite.getCurrentEvent());
	progress.setOctoPos(getPosition());
}

void	CharacterOcto::replaceOcto(void)
{
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
}

void	CharacterOcto::updateCutscene(sf::Time frameTime)
{
	if (m_enableCutscene)
	{
		m_cutsceneTimer += frameTime;
		if (m_cutsceneTimer > m_cutsceneTimerMax)
		{
			m_cutsceneTimer = m_cutsceneTimerMax;
			m_cutscenePauseTimer += frameTime;
			if (m_autoDisableCutscene && m_cutscenePauseTimer > m_cutscenePauseTimerMax)
				m_enableCutscene = false;
		}
		m_cutsceneShader.setParameter("time", m_cutsceneTimer / m_cutsceneTimerMax);
		if (m_progress.getCurrentDestination() != Level::EndRocket)
			MusicManager::getInstance().startEvent();
	}
	else
	{
		m_cutsceneTimer -= frameTime;
		if (m_cutsceneTimer <= sf::Time::Zero)
			m_cutsceneTimer = sf::Time::Zero;
		m_cutsceneShader.setParameter("time", m_cutsceneTimer / m_cutsceneTimerMax);
		if (m_cutsceneTimer <= sf::Time::Zero)
			PostEffectLayer::getInstance().enableShader(CUTSCENE_FRAG, false);
		MusicManager::getInstance().endEvent();
	}
}

bool	CharacterOcto::isZooming(void) const
{
	return (m_keyZoomIn || (isCollidingPortal() && Progress::getInstance().getCurrentDestination() != Level::Portal));
}

bool	CharacterOcto::isInRocketEnd(void) const
{
	return m_isRocketEnd;
}

bool	CharacterOcto::isInWater(void) const
{
	return (getPosition().y > m_waterLevel);
}

bool	CharacterOcto::isCenteredCamera(void) const
{
	Progress const & progress = Progress::getInstance();

	if (progress.getCurrentDestination() == Level::DesertC || progress.getCurrentDestination() == Level::JungleC)
		return true;
	if (m_repairShip && progress.getCurrentDestination() == Level::EndRocket)
		return true;
	if (isInWater() && m_waterLevel != -1.f)
		return true;
	return false;
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
			//if (collisionDirection.x == 0.f && collisionDirection.y <= 0.f)
			//TODO  : Keep an eye on octo behavior when touching the ground
			if (collisionDirection.y < 0.f)
				m_collisionTile = true;
			if (collisionDirection.y > 0.f)
				m_collisionTileHead = true;
			else
				m_collisionTileHead = false;
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
	if (m_progress.canRepair() && m_keyElevator)
	{
		if (!elevator.isActivated())
		{
			elevator.activate();
			m_repairNanoRobot->setState(NanoRobot::State::Repair);
			m_repairNanoRobot->setTarget(elevator.getRepairPosition());
		}
		else
		{
			elevator.setSmokeVelocity((m_repairNanoRobot->getPosition() - elevator.getPosition()) * 0.8f);
			m_repairNanoRobot->setState(NanoRobot::State::FollowOcto);
		}
	}
	else if (m_progress.canRepair())
		m_repairNanoRobot->setState(NanoRobot::State::FollowOcto);
	if (!elevator.isActivated())
		m_collisionElevatorEvent = true;
}

void	CharacterOcto::collideSpaceShip(SpaceShip * spaceShip)
{
	if (m_progress.canRepairShip() && m_keyElevator)
	{
		for (auto & robot : m_nanoRobots)
		{
			if (robot->getState() == NanoRobot::State::GoingToRepairShip || robot->getState() == NanoRobot::State::RepairShip)
				continue;
			robot->setState(NanoRobot::State::GoingToRepairShip);
			robot->setTarget(spaceShip->getPosition());
		}
		m_repairShip = true;
		spaceShip->setRepairProgression(m_timeRepairSpaceShip / m_timeRepairSpaceShipMax);
	}
	m_collisionSpaceShip = true;
}

void	CharacterOcto::collideDoor(void)
{
	if (m_keyEntrance)
	{
		m_doorAction = true;
	}
	else
		m_doorAction = false;
}

void	CharacterOcto::usePortal(Portal & portal)
{
	octo::Camera&				camera = octo::Application::getCamera();
	sf::Vector2f const&			cameraPos = sf::Vector2f(camera.getRectangle().left, camera.getRectangle().top);

	m_collisionPortal = true;
	if (m_sprite.getCurrentEvent() == PortalEvent && m_sprite.isTerminated())
	{
		if (portal.getDestination() == m_progress.getCurrentDestination())
		{
			if (portal.getKey() == OBJECT_PORTAL_SNOW_OSS)
				m_box->setPosition(sf::Vector2f(20430.f, 3255.f));
			else if (portal.getKey() == OBJECT_PORTAL_DESERT_OSS)
				m_box->setPosition(sf::Vector2f(20480.f, 2570.f));
			else if (portal.getKey() == OBJECT_PORTAL_JUNGLE_OSS)
				m_box->setPosition(sf::Vector2f(20245.f, 1190.f));
			else if (portal.getKey() == OBJECT_PORTAL_BEACH_OSS)
				m_box->setPosition(sf::Vector2f(19936.f, 35.f));
			m_numberOfJump = 0u;
			m_timeSlowFall = sf::Time::Zero;
		}
		else
		{
			m_progress.setOctoPosTransition(m_sprite.getPosition() + m_sprite.getGlobalSize() - cameraPos);
			m_progress.setReverseSprite(m_originMove);
			m_progress.setNextDestination(portal.getDestination());
			if (!m_progress.isMenu())
				m_progress.setRespawnType(Progress::RespawnType::Portal);
		}
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
			if (m_box->getGlobalBounds().top > m_previousTop && m_jumpVelocity != m_pixelSecondJump)
			{
				m_afterJump = true;
				m_afterJumpVelocity = m_pixelSecondAfterFullJump;
				if (m_keyCapacity && m_progress.canSlowFall())
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
			if (m_keyCapacity && m_progress.canSlowFall() && m_timeSlowFall < m_timeSlowFallMax)
				m_sprite.setNextEvent(StartSlowFall);
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
		if (!m_onGround && m_sprite.getCurrentEvent() != DoubleJump && m_sprite.getCurrentEvent() != StartJump)
			m_numberOfJump = 1;

		if (m_keyElevator)
		{
			if (!m_useElevator && m_progress.canUseElevator())
				m_sprite.setNextEvent(StartElevator);
			if (m_sprite.getCurrentEvent() == StartElevator || m_sprite.getCurrentEvent() == Elevator)
			{
				if (!m_useElevator)
				{
					m_onTopElevator = false;
					m_useElevator = true;
					m_box->setApplyGravity(false);
				}
				if (m_sprite.isTerminated())
					m_sprite.setNextEvent(Elevator);
			}
		}
		else
		{
			Events	event = static_cast<Events>(m_sprite.getCurrentEvent());

			if (event == DieFall || event == Elevator || event == StartElevator)
				m_sprite.setNextEvent(Fall);
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
			if (m_sprite.getCurrentEvent() != DoubleJump && m_sprite.getCurrentEvent() != StartJump)
				m_sprite.setNextEvent(Fall);
			m_useElevator = false;
			m_box->setApplyGravity(true);
		}
	}
}

void	CharacterOcto::updateBox(sf::Time frameTime)
{
	//TODO: Check on use if it's a good improvement
	Events event = static_cast<Events>(m_sprite.getCurrentEvent());

	if (event == StartSlowFall || event == SlowFall1 || event == SlowFall2 || event == SlowFall3)
		m_adaptBoxTimer = std::min(m_adaptBoxTimer + frameTime, m_adaptBoxTimerMax);
	else
	{
		if (!m_onGround)
			m_adaptBoxTimer = std::max(m_adaptBoxTimer - frameTime * 2.f, sf::Time::Zero);
		else
			m_adaptBoxTimer = std::max(m_adaptBoxTimer - frameTime * 10.f, sf::Time::Zero);
	}

	m_adaptBoxDelta = 35.f * (m_adaptBoxTimer / m_adaptBoxTimerMax);
	m_box->setSize(sf::Vector2f(m_boxSize.x, m_boxSize.y - m_adaptBoxDelta));
}

void	CharacterOcto::updateGroundDelay(sf::Time frameTime)
{
	if (m_onGround)
	{
		m_lastPositionOnGround = getPosition();
		m_onGroundDelay = m_onGroundDelayMax;
		m_timeInAir = sf::Time::Zero;
	}
	else
		m_timeInAir += frameTime;
	m_onGroundDelay -= frameTime;
}

void	CharacterOcto::updateDoorAction(sf::Time frameTime)
{
	octo::AudioManager& audio = octo::Application::getAudioManager();

	if (m_doorAction)
	{
		m_timerStartUseDoor -= frameTime;
		m_timerStartUseDoor = std::max(m_timerStartUseDoor, sf::Time::Zero);
		sf::Color c = m_sprite.getColor();
		c.a = m_timerStartUseDoor / m_timerStartUseDoorMax * 255.f;
		m_sprite.setColor(c);
	}
	else
	{
		m_timerStartUseDoor += frameTime;
		m_timerStartUseDoor = std::min(m_timerStartUseDoor, m_timerStartUseDoorMax);
		sf::Color c = m_sprite.getColor();
		c.a = m_timerStartUseDoor / m_timerStartUseDoorMax * 255.f;
		m_sprite.setColor(c);
	}

	m_soundUseDoor->setVolume((1.f - m_timerStartUseDoor / m_timerStartUseDoorMax) * audio.getSoundVolume());
}

void	CharacterOcto::updateNanoRobots(sf::Time frameTime)
{
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
			if (m_progress.getCurrentDestination() != Level::EndRocket)
				states.change("zero");
		}
	}

	for (auto & robot : m_nanoRobots)
	{
		robot->update(frameTime);
		robot->setPosition(m_box->getPosition() + sf::Vector2f(20.f, 0.f));
	}

	updateOctoEvent();
}

void	CharacterOcto::updateOctoEvent(void)
{
	std::string	nanoRobot = "";
	float		value = 0.f;

	Events const & currentEvent = static_cast<Events>(m_sprite.getCurrentEvent());

	if (m_keyGroundRight || m_keyGroundLeft)
		nanoRobot = NANO_GROUND_TRANSFORM_OSS;

	switch (currentEvent)
	{
		case StartJump:
			nanoRobot = NANO_JUMP_OSS;
			break;
		case DoubleJump:
			nanoRobot = NANO_DOUBLE_JUMP_OSS;
			break;
		case StartSlowFall:
		case SlowFall1:
		case SlowFall2:
		case SlowFall3:
			nanoRobot = NANO_SLOW_FALL_OSS;
			value = m_timeSlowFall / m_timeSlowFallMax;
			break;
		case StartWaterJump:
		case WaterJump:
			nanoRobot = NANO_JUMP_WATER_OSS;
			break;
		default:
			break;
	}

	if (m_collisionElevatorEvent)
		nanoRobot = NANO_REPAIR_OSS;

	for (auto & robot : m_nanoRobots)
		robot->updateOctoEvent(nanoRobot, value);
}

void	CharacterOcto::updateParticles(sf::Time frameTime)
{
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
}

void	CharacterOcto::resetColisionBolean()
{
	m_collisionTile = false;
	m_collisionElevator = false;
	m_collisionPortal = false;
	m_collidePortalEvent = false;
	m_collisionElevatorEvent = false;
	m_collisionSpaceShip = false;
	m_doorAction = false;
}

void	CharacterOcto::kill()
{
	if (Progress::getInstance().getNextDestination() != Level::Rewards)
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
	else
		m_sprite.setNextEvent(Idle);
}

void	CharacterOcto::dieFall()
{
	if (m_timeEventFall > m_timeEventDieFallMax && m_sprite.getCurrentEvent() != DieFall)
		m_sprite.setNextEvent(DieFall);
	else if (m_timeEventDieVoidMax != sf::Time::Zero)
	{
		if (m_timeEventFall > m_timeEventDieVoidMax)
		{
			float const speedOutOfScreen = m_timeEventFall.asSeconds() - m_timeEventDieVoidMax.asSeconds();

			stopFollowCamera(true);
			m_box->setPosition(m_box->getPosition() + sf::Vector2f(0.f, speedOutOfScreen * 40.f));
			m_sprite.setOrigin(m_sprite.getOrigin() - sf::Vector2f(0.f, speedOutOfScreen * 10.f));
			if (m_timeEventFall > m_timeEventDieVoidMax + sf::seconds(0.6f))
				kill();
		}
		else
			stopFollowCamera(false);
	}

	if (m_sprite.getCurrentEvent() == DieFall && m_onGround && !m_inWater)
		kill();
}

bool	CharacterOcto::dieGrass()
{
	Progress & progress = Progress::getInstance();

	if (progress.getKillOcto())
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

void	CharacterOcto::wait()
{
	typedef octo::CharacterAnimation::Frame			Frame;
	Events	event = static_cast<Events>(m_sprite.getCurrentEvent());

	if (m_timeEventIdle > m_timeEventIdleMax && event == Idle)
	{
		m_timeEventIdleMax = sf::seconds(m_generator.randomFloat(3.f, 10.f));

		switch (m_generator.randomInt(0, 2))
		{
			case 0:
			{
				m_waitAnimation.setFrames({
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
				m_waitAnimation.setLoop(octo::LoopMode::NoLoop);
				break;
			}
			case 1:
			{
				m_waitAnimation.setFrames({
						Frame(sf::seconds(0.2f), {77, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.2f), {78, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.2f), {79, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.2f), {80, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.2f), {81, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.2f), {82, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.2f), {84, sf::FloatRect(), sf::Vector2f()}),
						});
				m_waitAnimation.setLoop(octo::LoopMode::NoLoop);
				break;
			}
			case 2:
			{
				m_waitAnimation.setFrames({
						Frame(sf::seconds(0.4f), {7, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.4f), {8, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.4f), {9, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.4f), {10, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.4f), {11, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.4f), {12, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.4f), {13, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.4f), {14, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.4f), {15, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.4f), {16, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.4f), {17, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.4f), {18, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.4f), {19, sf::FloatRect(), sf::Vector2f()}),
						Frame(sf::seconds(0.4f), {20, sf::FloatRect(), sf::Vector2f()}),
						});
				m_waitAnimation.setLoop(octo::LoopMode::NoLoop);
				break;
			}
		}
		m_sprite.setNextEvent(Wait);
	}

	if (event == Wait && m_sprite.isTerminated())
		m_sprite.setNextEvent(Idle);
}

void	CharacterOcto::inWater()
{
	Progress & progress = Progress::getInstance();
	bool	emit = false;

	if (progress.isInCloud() || (m_waterLevel != -1.f && m_box->getBaryCenter().y + m_box->getSize().y / 2.f > m_waterLevel))
	{
		if (!m_inWater)
		{
			m_numberOfJump = 0;
			emit = true;
			m_inWater = true;
			m_timeSlowFall = sf::Time::Zero;
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
	if (emit && !progress.isInCloud() && m_waterLevel != -1.f)
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
	if (m_sprite.getCurrentEvent() != PortalEvent && m_sprite.getCurrentEvent() != KonamiCode && m_sprite.getCurrentEvent() != Drink)
	{
		sf::Vector2f const&	pos = m_box->getRenderCenter();
		float				xPos = pos.x - ((m_sprite.getLocalSize().x  * m_spriteScale) / 2.f);
		float				yPos =  pos.y - ((m_sprite.getLocalSize().y * m_spriteScale) - (m_box->getSize().y / 2.f));
	
		m_sprite.setPosition(sf::Vector2f(xPos, yPos + m_deltaPositionY + m_adaptBoxDelta));
		m_eventBox->setPosition(pos.x - m_eventBox->getRadius(), pos.y - m_eventBox->getRadius());
	}
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

	if (m_collisionPortal && m_keyPortal)
	{
		m_box->setApplyGravity(false);
		m_sprite.setNextEvent(PortalEvent);
	}
}

void	CharacterOcto::commitControlsToPhysics(float frametime)
{
	sf::Vector2f	velocity = m_box->getVelocity();
	Events			event = static_cast<Events>(m_sprite.getCurrentEvent());

	if (m_progress.canWalk())
	{
		if (m_keyLeft || m_keyRight)
		{
			if (m_keyLeft)
				m_factorDirectionVelocityX = -1.f;
			if (m_keyRight)
				m_factorDirectionVelocityX = 1.f;

			velocity.x = m_factorDirectionVelocityX * m_pixelSecondWalk;
			m_timeStopVelocity = sf::Time::Zero;
		}
		else if (m_timeStopVelocity <= m_timeStopVelocityMax)
		{
			if (!m_onGround)
				velocity.x = m_pixelSecondWalk * m_factorDirectionVelocityX * (1.f - (m_timeStopVelocity / m_timeStopVelocityMax));
			m_timeStopVelocity += sf::seconds(frametime);
		}
	}

	if (m_keyJump && m_numberOfJump < 3 && (event == DoubleJump || event == StartJump))
	{
		velocity.y = m_jumpVelocity;
		m_jumpVelocity += m_pixelSecondMultiplier * frametime;
	}
	else if (m_afterJump && m_afterJumpVelocity < 0.f && !m_useElevator)
	{
		velocity.y = m_afterJumpVelocity;
		m_afterJumpVelocity += m_pixelSecondMultiplier * frametime;
	}

	if (m_keyCapacity)
	{
		if (event == StartSlowFall)
		{
			velocity.x *= 1.3f;
			velocity.y = m_pixelSecondSlowFall;
		}
		else if (event == SlowFall1 || event == SlowFall2 || event == SlowFall3)
		{
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

	if (!m_onTopElevator && m_keyElevator && !m_collisionTileHead)
	{
		if (event == StartElevator)
			velocity.y = (1.2f * m_pixelSecondSlowFall);
		else if (event == Elevator)
			velocity.y = (2.5f * m_pixelSecondSlowFall);
	}

	if (m_collisionElevator && (event == Fall || event == DieFall))
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

void CharacterOcto::moveGround(sf::Time frameTime, std::unique_ptr<GroundManager> & groundManager)
{
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	float						volume = 0.f;

	if (m_soundGeneration != nullptr && m_groundSoundTime > sf::Time::Zero && !Progress::getInstance().isMapMoving())
		m_groundSoundTime -= frameTime;
	else if (m_groundSoundTime < m_groundSoundTimeMax && !Progress::getInstance().isMenu())
		m_groundSoundTime += frameTime;

	if ((m_keyGroundRight || m_keyGroundLeft || m_progress.forceMapToMove()) && m_progress.canMoveMap())
	{
		if (m_progress.forceMapToMove())
			groundManager->setNextGenerationState(GroundManager::GenerationState::Next, getPosition());
		if (m_keyGroundLeft == true && m_progress.canOctoMoveMap())
			groundManager->setNextGenerationState(GroundManager::GenerationState::Next, getPosition());
		else if (m_keyGroundRight == true && m_progress.canOctoMoveMap())
			groundManager->setNextGenerationState(GroundManager::GenerationState::Previous, getPosition());
	}

	volume = m_groundVolume * (m_groundSoundTime / m_groundSoundTimeMax);
	m_soundGeneration->setVolume(volume * audio.getSoundVolume());

	if (m_progress.isMenu() ||
		(ChallengeManager::getInstance().getEffect(ChallengeManager::Effect::Duplicate).enable() && !m_progress.isValidateChallenge(ChallengeManager::Effect::Duplicate)) ||
		(ChallengeManager::getInstance().getEffect(ChallengeManager::Effect::Displacement).enable() && !m_progress.isValidateChallenge(ChallengeManager::Effect::Displacement)) ||
		(m_progress.getCurrentDestination() == Level::Blue || m_progress.getCurrentDestination() == Level::Red)
		)
		groundManager->setNextGenerationState(GroundManager::GenerationState::Previous, getPosition());
}

void	CharacterOcto::caseJump()
{
	if (!m_keyJump)
	{
		randomJumpAnimation();
		m_keyJump = true;
		m_box->setApplyGravity(true);
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

void CharacterOcto::caseCapacity()
{
	if (!m_keyCapacity)
	{
		m_keyCapacity = true;
		if (m_inWater && m_progress.canUseWaterJump() && !Progress::getInstance().isInCloud())
		{
			m_jumpVelocity = m_pixelSecondJump * 0.9f;
			m_sprite.setNextEvent(StartWaterJump);
		}
		else if (!m_onGround && !m_inWater && m_progress.canSlowFall() && m_timeSlowFall < m_timeSlowFallMax)
			m_sprite.setNextEvent(StartSlowFall);
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
		case OctoKeys::GroundLeft:
		{
			if (m_keyGroundLeft == false)
			{
				m_progress.moveMap();
				m_keyGroundLeft = true;
			}
			break;
		}
		case OctoKeys::GroundRight:
		{
			if (m_keyGroundRight == false)
			{
				m_progress.moveMap();
				m_keyGroundRight = true;
			}
			break;
		}
		case OctoKeys::Jump:
			caseJump();
			break;
		case OctoKeys::Capacity:
			caseCapacity();
			break;
		case OctoKeys::Down:
			m_keyDown = true;
			break;
		case OctoKeys::Zoom:
			m_keyZoomIn = true;
			break;
		case OctoKeys::Elevator:
			m_keyElevator = true;
			break;
		case OctoKeys::Entrance:
			m_keyEntrance = true;
			m_keyPortal = true;
			break;
		default:
			break;
	}
	return (true);
}

bool	CharacterOcto::isMovingLeft(void)
{
	return m_originMove;
}

bool	CharacterOcto::isFalling(void)
{
	Events	state = static_cast<Events>(m_sprite.getCurrentEvent());
	sf::Time limit;
	if (m_numberOfJump == 2u)
		limit = sf::seconds(1.9f);
	else
		limit = sf::seconds(1.1f);

	if ((state == Fall && (m_lastPositionOnGround.y < getPosition().y || m_timeInAir > limit)) || state == DieFall || state == SlowFall1 || state == SlowFall2 || state == SlowFall3)
		return true;
	return false;
}

bool	CharacterOcto::isRaising(void)
{
	Events	state = static_cast<Events>(m_sprite.getCurrentEvent());

	if (state == WaterJump || state == Elevator || state == PortalEvent)
		return true;
	return false;
}

bool	CharacterOcto::isInAir(void) const
{
	return (!m_onGround);
}

bool	CharacterOcto::isMeetingNpc(void) const
{
	return m_meetNpc;
}

bool	CharacterOcto::isCollidingPortal(void) const
{
	return m_collidePortalEvent;
}

bool	CharacterOcto::isStopFollowCamera(void) const
{
	return m_stopFollowCamera;
}

void	CharacterOcto::stopFollowCamera(bool stop)
{
	m_stopFollowCamera = stop;
}

void	CharacterOcto::setOctoInRocketEnd(void)
{
	m_isRocketEnd = true;
	m_box->setApplyGravity(false);
}

void	CharacterOcto::endInRocket(void)
{
	stopFollowCamera(true);
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

void	CharacterOcto::collidePortalEvent(bool collidePortal)
{
	m_collidePortalEvent = collidePortal;
}

void	CharacterOcto::enableCutscene(bool enable, bool autoDisable)
{
	if (enable && !m_enableCutscene)
		PostEffectLayer::getInstance().enableShader(CUTSCENE_FRAG, true);
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
		case OctoKeys::GroundLeft:
			m_keyGroundLeft = false;
			break;
		case OctoKeys::GroundRight:
			m_keyGroundRight = false;
			break;
		case OctoKeys::Jump:
			m_keyJump = false;
			if (state == DoubleJump || state == StartJump)
			{
				m_afterJump = true;
				m_afterJumpVelocity = m_pixelSecondAfterJump;
			}
			break;
		case OctoKeys::Capacity:
			m_keyCapacity = false;
			if (state == WaterJump || state == SlowFall1 || state == SlowFall2 || state == SlowFall3)
			{
				m_afterJump = true;
				m_afterJumpVelocity = m_pixelSecondAfterJump;
			}
			break;
		case OctoKeys::Down:
			m_keyDown = false;
			break;
		case OctoKeys::Elevator:
			m_keyElevator = false;
			break;
		case OctoKeys::Entrance:
			m_keyPortal = false;
			m_keyEntrance = false;
			break;
		case OctoKeys::Zoom:
			m_keyZoomIn = false;
			break;
		default:
			otherKeyReleased = true;
			break;
	}
	if (state == Death || state == PortalEvent || state == KonamiCode || state == Drink || otherKeyReleased)
		return true;
	if (!m_onGround && !m_keyCapacity && !m_keyElevator)
	{
		if (state != Fall && state != DieFall && state != StartJump && state != DoubleJump)
			m_sprite.setNextEvent(Fall);
	}

	if (m_onGround && !m_keyLeft && !m_keyRight && !m_keyCapacity && state != Wait)
		m_sprite.setNextEvent(Idle);

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
	return (m_box->getRenderCenter() + sf::Vector2f(-40.f, -80.f));
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
	if (Progress::getInstance().isMenu())
	{
		InputListener::removeInputListener();
	
		m_randomJumpTimer = sf::seconds(m_generator.randomFloat(1.f, 30.f));
		m_doubleJumpTimer = sf::seconds(m_generator.randomFloat(1.5f, 3.5f));
		m_directionTimer = sf::seconds(m_generator.randomFloat(30.f, 300.f));
		m_slowFallTimer = sf::seconds(m_generator.randomFloat(4.f, 10.f));
		m_portalTimer = sf::seconds(m_generator.randomFloat(45.f, 90.f));
		m_keyRight = true;
		m_keyLeft = false;
	}
}

void	CharacterOcto::initAIEnd(void)
{
	if (Progress::getInstance().getCurrentDestination() == Level::EndRocket)
	{
//		InputListener::removeInputListener();
	
		m_repairShip = true;
		enableCutscene(true, false);
		m_goLeftTimer = sf::seconds(6.f);
	}
}

void	CharacterOcto::updateAIEnd(sf::Time frameTime)
{
	if (m_repairShip)
	{
		if (m_timeRepairSpaceShip <= m_timeRepairSpaceShipMax)
		{
			m_timeEventIdleMax = sf::Time::Zero;
			wait();
			m_keyElevator = true;
		}
		else
		{
			for (auto & robot : m_nanoRobots)
				robot->setState(NanoRobot::State::FollowOcto);
			m_repairShip = false;
			m_keyElevator = false;
			m_keyLeft = true;
		}
	}
	else
	{
		m_goLeftTimer -= frameTime;
		if (m_goLeftTimer <= sf::Time::Zero)
			m_keyLeft = true;
		else
			m_keyLeft = false;
	}
}

void	CharacterOcto::updateAI(sf::Time frameTime)
{
	//Jump if blocked
	m_jumpTimer -= frameTime;
	if (m_jumpTimer <= sf::Time::Zero && m_numberOfJump == 0)
	{
		m_keyJump = false;
		if (std::round(m_saveOctoPos.x) == std::round(getPosition().x))
		{
			m_doubleJumpTimer = sf::seconds(m_generator.randomFloat(1.5f, 3.5f));
			caseJump();
		}
		m_saveOctoPos = getPosition();
	}

	//Random jump
	m_randomJumpTimer -= frameTime;
	if (m_randomJumpTimer <= sf::Time::Zero)
	{
		m_randomJumpTimer = sf::seconds(m_generator.randomFloat(1.f, 30.f));
		m_doubleJumpTimer = sf::seconds(m_generator.randomFloat(1.5f, 3.5f));
		caseJump();
	}

	//Double jump
	if (m_numberOfJump == 1)
	{
		m_doubleJumpTimer -= frameTime;
		if (m_doubleJumpTimer <= sf::Time::Zero)
		{
			m_keyJump = false;
			caseJump();
		}
	}

	//SlowFall
	m_slowFallTimer -= frameTime;
	if (m_slowFallTimer <= sf::Time::Zero)
	{
		m_slowFallTimer = sf::seconds(m_generator.randomFloat(4.f, 10.f));
		if (m_generator.randomBool(0.5f))
			caseCapacity();
		else
			m_keyCapacity = false;
	}

	//Portal
	m_portalTimer -= frameTime;
	if (m_portalTimer <= sf::Time::Zero)
	{
		m_keyEntrance = true;
		m_keyPortal = true;
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
