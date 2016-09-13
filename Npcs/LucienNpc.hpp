#ifndef LUCIENNPC_HPP
# define LUCIENNPC_HPP

# include "ANpc.hpp"

class LucienNpc : public ANpc, public AGameObject<GameObjectType::LucienNpc>
{
public:
	LucienNpc(void);
	virtual ~LucienNpc(void) = default;

	virtual void setup(void);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

private:
	void		playSound(void);
	bool		m_side;
	bool		m_sound;

};

#endif
