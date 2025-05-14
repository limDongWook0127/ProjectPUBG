#pragma once

UENUM()
enum class EBaseConfirmType : uint8
{
	Yes,
	No,
};

UENUM()
enum class EBaseValidType : uint8
{
	Valid,
	InValid,
};

UENUM()
enum class EBaseSuccessType : uint8
{
	Success,
	Failed,
};

UENUM(BlueprintType)
enum class EItemZoneType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	NoneArea UMETA(DisplayName = "NoneArea"),
	NearArea UMETA(DisplayName = "NearArea"),
	InventoryArea UMETA(DisplayName = "InventoryArea"),
	EquippedArea UMETA(DisplayName = "EquippedArea"),
};

UENUM(BlueprintType)
enum class HitSpaceType : uint8
{
	head,			// 머리
	spine_03,		// 상체 위쪽
	spine_01,		// 상체 가운데
	pelvis,			// 상체 아래쪽
	upperarm_l,		// 왼팔 ( 어깨 -> 팔꿈치 )
	lowerarm_l,		// 왼팔 ( 팔꿈치 -> 손목 ) 
	hand_l,			// 왼팔 ( 손 )
	upperarm_r,		// 오른팔 ( 어깨 -> 팔꿈치)
	lowerarm_r,		// 오른팔 ( 팔꿈치 -> 손목 )
	hand_r,			// 오른팔 ( 손 )
	thigh_l,		// 왼다리 ( 허벅지 )
	calf_l,			// 왼다리 ( 종아리 )
	foot_l,			// 왼다리 ( 발 )
	thigh_r,		// 오른다리 ( 허벅지 )
	calf_r,			// 오른다리 ( 종아리 )
	foot_r,			// 오른다리 ( 발 )
	
};
