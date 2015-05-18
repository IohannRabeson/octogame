#ifndef MAPMANAGER_HPP
# define MAPMANAGER_HPP

# include "TransitionManager.hpp"
# include "DecorManager.hpp"
# include "CollisionManager.hpp"
# include "CameraManager.hpp"
# include "Player.hpp"

class MapManager : public sf::Drawable
{
public:
	MapManager(void);
	virtual ~MapManager(void);

	void init(Map::EMapType p_mapType);
	void update(float pf_deltatime);
	void draw(sf::RenderTarget& render, sf::RenderStates states) const;

	inline Player & getPlayer(void) const { return m_collisionManager.getPlayer(); }
	inline TransitionManager & getTransitionManager(void) { return m_transitionManager; }
	inline DecorManager & getDecorManager(void) { return m_decorManager; }
	inline CameraManager & getCameraManager(void) { return m_cameraManager; }

private:
	Biome					m_biome;
	TransitionManager		m_transitionManager;
	DecorManager			m_decorManager;
	CollisionManager		m_collisionManager;
	CameraManager			m_cameraManager;

	//TODO: remove
	sf::Texture const*		m_texture;
};

#endif
