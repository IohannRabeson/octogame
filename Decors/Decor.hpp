#ifndef DECOR_HPP
# define DECOR_HPP

# include "GameObject.hpp"
# include <memory>

# define PI		3.14159265f
# define RADIAN		PI / 180.0f

struct Biome;

class Decor : public GameObject, public sf::Drawable
{
public:
	Decor(void);
	virtual ~Decor(void);

	sf::Vertex * getTriangle(void);
	int getCount(void);
	virtual void init(Biome * p_biome);
	virtual void update(float pf_deltatime);
	virtual void draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const;

protected:
	enum EState
	{
		e_state_sleep,
		e_state_grow,
		e_state_die,

		e_state_nb
	};

	std::unique_ptr<sf::Vertex[]>	m_triangle;
	int								mn_countTriangle;
	EState							me_currentState;
	sf::Vector2f					m_size;
	sf::Color						m_color;
	Biome							*m_biome;
	float							mf_timer;
	float							mf_dieTimer;
	float							mf_liveTime;
	float							mf_mouvement;

	// TODO: Think about random numbers generation
	float randomRange(int pn_min, int pn_max);
	float randomRangeTile(int pn_min, int pn_max);
	void  rotateVec(sf::Vector2f *p_point, float p_cosAngle, float p_sinAngle);
	void  createVertex(sf::Vector2f p_pos, sf::Color const & p_color, int * pn_count);
	void  createTriangle(sf::Vector2f const & p_a, sf::Vector2f const & p_b, sf::Vector2f const & p_c, sf::Vector2f const & p_origin, sf::Color const & p_color);
	void  createRectangle(sf::Vector2f const & p_a, sf::Vector2f const & p_b, sf::Vector2f const & p_c, sf::Vector2f const & p_d, sf::Vector2f const & p_origin, sf::Color const & p_color);
	void  computeStates(float pf_deltatime);

	virtual void randomDecor(void);

private:
};

#endif
