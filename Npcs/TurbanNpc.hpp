#ifndef TURBANNPC_HPP
# define TURBANNPC_HPP

# include "AIdleNpc.hpp"

class TurbanNpc : public AIdleNpc
{
public:
	TurbanNpc(void);
	virtual ~TurbanNpc(void) = default;

	virtual void setup(void);

};

#endif
