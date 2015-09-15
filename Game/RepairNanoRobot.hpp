#ifndef REPAIRNANOROBOT_HPP
# define REPAIRNANOROBOT_HPP

# include "NanoRobot.hpp"

class RepairNanoRobot : public NanoRobot, public AGameObject<GameObjectType::RepairNanoRobot>
{
public:
	RepairNanoRobot(void);
	virtual ~RepairNanoRobot(void) = default;

	void setTarget(sf::Vector2f const & target);

	virtual void update(sf::Time frameTime);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;

private:
	std::unique_ptr<sf::Vertex[]>	m_ray;
	sf::Vector2f					m_target;
	sf::Texture const *				m_texture;

	void makeLaser(sf::Vertex* vertices, sf::Vector2f const& p0, sf::Vector2f const& p1, float thickness);

};

#endif
