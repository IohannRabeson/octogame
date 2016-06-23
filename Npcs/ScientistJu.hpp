#ifndef SCIENTISTJU_HPP
# define SCIENTISTJU_HPP

# include "ScientistNpc.hpp"

class ScientistJu : public ScientistNpc, public AGameObject<GameObjectType::ScientistJu>
{
public:
	ScientistJu(void);
	virtual ~ScientistJu(void) = default;

};

#endif
