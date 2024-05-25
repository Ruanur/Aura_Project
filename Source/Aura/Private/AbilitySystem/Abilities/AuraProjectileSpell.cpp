// Copyright Min Creater


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Aura/Public/AuraGameplayTags.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//ActivateAbility�� �⺻ Ŭ���� ���� ȣ��
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//����: ���⿡�� �ɷ��� Ȱ��ȭ�ϴ� �߰� ����� �߰��� �� �ִ�.
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	//������ ����ü�� ������ ������ �ִ��� Ȯ�� 
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	//������ �ƴϸ� �Լ� ����
	if (!bIsServer) return;

	//CombatInterface�� ĳ�����Ͽ� ������
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		//���� ���� ��ġ�� ������
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		Rotation.Pitch = 0.f;

		//���� Ʈ������ ����
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());
		//TODO : ����ü ȸ�� ����

		//����ü�� ���� ����
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		//TODO : ����ü�� ���ظ� ������ ���� Gameplay Effect Spec �ο�
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());

		const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());

		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("FireBolt Damage: %f"), ScaledDamage));

		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, ScaledDamage);
		Projectile->DamageEffectSpecHandle = SpecHandle;

		//����ü ���� �Ϸ�
		Projectile->FinishSpawning(SpawnTransform);
	}
}
