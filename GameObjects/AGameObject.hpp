#ifndef AGAMEOBJECT_HPP
# define AGAMEOBJECT_HPP
# include <type_traits>

enum class GameObjectType : std::size_t
{
	// Need mask
	Player						= 1 << 0,
	PlayerEvent					= 1 << 1,
	Tile						= 1 << 2,
	Decor						= 1 << 3,
	Portal						= 1 << 4,
	Elevator					= 1 << 5,
	GroundTransformNanoRobot	= 1 << 6,
	RepairNanoRobot				= 1 << 7,
	JumpNanoRobot				= 1 << 8,
	DoubleJumpNanoRobot			= 1 << 9,
	SlowFallNanoRobot			= 1 << 10,
	RepairShipNanoRobot			= 1 << 11,
	WaterNanoRobot				= 1 << 12,
	SpiritNanoRobot				= (1 << 12) + 1,
	Water						= 1 << 13,

	//Object
	Object						= (1 << 14),
	SpaceShip					= (1 << 14) + 1,
	Bouibouik					= (1 << 14) + 2,
	HouseFlatSnow				= (1 << 14) + 3,
	WeirdHouseSnow				= (1 << 14) + 5,
	Tent						= (1 << 14) + 6,
	Firecamp					= (1 << 14) + 7,
	Concert						= (1 << 14) + 8,
	Well						= (1 << 14) + 9,
	PortalWater					= (1 << 14) + 10,
	PortalJungle				= (1 << 14) + 11,
	PortalDesert				= (1 << 14) + 12,
	PortalSnow					= (1 << 14) + 13,
	PortalRandom				= (1 << 14) + 14,
	PortalRed					= (1 << 14) + 15,
	PortalBlue					= (1 << 14) + 16,
	Monolith					= (1 << 14) + 17,
	Door						= (1 << 14) + 18,
	SmokeInstance				= (1 << 14) + 19,
	Pyramid						= (1 << 14) + 20,

