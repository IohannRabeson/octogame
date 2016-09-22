#ifndef PIERRE_HPP
# define PIERRE_HPP

# include "ASpecialNpc.hpp"

class PierreNpc : public ASpecialNpc
{
public:
	PierreNpc(void);
	virtual ~PierreNpc(void) = default;

	virtual void setup(void);

};

#endif
