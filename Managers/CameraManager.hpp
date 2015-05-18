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

	inline sf::Vector2f const & getCenter(void) const { return m_position; }

	void init(MapManager * map_manager);
	void update(float pf_deltatime);

private:
	MapManager *	m_mapManager;
	sf::Vector2f	m_position;
	octo::Camera &	m_camera;

};

#endif
