#ifndef NANOROBOT_HPP
# define NANOROBOT_HPP

# include <AnimatedSprite.hpp>

# include "AGameObject.hpp"
# include "FireflySwarm.hpp"
# include "FireflyPopulation.hpp"
# include "FireflyPositionBehaviors.hpp"
# include "IPlaceable.hpp"
# include "BubbleText.hpp"
# include "NanoEffect.hpp"

class CircleShape;

class NanoRobot : public IPlaceable
{
public:
	enum State
	{
		Idle,
		Speak,
		FollowOcto
	};

	virtual ~NanoRobot(void);

	void addMapOffset(float x, float y);
	void transfertToOcto(void);
	void setPosition(sf::Vector2f const & position);
	void setState(NanoRobot::State state);
	sf::Vector2f const & getPosition(void) const;
	sf::Vector2f const & getTargetPosition(void);
	NanoRobot::State getState(void) const;
	bool isTravelling(void) const;

	virtual void update(sf::Time frameTime);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;

protected:
	NanoRobot(sf::Vector2f const & position, std::string id, std::size_t nbFrames, int seed);
	void setup(AGameObjectBase * gameObject);

private:
	FireflySwarm							m_swarm;
	FireflySwarm::UniformPopulation			m_uniformPopulation;
	FireflySwarm::SpawnMode					m_spawnMode;
	FireflySwarm::CirclePositionBehavior *	m_positionBehavior;

	octo::AnimatedSprite					m_sprite;
	octo::SpriteAnimation					m_animation;

	CircleShape *							m_box;
	std::unique_ptr<BubbleText>				m_text;

	State									m_state;
	sf::Time								m_timer;
	sf::Time								m_timerMax;

	bool									m_isTravelling;
	NanoEffect								m_glowingEffect;
};

#endif
