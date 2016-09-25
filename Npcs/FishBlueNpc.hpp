#ifndef FISHBLUENPC_HPP
# define FISHBLUENPC_HPP

# include "ASwimNpc.hpp"

class FishBlueNpc : public ASwimNpc
{
public:
	FishBlueNpc(void);
	virtual ~FishBlueNpc(void) = default;

	virtual void setup(void);

};

#endif
