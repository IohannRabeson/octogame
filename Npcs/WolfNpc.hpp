#ifndef WOLFNPC_HPP
# define WOLFNPC_HPP

# include "AWalkNpc.hpp"
# include "RandomGenerator.hpp"

class WolfNpc : public AWalkNpc
{
public:
	WolfNpc(void);
	virtual ~WolfNpc(void) = default;

	virtual void setup(void);
};

#endif
