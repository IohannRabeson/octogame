#ifndef FISHPINKNPC_HPP
# define FISHPINKNPC_HPP

# include "ASwimNpc.hpp"

class FishPinkNpc : public ASwimNpc
{
public:
	FishPinkNpc(void);
	virtual ~FishPinkNpc(void) = default;

	virtual void setup(void);

};

#endif
