#ifndef REPAIRNANOROBOT_HPP
# define REPAIRNANOROBOT_HPP

# include "NanoRobot.hpp"

class RepairNanoRobot : public NanoRobot, public AGameObject<GameObjectType::RepairNanoRobot>
{
public:
	RepairNanoRobot(void);
	virtual ~RepairNanoRobot(void) = default;

	virtual void update(sf::Time frameTime);

private:
	sf::Time	m_timer;
	sf::Time	m_timerMax;

};

#endif
