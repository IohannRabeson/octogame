#ifndef OLDDESERTSTATICNPC_HPP
# define OLDDESERTSTATICNPC_HPP

# include "AIdleNpc.hpp"

class OldDesertStaticNpc : public AIdleNpc
{
public:
	OldDesertStaticNpc(void);
	virtual ~OldDesertStaticNpc(void) = default;

	virtual void setup(void);

};

#endif
