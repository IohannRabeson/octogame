#ifndef CLEMENTINENPC_HPP
# define CLEMENTINENPC_HPP

# include "ASpecialNpc.hpp"

class ClementineNpc : public ASpecialNpc
{
public:
	ClementineNpc(void);
	virtual ~ClementineNpc(void) = default;

	virtual void setup(void);

};

#endif
