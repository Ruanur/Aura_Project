// Copyright Min Creater

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;

/**
 * UAuraProjectileSpell Ŭ����
 * UAuraGameplayAbility�� ��ӹ޾� ����ü ������ �����ϴ� Ŭ����
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraGameplayAbility
{
	GENERATED_BODY()

protected:
	//�ɷ��� Ȱ��ȭ�� �� ȣ��Ǵ� �Լ�, �⺻ Ŭ������ ActivateAbility�� �������̵� �� 
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	//�������Ʈ���� ȣ�� ������ ����ü ���� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);

	//����ü Ŭ������ �����ϴ� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;
};
