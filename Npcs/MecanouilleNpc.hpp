#ifndef MECANOUILLENPC_HPP
# define MECANOUILLENPC_HPP

# include "ASpecialNpc.hpp"

class MecanouilleNpc : public ASpecialNpc
{
public:
	MecanouilleNpc(void);
	virtual ~MecanouilleNpc(void) = default;

	virtual void setup(void);

};

#endif
