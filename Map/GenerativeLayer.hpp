#ifndef GENERATIVELAYER_HPP
# define GENERATIVELAYER_HPP

# include <memory>
# include <RingBuffer.hpp>
# include "Noise.hpp"
# include "ParallaxScrolling.hpp"

namespace octo
{
	class Camera;
}

class GenerativeLayer : public ParallaxScrolling::ALayer
{
public:
	typedef std::function<float(Noise & noise, float x)>	BackgroundSurfaceGenerator;

	GenerativeLayer(void);
	GenerativeLayer(sf::Color const & color, sf::Vector2f const & speed);
	virtual ~GenerativeLayer(void) = default;

	void init(void);
	void setColor(sf::Color const & color);
	inline void setMapSize(sf::Vector2u const & mapSize) { m_mapSize = mapSize; }
	void setBackgroundSurfaceGenerator(BackgroundSurfaceGenerator mapSurface);

//TODO: put all in game
//TODO: bouger la map
//TODO: use push with ring buffer

	//TODO remove parameter ?
	void update(float);
	void draw(sf::RenderTarget & render, sf::RenderStates states) const;

private:
	typedef std::function<float(float x)>	BackgroundSurfaceGeneratorBind;

	octo::Camera &						m_camera;
	std::unique_ptr<sf::Vertex[]>		m_vertices;
	Noise								m_noise;
	sf::Vector2u						m_mapSize;
	std::size_t							m_widthScreen;
	octo::RingBuffer<sf::Vector2f>		m_positions;
	float								m_tileSize;
	sf::Color							m_color;
	BackgroundSurfaceGeneratorBind		m_backgroundSurface;

	void updateHeight(void);
//TODO: rename
	float getHeightValue(int x);

};

#endif
