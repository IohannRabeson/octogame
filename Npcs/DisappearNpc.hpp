#ifndef DISAPPEARNPC_HPP
# define DISAPPEARNPC_HPP

# include "ANpc.hpp"
# include "RandomGenerator.hpp"

class DisappearNpc : public ANpc
{
public:
	DisappearNpc(ResourceKey const & key);
	virtual ~DisappearNpc(void) = default;

	virtual void setup(void);
	virtual void update(sf::Time frametime);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

private:
	void					makeDisappear(sf::Time frametime);

	bool					m_isVisible;
	static RandomGenerator	m_generator;
	float					m_transparency;
	sf::Time				m_randomDisappearTimer;
	sf::Time				m_randomAppearTimer;
};

#endif
