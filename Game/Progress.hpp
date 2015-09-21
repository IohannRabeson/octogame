#ifndef PROGRESS_HPP
# define PROGRESS_HPP
# include "GroundManager.hpp"
# include <SFML/System/Vector2.hpp>
# include <string>
# include <memory>

class ABiome;
class Progress
{
public:
	static Progress & getInstance(void);

	inline void			addNanoRobot() { m_data.nanoRobotCount++; }
	inline void			removeNanoRobot() { m_data.nanoRobotCount--; }
	inline std::size_t	getNanoRobotCount() { return m_data.nanoRobotCount; }

	void				setNextDestination(Level destination);
	Level				getNextDestination(void) const;

	bool				canMoveMap();
	bool				canRepair();
	bool				canWalk();
	bool				canJump();
	bool				canDoubleJump();
	bool				canSlowFall();
	bool				canUseElevator();
	bool				changeLevel() const;
	void				levelChanged();

	inline bool			musicEnabled(){ return m_data.music; }
	inline std::size_t	getMusicVolume(){ return m_data.musicVol; }
	inline void			setMusic(bool music){ m_data.music = music; }
	inline void			setMusicVolume(std::size_t volume){ m_data.musicVol = volume; }

	inline bool			soundEnabled(){ return m_data.sound; }
	inline std::size_t	getSoundVolume(){ return m_data.soundVol; }
	inline void			setSound(bool sound){ m_data.sound = sound; }
	inline void			setSoundVolume(std::size_t volume){ m_data.soundVol = volume; }

	inline bool			fullscreenEnabled(){ return m_data.fullscreen; }
	inline bool			vsyncEnabled(){ return m_data.vsync; }
	inline void			setFullscreen(bool fullscreen){ m_data.fullscreen = fullscreen; }
	inline void			setVsync(bool vsync){ m_data.vsync = vsync; }

	void				load(std::string const & filename);
	void				save();
	void				reset();

private:
	Progress();
	void	init();
	void	saveToFile();
	void	setup();
	struct data{
		data() :
			data(5u, Level::Default,
					10u, 10u, true, true,
					true, true)
		{
		}
		data(std::size_t nanoRobot, Level biome,
				std::size_t musicVol, std::size_t soundVol, bool music, bool sound,
				bool fullscreen, bool vsync) :
			nanoRobotCount(nanoRobot),
			destination(biome),
			musicVol(musicVol),
			soundVol(soundVol),
			music(music),
			sound(sound),
			fullscreen(fullscreen),
			vsync(vsync)
		{
		}
		std::size_t		nanoRobotCount;
		Level			destination;
		std::size_t		musicVol;
		std::size_t		soundVol;
		bool			music;
		bool			sound;
		bool			fullscreen;
		bool			vsync;

	};

	static std::unique_ptr<Progress>		m_instance;
	std::string								m_filename;
	data									m_data;
	bool									m_newSave;
	bool									m_changeLevel;

};

#endif
