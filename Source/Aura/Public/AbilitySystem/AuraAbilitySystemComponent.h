//Copyright Min Creator

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

// 다양한 게임플레이 이벤트를 위한 델리게이트 선언
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGiven, UAuraAbilitySystemComponent*)
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);

/**
 * Aura 게임을 위한 커스텀 능력 시스템 컴포넌트
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	//AbilityActorInfoSet가 설정될 때 호출
	void AbilityActorInfoSet();

	//게임플레이 효과 태그 델리게이트
	FEffectAssetTags EffectAssetTags;
	//능력이 부여된 후 호출되는 델리게이트
	FAbilitiesGiven AbilitiesGivenDelegate;

	//초기 능력 추가
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	//초기 패시브 능력 추가
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);
	//초기 능력이 부여되었는지 확인
	bool bStartupAbilitiesGiven = false;

	//입력 태그가 눌렸을 때 호출
	void AbilityInputTagHeld(const FGameplayTag& InputTag);

	//입력 태그가 떼어졌을 때 호출
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	//모든 활성화 가능한 능력에 대해 델리게이트 실행
	void ForEachAbility(const FForEachAbility& Delegate);

	//AbilitySpec에서 어빌리티 태그를 가져옴
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	//AbilitySpec에서 입력 태그를 가져옴
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

protected:

	virtual void OnRep_ActivateAbilities() override;

	//클라이언트에서 게임 플레이 효과가 적용될 때 호출
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
