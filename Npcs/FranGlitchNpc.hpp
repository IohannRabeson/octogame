#ifndef FRANGLITCHNPC_HPP
# define FRANGLITCHNPC_HPP

# include "DisappearNpc.hpp"

class FranGlitchNpc : public DisappearNpc, public AGameObject<GameObjectType::FranGlitchNpc>
{
public:
	FranGlitchNpc(void);
	virtual ~FranGlitchNpc(void) = default;

	virtual void setup(void);
};

#endif
