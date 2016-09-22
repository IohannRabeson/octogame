#ifndef FRANGLITCHNPC_HPP
# define FRANGLITCHNPC_HPP

# include "ADisappearNpc.hpp"

class FranGlitchNpc : public ADisappearNpc
{
public:
	FranGlitchNpc(void);
	virtual ~FranGlitchNpc(void) = default;

	virtual void setup(void);
};

#endif
