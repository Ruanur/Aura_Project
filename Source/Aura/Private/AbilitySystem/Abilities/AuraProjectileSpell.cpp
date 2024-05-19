// Copyright Min Creater


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//ActivateAbility�� �⺻ Ŭ���� ���� ȣ��
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//����: ���⿡�� �ɷ��� Ȱ��ȭ�ϴ� �߰� ����� �߰��� �� �ִ�.
}

void UAuraProjectileSpell::SpawnProjectile()
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

		//���� Ʈ������ ����
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		//TODO : ����ü ȸ�� ����

		//����ü�� ���� ����
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		//TODO : ����ü�� ���ظ� ������ ���� �����÷��� ȿ�� ���� �ο�

		//����ü ���� �Ϸ�
		Projectile->FinishSpawning(SpawnTransform);
	}
}
