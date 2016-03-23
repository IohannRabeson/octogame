#ifndef INPUTLISTENER_HPP
# define INPUTLISTENER_HPP

#include <DefaultGraphicsListeners.hpp>

class InputListener : public octo::DefaultKeyboardListener,
					  public octo::DefaultJoystickListener
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
		Return,
		Use,
		SlowFall,
		GroundLeft,
		GroundRight
	};

	InputListener(void);
	~InputListener(void);

	virtual bool	onInputPressed(InputListener::OctoKeys const & key);
	virtual bool	onInputReleased(InputListener::OctoKeys const & key);
	void			addInputListener(void);
	void			removeInputListener(void);

private:
	// Keyboard
	virtual bool	onPressed(sf::Event::KeyEvent const& event);
	virtual bool	onReleased(sf::Event::KeyEvent const& event);

	// Joystick
	virtual void	onMoved(sf::Event::JoystickMoveEvent const& event);
	virtual void	onPressed(sf::Event::JoystickButtonEvent const& event);
	virtual void	onReleased(sf::Event::JoystickButtonEvent const& event);
};

#endif
