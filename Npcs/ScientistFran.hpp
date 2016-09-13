#ifndef SCIENTISTFRAN_HPP
# define SCIENTISTFRAN_HPP

# include "ScientistNpc.hpp"

class ScientistFran : public ScientistNpc, public AGameObject<GameObjectType::ScientistFran>
{
public:
	ScientistFran(void);
	virtual ~ScientistFran(void) = default;

};

#endif
