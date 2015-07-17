#ifndef GENERATIVELAYER_HPP
# define GENERATIVELAYER_HPP

# include <memory>
# include <vector>
# include "Noise.hpp"
# include "ParallaxScrolling.hpp"

namespace octo
{
	class Camera;
}

class GenerativeLayer : public ParallaxScrolling::ALayer
{
public:
	typedef std::function<float(Noise & noise, float x, float y)>	BackgroundSurfaceGenerator;

	GenerativeLayer(void);
	GenerativeLayer(sf::Color const & color, sf::Vector2f const & speed, float transitionDuration = 20.f);
	virtual ~GenerativeLayer(void) = default;

	void init(void);
	void setColor(sf::Color const & color);
	//TODO: friend
	inline void setMapSize(sf::Vector2u const & mapSize) { m_mapSize = mapSize; }
	/*! Set the transition duration
	 * If transitionDuration < 0.f, the map won't move
	 */
	inline void setTransitionDuration(float transitionDuration) { m_transitionTimerDuration = transitionDuration; }
	void setBackgroundSurfaceGenerator(BackgroundSurfaceGenerator mapSurface);

	void update(float);
	void draw(sf::RenderTarget & render, sf::RenderStates states) const;

private:
	typedef std::function<float(float x, float y)>	BackgroundSurfaceGeneratorBind;

	octo::Camera &						m_camera;
	std::unique_ptr<sf::Vertex[]>		m_vertices;
	Noise								m_noise;
	sf::Vector2u						m_mapSize;
	std::size_t							m_widthScreen;
	std::vector<sf::Vector2f>			m_positions;
	std::vector<sf::Vector2f>			m_positions2;
	float								m_tileSize;
	sf::Color							m_color;
	float								m_depth;
	float								m_transitionTimer;
	float								m_transitionTimerDuration;
	BackgroundSurfaceGeneratorBind		m_backgroundSurface;

	void computeVertices(std::vector<sf::Vector2f> & positions);
	void swap(void);
	float getHeightValue(int x);

};

#endif
