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
	Water						= 1 << 13,
	Mushroom					= 1 << 15,

	//Object
	Object						= (1 << 15),
	SpaceShip					= (1 << 15) + 1,
	Bouibouik					= (1 << 15) + 2,
	HouseFlatSnow				= (1 << 15) + 3,
	EngineSnow					= (1 << 15) + 4,
	WeirdHouseSnow				= (1 << 15) + 5,
	Tent						= (1 << 15) + 6,
	Firecamp					= (1 << 15) + 7,
	Concert						= (1 << 15) + 8,
	Well						= (1 << 15) + 9,
	PortalWater					= (1 << 15) + 10,
	PortalJungle				= (1 << 15) + 11,
	PortalDesert				= (1 << 15) + 12,
	PortalSnow					= (1 << 15) + 13,
	PortalRandom				= (1 << 15) + 14,

	//Npc
	Npc							= (1 << 16),
	CedricStartNpc				= (1 << 16) + 1,
	JuNpc						= (1 << 16) + 2,
	OldDesertStaticNpc			= (1 << 16) + 3,
	GuiNpc						= (1 << 16) + 4,
	FannyNpc					= (1 << 16) + 5,
	TurbanNpc					= (1 << 16) + 6,
	PunkNpc						= (1 << 16) + 7,
	FatNpc						= (1 << 16) + 8,
	IohannNpc					= (1 << 16) + 9,
	LucienNpc					= (1 << 16) + 10,
	ClementineNpc				= (1 << 16) + 11,
	VinceNpc					= (1 << 16) + 12,
	ConstanceNpc				= (1 << 16) + 13,
	AmandineNpc					= (1 << 16) + 14,
	JeffMouffyNpc				= (1 << 16) + 15,
	JellyfishNpc				= (1 << 16) + 16,
	BirdRedNpc					= (1 << 16) + 17,
	EvaNpc						= (1 << 16) + 18,
	FranfranNpc					= (1 << 16) + 19,
	FaustNpc					= (1 << 16) + 20,
	PierreNpc					= (1 << 16) + 21,
	PeaNpc						= (1 << 16) + 22,
	CanouilleNpc				= (1 << 16) + 23,
	WolfNpc						= (1 << 16) + 24,
	WellKeeperNpc				= (1 << 16) + 25,
	Snowman2Npc					= (1 << 16) + 26,
	Snowman1Npc					= (1 << 16) + 27,
	Snowman3Npc					= (1 << 16) + 28,
	SnowGirl1Npc				= (1 << 16) + 29,
	SnowGirl2Npc				= (1 << 16) + 30,
	StrangerGirlSnowNpc			= (1 << 16) + 31,
	StrangerSnowNpc				= (1 << 16) + 32,
	BirdBlueNpc					= (1 << 16) + 33,
	BrayouNpc					= (1 << 16) + 34,
	ForestSpirit1Npc			= (1 << 16) + 35,
	ForestSpirit2Npc			= (1 << 16) + 36,
	Pedestal					= (1 << 16) + 37,
	OverCoolNpc					= (1 << 16) + 38,
	CheckPoint					= (1 << 16) + 39,
	TVScreen					= (1 << 16) + 40,
	TVBlack						= (1 << 16) + 41,
	TVWhite						= (1 << 16) + 42,
	FabienNpc					= (1 << 16) + 43,
	CedricEndNpc				= (1 << 16) + 44,
	OctoDeathNpc				= (1 << 16) + 45,
	Rocket						= (1 << 16) + 46,
	LongChairNpc				= (1 << 16) + 47,
	LuGlitchNpc					= (1 << 16) + 48,
	JuGlitchNpc					= (1 << 16) + 49,
	FranGlitchNpc				= (1 << 16) + 50,
	WindowGlitchNpc				= (1 << 16) + 51,
//Script AddNpc








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
