#ifndef WELLKEEPERNPC_HPP
# define WELLKEEPERNPC_HPP

# include "ANpc.hpp"

class SkyCycle;

class WellKeeperNpc : public ANpc, public AGameObject<GameObjectType::WellKeeperNpc>
{
public:
	WellKeeperNpc(void);
	virtual ~WellKeeperNpc(void) = default;

	virtual void setup(void);
	void stopBalle(void);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

};

#endif
