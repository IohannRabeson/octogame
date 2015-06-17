#ifndef DECOR_HPP
# define DECOR_HPP

# include "GameObject.hpp"
# include <memory>
# include <random>

# define PI		3.14159265f
# define RADIAN	PI / 180.0f

struct Biome;

class Decor : public GameObject, public sf::Drawable
{
public:
	Decor(void);
	virtual ~Decor(void);

	virtual void	init(Biome * p_biome);
	virtual void	update(float pf_deltatime);
	virtual void	draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const;
	virtual void	randomDecor(void);
	void			iceDecor(void);
	float			getOriginX(void) const;
	void			updateOrigin(float pf_deltatime);

protected:
	enum EState
	{
		e_state_sleep,
		e_state_grow,
		e_state_die,
		e_state_stop,

		e_state_nb
	};

	sf::VertexArray					m_triangle;
	int								mn_countVertex;
	int								mn_maxTriangle;
	bool							b_isIce;
	EState							me_currentState;
	sf::Vector2f					m_size;
	sf::Color						m_color;
	Biome							*m_biome;
	float							mf_timer;
	float							mf_maxTimer;
	float							mf_dieTimer;
	float							mf_liveTime;
	float							mf_mouvement;
	float							mf_growSpeed;
	float							mf_dieSpeed;

	// TODO: Think about random numbers generation
	float randomRange(int pn_min, int pn_max);
	void  rotateVec(sf::Vector2f *p_vertex, float const & p_cosAngle, float const & p_sinAngle);
	void  createVertex(sf::Vector2f p_pos, sf::Color const & p_color, int * pn_count);
	void  createTriangle(sf::Vector2f const & p_a, sf::Vector2f const & p_b, sf::Vector2f const & p_c, sf::Vector2f const & p_origin, sf::Color const & p_color);
	void  createRectangle(sf::Vector2f const & p_a, sf::Vector2f const & p_b, sf::Vector2f const & p_c, sf::Vector2f const & p_d, sf::Vector2f const & p_origin, sf::Color const & p_color);
	void  computeStates(float pf_deltatime);
	void  putOnMap(void);
	void  allocateVertex(int pn_count);

};

#endif
