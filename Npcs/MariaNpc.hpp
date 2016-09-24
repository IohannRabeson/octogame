#ifndef MARIANPC_HPP
# define MARIANPC_HPP

# include "ASpecialNpc.hpp"

class MariaNpc : public ASpecialNpc
{
public:
	MariaNpc(void);
	virtual ~MariaNpc(void) = default;

	virtual void setup(void);

};

#endif
