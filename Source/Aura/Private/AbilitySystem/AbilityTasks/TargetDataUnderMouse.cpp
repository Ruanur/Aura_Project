// Copyright Min Creater


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);

	return MyObj;
}

//로컬 플레이어가 활성화 되었을 때 호출되는 함수
void UTargetDataUnderMouse::Activate()
{
	//현재 플레이어가 로컬 플레이어인지 확인
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		//로컬 플레이어인 경우 마우스 커서 아래의 데이터를 전송
		SendMouseCursorData();
	}
	else
	{
		//로컬 플레이어가 아닌 경우 리모트 플레이어로부터 대상 데이터를 기다림
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

//로컬 플레이어의 마우스 커서 아래의 대상을 확인, 해당 정보를 서버로 전송하는 함수
void UTargetDataUnderMouse::SendMouseCursorData()
{
	//서버에 대상 데이터를 전송하기 위해 사용
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	//플레이어 컨트롤러를 사용하여 마우스 커서 아래의 대상을 검출
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	
	//대상 데이터를 생성, HitResult를 설정
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);

	//서버로 대상 데이터를 전송
	FGameplayTag ApplicationTag;
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(), 
		AbilitySystemComponent->ScopedPredictionKey);

	//능력 태스크 델리케이트를 브로드캐스트하여 다른 시스템에서 사용할 수 있도록 함.
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

//리모트 플레이어로부터 대상 데이터가 복제될 때 호출되는 콜백 함수이다.
void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}

}
