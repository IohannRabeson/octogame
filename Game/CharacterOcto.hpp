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
		Left,
		Right
	};
public:
	CharacterOcto();

	void			update(sf::Time frameTime);
	void			draw(sf::RenderTarget& render, sf::RenderStates states = sf::RenderStates())const;

	bool			onPressed(sf::Event::KeyEvent const& event);
	bool			onReleased(sf::Event::KeyEvent const& event);
	sf::Vector2f	getPosition()const;
private:
	void	commitControlsToPhysics();
	void	commitPhysicsToGraphics();
private:
	octo::CharacterSprite		m_sprite;
	octo::CharacterAnimation	m_idleAnimation;
	octo::CharacterAnimation	m_walkAnimation;
	RectangleShape*				m_box;
	std::array<bool, 2>			m_controls;
};

#endif
