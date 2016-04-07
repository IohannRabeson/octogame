#ifndef HOUSEFLATSNOW_HPP
# define HOUSEFLATSNOW_HPP

# include "SimpleObject.hpp"
# include "SmokeSystem.hpp"

class HouseFlatSnow : public SimpleObject, public AGameObject<GameObjectType::HouseFlatSnow>
{
public:
	HouseFlatSnow(void);
	virtual ~HouseFlatSnow(void);

	virtual void setPosition(sf::Vector2f const & position);

	virtual void startBalle(void);
	virtual void update(sf::Time frametime);
	virtual void draw(sf::RenderTarget& render, sf::RenderStates states) const;
	virtual void drawFront(sf::RenderTarget& render, sf::RenderStates states) const;

private:
	SmokeSystem					m_smoke;
	SmokeSystem					m_smoke2;

	void setupSmoke(void);

};

#endif
