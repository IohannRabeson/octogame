#ifndef ROCK_HPP
# define ROCK_HPP

# include "Decor.hpp"
# include <vector>
# include <cmath>

class Rock : public Decor
{
private:
	struct OctogonValue
	{
		sf::Vector2f			size;
		sf::Vector2f			origin;
		float					sizeLeft;
		float					sizeRight;
		float					sizeRec;
	};
	int							mn_countOctogon;
	std::vector<OctogonValue>	m_values;

	sf::Vector2f				m_left;
	sf::Vector2f				m_right;

public:
	Rock(void);
	virtual ~Rock(void);

	void init(Biome * p_biome);

	void createOctogon(sf::Vector2f p_size, sf::Vector2f p_origin, sf::Color p_color, float p_sizeLeft, float p_sizeRight, float p_sizeRec);
	void createRock(void);
	void randomDecor(void);

	virtual void update(float pf_deltatime);
};

#endif
