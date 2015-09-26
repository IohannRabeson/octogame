#ifndef CONCERT_HPP
# define CONCERT_HPP

# include "SimpleObject.hpp"
# include "MusicSystem.hpp"

class Concert : public SimpleObject, public AGameObject<GameObjectType::Concert>
{
public:
	Concert(void);
	virtual ~Concert(void);

	virtual void update(sf::Time frameTime);
	virtual void setPosition(sf::Vector2f const & position);
	virtual void draw(sf::RenderTarget &, sf::RenderStates) const;
	virtual void drawFront(sf::RenderTarget &, sf::RenderStates) const;

private:
	MusicSystem		m_particles;

};

#endif
