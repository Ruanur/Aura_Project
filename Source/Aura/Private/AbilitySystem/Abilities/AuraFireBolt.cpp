// Copyright Min Creator


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			//능력 이름
			"<Title>Fire Bolt</>\n\n"

			//능력 레벨
			"<Small>Level: </><Level>%d</>\n"

			//능력 마나 소모량
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"

			//쿨타임
			"<Small>Cooldown: </><Cooldown>: %.1f</>\n\n"

			//설명
			"<Default>화염 구체를 발사하여 " 

			//피해량
			"</><Damage>%d</> 피해를 입힙니다."
			"<Default> 일정 확률로 화염 피해를 입습니다.</>"),

			//값
			Level,
			ManaCost,
			Cooldown,
			ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			//능력 이름
			"<Title>Fire Bolt</>\n\n"

			//능력 레벨
			"<Small>Level: </><Level>%d</>\n"

			//능력 마나 소모량
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"

			//쿨타임
			"<Small>Cooldown: </><Cooldown>: %.1f</>\n\n"

			//화염 구체 개수
			"<Default>화염 구체를 %d개 발사하여 "

			//피해량
			"</><Damage>%d</> 피해를 입힙니다."
			"<Default> 일정 확률로 화염 피해를 입습니다.</>"),

			//값
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, NumProjectiles),
			ScaledDamage);
	}
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		//능력 이름
		"<Title>Fire Bolt</>\n\n"

		//능력 레벨
		"<Small>Level: </><Level>%d</>\n"

		//능력 마나 소모량
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"

		//쿨타임
		"<Small>Cooldown: </><Cooldown>: %.1f</>\n\n"

		//화염 구체 개수
		"<Default>화염 구체를 %d개 발사하여 "

		//피해량
		"</><Damage>%d</> 피해를 입힙니다."
		"<Default> 일정 확률로 화염 피해를 입습니다.</>"),

		//값
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, NumProjectiles),
		ScaledDamage);
}

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}

	const FVector Forward = Rotation.Vector();

	TArray<FRotator> Rotations = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, NumProjectiles);

	for (const FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

		Projectile->FinishSpawning(SpawnTransform);
	}

	
}
