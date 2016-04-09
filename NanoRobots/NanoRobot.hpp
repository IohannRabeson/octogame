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

	void setTarget(sf::Vector2f const & target);
	void setPosition(sf::Vector2f const & position);
	void setState(NanoRobot::State state);
	void setTextIndex(std::size_t index);
	sf::Vector2f const & getPosition(void) const;
	sf::Vector2f const & getTargetPosition(void);
	NanoRobot::State getState(void) const;
	bool isTravelling(void) const;

	bool onInputPressed(InputListener::OctoKeys const & key);
	bool onInputReleased(InputListener::OctoKeys const & key);

	void addMapOffset(float x, float y);
	void transfertToOcto(bool inInit = false);
	virtual void update(sf::Time frameTime);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;
	virtual void drawText(sf::RenderTarget & render, sf::RenderStates states) const;

protected:
	NanoRobot(sf::Vector2f const & position, std::string const & id, std::size_t nbFrames, int seed, sf::Vector2f const & offsetLaser, float multiplier = 0.f);

	std::unique_ptr<BubbleText> const & getCurrentText(void) const { return m_texts[m_textIndex]; }
	void setInfoText(std::wstring const & infoText) { m_infoText = infoText; }

	void setup(AGameObjectBase * gameObject);
	void setUsePathLaser(bool usePathLaser);
	void setLaserColor(sf::Color const & color);
	void setRepairShipPosition(sf::Vector2f const & position);
	void setTargets(std::vector<sf::Vector2f> const & targets, float travelDuration);
	void setSwarmTarget(sf::Vector2f const & position);
	void makeLaser(sf::Vertex* vertices, sf::Vector2f const& p0, sf::Vector2f const& p1, float thickness);
	void playSoundRepair(void);
	virtual void updateRepairShip(sf::Time frameTime);

	bool isReparingShip(void) const;
	static void setLaserConvergence(sf::Vector2f const & position);
	static sf::Vector2f const & getLaserConvergence(void);

private:
	FireflySwarm								m_swarm;
	FireflySwarm::UniformPopulation				m_uniformPopulation;
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
	bool										m_infoSetup;

	State										m_state;
	sf::Time									m_timer;
	sf::Time									m_timerMax;

	bool										m_isTravelling;

	NanoEffect									m_glowingEffect;
	std::mt19937								m_engine;
	std::uniform_int_distribution<int>			m_soundDistri;
	std::shared_ptr<sf::Sound>					m_sound;
};

#endif
