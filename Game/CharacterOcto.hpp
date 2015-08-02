/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CharacterOcto.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/23 00:32:59 by irabeson          #+#    #+#             */
/*   Updated: 2015/07/23 03:25:40 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHARACTEROCTO_HPP
# define CHARACTEROCTO_HPP
# include <CharacterSprite.hpp>
# include <CharacterAnimation.hpp>
# include <DefaultGraphicsListeners.hpp>
# include "AGameObject.hpp"
# include "RectangleShape.hpp"

# include <SFML/Graphics/Drawable.hpp>

# include <array>

class CharacterOcto : public AGameObject<GameObjectType::Player>,
					  public octo::DefaultKeyboardListener,
					  public sf::Drawable
{
	enum Events
	{
		Idle,
		Right,
		Left,
		Jump,
		DoubleJump,
		Fall,
		Dance,
		Umbrella,
		Elevator,
		Death,
		Drink,
	};
public:
	CharacterOcto();

	void			update(sf::Time frameTime);
	void			draw(sf::RenderTarget& render, sf::RenderStates states = sf::RenderStates())const;

	bool			onPressed(sf::Event::KeyEvent const& event);
	bool			onReleased(sf::Event::KeyEvent const& event);
	sf::Vector2f	getPosition()const;
	void			onCollision(GameObjectType type);
private:
	void	setupAnimation();
	void	setupMachine();
	void	commitControlsToPhysics(sf::Time frameTime);
	void	commitPhysicsToGraphics();
	void	dance();
private:
	octo::CharacterSprite		m_sprite;
	octo::CharacterAnimation	m_idleAnimation;
	octo::CharacterAnimation	m_walkAnimation;
	octo::CharacterAnimation	m_jumpAnimation;
	octo::CharacterAnimation	m_fallAnimation;
	octo::CharacterAnimation	m_danceAnimation;
	octo::CharacterAnimation	m_umbrellaAnimation;
	octo::CharacterAnimation	m_elevatorAnimation;
	octo::CharacterAnimation	m_deathAnimation;
	octo::CharacterAnimation	m_drinkAnimation;
	RectangleShape*				m_box;
	float						m_jumpVelocity;
	sf::Clock					m_clockAFK;
	sf::Clock					m_clockCollision;
	float						m_pixelSecond;
	bool						m_originMoove;
	bool						m_canDoubleJump;
	bool						m_doubleJump;
	bool						m_onGround;
	bool						m_afterJump;
	float						m_prevY;
	std::array<bool, 3>			m_controls;
};

#endif
