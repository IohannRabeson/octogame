#ifndef CHARACTEROCTO_HPP
# define CHARACTEROCTO_HPP
# include <CharacterSprite.hpp>
# include <CharacterAnimation.hpp>
# include <DefaultGraphicsListeners.hpp>
# include "AGameObject.hpp"
# include "Progress.hpp"
# include "RectangleShape.hpp"
# include "CircleShape.hpp"
# include "NanoRobot.hpp"
# include "SmokeSystem.hpp"
# include "HelmetSystem.hpp"
# include <SFML/Graphics/Drawable.hpp>
# include <array>

class ElevatorStream;
class RepairNanoRobot;

class CharacterOcto : public AGameObject<GameObjectType::Player>,
	public octo::DefaultKeyboardListener,
	public sf::Drawable
{
	enum Events
	{
		Idle,
		Right,
		Left,
		StartJump,
		Jump,
		DoubleJump = 5,
		Fall,
		Dance,
		DanceWithMusic,
		SlowFall,
		Death,
		Drink,
		StartElevator,
		Elevator = 13,
	};
public:
	friend class OctoEvent;

	class OctoEvent : public AGameObject<GameObjectType::PlayerEvent>
	{
	public:
		CharacterOcto *	m_octo;
	};
	CharacterOcto();
	virtual ~CharacterOcto();

	void					setup(ABiome & biome);
	void					update(sf::Time frameTime);
	void					draw(sf::RenderTarget& render, sf::RenderStates states = sf::RenderStates())const;
	void					drawNanoRobot(sf::RenderTarget& render, sf::RenderStates states)const;

	bool					onPressed(sf::Event::KeyEvent const& event);
	bool					onReleased(sf::Event::KeyEvent const& event);
	void					onCollision(GameObjectType type, sf::Vector2f const& collisionDirection);
	void					setTopElevator(float top);
	sf::Vector2f const &	getPhysicsPosition() const;
	sf::Vector2f const &	getPosition() const;
	sf::Vector2f			getBubblePosition() const;
	void					setPosition(sf::Vector2f const & position);
	void					giveNanoRobot(NanoRobot * robot);
	void					giveRepairNanoRobot(RepairNanoRobot * robot);
	void					repairElevator(ElevatorStream & elevator);
	void					usePortal(Portal & portal);

private:
	bool	dieFall();
	bool	endDeath();
	void	dance();
	void	inWater();
	void	randomJumpAnimation();
	void	timeEvent(sf::Time frameTime);
	void	resetTimeEvent();
	void	setupAnimation();
	void	setupMachine();
	void	collisionTileUpdate();
	void	onSky(Events event);
	void	collisionElevatorUpdate();
	void	commitControlsToPhysics(float frametime);
	void	commitEnvironmentToPhysics();
	void	commitPhysicsToGraphics();
	void	commitEventToGraphics();
	void	caseLeft();
	void	caseRight();
	void	caseSpace();
	void	caseUp();
	void	caseAction();
	void	casePortal();

private:
	class OctoSound;
	std::unique_ptr<OctoSound>	m_sound;
	octo::CharacterSprite		m_sprite;
	octo::CharacterAnimation	m_idleAnimation;
	octo::CharacterAnimation	m_walkAnimation;
	octo::CharacterAnimation	m_startJumpAnimation;
	octo::CharacterAnimation	m_jumpAnimation;
	octo::CharacterAnimation	m_fallAnimation;
	octo::CharacterAnimation	m_danceAnimation;
	octo::CharacterAnimation	m_danceWithMusicAnimation;
	octo::CharacterAnimation	m_answerWolfAnimation;
	octo::CharacterAnimation	m_slowFallAnimation;
	octo::CharacterAnimation	m_deathAnimation;
	octo::CharacterAnimation	m_drinkAnimation;
	octo::CharacterAnimation	m_startElevatorAnimation;
	octo::CharacterAnimation	m_elevatorAnimation;
	RectangleShape *			m_box;
	CircleShape *				m_eventBox;
	OctoEvent					m_octoEvent;
	std::vector<std::unique_ptr<NanoRobot>>		m_nanoRobots;
	RepairNanoRobot *			m_repairNanoRobot;
	Progress &					m_progress;
	std::mt19937				m_engine;
	std::uniform_int_distribution<std::size_t>		m_jumpDistribution;
	std::uniform_real_distribution<float>			m_danceDistribution;

	sf::Time					m_timeEventFall;
	sf::Time					m_timeEventIdle;
	sf::Time					m_timeEventIdleMax;
	sf::Time					m_timeEventDeath;
	sf::Time					m_timeEventInk;
	float						m_spriteScale;
	float						m_pixelSecondJump;
	float						m_pixelSecondSlowFall;
	float						m_pixelSecondWalk;
	float						m_pixelSecondAfterJump;
	float						m_pixelSecondAfterFullJump;
	float						m_pixelSecondMultiplier;
	float						m_jumpVelocity;
	float						m_afterJumpVelocity;
	float						m_previousTop;
	float						m_topElevator;
	float						m_deltaPositionY;
	float						m_waterLevel;
	std::size_t					m_numberOfJump;
	bool						m_originMove;
	bool						m_onGround;
	bool						m_onElevator;
	bool						m_useElevator;
	bool						m_onTopElevator;
	bool						m_afterJump;
	bool						m_keyLeft;
	bool						m_keyRight;
	bool						m_keySpace;
	bool						m_keyUp;
	bool						m_keyAction;
	bool						m_keyPortal;
	bool						m_collisionTile;
	bool						m_collisionElevator;
	bool						m_collisionElevatorEvent;
	bool						m_doScale;
	bool						m_inWater;
	SmokeSystem					m_inkParticle;
	HelmetSystem				m_helmetParticle;
	Events						m_prevEvent;
};

#endif
