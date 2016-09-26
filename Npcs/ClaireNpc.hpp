#ifndef CLAIRENPC_HPP
# define CLAIRENPC_HPP

# include "ASpecialNpc.hpp"

class ClaireNpc : public ASpecialNpc
{
public:
	ClaireNpc(void);
	virtual ~ClaireNpc(void) = default;

	virtual void setup(void);

};

#endif
