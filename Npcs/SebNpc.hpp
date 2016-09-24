#ifndef SEBNPC_HPP
# define SEBNPC_HPP

# include "ASwimNpc.hpp"

class SebNpc : public ASwimNpc
{
public:
	SebNpc(void);
	virtual ~SebNpc(void) = default;

	virtual void setup(void);

};

#endif
