#ifndef CONSTANCENPC_HPP
# define CONSTANCENPC_HPP

# include "ASpecialNpc.hpp"
# include "SmokeSystem.hpp"

class ConstanceNpc : public ASpecialNpc
{
public:
	ConstanceNpc(void);
	virtual ~ConstanceNpc(void) = default;

	virtual void setup(void);
	virtual void update(sf::Time frametime);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;

private:
	SmokeSystem		m_smoke;
};

#endif
