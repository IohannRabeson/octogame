#ifndef WINDOWGLITCHNPC_HPP
# define WINDOWGLITCHNPC_HPP

# include "DisappearNpc.hpp"

class WindowGlitchNpc : public DisappearNpc, public AGameObject<GameObjectType::WindowGlitchNpc>
{
public:
	WindowGlitchNpc(void);
	virtual ~WindowGlitchNpc(void) = default;

	virtual void setup(void);
};

#endif
