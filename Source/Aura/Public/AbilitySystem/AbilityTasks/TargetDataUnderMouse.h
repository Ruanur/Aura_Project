// Copyright Min Creater

#pragma once


#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

/**
 * �� Task�� AbilityTask Ŭ������ ����Ͽ� ������
 */
UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	//��� �����Ͱ� ��ȿ�� �� �߻��ϴ� ��������Ʈ
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName = "TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	//���ο� UTargetDataUnderMouse ��ü�� �����Ͽ� ��ȯ
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;

private:
	virtual void Activate() override;
	
	//���콺 Ŀ�� �Ʒ��� �����͸� �����ϴ� �Լ�
	void SendMouseCursorData();

	//����Ʈ �÷��̾�κ��� ��� �����Ͱ� ������ �� ȣ��Ǵ� �ݹ� �Լ�
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
