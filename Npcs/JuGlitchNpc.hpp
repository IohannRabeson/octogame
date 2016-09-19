#ifndef JUGLITCHNPC_HPP
# define JUGLITCHNPC_HPP

# include "ADisappearNpc.hpp"

class JuGlitchNpc : public ADisappearNpc
{
public:
	JuGlitchNpc(void);
	virtual ~JuGlitchNpc(void) = default;

	virtual void setup(void);
};

#endif
