#ifndef ELLIOTNPC_HPP
# define ELLIOTNPC_HPP

# include "ASpecialNpc.hpp"

class ElliotNpc : public ASpecialNpc
{
public:
	ElliotNpc(void);
	virtual ~ElliotNpc(void) = default;

	virtual void setup(void);

private:

};

#endif
