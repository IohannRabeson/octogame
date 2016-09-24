#ifndef FLORENTNPC_HPP
# define FLORENTNPC_HPP

# include "ASpecialNpc.hpp"

class FlorentNpc : public ASpecialNpc
{
public:
	FlorentNpc(void);
	virtual ~FlorentNpc(void) = default;

	virtual void setup(void);

};

#endif