	//Npc
	Npc							= (1 << 15),
	IdleNpc						= (1 << 15) + 1,
	DisappearNpc				= (1 << 15) + 2,
	WalkNpc						= (1 << 15) + 3,
	FlyNpc						= (1 << 15) + 4,
	SinkNpc						= (1 << 15) + 5,
	SwimNpc						= (1 << 15) + 6,
	FishNpc						= (1 << 15) + 7,
	SpecialNpc					= (1 << 15) + 8,
	IohannNpc					= (1 << 15) + 9,
	LucienNpc					= (1 << 15) + 10,
	ClementineNpc				= (1 << 15) + 11,
	VinceNpc					= (1 << 15) + 12,
	ConstanceNpc				= (1 << 15) + 13,
	AmandineNpc					= (1 << 15) + 14,
	JeffMouffyNpc				= (1 << 15) + 15,
	JellyfishNpc				= (1 << 15) + 16,
	BirdRedNpc					= (1 << 15) + 17,
	EvaNpc						= (1 << 15) + 18,
	FranfranNpc					= (1 << 15) + 19,
	FaustNpc					= (1 << 15) + 20,
	PierreNpc					= (1 << 15) + 21,
	PeaNpc						= (1 << 15) + 22,
	CanouilleNpc				= (1 << 15) + 23,
	WolfNpc						= (1 << 15) + 24,
	WellKeeperNpc				= (1 << 15) + 25,
	Snowman2Npc					= (1 << 15) + 26,
	Snowman1Npc					= (1 << 15) + 27,
	Snowman3Npc					= (1 << 15) + 28,
	SnowGirl1Npc				= (1 << 15) + 29,
	SnowGirl2Npc				= (1 << 15) + 30,
	StrangerGirlSnowNpc			= (1 << 15) + 31,
	StrangerSnowNpc				= (1 << 15) + 32,
	BirdBlueNpc					= (1 << 15) + 33,
	BrayouNpc					= (1 << 15) + 34,
	ForestSpirit1Npc			= (1 << 15) + 35,
	ForestSpirit2Npc			= (1 << 15) + 36,
	Pedestal					= (1 << 15) + 37,
	OverCoolNpc					= (1 << 15) + 38,
	CheckPoint					= (1 << 15) + 39,
	TVScreen					= (1 << 15) + 40,
	FabienNpc					= (1 << 15) + 41,
	ScientistJu					= (1 << 15) + 42,
	ScientistFran				= (1 << 15) + 43,
	ScientistLu					= (1 << 15) + 44,
	ScientistCedric				= (1 << 15) + 45,
	TVBlack						= (1 << 15) + 46,
	TVWhite						= (1 << 15) + 47,
	CedricEndNpc				= (1 << 15) + 48,
	OctoDeathNpc				= (1 << 15) + 49,
	Rocket						= (1 << 15) + 50,
	LongChairNpc				= (1 << 15) + 51,
	LuGlitchNpc					= (1 << 15) + 52,
	JuGlitchNpc					= (1 << 15) + 53,
	FranGlitchNpc				= (1 << 15) + 54,
	WindowGlitchNpc				= (1 << 15) + 55,
	RocketDoor					= (1 << 15) + 56,
	PunkNpc						= (1 << 15) + 57,
	CavemanNpc					= (1 << 15) + 58,
	CavemanSinkingNpc			= (1 << 15) + 60,
	CavemanClimbingNpc			= (1 << 15) + 60,
	ElliotNpc					= (1 << 15) + 61,
	DeepoNpc					= (1 << 15) + 62,
	CedricStartNpc				= (1 << 15) + 63,
	JuNpc						= (1 << 15) + 64,
	OldDesertStaticNpc			= (1 << 15) + 65,
	GuiNpc						= (1 << 15) + 66,
	ChamanMonsterNpc			= (1 << 15) + 67,
	BeachGuyNpc					= (1 << 15) + 68,
	LucieNpc					= (1 << 15) + 69,
	SylvieNpc					= (1 << 15) + 70,
	AymericNpc					= (1 << 15) + 71,
	MysticanouilleNpc			= (1 << 15) + 72,
	FannyNpc					= (1 << 15) + 73,
	AnthemJungle				= (1 << 15) + 74,
	TurbanNpc					= (1 << 15) + 75,
	FlorentNpc					= (1 << 15) + 76,
	EngineSnow					= (1 << 15) + 77,
	UlaNpc						= (1 << 15) + 78,
	AntoineNpc					= (1 << 15) + 79,
	BeachBoySubNpc				= (1 << 15) + 80,
	BeachBoyFlyNpc				= (1 << 15) + 81,
	SebNpc						= (1 << 15) + 82,
	TiboNpc						= (1 << 15) + 83,
	ColumnNpc					= (1 << 15) + 84,
	WaterHouseBroken			= (1 << 15) + 85,
	JihemNpc					= (1 << 15) + 86,
	MariaNpc					= (1 << 15) + 87,
	FishRedNpc					= (1 << 15) + 88,
	FishBlueNpc					= (1 << 15) + 89,
	FishBlackNpc				= (1 << 15) + 90,
	FishPinkNpc					= (1 << 15) + 91,
	MaryvonneNpc				= (1 << 15) + 92,
	ClaireNpc					= (1 << 15) + 93,
	FatNpc						= (1 << 15) + 94,
	DesertEngine				= (1 << 15) + 95,
	PepetteNpc				= (1 << 15) + 96,
	TVFanNpc				= (1 << 15) + 97,
	MecanouilleNpc				= (1 << 15) + 98,
	TheoNpc				= (1 << 15) + 99,
	CedricWalkNpc				= (1 << 15) + 100,
	ChristianNpc				= (1 << 15) + 101,
	SkeletonNpc				= (1 << 15) + 102,
	CedricIceANpc				= (1 << 15) + 103,
//Script AddNpc




























	None						= (1 << 16),
};

/*!
 *	\class AGameObject
 *	\brief Base of all game object
 *
 *	You can use gameObjectCast() instead dynamic_cast. 
 *	\code
 *	class Test0 : public AGameObject<GameObjectType::Player>
 *	{
 *	};
 *	
 *	class Test1 : public AGameObject<GameObjectType::Npc>
 *	{
 *	};
 *	
 *	AGameObjectBase*	o0 = new Test0;
 *	AGameObjectBase*	o1 = new Test1;
 *	Test0*				result0 = gameObjectCast<Test0>(o0);
 *	Test1*				result1 = gameObjectCast<Test1>(o1);
 *	Test0*				result2 = gameObjectCast<Test0>(o1);
 *	Test1*				result3 = gameObjectCast<Test1>(o0);
 *	assert (result0 != nullptr);
 *	assert (result1 != nullptr);
 *	assert (result2 == nullptr);
 *	assert (result3 == nullptr);
 *	\endcode
 */
class AGameObjectBase
{
public:
	virtual					~AGameObjectBase() = 0;
	virtual GameObjectType	getObjectType()const = 0;
};

template <GameObjectType Type>
class AGameObject : public AGameObjectBase
{
public:
	static constexpr GameObjectType const	ObjectType = Type;

	virtual inline GameObjectType	getObjectType()const
	{
		return (Type);
	}
};

template <class T, class O>
static T*	gameObjectCast(O* object)
{
	static_assert( std::is_base_of<AGameObjectBase, T>::value, " T must be derived from AGameObject" );
	static_assert( std::is_base_of<AGameObjectBase, O>::value, " O must be derived from AGameObject" );
	
	T*	result = nullptr;

	if (T::ObjectType == object->getObjectType())
	{
		result = static_cast<T*>(object);
	}
	return (result);
}

#endif
