#ifndef JUMPNANOROBOT_HPP
# define JUMPNANOROBOT_HPP

# include "NanoRobot.hpp"

class JumpNanoRobot : public NanoRobot, public AGameObject<GameObjectType::JumpNanoRobot>
{
public:
	JumpNanoRobot(void);
	virtual ~JumpNanoRobot(void) = default;

private:

};

#endif
