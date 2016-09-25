#ifndef FISHBLACKNPC_HPP
# define FISHBLACKNPC_HPP

# include "ASwimNpc.hpp"

class FishBlackNpc : public ASwimNpc
{
public:
	FishBlackNpc(void);
	virtual ~FishBlackNpc(void) = default;

	virtual void setup(void);

};

#endif
