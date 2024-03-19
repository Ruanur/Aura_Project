// Copyright Min Creater

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet(); //속성
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health; //체력

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana; //마나

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;

	//BluepritnReadOnly - 엔진 내 블루프린트로 접근 

	/*
	* 새 속성을 추가하기 위한 단계.
	* 1. 변수 선언(OnRep_Health), 복제, 담당자 정의(UFUNCTION)
	* 2. 이를 정의하고 알림 매크로(cpp. GAMEPLAYATTRIBUTE_REPNOTIFY) 추가 
	* 3. 복제 변수를 추가하여(DOREPLIFETIME_CONDITION_NOTIFY, 조건 없음) 복제 값을 얻음
	* 
	* 이러한 모든 단계를 Attribute를 사용하고 추가하기 위한 상용구로 칭함. 
	* 속성이 필요한 모든 작업에는 이와 같은 상용구 단계를 따라야 한다. 
	*/

	UFUNCTION();
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION();
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION();
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION();
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
};
