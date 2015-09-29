#ifndef FAUST_HPP
# define FAUST_HPP

# include "ANpc.hpp"

class FaustNpc : public ANpc, public AGameObject<GameObjectType::FaustNpc>
{
public:
	FaustNpc(void);
	virtual ~FaustNpc(void) = default;

	virtual void setup(void);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

};

#endif
