#ifndef STAR_HPP
# define STAR_HPP

# include "Decor.hpp"
# include <vector>

class Star : public Decor
{
private:
	static constexpr int		minX = 60;
	static constexpr int		maxX = 140;
	static constexpr int		minY = 60;
	static constexpr int		maxY = 170;
	static constexpr int		minA = 0;
	static constexpr int		maxA = 90;

	sf::ConvexShape				m_starFront;
	sf::ConvexShape				m_starBack;
	sf::CircleShape				m_light;
	sf::Vector2f				m_sizeHeart;
	sf::Vector2f				m_sizeRay;
	float						mf_timer;
	float						mf_mouvement;
public:
	Star(void);
	virtual ~Star(void);

	void init(Biome * p_biome);

	void randomDecor(void);
	void randomStar(sf::ConvexShape * p_star);
	void randomLight(void);

	virtual void update(float pf_deltatime);
	virtual void draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const;
};

#endif
