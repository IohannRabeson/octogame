#ifndef CAVEMANCLIMBINGNPC_HPP
# define CAVEMANCLIMBINGNPC_HPP

# include "AIdleNpc.hpp"

class CavemanClimbingNpc : public AIdleNpc
{
public:
	CavemanClimbingNpc(void);
	virtual ~CavemanClimbingNpc(void) = default;

	virtual void setup(void);

private:

};

#endif
