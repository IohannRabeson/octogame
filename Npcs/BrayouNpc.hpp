#ifndef BRAYOU_HPP
# define BRAYOU_HPP

# include "ASpecialNpc.hpp"

class BrayouNpc : public ASpecialNpc, public AGameObject<GameObjectType::BrayouNpc>
{
public:
	BrayouNpc(void);
	virtual ~BrayouNpc(void) = default;

	virtual void setup(void);

};

#endif
