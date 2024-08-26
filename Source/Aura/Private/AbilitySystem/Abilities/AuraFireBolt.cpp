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
			//�ɷ� �̸�
			"<Title>Fire Bolt</>\n\n"

			//�ɷ� ����
			"<Small>Level: </><Level>%d</>\n"

			//�ɷ� ���� �Ҹ�
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"

			//��Ÿ��
			"<Small>Cooldown: </><Cooldown>: %.1f</>\n\n"

			//����
			"<Default>ȭ�� ��ü�� �߻��Ͽ� " 

			//���ط�
			"</><Damage>%d</> ���ظ� �����ϴ�."
			"<Default> ���� Ȯ���� ȭ�� ���ظ� �Խ��ϴ�.</>"),

			//��
			Level,
			ManaCost,
			Cooldown,
			ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			//�ɷ� �̸�
			"<Title>Fire Bolt</>\n\n"

			//�ɷ� ����
			"<Small>Level: </><Level>%d</>\n"

			//�ɷ� ���� �Ҹ�
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"

			//��Ÿ��
			"<Small>Cooldown: </><Cooldown>: %.1f</>\n\n"

			//ȭ�� ��ü ����
			"<Default>ȭ�� ��ü�� %d�� �߻��Ͽ� "

			//���ط�
			"</><Damage>%d</> ���ظ� �����ϴ�."
			"<Default> ���� Ȯ���� ȭ�� ���ظ� �Խ��ϴ�.</>"),

			//��
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
		//�ɷ� �̸�
		"<Title>Fire Bolt</>\n\n"

		//�ɷ� ����
		"<Small>Level: </><Level>%d</>\n"

		//�ɷ� ���� �Ҹ�
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"

		//��Ÿ��
		"<Small>Cooldown: </><Cooldown>: %.1f</>\n\n"

		//ȭ�� ��ü ����
		"<Default>ȭ�� ��ü�� %d�� �߻��Ͽ� "

		//���ط�
		"</><Damage>%d</> ���ظ� �����ϴ�."
		"<Default> ���� Ȯ���� ȭ�� ���ظ� �Խ��ϴ�.</>"),

		//��
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

	//�߻�ü ���� 1���� Ŭ ���
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
		//�߻�ü 1��
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

	//����� ȭ��ǥ, 
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + Forward * 100.f, 1, FLinearColor::White, 120, 2);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + LeftOfSpread * 100.f, 1, FLinearColor::Gray, 120, 2);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + RightOfSpread * 100.f, 1, FLinearColor::Gray, 120, 2);
}
