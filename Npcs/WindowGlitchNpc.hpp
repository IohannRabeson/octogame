#ifndef WINDOWGLITCHNPC_HPP
# define WINDOWGLITCHNPC_HPP

# include "ADisappearNpc.hpp"

class WindowGlitchNpc : public ADisappearNpc
{
public:
	WindowGlitchNpc(void);
	virtual ~WindowGlitchNpc(void) = default;

	virtual void setup(void);
};

#endif
