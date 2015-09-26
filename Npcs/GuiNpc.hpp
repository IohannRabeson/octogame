#ifndef GUINPC_HPP
# define GUINPC_HPP

# include "ANpc.hpp"

class GuiNpc : public ANpc, public AGameObject<GameObjectType::GuiNpc>
{
public:
	GuiNpc(void);
	virtual ~GuiNpc(void) = default;

	virtual void setup(void);

};

#endif
