// Copyright Min Creater


#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

AAuraPlayerState::AAuraPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	//복제 모드 : Mixed (Minimal, Mixed, Full)

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
	//서버가 클라이언트 업데이트를 시도하는 빈도
	//서버에서 플레이어 상태에 대한 변경이 발생하면 업데이트를 외부로 보냄
	//이는 모든 클라이언트에서 서버 버전과 동기화 할 수 있도록 하기 위함



	
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
