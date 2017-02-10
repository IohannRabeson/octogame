#ifndef LUGLITCHNPC_HPP
# define LUGLITCHNPC_HPP

# include "ADisappearNpc.hpp"

class LuGlitchNpc : public ADisappearNpc
{
public:
	LuGlitchNpc(void);
	virtual ~LuGlitchNpc(void) = default;

	virtual void setup(void);
};

#endif
