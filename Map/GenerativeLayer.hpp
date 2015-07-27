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
	GenerativeLayer(sf::Color const & color, sf::Vector2f const & speed, sf::Vector2u const & mapSize, float tileSize, int heightOffset, float topOpacity, float botOpacity, float transitionDuration);
	virtual ~GenerativeLayer(void) = default;

	void setup(ABiome & biome);
	void setColor(sf::Color const & color);
	void setOpacityColor(sf::Color const & color) { m_opacityColor = color; }

	//TODO: use bool to manage this
	/*! Set the transition duration
	 * If transitionDuration < 0.f, the map won't move
	 */
	inline void setTransitionDuration(float transitionDuration) { m_transitionTimerDuration = transitionDuration; }
	void setBackgroundSurfaceGenerator(BackgroundSurfaceGenerator mapSurface);
	inline void setMapSize(sf::Vector2u const & mapSize) { m_mapSize = mapSize; }
	inline sf::Vector2u const & getMapSize(void) const { return m_mapSize; }

	void update(float);
	void draw(sf::RenderTarget & render, sf::RenderStates states) const;

private:
	typedef std::function<float(float x, float y)>	BackgroundSurfaceGeneratorBind;

	octo::Camera &						m_camera;
	std::unique_ptr<sf::Vertex[]>		m_vertices;
	std::vector<sf::Vector2f>			m_positions;
	std::vector<sf::Vector2f>			m_positionsPrev;
	sf::Vector2u						m_mapSize;
	//TODO: bot and top opacity color in biome
	sf::Color							m_color;
	sf::Color							m_opacityColor;
	Noise								m_noise;
	float								m_tileSize;
	float								m_depth;
	float								m_transitionTimer;
	float								m_transitionTimerDuration;
	float								m_topOpacity;
	float								m_botOpacity;
	float								m_highestY;
	int									m_heightOffset;
	std::size_t							m_widthScreen;
	std::size_t							m_verticesCount;
	BackgroundSurfaceGeneratorBind		m_backgroundSurface;

	void computeVertices(std::vector<sf::Vector2f> & positions);
	void swap(void);
	float getHeightValue(int x);

};

#endif
