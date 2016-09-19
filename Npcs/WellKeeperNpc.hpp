#ifndef WELLKEEPERNPC_HPP
# define WELLKEEPERNPC_HPP

# include "AIdleNpc.hpp"

class WellKeeperNpc : public AIdleNpc
{
public:
	WellKeeperNpc(void);
	virtual ~WellKeeperNpc(void) = default;

	virtual void setup(void);

};

#endif
