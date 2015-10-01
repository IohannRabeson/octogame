#ifndef PEA_HPP
# define PEA_HPP

# include "ANpc.hpp"

class PeaNpc : public ANpc, public AGameObject<GameObjectType::PeaNpc>
{
public:
	PeaNpc(void);
	virtual ~PeaNpc(void) = default;

	virtual void setup(void);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

};

#endif
