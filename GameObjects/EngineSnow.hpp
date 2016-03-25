#ifndef ENGINESNOW_HPP
# define ENGINESNOW_HPP

# include "SimpleObject.hpp"
# include "SmokeSystem.hpp"

class EngineSnow : public SimpleObject, public AGameObject<GameObjectType::EngineSnow>
{
public:
	EngineSnow(void);
	virtual ~EngineSnow(void);

	virtual void setPosition(sf::Vector2f const & position);

	virtual void update(sf::Time frametime);
	virtual void draw(sf::RenderTarget& render, sf::RenderStates states) const;

private:
};

#endif
