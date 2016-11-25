#ifndef CHARACTEROCTO_HPP
# define CHARACTEROCTO_HPP

# include <CharacterSprite.hpp>
# include <CharacterAnimation.hpp>
# include <DefaultGraphicsListeners.hpp>
# include <AudioManager.hpp>

# include "AGameObject.hpp"
# include "ABiome.hpp"
# include "SmokeSystem.hpp"
# include "HelmetSystem.hpp"
# include "PloufSystem.hpp"
# include "WaterDropSystem.hpp"
# include "InputListener.hpp"
# include "RandomGenerator.hpp"

# include <SFML/Graphics/Drawable.hpp>
# include <array>

class ElevatorStream;
class RepairNanoRobot;
class SpaceShip;
class RectangleShape;
class CircleShape;
class TileShape;
class Progress;
class NanoRobot;
class ABiome;
class Portal;
class GroundManager;
namespace sf
{
	class Shader;
}

class CharacterOcto : public AGameObject<GameObjectType::Player>,
	public InputListener,
	public sf::Drawable
{
public:
	friend class OctoEvent;

	class OctoEvent : public AGameObject<GameObjectType::PlayerEvent>
	{
	public:
		CharacterOcto *	m_octo;
	};

	enum Events
	{
		Idle,
		Right,
		Left,
		StartJump,
		DoubleJump,
		Fall,
		DieFall,
		Wait,
		StartSlowFall,
		Death,
		Drink,
		StartElevator,
		Elevator,
		SlowFall1,
		SlowFall2,
		SlowFall3,
		StartWaterJump,
		WaterJump,
		PortalEvent,
		KonamiCode
	};

	CharacterOcto();
	virtual ~CharacterOcto();

	void					setup(ABiome & biome);
	void					update(sf::Time frameTime, sf::Time realFrameTime);
	void					initAI(void);
	void					initAIEnd(void);
	void					updateAI(sf::Time frameTime);
	void					updateAIEnd(sf::Time frameTime);
	void					draw(sf::RenderTarget& render, sf::RenderStates states = sf::RenderStates())const;
	void					drawNanoRobot(sf::RenderTarget& render, sf::RenderStates states)const;
	void					drawText(sf::RenderTarget& render, sf::RenderStates states)const;

	bool					onInputPressed(InputListener::OctoKeys const & key);
	bool					onInputReleased(InputListener::OctoKeys const & key);
	void					onCollision(TileShape * tileshape, GameObjectType type, sf::Vector2f const& collisionDirection);
	void					setTopElevator(float top);
	sf::Vector2f const &	getPhysicsPosition() const;
	sf::Vector2f const &	getPosition() const;
	sf::Vector2f			getBubblePosition() const;
	bool					getDoubleJump(void);
	float					getWaterLevel(void) const;
	void					setStartPosition(sf::Vector2f const & position);
	void					giveNanoRobot(NanoRobot * robot, bool firstTime = false);
	void					giveSpirit(NanoRobot * robot, bool firstTime = false);
	void					giveRepairNanoRobot(RepairNanoRobot * robot, bool firstTime = false);
	void					repairElevator(ElevatorStream & elevator);
	void					collideSpaceShip(SpaceShip * spaceShip);
	void					collideDoor(void);
	void					usePortal(Portal & portal);
	void					startKonamiCode(bool canStart);
	void					startDrinkPotion(void);
	bool					isMovingLeft(void);
	bool					isFalling(void);
	bool					isRaising(void);
	bool					isInAir(void) const;
	bool					isOnGround(void) const;
	bool					isMeetingNpc(void) const;
	bool					isCollidingPortal(void) const;
	bool					isStopFollowCamera(void) const;
	std::pair<float, float>	look(void) const;
	float					getSpeedCamera(void) const;
	bool					isFinalEvent(void);
	bool					isZooming(void) const;
	bool					isInRocketEnd(void) const;
	bool					isInWater(void) const;
	bool					isCenteredCamera(void) const;
	void					stopFollowCamera(bool stop);
	void					setOctoInRocketEnd(void);
	void					endInRocket(void);
	void					meetNpc(bool meetNpc);
	void					collidePortalEvent(bool collidePortal);
	void					resetCollidingTileCount(void);
	void					enableCutscene(bool enable, bool autoDisable = false);
	void					moveGround(sf::Time frameTime, std::unique_ptr<GroundManager> & groundManager);

private:
	void					startGameIceA(void);
	void					updateBox(sf::Time frameTime);
	void					updateGroundDelay(sf::Time frameTime);
	void					updateDoorAction(sf::Time frameTime);
	void					updatePortalVacuum(sf::Time frameTime);
	void					updateNanoRobots(sf::Time frameTime);
	void					updateOctoEvent(void);
	void					updateParticles(sf::Time frameTime);
	void					resetColisionBolean();
	void					dieFall();
	bool					dieGrass();
	void					kill();
	bool					endDeath();
	void					portalEvent();
	void					wait();
	void					inWater();
	void					randomJumpAnimation();
	void					timeEvent(sf::Time frameTime);
	void					resetTimeEvent();
	void					setupAnimation();
	void					setupMachine();
	void					collisionTileUpdate();
	void					onSky(Events event);
	void					collisionElevatorUpdate();
	void					commitControlsToPhysics(float frametime);
	void					commitEnvironmentToPhysics();
	void					commitPhysicsToGraphics();
	void					commitEventToGraphics();
	void					caseLeft();
	void					caseRight();
	void					caseJump();
	void					caseCapacity();

private:
	class OctoSound;

	void replaceOcto(void);
	void updateCutscene(sf::Time frameTime);

	Level										m_level;
	octo::CharacterSprite						m_sprite;
	octo::CharacterAnimation					m_idleAnimation;
	octo::CharacterAnimation					m_walkAnimation;
	octo::CharacterAnimation					m_startJumpAnimation;
	octo::CharacterAnimation					m_jumpAnimation;
	octo::CharacterAnimation					m_fallAnimation;
	octo::CharacterAnimation					m_dieFallAnimation;
	octo::CharacterAnimation					m_waitAnimation;
	octo::CharacterAnimation					m_answerWolfAnimation;
	octo::CharacterAnimation					m_startSlowFallAnimation;
	octo::CharacterAnimation					m_slowFallAnimation1;
	octo::CharacterAnimation					m_slowFallAnimation2;
	octo::CharacterAnimation					m_slowFallAnimation3;
	octo::CharacterAnimation					m_deathAnimation;
	octo::CharacterAnimation					m_drinkAnimation;
	octo::CharacterAnimation					m_startElevatorAnimation;
	octo::CharacterAnimation					m_elevatorAnimation;
	octo::CharacterAnimation					m_portalAnimation;
	octo::CharacterAnimation					m_konamiCodeAnimation;

	std::unique_ptr<OctoSound>					m_sound;
	RectangleShape *							m_box;
	sf::Vector2f								m_boxSize;
	CircleShape *								m_eventBox;
	OctoEvent									m_octoEvent;
	std::vector<std::unique_ptr<NanoRobot>>		m_nanoRobots;
	std::vector<std::unique_ptr<NanoRobot>>		m_spirits;
	RepairNanoRobot *							m_repairNanoRobot;
	Progress &									m_progress;

	SmokeSystem									m_inkParticle;
	HelmetSystem								m_helmetParticle;
	sf::Color									m_waterColor;
	sf::Color									m_secondWaterColor;
	PloufSystem									m_ploufParticle;
	WaterDropSystem								m_waterParticle;
	SmokeSystem									m_bubbleParticle;
	sf::Time									m_timeInAir;
	sf::Time									m_timeEventFall;
	sf::Time									m_timeEventDieFallMax;
	sf::Time									m_timeEventDieVoidMax;
	sf::Time									m_timeEventIdle;
	sf::Time									m_timeEventIdleMax;
	sf::Time									m_timeEventDeath;
	sf::Time									m_timeEventInk;
	sf::Time									m_timeRepairSpaceShip;
	sf::Time									m_timeRepairSpaceShipMax;
	sf::Time									m_timeSlowFall;
	sf::Time									m_timeSlowFallMax;
	sf::Time									m_timeStopVelocity;
	sf::Time									m_timeStopVelocityMax;
	sf::Time									m_timerPortalVacuum;
	sf::Time									m_timerPortalVacuumMax;
	sf::Vector2f								m_portalCenterPos;
	sf::Time									m_timerStartUseDoor;
	sf::Time									m_timerStartUseDoorMax;
	std::shared_ptr<sf::Sound>					m_soundUseDoor;
	float										m_factorDirectionVelocityX;
	float										m_spriteScale;
	float										m_maxJumpWaterVelocity;
	float										m_pixelSecondJump;
	float										m_pixelSecondSlowFall;
	float										m_pixelSecondWalk;
	float										m_pixelSecondAfterJump;
	float										m_pixelSecondAfterFullJump;
	float										m_pixelSecondMultiplier;
	float										m_jumpVelocity;
	float										m_afterJumpVelocity;
	float										m_previousTop;
	float										m_topElevator;
	float										m_deltaPositionY;
	float										m_waterLevel;
	std::size_t									m_numberOfJump;
	bool										m_originMove;
	bool										m_onGround;
	sf::Time									m_onGroundDelay;
	sf::Time									m_onGroundDelayMax;
	sf::Vector2f								m_lastPositionOnGround;
	std::shared_ptr<sf::Sound>					m_soundGeneration;
	float										m_groundVolume;
	sf::Time									m_groundSoundTime;
	sf::Time									m_groundSoundTimeMax;
	bool										m_onElevator;
	bool										m_useElevator;
	bool										m_onTopElevator;
	bool										m_afterJump;
	bool										m_keyLeft;
	bool										m_keyRight;
	bool										m_keyGroundRight;
	bool										m_keyGroundLeft;
	bool										m_keyJump;
	bool										m_keyCapacity;
	bool										m_keyDown;
	bool										m_keyEntrance;
	bool										m_keyPortal;
	bool										m_keyElevator;
	bool										m_keyZoomIn;
	bool										m_jumpTic;
	bool										m_capacityTic;
	bool										m_collisionTile;
	bool										m_collisionTileHead;
	bool										m_collisionElevator;
	bool										m_collisionPortal;
	bool										m_collisionElevatorEvent;
	bool										m_collisionSpaceShip;
	bool										m_repairShip;
	bool										m_inWater;
	bool										m_isDeadlyWater;
	bool										m_meetNpc;
	bool										m_collidePortalEvent;
	bool										m_replaceOcto;
	bool										m_enableCutscene;
	bool										m_stopFollowCamera;
	std::pair<float, float>						m_lookCamera;
	float										m_speedCamera;
	bool										m_autoDisableCutscene;
	bool										m_doorAction;
	bool										m_isRocketEnd;
	Events										m_prevEvent;
	bool										m_isAI;

	// AI Menu
	RandomGenerator								m_generator;
	sf::Time									m_directionTimer;
	sf::Time									m_jumpTimer;
	sf::Time									m_randomJumpTimer;
	sf::Time									m_doubleJumpTimer;
	sf::Time									m_slowFallTimer;
	sf::Time									m_portalTimer;

	// AI End Rocket
	enum EndRocketState
	{
		RepairShip,
		SpeakNano,
		LookLeft,
		WaitCedricSpeak,
		GoLeft,
		StopRight,
		GoRocket,
		InRocket,
		RocketTakeOff,
		None
	};

	EndRocketState								m_endRocketState;
	sf::Time									m_speakNanoTimer;
	sf::Time									m_lookLeftTimer;
	sf::Time									m_speakCedricTimer;
	sf::Time									m_goLeftTimer;
	sf::Time									m_inRocketTimer;
	sf::Time									m_stopRightTimer;
	sf::Time									m_rocketTakeOffTimer;

	sf::Time									m_cutsceneTimer;
	sf::Time									m_cutsceneTimerMax;
	sf::Time									m_cutscenePauseTimer;
	sf::Time									m_cutscenePauseTimerMax;
	sf::Time									m_adaptBoxTimer;
	sf::Time									m_adaptBoxTimerMax;
	float										m_adaptBoxDelta;
	sf::Vector2f								m_saveOctoPos;
	sf::Vector2f								m_highestPosition;
	std::vector<sf::Vector2f>					m_collidingTile;
	sf::Shader &								m_cutsceneShader;

};

#endif
