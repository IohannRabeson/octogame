#ifndef PEDESTAL_HPP
# define PEDESTAL_HPP

# include "ANpc.hpp"

class Pedestal : public ANpc, public AGameObject<GameObjectType::Pedestal>
{
public:
	Pedestal(void);
	virtual ~Pedestal(void) = default;

	virtual void setup(void);

	virtual void setupMachine(void);
	virtual void updateState(void);

};

#endif
