// Copyright Min Creater


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);

	return MyObj;
}

//���� �÷��̾ Ȱ��ȭ �Ǿ��� �� ȣ��Ǵ� �Լ�
void UTargetDataUnderMouse::Activate()
{
	//���� �÷��̾ ���� �÷��̾����� Ȯ��
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		//���� �÷��̾��� ��� ���콺 Ŀ�� �Ʒ��� �����͸� ����
		SendMouseCursorData();
	}
	else
	{
		//���� �÷��̾ �ƴ� ��� ����Ʈ �÷��̾�κ��� ��� �����͸� ��ٸ�
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(GetAbilitySpecHandle(), GetActivationPredictionKey()).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}


}

//���� �÷��̾��� ���콺 Ŀ�� �Ʒ��� ����� Ȯ��, �ش� ������ ������ �����ϴ� �Լ�
void UTargetDataUnderMouse::SendMouseCursorData()
{
	//������ ��� �����͸� �����ϱ� ���� ���
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	//�÷��̾� ��Ʈ�ѷ��� ����Ͽ� ���콺 Ŀ�� �Ʒ��� ����� ����
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	
	//��� �����͸� ����, HitResult�� ����
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);

	//������ ��� �����͸� ����
	FGameplayTag ApplicationTag;
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(), 
		AbilitySystemComponent->ScopedPredictionKey);

	//�ɷ� �½�ũ ��������Ʈ�� ��ε�ĳ��Ʈ�Ͽ� �ٸ� �ý��ۿ��� ����� �� �ֵ��� ��.
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

//����Ʈ �÷��̾�κ��� ��� �����Ͱ� ������ �� ȣ��Ǵ� �ݹ� �Լ��̴�.
void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}

}
