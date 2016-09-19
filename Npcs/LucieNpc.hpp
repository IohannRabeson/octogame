#ifndef LUCIENPC_HPP
# define LUCIENPC_HPP

# include "ASpecialNpc.hpp"

class LucieNpc : public ASpecialNpc
{
public:
	LucieNpc(void);
	virtual ~LucieNpc(void) = default;

	virtual void setup(void);

};

#endif
