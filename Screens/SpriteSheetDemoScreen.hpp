/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SpriteSheetDemoScreen.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/23 01:24:48 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/18 21:15:32 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPRITESHEETDEMOSCREEN_HPP
# define SPRITESHEETDEMOSCREEN_HPP
# include <AbstractState.hpp>
# include <SpriteSheet.hpp>
# include <DefaultGraphicsListeners.hpp>

# include <SFML/Graphics/Sprite.hpp>

class SpriteSheetDemoScreen : public octo::AbstractState,
							  public octo::DefaultKeyboardListener
{
public:
	SpriteSheetDemoScreen();
	~SpriteSheetDemoScreen();

	virtual void	start();
	virtual void	pause();
	virtual void	resume();
	virtual void	stop();

	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render)const;

	void			setSpriteSheet(octo::SpriteSheet const* spriteSheet);
	bool			onPressed(sf::Event::KeyEvent const& event);
private:
	octo::SpriteSheet const*	m_spriteSheet;
	sf::Sprite					m_sprite;
	std::size_t					m_subTextureId;
};

#endif
