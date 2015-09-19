#ifndef BOUIBOUIK_HPP
# define BOUIBOUIK_HPP

# include "AGameObject.hpp"
# include "SimpleObject.hpp"
# include "SmokeSystem.hpp"

class Bouibouik : public SimpleObject, public AGameObject<GameObjectType::Bouibouik>
{
public:
	Bouibouik(void);
	virtual ~Bouibouik(void);

	virtual void setPosition(sf::Vector2f const & position);
	virtual void update(sf::Time frametime);
	virtual void draw(sf::RenderTarget& render, sf::RenderStates states) const;

private:
	SmokeSystem		m_smoke;
};

#endif
