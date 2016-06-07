#ifndef HOUSEFLATSNOW_HPP
# define HOUSEFLATSNOW_HPP

# include "SimpleObject.hpp"
# include "SmokeSystem.hpp"

class HouseFlatSnow : public SimpleObject, public AGameObject<GameObjectType::HouseFlatSnow>
{
public:
	HouseFlatSnow(void);
	virtual ~HouseFlatSnow(void) = default;

	virtual void setPosition(sf::Vector2f const & position);

};

#endif
