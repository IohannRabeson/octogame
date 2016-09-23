#ifndef SEBNPC_HPP
# define SEBNPC_HPP

# include "ASpecialNpc.hpp"

class SebNpc : public ASpecialNpc
{
public:
	SebNpc(void);
	virtual ~SebNpc(void) = default;

	virtual void setup(void);

};

#endif
