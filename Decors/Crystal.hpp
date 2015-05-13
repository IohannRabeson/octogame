#ifndef CRYSTAL_HPP
# define CRYSTAL_HPP

# include "Decor.hpp"
# include <vector>
# include <cmath>

class Crystal : public Decor
{
private:
	int							mn_maxTriangle;
	int							mn_countCrystal;
	std::vector<sf::Vector2f>	m_refSize;
	std::vector<sf::Color>		m_refColor;
	std::vector<float>			m_refAngle;

public:
	Crystal(void);
	virtual ~Crystal(void);

	void init(Biome * p_biome);

	void createOneCrystal(sf::Vector2f p_size, float pf_angle, sf::Color p_color);
	void createCrystal(void);
	void randomDecor(void);

	virtual void update(float pf_deltatime);
};

#endif
