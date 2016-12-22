#ifndef STEAMWORKS_HPP
# define STEAMWORKS_HPP

#include <memory>

class Steamworks
{
public:

	static Steamworks &					getInstance(void);

	Steamworks(void);
	~Steamworks(void) = default;

private:
	static std::unique_ptr<Steamworks>	m_instance;

	void	initAPI(void);
	void	clearAchievement(void);
	void	setAchievement(std::string name);
};

#endif
