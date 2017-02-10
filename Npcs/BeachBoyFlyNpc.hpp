#ifndef BEACHBOYFLYNPC_HPP
# define BEACHBOYFLYNPC_HPP

# include "ASpecialNpc.hpp"

class BeachBoyFlyNpc : public ASpecialNpc
{
public:
	BeachBoyFlyNpc(void);
	virtual ~BeachBoyFlyNpc(void) = default;

	virtual void setup(void);

};

#endif
