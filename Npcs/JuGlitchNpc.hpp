#ifndef JUGLITCHNPC_HPP
# define JUGLITCHNPC_HPP

# include "DisappearNpc.hpp"

class JuGlitchNpc : public DisappearNpc, public AGameObject<GameObjectType::JuGlitchNpc>
{
public:
	JuGlitchNpc(void);
	virtual ~JuGlitchNpc(void) = default;

	virtual void setup(void);
};

#endif
