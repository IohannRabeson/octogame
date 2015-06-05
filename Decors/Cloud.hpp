#ifndef CLOUD_HPP
# define CLOUD_HPP

# include "Decor.hpp"
# include <vector>
# include <cmath>

class Cloud : public Decor
{
private:
	struct OctogonValue
	{
		sf::Vector2f			size;
		sf::Vector2f			origin;
		float					sizeUp;
		float					sizeDown;
		float					sizeRec;
	};

	int							mn_coundOctogon;
	std::vector<OctogonValue>	m_values;
	int							mn_alpha;

public:
	Cloud(void);
	virtual ~Cloud(void);

	void init(Biome * p_biome);

	void createOctogon(sf::Vector2f p_size, sf::Vector2f p_origin, sf::Color & p_color, float p_sizeUp, float p_sizeDown, float p_sizeRec);
	void createCloud(void);
	void randomDecor(void);

	virtual void update(float pf_deltatime);
};

#endif
