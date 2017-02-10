#ifndef LONGCHAIRNPC_HPP
# define LONGCHAIRNPC_HPP

# include "AIdleNpc.hpp"

class LongChairNpc : public AIdleNpc
{
public:
	LongChairNpc(void);
	virtual ~LongChairNpc(void) = default;

	virtual void setup(void);

};

#endif
