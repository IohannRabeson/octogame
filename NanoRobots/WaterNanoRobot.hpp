#ifndef WATERNANOROBOT_HPP
# define WATERNANOROBOT_HPP

# include "NanoRobot.hpp"

class WaterNanoRobot : public NanoRobot, public AGameObject<GameObjectType::WaterNanoRobot>
{
public:
	WaterNanoRobot(void);
	virtual ~WaterNanoRobot(void) = default;

};

#endif
