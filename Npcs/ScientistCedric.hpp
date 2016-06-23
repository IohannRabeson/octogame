#ifndef SCIENTISTCEDRIC_HPP
# define SCIENTISTCEDRIC_HPP

# include "ScientistNpc.hpp"

class ScientistCedric : public ScientistNpc, public AGameObject<GameObjectType::ScientistCedric>
{
public:
	ScientistCedric(void);
	virtual ~ScientistCedric(void) = default;

};

#endif
