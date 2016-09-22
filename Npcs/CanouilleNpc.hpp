#ifndef CANOUILLE_HPP
# define CANOUILLE_HPP

# include "ASpecialNpc.hpp"

class CanouilleNpc : public ASpecialNpc
{
public:
	CanouilleNpc(void);
	virtual ~CanouilleNpc(void) = default;

	virtual void setup(void);

};

#endif
