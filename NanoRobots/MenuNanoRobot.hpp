#ifndef MENUNANOROBOT_HPP
# define MENUNANOROBOT_HPP

# include "NanoRobot.hpp"
# include "RandomGenerator.hpp"

class MenuNanoRobot : public NanoRobot, public AGameObject<GameObjectType::MenuNanoRobot>
{
public:
	MenuNanoRobot(sf::Vector2f const & position);
	virtual ~MenuNanoRobot(void) = default;
	virtual void update(sf::Time frametime);
	virtual void transfertToOcto(bool inInit = false);

private:
	static std::size_t	s_seed;
	RandomGenerator		m_generator;
	float				m_scale;
	float				m_alphaMax;
};

#endif
