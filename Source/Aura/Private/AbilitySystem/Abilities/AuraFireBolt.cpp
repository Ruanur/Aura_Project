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
