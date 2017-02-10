#ifndef JUNPC_HPP
# define JUNPC_HPP

# include "ASpecialNpc.hpp"

class JuNpc : public ASpecialNpc
{
public:
	JuNpc(void);
	virtual ~JuNpc(void) = default;

	virtual void setup(void);

};

#endif
