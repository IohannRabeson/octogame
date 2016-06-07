#ifndef CONCERT_HPP
# define CONCERT_HPP

# include "SimpleObject.hpp"
# include "MusicSystem.hpp"

class Concert : public SimpleObject, public AGameObject<GameObjectType::Concert>
{
public:
	Concert(void);
	virtual ~Concert(void) = default;

	virtual void update(sf::Time frameTime);
	virtual void setPosition(sf::Vector2f const & position);
	virtual void drawFront(sf::RenderTarget &, sf::RenderStates) const;

private:
	std::size_t						m_particlesCount;
	std::unique_ptr<MusicSystem[]>	m_particles;

};

#endif
