// Copyright Min Creater

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"


class UDamageTextComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
class USplineComponent;

/**
 * �÷��̾� ��Ʈ�ѷ� Ŭ����
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AAuraPlayerController(); //������
	virtual void PlayerTick(float DeltaTime) override; //�� ������ ȣ��

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

	

protected:
	virtual void BeginPlay() override; //���� ���� �� ȣ��
	virtual void SetupInputComponent() override; //�Է� ������Ʈ ����

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext; //�Է� ���� ���ؽ�Ʈ

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction; //�̵� �׼�

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction; //����Ʈ �׼�

	void ShiftPressed() { bShiftKeyDown = true; };
	void ShiftReleased() { bShiftKeyDown = false; };
	bool bShiftKeyDown = false;

	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();
	IEnemyInterface* LastActor; //���� ����
	IEnemyInterface* ThisActor; //���� ����
	FHitResult CursorHit;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig; //�Է� ����


	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent; //�����Ƽ �ý��� ������Ʈ

	UAuraAbilitySystemComponent* GetASC(); 


	
	FVector CachedDestination = FVector::ZeroVector; //������
	float FollowTime = 0.f; //���󰡴� �ð�
	float ShortPressThreshold = 0.5f; //�󸶳� ª�� ��������, 0.5��
	bool bAutoRunning = false; //�ڵ� �̵� ����
	bool bTargeting = false; //��� ���� ����

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f; //�޸��� ��� �ݰ�

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline; //���ö��� ������Ʈ

	void AutoRun();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};

