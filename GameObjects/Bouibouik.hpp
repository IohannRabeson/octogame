#ifndef BOUIBOUIK_HPP
# define BOUIBOUIK_HPP

# include "AGameObject.hpp"
# include "SimpleObject.hpp"

class Bouibouik : public SimpleObject, public AGameObject<GameObjectType::Bouibouik>
{
public:
	Bouibouik(void);
	virtual ~Bouibouik(void);

	virtual void setPosition(sf::Vector2f const & position);

private:

};

#endif
