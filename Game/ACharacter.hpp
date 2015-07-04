/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACharacter.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/04 19:20:48 by irabeson          #+#    #+#             */
/*   Updated: 2015/07/14 21:21:32 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACHARACTER_HPP
# define ACHARACTER_HPP
# include "FiniteStateMachine.hpp"
# include "PhysicsEngine.hpp"

# include <AnimatedSprite.hpp>

class ACharacterState : public FiniteStateMachine::AState
{
public:
	ACharacterState(std::string const& name,
					octo::SpriteAnimation const& animation,
					octo::AnimatedSprite& sprite,
					RectangleShape* box);

	octo::AnimatedSprite&	getSprite();
	RectangleShape*			getBox();
private:
	octo::AnimatedSprite&			m_sprite;
	octo::SpriteAnimation const&	m_animation;
	RectangleShape* const			m_box;
};

class ACharacter
{
public:
	void			setSleep(bool sleep);
	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render, sf::RenderStates states)const;
protected:
	typedef FiniteStateMachine::EventId	EventId;

	/*!
	 *	This method must be called to setup a character.
	 *	\param spriteSheet Sprite sheet with all textures for the character.
	 *	\param machine FSM configured with your own ACharacterState.<br>
	 *	The states into this machine defines the behavior and the animation of the
	 *	character.
	 *	\param boxSize Physical box size
	 */
	void			setup(octo::SpriteSheet const& spriteSheet,
						  FiniteStateMachine&& machine,
						  sf::Vector2f const& boxSize);

	/*!	State the machine */
	void			start();

	/*!
	 *	Define the next event sent to the machine.<br>
	 *
	 *	Typically this method is called when something happens to your character.<br>
	 *	It can be a player input, or a collision with the ground or with an another game object.<br>
	 *	In your own character implementation, the best way seems to provides a
	 *	set of method, one for each event, within you call setEvent().
	 *	\code
	 *	class MyCharacter :	public IPlayerInputListener	// force to implement onJumpInput()
	 *						public IPhysicsListener		// force to implement onCollision()
	 *	{
	 *		enum MyEvents
	 *		{
	 *			Walk,
	 *			Idel,
	 *			Jump,
	 *			Die
	 *		};
	 *	public:
	 *		void	onJumpInput()
	 *		{
	 *			setEvent(Jump);
	 *		}
	 *
	 *		void	onCollision(...)
	 *		{
	 *			if ( *collide with an enemy* )
	 *			{
	 *				setEvent(Die);
	 *			}
	 *		}
	 *	};
	 *	\endcode
	 *	It can be has only one next event, if an event has been already defined it will be
	 *	replaced by the new one.<br>
	 *	If eventId is not an acceptable event for the current state this method
	 *	does nothing and return false.
	 *
	 */
	bool			setEvent(EventId eventId);

	/*!	Access to the physical box */
	RectangleShape*	getBox()const;
private:
	/*!	Refresh the graphical representation with the updated physical state */
	void			commitPhysicsToGraphics();
private:
	octo::AnimatedSprite	m_sprite;
	RectangleShape*			m_box;
	FiniteStateMachine		m_machine;
};

#endif
