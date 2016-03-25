#ifndef AGAMEOBJECT_HPP
# define AGAMEOBJECT_HPP
# include <type_traits>

enum class GameObjectType : std::size_t
{
	Player = 1u,
	PlayerEvent = 2u,
	Tile = 4u,
	Decor = 8u,
	Portal = 16u,
	Elevator = 32u,
	CedricNpc = 64u,
	SpaceShip = 128u,
	RepairNanoRobot = 256u,
	JumpNanoRobot = 512u,
	DoubleJumpNanoRobot = 1024u,
	SlowFallNanoRobot = 2048u,
	GroundTransformNanoRobot = 4096u,
	RepairShipNanoRobot = 8192u,
	JuNpc = 16384u,
	OldDesertStaticNpc = 32768u,
	Bouibouik = 65536u,
	HouseFlatSnow = 65537u,
	EngineSnow = 65538u,
	WeirdHouseSnow = 65539,
	GuiNpc = 131072u,
	FannyNpc = 262144,
	TurbanNpc = 524288,
	Tent = 1048576u,
	Firecamp = 2097152u,
	Concert = 4194304u,
	PunkNpc = 8388608u,
	FatNpc = 16777216u,
	IohannNpc = 33554432u,
	LucienNpc = 67108864u,
	ClementineNpc = 67108868u,
	WaterNanoRobot = 134217728u,
	VinceNpc = 268435456u,
	ConstanceNpc = 536870912u,
	AmandineNpc = 1073741824u,
	JeffMouffyNpc = 2147483648u,
	JellyfishNpc = 4294967296u,
	EvaNpc = 8589934592u,
	FranfranNpc = 130u,
	Water = 131u,
	FaustNpc = 132u,
	PierreNpc = 133u,
	PeaNpc = 134u,
	CanouilleNpc = 135u,
	WolfNpc = 136u,
	WellKeeperNpc = 138u,
	Snowman2Npc = 137u,
	Well = 140u,
	Snowman1Npc = 141u,
	Snowman3Npc = 1142u,
//Script AddNpc


	BrayouNpc = 139u
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
