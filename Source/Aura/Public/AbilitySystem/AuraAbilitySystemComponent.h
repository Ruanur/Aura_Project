//Copyright Min Creator

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

// �پ��� �����÷��� �̺�Ʈ�� ���� ��������Ʈ ����
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGiven, UAuraAbilitySystemComponent*)
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);

/**
 * Aura ������ ���� Ŀ���� �ɷ� �ý��� ������Ʈ
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	//AbilityActorInfoSet�� ������ �� ȣ��
	void AbilityActorInfoSet();

	//�����÷��� ȿ�� �±� ��������Ʈ
	FEffectAssetTags EffectAssetTags;
	//�ɷ��� �ο��� �� ȣ��Ǵ� ��������Ʈ
	FAbilitiesGiven AbilitiesGivenDelegate;

	//�ʱ� �ɷ� �߰�
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	//�ʱ� �нú� �ɷ� �߰�
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);
	//�ʱ� �ɷ��� �ο��Ǿ����� Ȯ��
	bool bStartupAbilitiesGiven = false;

	//�Է� �±װ� ������ �� ȣ��
	void AbilityInputTagHeld(const FGameplayTag& InputTag);

	//�Է� �±װ� �������� �� ȣ��
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	//��� Ȱ��ȭ ������ �ɷ¿� ���� ��������Ʈ ����
	void ForEachAbility(const FForEachAbility& Delegate);

	//AbilitySpec���� �����Ƽ �±׸� ������
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	//AbilitySpec���� �Է� �±׸� ������
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

protected:

	virtual void OnRep_ActivateAbilities() override;

	//Ŭ���̾�Ʈ���� ���� �÷��� ȿ���� ����� �� ȣ��
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
