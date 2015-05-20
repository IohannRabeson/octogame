#ifndef SUN_HPP
# define SUN_HPP

# include "Decor.hpp"
# include <vector>
# include <cmath>

class Sun : public Decor
{
public:
	Sun(void);
	virtual ~Sun(void);

	void init(Biome * p_biome);

	void createOneSun(sf::Vector2f p_size, sf::Vector2f p_sizeCorner, sf::Vector2f p_origin, sf::Color p_color);
	void createSun(void);
	void randomDecor(void);

	virtual void update(float pf_deltatime);

private:
	sf::Vector2f	m_sizeCorner;
	sf::Color		m_transparency;
};

#endif
