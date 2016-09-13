#ifndef SCIENTISTLU_HPP
# define SCIENTISTLU_HPP

# include "ScientistNpc.hpp"

class ScientistLu : public ScientistNpc, public AGameObject<GameObjectType::ScientistLu>
{
public:
	ScientistLu(void);
	virtual ~ScientistLu(void) = default;

};

#endif
