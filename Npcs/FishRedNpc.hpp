#ifndef FISHREDNPC_HPP
# define FISHREDNPC_HPP

# include "ASwimNpc.hpp"

class FishRedNpc : public ASwimNpc
{
public:
	FishRedNpc(void);
	virtual ~FishRedNpc(void) = default;

	virtual void setup(void);

};

#endif
