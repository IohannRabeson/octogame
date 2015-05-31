#ifndef DECORMANAGER_HPP
# define DECORMANAGER_HPP

# include <vector>
# include <SFML/Graphics.hpp>
# include <DefaultGraphicsListeners.hpp>

class MapManager;
struct Biome;
class Decor;

class DecorManager : public sf::Drawable, public octo::DefaultKeyboardListener
{
public:
	DecorManager(void);
	virtual ~DecorManager(void);

	// Called by the TransitionManager after each new transition
	void setPosition(void);
	void init(MapManager * m_mapManager, Biome * p_biome);
	void update(float pf_deltatime);
	void draw(sf::RenderTarget& render, sf::RenderStates states) const;
	virtual bool onPressed (sf::Event::KeyEvent const &event);

private:
	Biome *					m_biome;
	// Owner
	MapManager *			m_mapManager;
	std::vector<Decor *>	m_decors;
	int						m_offsetX;

};

#endif
