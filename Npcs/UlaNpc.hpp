#ifndef ULANPC_HPP
# define ULANPC_HPP

# include "ASpecialNpc.hpp"

class UlaNpc : public ASpecialNpc
{
public:
	UlaNpc(void);
	virtual ~UlaNpc(void) = default;

	virtual void setup(void);

};

#endif
