#ifndef PUNKNPC_HPP
# define PUNKNPC_HPP

# include "ANpc.hpp"

class PunkNpc : public ANpc, public AGameObject<GameObjectType::PunkNpc>
{
public:
	PunkNpc(void);
	virtual ~PunkNpc(void) = default;

	virtual void setup(void);

};

#endif
