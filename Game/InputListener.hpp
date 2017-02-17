#ifndef INPUTLISTENER_HPP
# define INPUTLISTENER_HPP

# include <DefaultGraphicsListeners.hpp>
# include <vector>

class InputListener : public octo::DefaultKeyboardListener,
					  public octo::DefaultJoystickListener
{
public:
	//TODO: Rename those keys
	enum class OctoKeys : std::size_t
	{
		Left = 0,
		Right = 1,
		Jump = 2,
		Up = 3,
		Down = 4,
		Menu = 5,
		SelectMenu = 6,
		Elevator = 7,
		Capacity = 8,
		GroundLeft = 9,
		GroundRight = 10,
		Entrance = 11,
		ViewLeft = 12,
		ViewRight = 13,
		ViewUp = 14,
		ViewDown = 15,
		None = 16,
		Zoom = 17,
		ShowOcto = 18,
		HideOcto = 19,
		IncreaseSpeed = 20,
		DecreaseSpeed = 21,
		Balle = 22
	};

	InputListener(void);
	virtual ~InputListener(void) = default;

	virtual bool		onInputPressed(InputListener::OctoKeys const & key);
	virtual bool		onInputReleased(InputListener::OctoKeys const & key);
	void				addInputListener(void);
	void				removeInputListener(void);

private:
	bool					m_isListeners;
	std::vector<OctoKeys>	m_inputs;
	bool					m_joystickLT;
	bool					m_joystickRT;
	bool					m_joystickAxisX;
	bool					m_joystickAxisY;
	bool					m_joystickAxisU;
	bool					m_joystickAxisV;
	bool					m_isShiftPressed;
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
