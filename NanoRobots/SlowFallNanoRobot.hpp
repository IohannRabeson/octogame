#ifndef SLOWFALLNANOROBOT_HPP
# define SLOWFALLNANOROBOT_HPP

# include "NanoRobot.hpp"

class SlowFallNanoRobot : public NanoRobot, public AGameObject<GameObjectType::SlowFallNanoRobot>
{
public:
	SlowFallNanoRobot(void);
	virtual ~SlowFallNanoRobot(void) = default;

private:

};

#endif
