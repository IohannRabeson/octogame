#ifndef MUSHROOM_HPP
# define MUSHROOM_HPP

# include "Decor.hpp"
# include <vector>
# include <cmath>

class Mushroom : public Decor
{
public:
	Mushroom(void);
	virtual ~Mushroom(void);

	void init(Biome * p_biome);

	void createOneMushroom(sf::Vector2f p_size, sf::Vector2f p_origin, sf::Color p_color);
	void createMushroom(void);
	void randomDecor(void);

	virtual void update(float pf_deltatime);

private:
};

#endif
