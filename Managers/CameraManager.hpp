#ifndef CAMERAMANAGER_HPP
# define CAMERAMANAGER_HPP

# include <Application.hpp>
# include <Camera.hpp>

class MapManager;

class CameraManager
{
public:
	CameraManager(void);
	virtual ~CameraManager(void);

	inline sf::Vector2f const & getCenter(void) const { return m_camera.getCenter(); }
	inline sf::Vector2f const * getUpLeft(void) const { return &m_upLeft; }

	void init(MapManager * map_manager);
	void update(float pf_deltatime);

private:
	MapManager *	m_mapManager;
	octo::Camera &	m_camera;
	sf::Vector2f	m_upLeft;
	std::size_t	m_halfWidth;
	std::size_t	m_halfHeight;

};

#endif
