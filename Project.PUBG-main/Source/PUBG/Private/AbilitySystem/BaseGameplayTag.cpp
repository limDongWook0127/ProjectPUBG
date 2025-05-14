

#include "AbilitySystem/BaseGameplayTag.h"


namespace BaseGameplayTag
{


	
	//Input Tags
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Jump, "InputTag.Jump");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Walk, "InputTag.Walk");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Sprint, "InputTag.Sprint");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Crouch, "InputTag.Crouch");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Prone, "InputTag.Prone");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Turn, "InputTag.Turn");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Punch, "InputTag.Punch");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Interaction, "InputTag.Interaction");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_LeftLeaning, "InputTag.LeftLeaning");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_RightLeaning, "InputTag.RightLeaning");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_CameraTransition, "InputTag.CameraTransition");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_RightShouldering, "InputTag.RightShouldering");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_LeftShouldering, "InputTag.LeftShouldering");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_PrimaryWeapon, "InputTag.PrimaryWeapon");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_SecondaryWeapon, "InputTag.SecondaryWeapon");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_PistolWeapon, "InputTag.PistolWeapon");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_MeleeWeapon, "InputTag.MeleeWeapon");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_ThrowableWeapon, "InputTag.ThrowableWeapon");
	
	// Airplane Input Tag
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Airplane_Look, "InputTag.Airplane.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Airplane_Fall, "InputTag.Airplane.Fall");
	
	// Inventory Tag
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggle, "InputTag.Toggle");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggle_Inventory, "InputTag.Toggle.Inventory");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggle_WorldMap, "InputTag.Toggle.WorldMap");

	// Vehicle Input Tag
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Vehicle_Break, "InputTag.Vehicle.Break");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Vehicle_Steering, "InputTag.Vehicle.Steering");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Vehicle_Throttle, "InputTag.Vehicle.Throttle");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Vehicle_HandBreak, "InputTag.Vehicle.HandBreak");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Vehicle_VehicleLook, "InputTag.Vehicle.VehicleLook");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Vehicle_GetOut, "InputTag.Vehicle.GetOut");
	
	// Weapon Input Tag
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Weapon_Equip, "InputTag.Weapon.Equip");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Weapon_UnEquip, "InputTag.Weapon.UnEquip");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Weapon_Fire, "InputTag.Weapon.Fire");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Weapon_Reload, "InputTag.Weapon.Reload");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Weapon_ChangeFireMode, "InputTag.Weapon.ChangeMode");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Weapon_Zoom, "InputTag.Weapon.Zoom");

	// Weapon Action Tag

	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Action_Fire, "Player.Ability.Weapon.Fire");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Action_Reload, "Player.Ability.Weapon.Reload");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Action_ChangeFireMode, "Player.Ability.Weapon.ChangeFireMode");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Action_Zoom, "Player.Ability.Weapon.Zoom");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Action_UseItem, "Player.Ability.Action.UseItem");

	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Pickup,"Player.Ability.Pickup");

	// Weapon Equip
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Action_Equip, "Player.Ability.Weapon.Equip");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Action_UnEquip, "Player.Ability.Weapon.UnEquip");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Action_Primary, "Player.Ability.Weapon.Primary");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Action_Secondary, "Player.Ability.Weapon.Secondary");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Action_Pistol, "Player.Ability.Weapon.Pistol");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Action_Melee, "Player.Ability.Weapon.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Action_Throwable, "Player.Ability.Weapon.Throwable");


	// Weapon Event Tag
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Action_Equip, "Player.Event.Action.Equip");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Action_UnEquip, "Player.Event.Action.UnEquip");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Action_Reload, "Player.Event.Action.Reload");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Action_ReloadEnd, "Player.Event.Action.ReloadEnd");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Action_HideMag, "Player.Event.Action.HideMag");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Action_VisibleMag, "Player.Event.Action.VisibleMag");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Action_Throw, "Player.Event.Action.Throw");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Action_CompleteUseItem, "Player.Event.Action.CompleteUseItem");

	// player Event Tag
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Prone, "Player.State.Prone");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Stand, "Player.State.Stand");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Crouch, "Player.State.Crouch");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Dead, "Player.State.Dead");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Zoom, "Player.State.Zoom");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_ThrowsReady, "Player.State.ThrowsReady");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Turn, "Player.Ability.Turn");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Swim, "Player.State.Swim");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Action_Hit, "Player.Event.Action.Hit");

	
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Melee, "Weapon.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Grenade,"Weapon.Grenade");
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Unarmed,"Weapon.Unarmed");
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Gun_AR,"Weapon.Gun.AR");
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Gun_Pistol,"Weapon.Gun.Pistol");
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Gun_SR,"Weapon.Gun.SR");
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Gun_SG,"Weapon.Gun.SG");
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Gun_DMR,"Weapon.Gun.DMR");
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Gun_SMG,"Weapon.Gun.SMG");

	
	// Data Tag
	UE_DEFINE_GAMEPLAY_TAG(Data_Damage, "Data.Damage");
	UE_DEFINE_GAMEPLAY_TAG(Effect_HealthRegen, "Effect.HealthRegen");
}
