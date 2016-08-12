#ifndef LUGLITCHNPC_HPP
# define LUGLITCHNPC_HPP

# include "DisappearNpc.hpp"

class LuGlitchNpc : public DisappearNpc, public AGameObject<GameObjectType::LuGlitchNpc>
{
public:
	LuGlitchNpc(void);
	virtual ~LuGlitchNpc(void) = default;

	virtual void setup(void);
};

#endif
