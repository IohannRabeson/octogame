#ifndef REPAIRSHIPNANOROBOT_HPP
# define REPAIRSHIPNANOROBOT_HPP

# include "NanoRobot.hpp"

class RepairShipNanoRobot : public NanoRobot, public AGameObject<GameObjectType::RepairShipNanoRobot>
{
public:
	RepairShipNanoRobot(void);
	virtual ~RepairShipNanoRobot(void) = default;

private:

};

#endif
