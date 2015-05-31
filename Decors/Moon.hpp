#ifndef MOON_HPP
# define MOON_HPP

# include "Decor.hpp"
# include <vector>
# include <cmath>

class Moon : public Decor
{
public:
	Moon(void);
	virtual ~Moon(void);

	void init(Biome * p_biome);

	void createOneMoon(sf::Vector2f p_size, sf::Vector2f p_origin, sf::Color p_color);
	void createMoon(void);
	void randomDecor(void);
	void createDarkness(void);
	void computeOrigin(float pf_deltatime);

	virtual void update(float pf_deltatime);

private:
	sf::Color		m_transparency;

	float			mf_angle;
};

#endif
