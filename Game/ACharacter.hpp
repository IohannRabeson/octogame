/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACharacter.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/04 19:20:48 by irabeson          #+#    #+#             */
/*   Updated: 2015/07/15 20:58:15 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACHARACTER_HPP
# define ACHARACTER_HPP
# include "FiniteStateMachine.hpp"
# include "PhysicsEngine.hpp"

# include <AnimatedSprite.hpp>

/*!
 *	\class ACharacter
 *	\brief Base of a character
 */
class ACharacter
{
public:
	virtual ~ACharacter() = 0;

	void			setSleep(bool sleep);
	void			update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render, sf::RenderStates states)const;
protected:
	friend class						CharacterState;
	class								CharacterState;
	typedef FiniteStateMachine::EventId	EventId;

	/*!
	 *	This method must be called to setup the character sprite sheet.
	 *	\param spriteSheet Sprite sheet with all textures for the character.
	 */
	void			setupSpriteSheet(octo::SpriteSheet const& spriteSheet);

	/*!
	 *	This method must be called to setup the physical representation of the character .
	 *	\param boxSize Size of the rectangle used as physics representation.
	 */
	void			setupPhysicsBox(sf::Vector2f const& boxSize);

	/*!	This method must be called to setup the finite state machine to
	 *	manage the animations.
	 */
	void			setupMachine(FiniteStateMachine const& machine);

	/*!	State the machine */
	void			start();

	/*!
	 *	Define the next event sent to the machine.<br>
	 *
	 *	Typically this method is called when something happens to your character.<br>
	 *	It can be a player input, or a collision with the ground or with an another
	 *	game object.<br>
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
	/*!	This method allows user to customize character updating.
	 *
	 *	This method is called after animation update, and before
	 *	refreshing graphics from physical datas.<br>
	 *	This is the good place to update the logic and the physical
	 *	components of your character.
	 */
	virtual void	updateCharacter(sf::Time frameTime);

	/*!	Refresh the graphical representation with the updated physical state */
	void			commitPhysicsToGraphics();
private:
	octo::AnimatedSprite	m_sprite;
	RectangleShape*			m_box;
	FiniteStateMachine		m_machine;
};

/*!
 *	Basic implementation of a state for an animated character.
 *
 *	The methods are implemented to manage the animation of the sprite.
 */
class ACharacter::CharacterState : public FiniteStateMachine::AState
{
public:
	CharacterState(std::string const& name,
				   octo::SpriteAnimation const& animation,
				   ACharacter* character);
	
	/*!	When started, this state begin the animation defined */
	virtual void					start();
	
	/*!	When stopped, this state break the current animation */
	virtual void					stop();

	/*!	When updated, this state update the state animation */
	virtual void					update(sf::Time frameTime);
protected:
	octo::SpriteAnimation const&	getAnimation();
	octo::AnimatedSprite&			getSprite();
	RectangleShape*					getBox();
private:
	octo::AnimatedSprite&			m_sprite;
	octo::SpriteAnimation const&	m_animation;
	RectangleShape* const			m_box;
};

#endif
