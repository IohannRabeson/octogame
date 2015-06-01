/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AnimatedSpriteDemoScreen.hpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/23 15:30:14 by irabeson          #+#    #+#             */
/*   Updated: 2015/05/23 15:39:51 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANIMATEDSPRITEDEMOSCREEN_HPP
# define ANIMATEDSPRITEDEMOSCREEN_HPP
# include <AbstractState.hpp>
# include <AnimatedSprite.hpp>
# include <DefaultGraphicsListeners.hpp>

# include <SFML/Graphics/Sprite.hpp>

class AnimatedSpriteDemoScreen : public octo::AbstractState,
							 	 public octo::DefaultKeyboardListener
{
public:
	AnimatedSpriteDemoScreen();
	~AnimatedSpriteDemoScreen();

	virtual void	start();
	virtual void	pause();
	virtual void	resume();
	virtual void	stop();

	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render)const;

	void			setSpriteSheet(octo::SpriteSheet const& spriteSheet);
	void			setAnimation(octo::SpriteAnimation const& animation);
	bool			onPressed(sf::Event::KeyEvent const& event);
private:
	octo::AnimatedSprite		m_sprite;
	octo::SpriteAnimation		m_animation;
};

#endif
