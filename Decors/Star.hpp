#ifndef STAR_HPP
# define STAR_HPP

# include "Decor.hpp"
# include <vector>

class Star : public Decor
{
private:
	sf::CircleShape				m_light;
	sf::Vector2f				m_sizeHeart;
	float						mf_shineTimer;
	float						mf_maxShineTime;

public:
	Star(void);
	virtual ~Star(void);

	void init(Biome * p_biome);

	void createOneStar(sf::Vector2f p_size, sf::Vector2f p_sizeHeart, float p_angle, sf::Color p_color);
	void randomDecor(void);
	void setOrigin(sf::Vector2f p_origin);
	void shine(float pf_deltatime);
	void computeStates(float pf_deltatime);

	virtual void update(float pf_deltatime);
	virtual void draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const;
};

#endif
