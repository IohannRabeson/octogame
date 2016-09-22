#ifndef AMANDINENPC_HPP
# define AMANDINENPC_HPP

# include "ASpecialNpc.hpp"

class AmandineNpc : public ASpecialNpc
{
public:
	AmandineNpc(void);
	virtual ~AmandineNpc(void) = default;

	virtual void setup(void);

};

#endif
