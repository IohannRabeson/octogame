#ifndef AYMERICNPC_HPP
# define AYMERICNPC_HPP

# include "ASpecialNpc.hpp"

class AymericNpc : public ASpecialNpc
{
public:
	AymericNpc(void);
	virtual ~AymericNpc(void) = default;

	virtual void setup(void);

};

#endif
