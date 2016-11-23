#ifndef INTROSCREEN_HPP
# define INTROSCREEN_HPP
# include <AbstractState.hpp>

# include "InputListener.hpp"
# include "StarSystem.hpp"
# include "MainMenu.hpp"
# include "SkyCycle.hpp"
# include "BiomeManager.hpp"
# include "SkyManager.hpp"
# include "MusicManager.hpp"
# include "ParallaxScrolling.hpp"
# include "SpaceShip.hpp"

class CameraMovement;

class IntroScreen : public octo::AbstractState, public InputListener
{
public:
	IntroScreen(void);

	virtual void		start();
	virtual void		pause();
	virtual void		resume();
	virtual void		stop();
	virtual void		update(sf::Time frameTime);
	virtual void		draw(sf::RenderTarget& render) const;

private:
	enum State
	{
		Fly,
		Crash,
		End,
		None
	};

	void			setupScene(void);
	void			updateSpaceShip(sf::Time frameTime);
	void			updateShaders(sf::Time frameTime);
	void			updateAlarm(sf::Time frameTime);
	sf::Vector2f	computeShake(sf::Time frameTime);
	void			shake(float duration, float intensity, float waveDuration);
	void			updateCamera(sf::Time frameTime);
	void			updateScene(sf::Time frametime);
	void			drawScene(sf::RenderTarget& render) const;

	virtual bool	onInputPressed(InputListener::OctoKeys const & key);
	virtual bool	onInputReleased(InputListener::OctoKeys const & key);

	State								m_state;
	bool								m_keyJump;
	bool								m_keyUp;
	bool								m_keyDown;
	bool								m_keyRight;
	bool								m_keyLeft;
	bool								m_ticJump;
	std::size_t							m_cloudCollidedCount;
	std::size_t							m_cloudCollidedCountMax;
	sf::Time							m_timerBeforeCrash;
	sf::Time							m_timerBeforeCrashMax;
	sf::Time							m_timerJump;
	sf::Time							m_timerJumpMax;
	sf::Time							m_timerFall;
	sf::Time							m_timerFallMax;
	float								m_cloudMinY;
	float								m_cloudMaxY;
	float								m_speedFall;
	float								m_speedYUp;

	MainMenu							m_menu;
	bool								m_doSave;
	MusicManager &						m_musicPlayer;
	BiomeManager						m_biomeManager;
	std::unique_ptr<SkyCycle>			m_skyCycle;
	std::unique_ptr<SkyManager>			m_skyManager;
	std::unique_ptr<ParallaxScrolling>	m_parallaxScrolling;

	std::size_t							m_starsCount;
	std::unique_ptr<StarSystem[]>		m_stars;
	SpaceShip							m_spaceShip;
	float								m_speedX;
	sf::Time							m_timerCamera;
	sf::Time							m_timerCameraMax;

	sf::Time							m_timer;
	sf::Time							m_timerMax;
	sf::Time							m_timerStartRedAlarm;
	sf::Time							m_timerStartRedAlarmMax;
	sf::Time							m_timerEnd;
	sf::Time							m_timerEndMax;
	sf::Time							m_timerBlinkShader;
	bool								m_blinkShaderState;
	sf::Time							m_timerShaderRocket;
	sf::Time							m_timerShaderRocketAppear;
	sf::Time							m_timerShaderRocketAppearMax;
	bool								m_shaderRocketState;

	Noise								m_noise;
	sf::Vector2f						m_prevShakeOffset;
	sf::Vector2f						m_shakeOffset;
	sf::Time							m_waveTimer;
	sf::Time							m_shakeTimer;
	sf::Time							m_shakeDuration;
	sf::Time							m_waveDuration;
	float								m_shakeIntensity;
};

#endif
