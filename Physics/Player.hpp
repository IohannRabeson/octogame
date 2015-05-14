#ifndef PLAYER_HPP
# define PLAYER_HPP

# include "DynamicPolygon.hpp"
# include "Tile.hpp"

class Player : public DynamicPolygon
{
public:
	Player(void);
	virtual ~Player(void);

	virtual void update(float pf_deltatime);
	virtual void onCollision(Polygon * p_polygon);

private:
	enum PlayerState
	{
		e_waiting,
		e_walking,
		e_jumping,
		e_falling
	};

	PlayerState	m_state;
	float		m_speed;
	float		m_timerAcceleration;
	float		m_timerMaxAcceleration;
	float		m_timerJump;
	float		m_timerJumpMax;
	float		m_jumpSpeed;

};

#endif
