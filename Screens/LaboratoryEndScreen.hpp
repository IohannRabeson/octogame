#ifndef LABORATORYENDSCREEN_HPP
# define LABORATORYENDSCREEN_HPP

# include "InputListener.hpp"
# include "DecorManager.hpp"
# include "ScientistOcto.hpp"
# include "TvLaboNpc.hpp"
# include <AbstractState.hpp>
# include <SFML/Graphics/Sprite.hpp>
# include <SFML/Graphics/Shader.hpp>
# include <memory>

class ScientistNpc;
class ABiome;

class LaboratoryEndScreen : public octo::AbstractState,
							public InputListener
{
public:
	enum State
	{
		Appear,
		Dialogs,
		CedricWalk,
		CedricPutPotion,
		VisualEffect,
		ChangeAquaColor,
		StartShaderEffect,
		DisappearShaderEffect,
		StopShaderEffect,
		Disappear
	};

	LaboratoryEndScreen(void);
	virtual void	start();
	virtual void	pause();
	virtual void	resume();
	virtual void	stop();
	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render)const;

	virtual bool	onInputPressed(InputListener::OctoKeys const & key);
	virtual bool	onInputReleased(InputListener::OctoKeys const & key);

private:
	std::vector<std::unique_ptr<ScientistNpc>>	m_npcs;
	std::vector<std::unique_ptr<TvLaboNpc>>		m_tvNpcs;
	State										m_state;
	ScientistOcto								m_octo;
	sf::Time									m_timer;
	sf::Time									m_globalTimer;
	sf::Time									m_timeBeforeNextText;
	sf::Time									m_appearDuration;
	sf::Time									m_cedricWalkTimer;
	sf::Time									m_cedricPutPotionTimer;
	sf::Time									m_changeColorAqua;
	sf::Time									m_changeColorAquaMax;
	sf::Time									m_appearTimerPostEffect;
	sf::Time									m_appearTimerPostEffectMax;
	sf::Time									m_startPostEffectDuration;
	sf::Time									m_startPostEffectDurationMax;
	sf::Time									m_disappearTimerPostEffect;
	sf::Time									m_disappearTimerPostEffectMax;
	sf::Time									m_endPostEffectDuration;
	sf::Time									m_endPostEffectDurationMax;
	sf::Sprite									m_background;
	sf::Sprite									m_water;
	sf::Sprite									m_foreground;
	std::size_t									m_textIndex;
	std::size_t									m_lastTextIndex;
	sf::Shader									m_shader;
	bool										m_stopDialog;
	std::unique_ptr<ABiome>						m_biome;
	DecorManager								m_decorManager;

};

#endif
