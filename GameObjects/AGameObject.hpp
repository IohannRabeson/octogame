#ifndef AGAMEOBJECT_HPP
# define AGAMEOBJECT_HPP
# include <type_traits>

enum class GameObjectType : std::size_t
{
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

	Object						= (1 << 14),
	SpaceShip					= (1 << 14) + 1,
	Bouibouik					= (1 << 14) + 2,
	HouseFlatSnow				= (1 << 14) + 3,
	EngineSnow					= (1 << 14) + 4,
	WeirdHouseSnow				= (1 << 14) + 5,
	Tent						= (1 << 14) + 6,
	Firecamp					= (1 << 14) + 7,
	Concert						= (1 << 14) + 8,
	Well						= (1 << 14) + 9,

	Npc							= (1 << 15),
	CedricNpc					= (1 << 15) + 1,
	JuNpc						= (1 << 15) + 2,
	OldDesertStaticNpc			= (1 << 15) + 3,
	GuiNpc						= (1 << 15) + 4,
	FannyNpc					= (1 << 15) + 5,
	TurbanNpc					= (1 << 15) + 6,
	PunkNpc						= (1 << 15) + 7,
	FatNpc						= (1 << 15) + 8,
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
