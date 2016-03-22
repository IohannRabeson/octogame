#ifndef INPUTLISTENER_HPP
# define INPUTLISTENER_HPP

#include <DefaultGraphicsListeners.hpp>

class InputListener : public octo::DefaultKeyboardListener
{
public:
	enum OctoKeys
	{
		Left,
		Right,
		Space,
		Up,
		Down,
		Escape,
		Use,
		SlowFall,
		GroundLeft,
		GroundRight
	};

	InputListener(void);
	~InputListener(void);

	virtual bool			onInputPressed(InputListener::OctoKeys const & key);
	virtual bool			onInputReleased(InputListener::OctoKeys const & key);

private:
	void					addInputListener(void);
	void					removeInputListener(void);
	bool					onPressed(sf::Event::KeyEvent const& event);
	bool					onReleased(sf::Event::KeyEvent const& event);

};

#endif
