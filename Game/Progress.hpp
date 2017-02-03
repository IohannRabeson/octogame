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
class SteamAPI;
class Progress
{
public:
	enum class Language : std::size_t
	{
		fr,
		en
	};

	enum RespawnType
	{
		Portal = 0,
		Die = 1
	};

	enum Difficulty
	{
		Normal,
		Hard
	};

	enum class MenuType : std::size_t
	{
		Classic,
		Simple
	};

	static constexpr std::size_t					DeathMax = 20.f;
	static constexpr std::size_t					RandomPortalMax = 17u;

	static Progress &								getInstance(void);
	static SteamAPI &								getSteamInstance(void);
	~Progress() = default;

	float											getTimePlayed() const { return m_data.timePlayed; }
	bool											isMenu(void) const;
	void											setIntro(bool isIntro);
	bool											isIntro(void) const;
	void											setMenu(bool isMenu);
	bool											isBubbleNpc(void) const;
	void											setBubbleNpc(bool isBubbleNpc);
	MenuType										getMenuType(void);
	void											setMenuType(MenuType type);
	bool											isGameFinished() const;
	void											setGameFinished(bool finish);
	void											increaseLaunchCount(void);
	void											setLongIntro(bool longIntro);
	void											setTryToEscape(bool tryToEscape);
	void											setDoorFound(bool doorFound);
	void											setBlueEnd(bool finish);
	void											setRedEnd(bool finish);
	void											increaseJumpCount(void);

	void											setLanguage(Language language);
	Progress::Language								getLanguage(void) const;
	ResourceKey										getTextFile(void) const;
	bool											isJoystick(void) const;

	void											setDifficulty(Difficulty difficulty);
	Progress::Difficulty							getDifficulty(void) const;

	void											addNanoRobot();
	void											removeNanoRobot() { m_data.nanoRobotCount--; }
	void											setNanoRobotCount(std::size_t count);
	std::size_t										getNanoRobotCount() const { return m_data.nanoRobotCount; }

	void											addSpirit();
	void											removeSpirit() { m_data.spiritCount--; }
	void											setSpiritCount(std::size_t count);
	std::size_t										getSpiritCount() const { return m_data.spiritCount; }

	void											setNextDestination(Level const & destination, bool hasTransition = true);
	Level											getNextDestination(void) const;

	void											setCurrentDestination(Level destination);
	Level											getCurrentDestination(void) const;

	void											setLastDestination(Level destination);
	Level											getLastDestination(void) const;

	void											walk(void) { m_data.walk = true; };
	void											moveMap(void) { m_data.moveMap = true; };

	bool											getWalk(void) const { return m_data.walk; }
	bool											getMoveMap(void) const { return m_data.moveMap; }

	bool											isFirstTime(void) const { return m_data.firstTime; }
	void											setFirstTime(bool firstTime) { m_data.firstTime = firstTime; }

	bool											isFirstTimeInIceA(void) const { return m_data.firstTimeInIceA; }
	void											setFirstTimeInIceA(bool firstTime) { m_data.firstTimeInIceA = firstTime; }

	void											setCanOpenDoubleJump(bool canOpen) { m_data.canOpenDoubleJump = canOpen; }
	bool											canOpenDoubleJump(void) const { return m_data.canOpenDoubleJump; }

	void											spaceShipRepair(bool isRepair) { m_data.spaceShipRepair = isRepair; }
	bool											spaceShipIsRepair() const { return m_data.spaceShipRepair; }

	float											getMusicVolume() const { return m_data.musicVol; }
	void											setMusicVolume(float volume) { m_data.musicVol = volume; }
	float											getSoundVolume() const { return m_data.soundVol; }
	void											setSoundVolume(float volume) { m_data.soundVol = volume; }
	void											setGlobalVolume(float volume) { m_data.globalVol = volume; }

	void											validateChallenge(ChallengeManager::Effect effect);
	bool											isValidateChallenge(ChallengeManager::Effect effect);

	void											setMapMoving(bool isMoving) { m_isMapMoving = isMoving; }
	bool											isMapMoving(void) const { return m_isMapMoving; }
	void											setIsResourceLoading(bool isLoading) { m_isResourceLoading = isLoading; }
	bool											isResourceLoading(void) const { return m_isResourceLoading; }

	void											setForceMapToMove(bool value) { m_forceMapToMove = value; }
	bool											forceMapToMove(void) const { return m_forceMapToMove; }

	void											setCanOctoMoveMap(bool value) { m_canOctoMoveMap = value; }
	bool											canOctoMoveMap(void) const { return m_canOctoMoveMap; }

