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

class CircleShape;

class NanoRobot : public IPlaceable
{
public:
	enum State
	{
		Idle,
		Speak,
		FollowOcto,
		Repair,
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

	void addMapOffset(float x, float y);
	void transfertToOcto(void);
	virtual void update(sf::Time frameTime);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;
	virtual void drawText(sf::RenderTarget & render, sf::RenderStates states) const;

protected:
	NanoRobot(sf::Vector2f const & position, std::string const & id, std::size_t nbFrames, int seed, sf::Vector2f const & offsetLaser);

	std::unique_ptr<BubbleText> const & getCurrentText(void) const { return m_texts[m_textIndex]; }

	void setup(AGameObjectBase * gameObject);
	void setTargets(std::vector<sf::Vector2f> const & targets);
	void playSound(void);
	void makeLaser(sf::Vertex* vertices, sf::Vector2f const& p0, sf::Vector2f const& p1, float thickness);
	void playSoundRepair(void);

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
	sf::Time									m_timerRepair;
	sf::Time									m_timerRepairMax;
	std::size_t									m_repairIndex;

	octo::AnimatedSprite						m_sprite;
	octo::SpriteAnimation						m_animation;

	CircleShape *								m_box;
	std::vector<std::unique_ptr<BubbleText>>	m_texts;
	std::size_t									m_textIndex;

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
