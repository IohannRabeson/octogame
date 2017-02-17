#ifndef MYSTICANOUILLENPC_HPP
# define MYSTICANOUILLENPC_HPP

# include "ASpecialNpc.hpp"

class MysticanouilleNpc : public ASpecialNpc
{
public:
	MysticanouilleNpc(void);
	virtual ~MysticanouilleNpc(void) = default;

	virtual void setup(void);
	virtual void update(sf::Time frameTime);

private:
	void createText(void);
	std::vector<std::wstring> m_countDownText;
};

#endif
