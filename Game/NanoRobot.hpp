#ifndef NANOROBOT_HPP
# define NANOROBOT_HPP

# include <AnimatedSprite.hpp>

# include "AGameObject.hpp"
# include "FireflySwarm.hpp"
# include "FireflyPopulation.hpp"
# include "IPlaceable.hpp"

class CircleShape;

class NanoRobot : public IPlaceable
{
public:
	virtual ~NanoRobot(void);

	void setPosition(sf::Vector2f const & position);

	void update(sf::Time frameTime);
	void draw(sf::RenderTarget& render, sf::RenderStates states) const;

protected:
	NanoRobot(sf::Vector2f const & position, std::string id, std::size_t nbFrames);
	void setup(AGameObjectBase * gameObject);

private:
	// TODO: add Bubble
	FireflySwarm					m_swarm;
	FireflySwarm::UniformPopulation	m_uniformPopulation;
	FireflySwarm::SpawnMode			m_spawnMode;

	octo::AnimatedSprite			m_sprite;
	octo::SpriteAnimation			m_animation;

	CircleShape *					m_box;

};

#endif
