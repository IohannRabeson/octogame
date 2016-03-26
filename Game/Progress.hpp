#ifndef PROGRESS_HPP
# define PROGRESS_HPP
# include "GroundManager.hpp"
# include "ResourceDefinitions.hpp"
# include "ABiome.hpp"
# include <SFML/System/Vector2.hpp>
# include <string>
# include <memory>

class ABiome;
class Progress
{
public:
	static Progress & getInstance(void);

	void				addNanoRobot();
	void				removeNanoRobot() { m_data.nanoRobotCount--; }
	void				setNanoRobotCount(std::size_t count);
	std::size_t			getNanoRobotCount() const { return m_data.nanoRobotCount; }

	void				setNextDestination(Level const & destination, bool hasTransition = true);
	Level				getNextDestination(void) const;

	void				setLastDestination(Level destination);
	Level				getLastDestination(void) const;

	void				walk(void) { m_data.walk = true; };
	void				moveMap(void) { m_data.moveMap = true; };

	bool				getWalk(void) const { return m_data.walk; }
	bool				getMoveMap(void) const { return m_data.moveMap; }

	bool				isFirstTime(void) const { return m_data.firstTime; }
	void				setFirstTime(bool firstTime) { m_data.firstTime = firstTime; }

	void				startChallenge(void) { m_validChallenge = true; }
	bool				canValidChallenge(void) const { return m_validChallenge; }
	void				endChallenge(void) { m_validChallenge = false; }

	void				setCanOpenDoubleJump(bool canOpen) { m_data.canOpenDoubleJump = canOpen; }
	bool				canOpenDoubleJump(void) const { return m_data.canOpenDoubleJump; }

	void				spaceShipRepair(bool isRepair) { m_spaceShipRepair = isRepair; }
	bool				spaceShipIsRepair() const { return m_spaceShipRepair; }

	float				getMusicVolume() const { return m_data.musicVol; }
	void				setMusicVolume(float volume) { m_data.musicVol = volume; }
	void				setSoundVolume(float volume) { m_data.soundVol = volume; }

	bool				canMoveMap();
	bool				canRepair();
	bool				canRepairShip();
	bool				canWalk();
	bool				canJump();
	bool				canDoubleJump();
	bool				canSlowFall();
	bool				canUseElevator();
	bool				canUseWaterJump();
	bool				changeLevel() const;
	void				levelChanged();

	void				registerLevel(Level const & biome);
	std::vector<Level> const & getRegisteredLevels(void) const;

	void				setOctoPos(sf::Vector2f const & position) { m_octoPos = position; }
	sf::Vector2f const&	getOctoPos() const { return m_octoPos; }

	void				setReverseSprite(bool reverse) { m_reverseSprite = reverse; }
	bool				getReverseSprite() const { return m_reverseSprite; }

	void				registerNpc(ResourceKey const & key);
	void				meetNpc(ResourceKey const & key);
	void				printNpc(void);

	void				load(std::string const & filename);
	void				save();
	void				reset();

private:
	struct data
	{
		data() :
			data(0u, Level::IceA, 6u, 100u, true, true)
		{}

		data(std::size_t nanoRobot, Level biome,
				std::size_t musicVol, std::size_t soundVol,
				bool fullscreen, bool vsync) :
			nanoRobotCount(nanoRobot),
			nextDestination(biome),
			lastDestination(biome),
			musicVol(musicVol),
			soundVol(soundVol),
			fullscreen(fullscreen),
			vsync(vsync),
			firstTime(true),
			walk(false),
			moveMap(false),
			canOpenDoubleJump(false)
		{}

		std::size_t		nanoRobotCount;
		Level			nextDestination;
		Level			lastDestination;
		std::size_t		musicVol;
		std::size_t		soundVol;
		bool			fullscreen;
		bool			vsync;
		bool			firstTime;
		bool			walk;
		bool			moveMap;
		bool			canOpenDoubleJump;
		char			npc[10000];
	};

	Progress();
	void				init();
	void				saveToFile();
	void				setup();
	void				saveNpc();
	void				loadNpc();
	void				split(const std::string &s, char delim, std::vector<std::string> &elems);

	static std::unique_ptr<Progress>				m_instance;
	std::string										m_filename;
	data											m_data;
	bool											m_newSave;
	bool											m_changeLevel;
	bool											m_reverseSprite;
	bool											m_validChallenge;
	bool											m_spaceShipRepair;
	sf::Vector2f									m_octoPos;

	std::map<Level, std::map<ResourceKey, bool>>	m_npc;
	std::vector<Level>								m_levels;
};

#endif
