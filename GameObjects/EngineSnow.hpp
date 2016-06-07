#ifndef ENGINESNOW_HPP
# define ENGINESNOW_HPP

# include "SimpleObject.hpp"
# include "SmokeSystem.hpp"

class EngineSnow : public SimpleObject, public AGameObject<GameObjectType::EngineSnow>
{
public:
	EngineSnow(void);
	virtual ~EngineSnow(void) = default;

	virtual void setPosition(sf::Vector2f const & position);

};

#endif
