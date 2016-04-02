#ifndef WEIRDHOUSESNOW_HPP
# define WEIRDHOUSESNOW_HPP

# include "SimpleObject.hpp"
# include "SmokeSystem.hpp"

class WeirdHouseSnow : public SimpleObject, public AGameObject<GameObjectType::WeirdHouseSnow>
{
public:
	WeirdHouseSnow(void);
	virtual ~WeirdHouseSnow(void);

	virtual void setPosition(sf::Vector2f const & position);

	virtual void update(sf::Time frametime);
	virtual void draw(sf::RenderTarget& render, sf::RenderStates states) const;
};

#endif
