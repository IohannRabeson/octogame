#ifndef GROUNDTRANSFORMNANOROBOT_HPP
# define GROUNDTRANSFORMNANOROBOT_HPP

# include "NanoRobot.hpp"

class GroundTransformNanoRobot : public NanoRobot, public AGameObject<GameObjectType::GroundTransformNanoRobot>
{
public:
	GroundTransformNanoRobot(void);
	virtual ~GroundTransformNanoRobot(void) = default;

private:

};

#endif