	bool											canMoveMap();
	bool											canRepair();
	bool											canRepairShip();
	bool											canWalk();
	bool											canJump();
	bool											canDoubleJump();
	bool											canSlowFall();
	bool											canUseElevator();
	bool											canUseWaterJump();
	bool											canUseBalle();
	bool											changeLevel() const;
	void											levelChanged();

	void											registerLevel(Level const & biome);
	std::vector<Level> const &						getRegisteredLevels(void) const;

	void											resetCheckpoint(std::size_t id);
	void											setCheckpointCountMax(std::size_t count);
	std::size_t										getCheckpointCountMax(void);
	std::size_t										getCheckpointCount(void);
	bool											isCheckpointValidated(std::size_t id);
	void											validateCheckpoint(std::size_t id);
	void											registerDeath(sf::Vector2f const & position);
	std::vector<sf::Vector2i> &						getDeathPos(void);
	std::size_t										getDeathCount(void);
	std::size_t										getDeathLevelCount(void);
	void											resetDeathLevel(void);

	void											registerNpc(GameObjectType key);
	bool											meetNpc(GameObjectType key);
	std::size_t										getNpcCount();
	std::size_t										getNpcMax();
	std::list<GameObjectType>						getNpcMet();

	void											registerPortal(Level destination);
	bool											meetPortal(Level destination);
	bool											meetPortal(Level source, Level destination);
	std::size_t										countRandomDiscover(void);
	bool											isMetPortal(Level destination);
	bool											isMetRandom(Level destination);
	void											setRandomDiscoverCount(std::size_t count) { m_countRandomDiscover = count; };
	void											setPortalPosition(Level destination, sf::Vector2f const & position);
	void											removePortalPosition(Level destination);
	sf::Vector2f									getInterestPoint();
	void											setActivatedMonolith(std::size_t count);
	std::size_t										getActivatedMonolith(void) const;
	bool											isMonolithImploded(void) { return m_data.monolithImploded; };
	void											setMonolithImploded(bool imploded) { m_data.monolithImploded = imploded; };
	sf::Vector2f const &							getMonolithCenter(void) { return m_centerMonolith; };
	void											setMonolithCenter(sf::Vector2f const & center) { m_centerMonolith = center; };


	void											setMapHighlight(bool isHighlight);
	bool											isMapHighlight(void) const;
	std::map<std::string, octo::Array3D<Tile>> &	getMapsTile(void);
	std::map<std::string, octo::Array3D<float>> &	getMapsHighlight(void);
	void											setIsOctoOnInstance(bool isInstance);
	bool											isOctoOnInstance(void);
	void											setRespawnType(RespawnType type);
	RespawnType										getRespawnType(void) const;
	void											setCheckPointPosition(sf::Vector2f const & position);
	sf::Vector2f const &							getCheckPointPosition(void) const;
	void											setLevelOfDetails(int levelOfDetails);
	int												getLevelOfDetails(void) const;

	void											setOctoPos(sf::Vector2f const & position) { m_octoPos = position; }
	sf::Vector2f const&								getOctoPos() const { return m_octoPos; }
	void											setOctoPosTransition(sf::Vector2f const & position) { m_octoPosTransition = position; }
	sf::Vector2f const&								getOctoPosTransition() const { return m_octoPosTransition; }
	void											setKillOcto(bool killOcto) { m_killOcto = killOcto; }
	bool											getKillOcto(void) { return m_killOcto; }
	void											setOctoDoubleJump(bool isDoubleJump) { m_isDoubleJump = isDoubleJump; }
	bool											getOctoDoubleJump(void) { return m_isDoubleJump; }
	void											setInCloud(bool inCloud, std::size_t cloudId);
	bool											isInCloud(void) const { return m_isInCloud; }
	void											setEasyUnlocked(bool unlock) { m_data.isEasyUnlocked = unlock; };
	bool											isEasyUnlocked(void) const { return m_data.isEasyUnlocked; }

	void											setBalleMultiplier(float multiplier);
	float											getBalleMultiplier(void);

	std::size_t										getProgression(void);

	void											setReverseSprite(bool reverse) { m_reverseSprite = reverse; }
	bool											getReverseSprite() const { return m_reverseSprite; }

	void											updateSteam(sf::Time frameTime);
	void											load(std::string const & filename);
	void											save(float timePlayed = 0.f);
	void											reset();

	struct data
	{
		data() :
			data(0u, 0u, Level::IceA, 30u, 100u, 100u, true, true, Language::fr, Difficulty::Normal)
		{}

