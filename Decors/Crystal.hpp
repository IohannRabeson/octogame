#ifndef CRYSTAL_HPP
# define CRYSTAL_HPP

# include "Decor.hpp"
# include "Star.hpp"
# include <vector>
# include <cmath>

class Crystal : public Decor
{
private:
	int							mn_countCrystal;
	std::vector<sf::Vector2f>	m_refSize;
	std::vector<sf::Color>		m_refColor;
	std::vector<float>			m_refAngle;
	std::vector<sf::Vector2f>	m_up;
	Star						m_star;
	int							m_picCrystal;
	int							m_nbCrystal;
	float						mf_starTimer;

public:
	Crystal(void);
	virtual ~Crystal(void);

	void init(Biome * p_biome);

	sf::Vector2f createOneCrystal(sf::Vector2f p_size, float pf_angle, sf::Color p_color);
	void createCrystal(void);
	void randomDecor(void);

	virtual void update(float pf_deltatime);
	virtual void draw(sf::RenderTarget& p_target, sf::RenderStates p_states) const;
};

#endif
