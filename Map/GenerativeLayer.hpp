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
	GenerativeLayer(void);
	GenerativeLayer(sf::Color const & color, sf::Vector2f const & speed);
	virtual ~GenerativeLayer(void) = default;

	void setColor(sf::Color const & color);
	inline void setMapSize(sf::Vector2u const & mapSize) { m_mapSize = mapSize; }

//TODO: background generator
//TODO: put all in game
//TODO: transition start/end
//TODO: bouger la map

//TODO: rename
	float getHeightValue(int x);
	//TODO remove parameter ?
	void update(float);
	void draw(sf::RenderTarget & render, sf::RenderStates states) const;

private:
	octo::Camera &						m_camera;
	std::unique_ptr<sf::Vertex[]>		m_vertices;
	Noise								m_noise;
	sf::Vector2u						m_mapSize;
	std::size_t							m_widthScreen;
	octo::RingBuffer<sf::Vector2f>		m_positions;
	float								m_tileSize;
	sf::Color							m_color;

	void init(void);

};

#endif
