#ifndef EVA_HPP
# define EVA_HPP

# include "ANpc.hpp"

class EvaNpc : public ANpc, public AGameObject<GameObjectType::EvaNpc>
{
public:
	EvaNpc(void);
	virtual ~EvaNpc(void) = default;

	virtual void setup(void);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

};

#endif
