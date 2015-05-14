#ifndef NPC_HPP
# define NPC_HPP

# include "DynamicPolygon.hpp"
# include "Tile.hpp"

class NPC : public DynamicPolygon
{
public:
	NPC(void);
	virtual ~NPC(void);

	virtual void update(float pf_deltatime);
	virtual void onCollision(Polygon * p_polygon);

private:
	enum NPCState
	{
		e_waiting,
		e_walking,
		e_jumping,
		e_falling
	};

	NPCState	m_state;
	float		m_speed;
	float		m_timerAcceleration;
	float		m_timerMaxAcceleration;
	float		m_timerJump;
	float		m_timerJumpMax;
	float		m_jumpSpeed;

};

#endif
