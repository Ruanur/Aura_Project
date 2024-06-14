// Copyright Min Creater

#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	// �ڽſ��� ����� �����÷��� ȿ���� �߻��� �� ȣ��Ǵ� ��������Ʈ�� ���� ��� �Լ��� �߰��Ѵ�.
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);

}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	//������ �� �����Ƽ�� �߰��Ѵ�.
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		//���� �����Ƽ�� AuraGameplayAbility Ŭ������ �ν��Ͻ����,
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			//���� �Է� �±׸� �������� �߰��ϰ� �ɷ��� �ο��Ѵ�.
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	//Ȧ��� �Է� �±׿� ��ġ�ϴ� Ȱ��ȭ ������ �ɷ��� ã�� Ȱ��ȭ�ϰų� �̹� Ȱ��ȭ�Ǿ� �ִ��� Ȯ���Ѵ�.
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}

}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	//�������� �Է� �±׿� ��ġ�ϴ� Ȱ��ȭ ������ �ɷ��� ã�� ������ ���·� �����Ѵ�.
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	//ȿ���� ���� ��� ���� �±׸� ������ ��ε�ĳ��Ʈ�Ѵ�.
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}
