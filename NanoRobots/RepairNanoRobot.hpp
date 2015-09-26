#ifndef REPAIRNANOROBOT_HPP
# define REPAIRNANOROBOT_HPP

# include "NanoRobot.hpp"
# include "SparkSystem.hpp"

# include <AudioManager.hpp>

class RepairNanoRobot : public NanoRobot, public AGameObject<GameObjectType::RepairNanoRobot>
{
public:
	enum State
	{
		None,
		Repair,
		Done
	};

	RepairNanoRobot(void);
	virtual ~RepairNanoRobot(void) = default;

	void			setTarget(sf::Vector2f const & target);
	void			setState(State state);

	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget & render, sf::RenderStates states) const;

private:
	void			playSoundRepair();
	std::unique_ptr<sf::Vertex[]>	m_ray;
	sf::Vector2f					m_target;
	sf::Texture const *				m_texture;
	State							m_state;
	SparkSystem						m_particles;
	std::shared_ptr<sf::Sound>		m_sound;

	void makeLaser(sf::Vertex* vertices, sf::Vector2f const& p0, sf::Vector2f const& p1, float thickness);

};

#endif
