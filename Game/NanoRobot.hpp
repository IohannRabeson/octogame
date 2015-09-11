#ifndef NANOROBOT_HPP
# define NANOROBOT_HPP

# include "AGameObject.hpp"
# include "FireflySwarm.hpp"
# include "FireflyPopulation.hpp"

class NanoRobot : public AGameObject<GameObjectType::NanoRobot>
{
public:
	NanoRobot(sf::Vector2f const & position);
	virtual ~NanoRobot(void);

	void setPosition(sf::Vector2f const & position);

	void update(sf::Time frameTime);
	void draw(sf::RenderTarget& render) const;

private:
	FireflySwarm							m_swarm;
	FireflySwarm::UniformPopulation			m_uniformPopulation;
	FireflySwarm::SpawnMode					m_spawnMode;

};

#endif
