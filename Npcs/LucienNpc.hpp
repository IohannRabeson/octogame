#ifndef LUCIENNPC_HPP
# define LUCIENNPC_HPP

# include "AUniqueNpc.hpp"

class LucienNpc : public AUniqueNpc
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
