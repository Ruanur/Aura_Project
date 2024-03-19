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
	UAuraAttributeSet(); //�Ӽ�
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health; //ü��

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana; //����

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;

	//BluepritnReadOnly - ���� �� �������Ʈ�� ���� 

	/*
	* �� �Ӽ��� �߰��ϱ� ���� �ܰ�.
	* 1. ���� ����(OnRep_Health), ����, ����� ����(UFUNCTION)
	* 2. �̸� �����ϰ� �˸� ��ũ��(cpp. GAMEPLAYATTRIBUTE_REPNOTIFY) �߰� 
	* 3. ���� ������ �߰��Ͽ�(DOREPLIFETIME_CONDITION_NOTIFY, ���� ����) ���� ���� ����
	* 
	* �̷��� ��� �ܰ踦 Attribute�� ����ϰ� �߰��ϱ� ���� ��뱸�� Ī��. 
	* �Ӽ��� �ʿ��� ��� �۾����� �̿� ���� ��뱸 �ܰ踦 ����� �Ѵ�. 
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
