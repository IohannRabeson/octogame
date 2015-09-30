#ifndef DOUBLEJUMPNANOROBOT_HPP
# define DOUBLEJUMPNANOROBOT_HPP

# include "NanoRobot.hpp"

class DoubleJumpNanoRobot : public NanoRobot, public AGameObject<GameObjectType::DoubleJumpNanoRobot>
{
public:
	DoubleJumpNanoRobot(void);
	virtual ~DoubleJumpNanoRobot(void) = default;

};

#endif
