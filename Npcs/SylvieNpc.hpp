#ifndef SYLVIENPC_HPP
# define SYLVIENPC_HPP

# include "AIdleNpc.hpp"

class SylvieNpc : public AIdleNpc
{
public:
	SylvieNpc(void);
	virtual ~SylvieNpc(void) = default;

	virtual void setup(void);

};

#endif
