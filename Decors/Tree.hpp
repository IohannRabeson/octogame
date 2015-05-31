#ifndef TREE_HPP
# define TREE_HPP

# include "Decor.hpp"
# include <vector>
# include <cmath>

# define DEPTH 8
# define SIZE_SCREEN_Y 1080

class Tree : public Decor
{
private:
	std::vector<float>				m_refAngle;
	sf::Color						m_leafColor;

	int								mn_maxRectangle;
	int								mn_maxLeaf;
	int								mn_countLeaf;
	int								mn_countAngle;

	bool							mb_growSide;
	bool							mb_isLeaf;

public:
	Tree(void);
	virtual ~Tree(void);

	void init(Biome * p_biome);

	void  createRectangle(sf::Vector2f const & p_center, sf::Vector2f const & p_size, sf::Color & p_color, float p_valueColor, int * pn_count,
						 float pf_cos, float pf_sin, sf::Vector2f * p_leftUp = NULL, sf::Vector2f * p_rightUp = NULL);
	void pythagorasTree(sf::Vector2f const & p_center, sf::Vector2f const & p_size, float const & pf_angle = 0.0f, float const & p_cos = cos(0.0f), float const & p_sin = sin(0.0f), const int pn_depth = 1);
	void randomDecor(void);

	virtual void update(float pf_deltatime);
};

#endif
