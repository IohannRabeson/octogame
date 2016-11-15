#ifndef THEONPC_HPP
# define THEONPC_HPP

# include "ASpecialNpc.hpp"

class TheoNpc : public ASpecialNpc
{
public:
	TheoNpc(void);
	virtual ~TheoNpc(void) = default;

	virtual void setup(void);

};

#endif
