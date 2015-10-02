#ifndef CANOUILLE_HPP
# define CANOUILLE_HPP

# include "ANpc.hpp"

class CanouilleNpc : public ANpc, public AGameObject<GameObjectType::CanouilleNpc>
{
public:
	CanouilleNpc(void);
	virtual ~CanouilleNpc(void) = default;

	virtual void setup(void);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

};

#endif
