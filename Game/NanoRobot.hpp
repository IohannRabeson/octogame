#ifndef NANOROBOT_HPP
# define NANOROBOT_HPP

# include <AnimatedSprite.hpp>

# include "AGameObject.hpp"
# include "FireflySwarm.hpp"
# include "FireflyPopulation.hpp"

//TODO NanoRobo n'héritera pas de game Object, mais les sous classes en hériteront
class NanoRobot : public AGameObject<GameObjectType::NanoRobot>
{
public:
	NanoRobot(sf::Vector2f const & position);
	virtual ~NanoRobot(void);

	void setPosition(sf::Vector2f const & position);

	void update(sf::Time frameTime);
	void draw(sf::RenderTarget& render) const;

private:
	// TODO: add CircleShape
	// TODO: add Bubble
	FireflySwarm					m_swarm;
	FireflySwarm::UniformPopulation	m_uniformPopulation;
	FireflySwarm::SpawnMode			m_spawnMode;

	octo::AnimatedSprite			m_sprite;
	octo::SpriteAnimation			m_animation;

};

#endif
