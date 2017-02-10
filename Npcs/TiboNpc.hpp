#ifndef TIBONPC_HPP
# define TIBONPC_HPP

# include "ASpecialNpc.hpp"

class TiboNpc : public ASpecialNpc
{
public:
	TiboNpc(void);
	virtual ~TiboNpc(void) = default;

	virtual void setup(void);

};

#endif
