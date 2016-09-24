#ifndef BEACHBOYSUBNPC_HPP
# define BEACHBOYSUBNPC_HPP

# include "ASwimNpc.hpp"

class BeachBoySubNpc : public ASwimNpc
{
public:
	BeachBoySubNpc(void);
	virtual ~BeachBoySubNpc(void) = default;

	virtual void setup(void);

};

#endif
