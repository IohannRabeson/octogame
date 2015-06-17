#ifndef MAPMANAGER_HPP
# define MAPMANAGER_HPP

# include "TerrainManager.hpp"
# include "DecorManager.hpp"
# include "CollisionManager.hpp"
# include "CameraManager.hpp"

class MapManager : public sf::Drawable
{
public:
	MapManager(void);
	virtual ~MapManager(void);

	void init(void);
	void update(float pf_deltatime);
	void draw(sf::RenderTarget& render, sf::RenderStates states) const;

	//inline Player & getPlayer(void) const { return m_collisionManager.getPlayer(); }
	inline TerrainManager & getTerrainManager(void) { return m_terrainManager; }
	inline CameraManager & getCameraManager(void) { return m_cameraManager; }

private:
	Biome				m_biome;
	TerrainManager			m_terrainManager;
	//CollisionManager		m_collisionManager;
	CameraManager			m_cameraManager;
	std::string			m_wave;
	std::string			m_dark;
	std::string			m_negative;
	sf::Shader			m_shader;

};

#endif
