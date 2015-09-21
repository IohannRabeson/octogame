#ifndef BOUIBOUIK_HPP
# define BOUIBOUIK_HPP

# include <SFML/Graphics.hpp>
# include "AGameObject.hpp"
# include "SimpleObject.hpp"
# include "SmokeSystem.hpp"

class CircleShape;

class Bouibouik : public SimpleObject, public AGameObject<GameObjectType::Bouibouik>
{
public:
	Bouibouik(void);
	virtual ~Bouibouik(void);

	void startBalle(void);

	virtual void setPosition(sf::Vector2f const & position);
	virtual void update(sf::Time frametime);
	virtual void draw(sf::RenderTarget& render, sf::RenderStates states) const;

private:
	SmokeSystem		m_smoke;
	CircleShape *	m_box;
	sf::Shader		m_shader;
	std::size_t		m_shaderIndex;
	sf::Time		m_timer;
	sf::Time		m_effectDuration;
	bool			m_startBalle;
	bool			m_collideWithOcto;

	void setupSmoke(void);

};

#endif
