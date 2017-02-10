#ifndef ANTOINENPC_HPP
# define ANTOINENPC_HPP

# include "ASpecialNpc.hpp"

class AntoineNpc : public ASpecialNpc
{
public:
	AntoineNpc(void);
	virtual ~AntoineNpc(void) = default;

	virtual void setup(void);

};

#endif
