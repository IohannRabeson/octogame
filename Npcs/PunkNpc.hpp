#ifndef PUNKNPC_HPP
# define PUNKNPC_HPP

# include "AWalkNpc.hpp"

class PunkNpc : public AWalkNpc
{
public:
	PunkNpc(void);
	virtual ~PunkNpc(void) = default;

	virtual void setup(void);

};

#endif
