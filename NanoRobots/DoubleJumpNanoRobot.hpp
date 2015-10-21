#ifndef DOUBLEJUMPNANOROBOT_HPP
# define DOUBLEJUMPNANOROBOT_HPP

# include "NanoRobot.hpp"

class DoubleJumpNanoRobot : public NanoRobot, public AGameObject<GameObjectType::DoubleJumpNanoRobot>
{
public:
	DoubleJumpNanoRobot(sf::Vector2f const & position);
	virtual ~DoubleJumpNanoRobot(void) = default;

};

#endif
