#ifndef PARALLAXSCROLLING_HPP
# define PARALLAXSCROLLING_HPP

# include <memory>
# include <vector>
# include <initializer_list>
# include <SFML/Graphics.hpp>

class ParallaxScrolling : public sf::Drawable
{
public:
	class ALayer : public sf::Drawable
	{
	public:
		ALayer(void) :
			ALayer(sf::Vector2f(0.5f, 0.5f))
		{}
		ALayer(sf::Vector2f const & speed) :
			m_speed(speed)
		{}
		virtual ~ALayer(void) = default;

		inline void setSpeed(sf::Vector2f const & speed) { m_speed = speed; }
		inline sf::Vector2f const & getSpeed(void) const { return m_speed; }
		virtual void setOpacityColor(sf::Color const & ) = 0;

		virtual void init(void) = 0;
		virtual void update(float deltatime) = 0;
		virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const = 0;

	private:
		sf::Vector2f	m_speed;
	};

	ParallaxScrolling(void) = default;
	ParallaxScrolling(std::initializer_list<ALayer *> list);
	virtual ~ParallaxScrolling(void);

		void setColor(sf::Color const & color);
	/*! Add a layer */
	void addLayer(ALayer * layer);
	/*! Remove the layer from the vector and release the allocated memory */
	void removeLayer(std::size_t index);
	/*! Remove all layers from the vector and release the allocated memory */
	void removeAllLayers(void);

	virtual void update(float deltatime);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;

private:
	std::vector<std::unique_ptr<ALayer>>	m_layers;

};

#endif
