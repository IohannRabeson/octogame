#ifndef MAPMANAGER_HPP
# define MAPMANAGER_HPP

# include "TransitionManager.hpp"
# include "DecorManager.hpp"
# include "CollisionManager.hpp"
# include "Player.hpp"

class MapManager : public sf::Drawable
{
public:
	MapManager(void);
	virtual ~MapManager(void);

	void init(Map::EMapType p_mapType);
	Player & getPlayer(void) const;
	TransitionManager & getTransitionManager(void);
	DecorManager & getDecorManager(void);
	void update(float pf_deltatime);
	void draw(sf::RenderTarget& render, sf::RenderStates states) const;

private:
	Biome					m_biome;
	TransitionManager		m_transitionManager;
	DecorManager			m_decorManager;
	CollisionManager		m_collisionManager;

	//TODO: remove
	sf::Texture const*		m_texture;
};

#endif
