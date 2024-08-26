// Copyright Min Creator


#include "AbilitySystem/Abilities/AuraFireBolt.h"
#include "Kismet/KismetSystemLibrary.h"

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
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-ProjectileSpread / 2.f, FVector::UpVector);
	const FVector RightOfSpread = Forward.RotateAngleAxis(ProjectileSpread / 2.f, FVector::UpVector);

	//NumProjectiles = FMath::Min(MaxNumProjectiles, GetAbilityLevel());

	//발사체 수가 1보다 클 경우
	if (NumProjectiles > 1)
	{
		const float DeltaSpread = ProjectileSpread / (NumProjectiles - 1);
		for (int32 i = 0; i < NumProjectiles; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			const FVector Start = SocketLocation + FVector(0, 0, 5);
			UKismetSystemLibrary::DrawDebugArrow(
				GetAvatarActorFromActorInfo(), 
				Start,
				Start + Direction * 75.f,
				1,
				FLinearColor::Red, 
				120, 
				1);
		}
	}
	else
	{
		//발사체 1개
		const FVector Start = SocketLocation + FVector(0, 0, 5);
		UKismetSystemLibrary::DrawDebugArrow(
			GetAvatarActorFromActorInfo(),
			Start,
			Start + Forward * 75.f,
			1,
			FLinearColor::Red,
			120,
			1);
	}

	//디버그 화살표, 
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + Forward * 100.f, 1, FLinearColor::White, 120, 2);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + LeftOfSpread * 100.f, 1, FLinearColor::Gray, 120, 2);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + RightOfSpread * 100.f, 1, FLinearColor::Gray, 120, 2);
}
