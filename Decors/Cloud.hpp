#ifndef CLOUD_HPP
# define CLOUD_HPP

# include "Decor.hpp"
# include <vector>
# include <cmath>

class Cloud : public Decor
{
private:
	int							mn_countCloud;
	std::vector<sf::Vector2f>	m_refSize;
	std::vector<sf::Vector2f>	m_refOrigin;
	std::vector<float>			m_refSizeLeft;
	std::vector<float>			m_refSizeRight;
	std::vector<float>			m_refSizeRec;

public:
	Cloud(void);
	virtual ~Cloud(void);

	void init(Biome * p_biome);

	void createOneCloud(sf::Vector2f p_size, sf::Vector2f p_origin, sf::Color p_color, float p_sizeLeft, float p_sizeRight, float p_sizeRec);
	void createCloud(void);
	void randomDecor(void);

	virtual void update(float pf_deltatime);
};

#endif
