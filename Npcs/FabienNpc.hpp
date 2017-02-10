#ifndef FABIENNPC_HPP
# define FABIENNPC_HPP

# include "AFlyNpc.hpp"

class FabienNpc : public AFlyNpc
{
public:
	FabienNpc(void);
	virtual ~FabienNpc(void) = default;

	virtual void setup(void);
};

#endif
