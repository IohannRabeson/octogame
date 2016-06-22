#ifndef SCIENTISTJU_HPP
# define SCIENTISTJU_HPP

# include "ANpc.hpp"

class ScientistJu : public ANpc, public AGameObject<GameObjectType::ScientistJu>
{
public:
	ScientistJu(void);
	virtual ~ScientistJu(void) = default;

	virtual void setup(void);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

};

#endif
