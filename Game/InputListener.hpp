#ifndef INPUTLISTENER_HPP
# define INPUTLISTENER_HPP

# include <DefaultGraphicsListeners.hpp>

class InputListener : public octo::DefaultKeyboardListener,
					  public octo::DefaultJoystickListener
{
public:
	enum class OctoKeys : std::size_t
	{
		Left = 0,
		Right = 1,
		Space = 2,
		Up = 3,
		Down = 4,
		Escape = 5,
		Return = 6,
		Use = 7,
		SlowFall = 8,
		GroundLeft = 9,
		GroundRight = 10,
		Infos = 11,
		None = 12
	};

	InputListener(void);
	virtual ~InputListener(void) = default;

	virtual bool		onInputPressed(InputListener::OctoKeys const & key);
	virtual bool		onInputReleased(InputListener::OctoKeys const & key);
	void				addInputListener(void);
	void				removeInputListener(void);

private:
	std::vector<OctoKeys>	m_inputs;
	bool					m_joystickLT;
	bool					m_joystickRT;
	bool					m_joystickAxisX;
	bool					m_joystickAxisY;
	float					m_triggerLimit;

	// Keyboard
	virtual bool		onPressed(sf::Event::KeyEvent const& event);
	virtual bool		onReleased(sf::Event::KeyEvent const& event);

	// Joystick
	virtual void		onMoved(sf::Event::JoystickMoveEvent const& event);
	virtual void		onPressed(sf::Event::JoystickButtonEvent const& event);
	virtual void		onReleased(sf::Event::JoystickButtonEvent const& event);
};

#endif
