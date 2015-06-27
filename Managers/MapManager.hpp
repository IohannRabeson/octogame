#ifndef MAPMANAGER_HPP
# define MAPMANAGER_HPP

# include <Application.hpp>
# include <Camera.hpp>
# include "GroundManager.hpp"
# include "DecorManager.hpp"

class MapManager : public sf::Drawable
{
public:
	MapManager(void);
	virtual ~MapManager(void);

	void init(void);
	void update(float pf_deltatime);
	void draw(sf::RenderTarget& render, sf::RenderStates states) const;

	//inline Player & getPlayer(void) const { return m_collisionManager.getPlayer(); }
	inline GroundManager & getGroundManager(void) { return m_terrainManager; }

private:
	Biome				m_biome;
	GroundManager		m_terrainManager;
	std::string			m_wave;
	std::string			m_dark;
	std::string			m_negative;
	sf::Shader			m_shader;
	octo::Camera &		m_camera;

};

#endif
