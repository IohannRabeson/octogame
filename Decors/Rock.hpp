#ifndef ROCK_HPP
# define ROCK_HPP

# include "Decor.hpp"
# include <vector>
# include <cmath>

class Rock : public Decor
{
private:
	int							mn_countRock;
	std::vector<sf::Vector2f>	m_refSize;
	std::vector<sf::Vector2f>	m_refOrigin;
	std::vector<float>			m_refSizeLeft;
	std::vector<float>			m_refSizeRight;
	std::vector<float>			m_refSizeRec;

	sf::Vector2f				m_left;
	sf::Vector2f				m_right;

public:
	Rock(void);
	virtual ~Rock(void);

	void init(Biome * p_biome);

	void createOneRock(sf::Vector2f p_size, sf::Vector2f p_origin, sf::Color p_color, float p_sizeLeft, float p_sizeRight, float p_sizeRec);
	void createRock(void);
	void randomDecor(void);

	virtual void update(float pf_deltatime);
};

#endif
