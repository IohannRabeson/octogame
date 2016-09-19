#ifndef FATNPC_HPP
# define FATNPC_HPP

# include "ASpecialNpc.hpp"

class FatNpc : public ASpecialNpc
{
public:
	FatNpc(void);
	virtual ~FatNpc(void) = default;

	virtual void setup(void);

};

#endif
