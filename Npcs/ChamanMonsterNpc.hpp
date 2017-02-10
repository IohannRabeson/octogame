#ifndef CHAMANMONSTERNPC_HPP
# define CHAMANMONSTERNPC_HPP

# include "ASpecialNpc.hpp"

class ChamanMonsterNpc : public ASpecialNpc
{
public:
	ChamanMonsterNpc(void);
	virtual ~ChamanMonsterNpc(void) = default;

	virtual void setup(void);
	virtual void update(sf::Time frameTime);

private:
	sf::Vector2f m_lastPosition;
};

#endif
