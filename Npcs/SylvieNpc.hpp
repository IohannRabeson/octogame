#ifndef SYLVIENPC_HPP
# define SYLVIENPC_HPP

# include "ASwimNpc.hpp"

class SylvieNpc : public ASwimNpc
{
public:
	SylvieNpc(void);
	virtual ~SylvieNpc(void) = default;

	virtual void setup(void);
};

#endif