		data(std::size_t nanoRobot, std::size_t spirit, Level biome,
				std::size_t musicVol, std::size_t soundVol, std::size_t globalVol,
				bool fullscreen, bool vsync, Language language, Difficulty difficulty) :
			timePlayed(0.f),
			launchCount(0u),
			isGameFinished(false),
			isGameFinishedHard(false),
			isGameFinishedZeroDeath(false),
			isBlueEnd(false),
			isRedEnd(false),
			validateChallenge(0u),
			nanoRobotCount(nanoRobot),
			spiritCount(spirit),
			npcCount(0u),
			jumpCount(0u),
			nextDestination(biome),
			currentDestination(biome),
			lastDestination(biome),
			musicVol(musicVol),
			soundVol(soundVol),
			globalVol(globalVol),
			fullscreen(fullscreen),
			vsync(vsync),
			language(language),
			difficulty(difficulty),
			menuType(MenuType::Classic),
			firstTime(true),
			firstTimeInIceA(true),
			walk(false),
			moveMap(false),
			canOpenDoubleJump(false),
			deathCount(0u),
			respawnType(Progress::RespawnType::Portal),
			checkpoints(0u),
			activatedMonolith(0u),
			monolithImploded(false),
			levelOfDetails(0),
			spaceShipRepair(false),
			longIntro(false),
			tryToEscape(false),
			doorFound(false),
			isEasyUnlocked(false)
		{}

		float					timePlayed;
		std::size_t				launchCount;
		bool					isGameFinished;
		bool					isGameFinishedHard;
		bool					isGameFinishedZeroDeath;
		bool					isBlueEnd;
		bool					isRedEnd;
		sf::Vector2f			checkPointPosition;
		std::size_t				validateChallenge;
		std::size_t				nanoRobotCount;
		std::size_t				spiritCount;
		std::size_t				npcCount;
		std::size_t				jumpCount;
		Level					nextDestination;
		Level					currentDestination;
		Level					lastDestination;
		std::size_t				musicVol;
		std::size_t				soundVol;
		std::size_t				globalVol;
		bool					fullscreen;
		bool					vsync;
		Language				language;
		Difficulty				difficulty;
		MenuType				menuType;
		bool					firstTime;
		bool					firstTimeInIceA;
		bool					walk;
		bool					moveMap;
		bool					canOpenDoubleJump;
		char					npc[10000];
		char					portals[10000];
		char					deaths[20000];
		std::size_t				deathCount;
		Progress::RespawnType	respawnType;
		std::size_t				checkpoints;
		std::size_t				activatedMonolith;
		bool					monolithImploded;
		int						levelOfDetails;
		bool					spaceShipRepair;
		bool					longIntro;
		bool					tryToEscape;
		bool					doorFound;
		bool					isEasyUnlocked;
	};

private:
	Progress();
	void											init();
	void											saveToFile();
	void											setup();
	void											saveNpc();
	void											loadNpc();
	void											savePortals();
	void											loadPortals();
	void											saveDeaths();
	void											loadDeaths();
	void											split(const std::string &s, char delim, std::vector<std::string> &elems);

	static std::unique_ptr<Progress>				m_instance;
	static std::unique_ptr<SteamAPI>				m_steam;

	bool											m_isMenu;
	bool											m_isIntro;
	bool											m_isBubbleNpc;
	std::string										m_filename;
	data											m_data;
	bool											m_newSave;
	bool											m_changeLevel;
	bool											m_reverseSprite;
	sf::Vector2f									m_octoPos;
	sf::Vector2f									m_octoPosTransition;
	bool											m_killOcto;
	bool											m_isDoubleJump;
	bool											m_isInCloud;
	std::size_t										m_cloudId;

	std::map<Level, std::map<GameObjectType, bool>>	m_npc;
	std::size_t										m_npcMax;
	std::map<Level, std::vector<sf::Vector2i>>		m_deaths;
	std::size_t										m_deathsLevelCount;
	std::map<Level, std::map<Level, bool>>			m_portals;
	std::map<Level, sf::Vector2f>					m_portalsToDiscover;
	std::vector<Level>								m_levels;
	std::size_t										m_countRandomDiscover;
	std::map<std::string, octo::Array3D<Tile>>		m_mapsTile;
	std::map<std::string, octo::Array3D<float>>		m_mapsHighlight;
	std::size_t										m_checkpointCountMax;

	bool											m_isOctoOnInstance;
	bool											m_isHighLight;
	bool											m_isMapMoving;
	bool											m_canOctoMoveMap;
	bool											m_forceMapToMove;

	float											m_balleMultiplier;
	sf::Time										m_timerSteamUpdate;
	bool											m_isResourceLoading;
	sf::Vector2f									m_centerMonolith;
};

#endif
