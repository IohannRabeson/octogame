#ifndef FANNYNPC_HPP
# define FANNYNPC_HPP

# include "ASpecialNpc.hpp"

class FannyNpc : public ASpecialNpc
{
public:
	FannyNpc(void);
	virtual ~FannyNpc(void) = default;

	virtual void setup(void);
};

#endif
