#ifndef PLAYER_HPP
# define PLAYER_HPP

# include "DynamicPolygon.hpp"

class Player : public DynamicPolygon
{
public:
	Player(void);
	virtual ~Player(void);

	virtual void update(float pf_deltatime);

};

#endif
