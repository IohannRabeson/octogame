#ifndef SCIENTISTNPC_HPP
# define SCIENTISTNPC_HPP

# include "ANpc.hpp"

class ScientistNpc : public ANpc
{
public:
	ScientistNpc(ResourceKey key);
	virtual ~ScientistNpc(void) = default;

	virtual void setup(void);
	virtual void updateText(sf::Time frametime);
	void setTextIndex(std::size_t index);
	std::size_t getLastIndex(void) const;
	void updateText(bool update);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

private:
	std::map<std::size_t, std::size_t>	m_indexText;
	std::size_t							m_currentIndex;
	bool								m_updateText;

};

#endif
