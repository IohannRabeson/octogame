#ifndef PARALLAXSCROLLING_HPP
# define PARALLAXSCROLLING_HPP

# include <SFML/Graphics.hpp>
# include <vector>
# include <initializer_list>

class ParallaxScrolling : public sf::Drawable
{
public:
	class ALayer : public sf::Drawable
	{
	public:
		ALayer(void) :
			ALayer(0.5f)
		{}
		ALayer(float speed) :
			m_speed(speed),
			m_isScrolling(true)
		{}
		virtual ~ALayer(void) = default;

		inline void setSpeed(float speed) { m_speed = speed; }
		inline void setIsScrolling(bool isScrolling) { m_isScrolling = isScrolling; }
		inline float getSpeed(void) const { return m_speed; }
		inline bool isScrolling(void) const { return m_isScrolling; }

//TODO: private and friend with parallax
		virtual void update(float deltatime) = 0;
		virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const = 0;

	private:
		float		m_speed;
		//TODO: rename static
		bool		m_isScrolling;
	};

	ParallaxScrolling(void);
	ParallaxScrolling(std::initializer_list<ALayer *> list);
	virtual ~ParallaxScrolling(void);

	//TODO
	void addLayer(ALayer * layer);
	void removeLayer(ALayer * layer);

	virtual void update(float deltatime);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;

private:
	//TODO: use unique_ptr
	std::vector<ALayer *>	m_layers;

};

#endif
