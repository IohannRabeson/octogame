#ifndef NANOROBOT_HPP
# define NANOROBOT_HPP

# include <AnimatedSprite.hpp>
# include <AudioManager.hpp>
# include <random>

# include "AGameObject.hpp"
# include "FireflySwarm.hpp"
# include "FireflyPopulation.hpp"
# include "FireflyPositionBehaviors.hpp"
# include "IPlaceable.hpp"
# include "BubbleText.hpp"
# include "NanoEffect.hpp"
# include "SparkSystem.hpp"
# include "InputListener.hpp"

class CircleShape;

class NanoRobot : public IPlaceable, public InputListener
{
public:
	enum State
	{
		Idle,
		Speak,
		FollowOcto,
		Repair,
		GoingToRepairShip,
		RepairShip,
		User
	};

	virtual ~NanoRobot(void);

	std::string const & getId(void);
	void setTarget(sf::Vector2f const & target);
	void setPosition(sf::Vector2f const & position);
	void setState(NanoRobot::State state);
	void setEffectState(NanoEffect::State state);
	void setTextIndex(std::size_t index);
	sf::Vector2f const & getPosition(void) const;
	sf::Vector2f const & getTargetPosition(void);
	bool getEffectEnable(void) const;
	NanoRobot::State getState(void) const;
	bool isTravelling(void) const;

	bool onInputPressed(InputListener::OctoKeys const & key);
	bool onInputReleased(InputListener::OctoKeys const & key);

	void addMapOffset(float x, float y);
	virtual void transfertToOcto(bool inInit = false);
	void updateOctoEvent(std::string const & event, float valueEvent);
	virtual void update(sf::Time frameTime);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;
	virtual void drawText(sf::RenderTarget & render, sf::RenderStates states) const;
	void popUpInfo(bool popUp);

protected:
	NanoRobot(sf::Vector2f const & position, std::string const & id, std::size_t nbFrames, int seed, sf::Vector2f const & offsetLaser, InputListener::OctoKeys key, float multiplier = 0.f);

	std::unique_ptr<BubbleText> const & getCurrentText(void) const { return m_texts[m_textIndex]; }
	void setInfoText(std::wstring const & infoText) { m_infoText = infoText; }
	void updatePopUpInfo(sf::Time frameTime);

	void setup(AGameObjectBase * gameObject);
	void setUsePathLaser(bool usePathLaser);
	void setLaserColor(sf::Color const & color);
	void setRepairShipPosition(sf::Vector2f const & position);
	void setTargets(std::vector<sf::Vector2f> const & targets, float travelDuration);
	void setEffectEnable(bool enable);
	void setSwarmTarget(sf::Vector2f const & position);
	void makeLaser(sf::Vertex* vertices, sf::Vector2f const& p0, sf::Vector2f const& p1, float thickness);
	void playSoundRepair(sf::Time frametime);
	virtual void updateRepairShip(sf::Time frameTime);
	octo::AnimatedSprite & getSprite(void);
	NanoEffect & getNanoEffect(void);

	bool isReparingShip(void) const;
	static void setLaserConvergence(sf::Vector2f const & position);
	static sf::Vector2f const & getLaserConvergence(void);

private:
	sf::Vector2f computeInterestPosition(sf::Vector2f const & position);
	void usingCapacity(sf::Time frameTime);

	std::string									m_id;
	FireflySwarm								m_swarm;
	FireflySwarm::SpawnMode						m_spawnMode;
	FireflySwarm::CirclePositionBehavior *		m_positionBehavior;

	std::unique_ptr<sf::Vertex[]>				m_ray;
	std::vector<sf::Vector2f>					m_targets;
	sf::Vector2f								m_target;
	SparkSystem									m_particles;
	sf::Texture const *							m_texture;
	sf::Vector2f								m_offsetLaser;
	sf::Time									m_timerRepairShip;
	sf::Time									m_timerRepair;
	sf::Time									m_timerRepairMax;
	sf::Time									m_timerUse;
	sf::Time									m_timerUseMax;
	sf::Time									m_timerStartSpirit;
	sf::Time									m_timerStartSpiritMax;
	bool										m_isUsing;
	std::size_t									m_repairIndex;
	sf::Vector2f								m_repairShipPosition;
	bool										m_startLastAnimation;
	bool										m_usePathLaser;
	static sf::Vector2f							m_laserConvergence;
	float										m_multiplier;

	octo::AnimatedSprite						m_sprite;
	octo::SpriteAnimation						m_animation;

	CircleShape *								m_box;
	std::vector<std::unique_ptr<BubbleText>>	m_texts;
	std::size_t									m_textIndex;
	BubbleText									m_infoBubble;
	std::wstring								m_infoText;

	State										m_state;
	bool										m_isSpeaking;
	sf::Time									m_timer;
	sf::Time									m_timerMax;

	bool										m_isTravelling;

	NanoEffect									m_nanoEffect;
	std::mt19937								m_engine;
	std::uniform_int_distribution<int>			m_soundDistri;
	std::shared_ptr<sf::Sound>					m_sound;
	sf::Time									m_soundTimer;
	sf::Time									m_soundTimerMax;
	bool										m_popUp;
	sf::Time									m_popUpTimer;
	sf::Time									m_popUpTimerMax;
	InputListener::OctoKeys const				m_inputKey;
	bool										m_stopSpeakinKeyPress;
	sf::Vector2f								m_lastPos;
};

#endif
