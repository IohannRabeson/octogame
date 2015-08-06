#ifndef PARALLAXSCROLLING_HPP
# define PARALLAXSCROLLING_HPP

# include <memory>
# include <vector>
# include <SFML/Graphics.hpp>

class ABiome;
class SkyCycle;

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

		inline ParallaxScrolling const & getOwner(void) const { return *m_owner; }
		inline void setOwner(ParallaxScrolling * parallaxScrolling) { m_owner = parallaxScrolling; }

		virtual void setup(void) = 0;
		virtual void update(float deltatime, ABiome & biome) = 0;
		virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const = 0;

	private:
		ParallaxScrolling *	m_owner;
		sf::Vector2f		m_speed;

	};

	ParallaxScrolling(void);
	virtual ~ParallaxScrolling(void);

	/*! Setup */
	void setup(ABiome & biome, SkyCycle & skyCycle);
	/*! Add a layer */
	void addLayer(ALayer * layer);
	/*! Add a vector of layers */
	void addLayer(std::vector<ALayer *> const & layers);
	/*! Remove the layer from the vector and release the allocated memory */
	void removeLayer(std::size_t index);
	/*! Remove all layers from the vector and release the allocated memory */
	void removeAllLayers(void);

	inline SkyCycle const & getSkyCycle(void) const { return *m_skyCycle; }

	virtual void update(float deltatime);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;

private:
	std::vector<std::unique_ptr<ALayer>>	m_layers;
	ABiome *								m_biome;
	SkyCycle *								m_skyCycle;

};

#endif
