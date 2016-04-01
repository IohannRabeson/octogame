#ifndef LANGUAGEMENU_HPP
# define LANGUAGEMENU_HPP

# include "AMenuSelection.hpp"

class LanguageMenu : public AMenuSelection
{
public:
	LanguageMenu(void);
	~LanguageMenu(void) = default;

	void createMenus();
	void onSelection();

private:
	std::size_t m_lastLanguage;
};

#endif
