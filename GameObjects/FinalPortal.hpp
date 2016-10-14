#ifndef FINALPORTAL_HPP
# define FINALPORTAL_HPP

# include "Portal.hpp"

class FinalPortal : public Portal
{
public:
	FinalPortal(Level destination, ResourceKey key, ResourceKey shader, sf::Color centerColor = sf::Color::Black);
	virtual ~FinalPortal(void);

	virtual bool zoom(void) const;
	void forceActivate(void);

	virtual void update(sf::Time frameTime);
	virtual void draw(sf::RenderTarget& render, sf::RenderStates states) const;

private:
	bool		m_forceActivate;

};

#endif
