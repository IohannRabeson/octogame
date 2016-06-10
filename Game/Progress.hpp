#ifndef PROGRESS_HPP
# define PROGRESS_HPP
# include "GroundManager.hpp"
# include "ResourceDefinitions.hpp"
# include "AGameObject.hpp"
# include "ABiome.hpp"
# include "ChallengeManager.hpp"
# include <SFML/System/Vector2.hpp>
# include <string>
# include <memory>

class ABiome;
class Progress
{
public:
	enum class Language : std::size_t
	{
		fr,
		en
	};

	static Progress & getInstance(void);

	bool				isMenu() const;
	void				setMenu(bool isMenu);
	//TODO: Set this value once the game is finished
	bool				isGameFinished() const;

	void				setLanguage(Language language);
	Progress::Language	getLanguage(void) const;
	ResourceKey			getTextFile(void) const;
	bool				isJoystick(void) const;

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

	void				setCanOpenDoubleJump(bool canOpen) { m_data.canOpenDoubleJump = canOpen; }
	bool				canOpenDoubleJump(void) const { return m_data.canOpenDoubleJump; }

	void				spaceShipRepair(bool isRepair) { m_spaceShipRepair = isRepair; }
	bool				spaceShipIsRepair() const { return m_spaceShipRepair; }

	float				getMusicVolume() const { return m_data.musicVol; }
	void				setMusicVolume(float volume) { m_data.musicVol = volume; }
	void				setSoundVolume(float volume) { m_data.soundVol = volume; }

	void				validateChallenge(ChallengeManager::Effect effect);
	bool				isValidateChallenge(ChallengeManager::Effect effect);

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

	void				registerDeath(float deathPosX);
	std::vector<int> &	getDeathPos(void);

	void				registerNpc(GameObjectType key);
	bool				meetNpc(GameObjectType key);
	std::size_t			getNpcCount();
	std::size_t			getNpcMax();
	std::vector<GameObjectType>	getNpcMet();

	void				registerPortal(Level destination);
	bool				meetPortal(Level destination);
	bool				isMetPortal(Level destination);
	std::size_t			getPortalsCount();
	std::size_t			getPortalsMax();

	void				setGroundInfos(std::size_t current, std::size_t max, std::wstring sign);
	std::wstring const & getGroundInfos(void);
	void				setEnableGroundInfos(bool isEnable);
	bool				isEnableGroundInfos(void) const { return m_data.isGroundInfos; }
	void				setMapHighlight(bool isHighlight);
	bool				isMapHighlight(void) const;
	void				setIsOctoOnInstance(bool isInstance);
	bool				isOctoOnInstance(void);

	void				setOctoPos(sf::Vector2f const & position) { m_octoPos = position; }
	sf::Vector2f const&	getOctoPos() const { return m_octoPos; }
	void				setOctoPosTransition(sf::Vector2f const & position) { m_octoPosTransition = position; }
	sf::Vector2f const&	getOctoPosTransition() const { return m_octoPosTransition; }

	void				setReverseSprite(bool reverse) { m_reverseSprite = reverse; }
	bool				getReverseSprite() const { return m_reverseSprite; }

	void				load(std::string const & filename);
	void				save();
	void				reset();

private:
	struct data
	{
		data() :
			data(0u, Level::IceA, 6u, 100u, true, true, Language::fr)
		{}

		data(std::size_t nanoRobot, Level biome,
				std::size_t musicVol, std::size_t soundVol,
				bool fullscreen, bool vsync, Language language) :
			nanoRobotCount(nanoRobot),
			nextDestination(biome),
			lastDestination(biome),
			musicVol(musicVol),
			soundVol(soundVol),
			fullscreen(fullscreen),
			vsync(vsync),
			language(language),
			firstTime(true),
			walk(false),
			moveMap(false),
			canOpenDoubleJump(false),
			isGroundInfos(true)
		{}

		std::size_t		validateChallenge;
		std::size_t		nanoRobotCount;
		Level			nextDestination;
		Level			lastDestination;
		std::size_t		musicVol;
		std::size_t		soundVol;
		bool			fullscreen;
		bool			vsync;
		Language		language;
		bool			firstTime;
		bool			walk;
		bool			moveMap;
		bool			canOpenDoubleJump;
		char			npc[10000];
		char			portals[10000];
		bool			isGroundInfos;
	};

	Progress();
	void				init();
	void				saveToFile();
	void				setup();
	void				saveNpc();
	void				loadNpc();
	void				savePortals();
	void				loadPortals();
	void				split(const std::string &s, char delim, std::vector<std::string> &elems);

	static std::unique_ptr<Progress>				m_instance;
	bool											m_isMenu;
	bool											m_isGameFinished;
	std::string										m_filename;
	data											m_data;
	bool											m_newSave;
	bool											m_changeLevel;
	bool											m_reverseSprite;
	bool											m_spaceShipRepair;
	sf::Vector2f									m_octoPos;
	sf::Vector2f									m_octoPosTransition;

	std::map<Level, std::map<GameObjectType, bool>>	m_npc;
	std::size_t										m_npcCount;
	std::size_t										m_npcMax;
	std::map<Level, std::vector<int>>				m_deathPos;
	std::map<Level, std::map<Level, bool>>			m_portals;
	std::size_t										m_portalsCount;
	std::size_t										m_portalsMax;
	std::vector<Level>								m_levels;

	std::wstring									m_groundInfos;
	bool											m_isOctoOnInstance;
	bool											m_isHighLight;
};

#endif
