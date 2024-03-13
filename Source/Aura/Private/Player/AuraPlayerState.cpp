// Copyright Min Creater


#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

AAuraPlayerState::AAuraPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
	//������ Ŭ���̾�Ʈ ������Ʈ�� �õ��ϴ� ��
	//�������� �÷��̾� ���¿� ���� ������ �߻��ϸ� ������Ʈ�� �ܺη� ����
	//�̴� ��� Ŭ���̾�Ʈ���� ���� ������ ����ȭ �� �� �ֵ��� �ϱ� ����



	
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
