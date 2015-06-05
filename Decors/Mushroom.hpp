#ifndef MUSHROOM_HPP
# define MUSHROOM_HPP

# include "Decor.hpp"
# include <vector>
# include <cmath>

class Mushroom : public Decor
{
public:
/*
	struct MushroomValue
	{
		float	sizeFeet;
		float	sizeHead;
	};
*/
	Mushroom(void);
	virtual ~Mushroom(void);

	void init(Biome * p_biome);

	void createMushroom1(sf::Vector2f p_size, sf::Vector2f p_origin, sf::Color p_color);
	void createMushroom(void);
	void randomDecor(void);

	virtual void update(float pf_deltatime);

private:
	std::vector<sf::Vector2f>	m_leftVertex;
	std::vector<sf::Vector2f>	m_rightVertex;
};

#endif
