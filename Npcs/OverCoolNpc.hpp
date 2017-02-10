#ifndef OVERCOOLNPC_HPP
# define OVERCOOLNPC_HPP

# include "AFlyNpc.hpp"

class OverCoolNpc : public AFlyNpc
{
public:
	OverCoolNpc(void);
	virtual ~OverCoolNpc(void) = default;

	virtual void setup(void);
};

#endif
