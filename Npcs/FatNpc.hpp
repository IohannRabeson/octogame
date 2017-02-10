#ifndef FATNPC_HPP
# define FATNPC_HPP

# include "AWalkNpc.hpp"

class FatNpc : public AWalkNpc
{
public:
	FatNpc(void);
	virtual ~FatNpc(void) = default;

	virtual void setup(void);

};

#endif
