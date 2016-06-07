#ifndef WEIRDHOUSESNOW_HPP
# define WEIRDHOUSESNOW_HPP

# include "SimpleObject.hpp"
# include "SmokeSystem.hpp"

class WeirdHouseSnow : public SimpleObject, public AGameObject<GameObjectType::WeirdHouseSnow>
{
public:
	WeirdHouseSnow(void);
	virtual ~WeirdHouseSnow(void) = default;

	virtual void setPosition(sf::Vector2f const & position);

};

#endif
