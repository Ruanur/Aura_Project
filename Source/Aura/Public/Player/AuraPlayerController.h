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
 * 플레이어 컨트롤러 클래스
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AAuraPlayerController(); //생성자
	virtual void PlayerTick(float DeltaTime) override; //매 프레임 호출

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

	

protected:
	virtual void BeginPlay() override; //게임 시작 시 호출
	virtual void SetupInputComponent() override; //입력 컴포넌트 설정

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext; //입력 매핑 컨텍스트

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction; //이동 액션

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction; //쉬프트 액션

	void ShiftPressed() { bShiftKeyDown = true; };
	void ShiftReleased() { bShiftKeyDown = false; };
	bool bShiftKeyDown = false;

	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();
	IEnemyInterface* LastActor; //이전 액터
	IEnemyInterface* ThisActor; //현재 액터
	FHitResult CursorHit;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig; //입력 구성


	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent; //어빌리티 시스템 컴포넌트

	UAuraAbilitySystemComponent* GetASC(); 


	
	FVector CachedDestination = FVector::ZeroVector; //목적지
	float FollowTime = 0.f; //따라가는 시간
	float ShortPressThreshold = 0.5f; //얼마나 짧게 눌렀는지, 0.5초
	bool bAutoRunning = false; //자동 이동 여부
	bool bTargeting = false; //대상 지정 여부

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f; //달리기 허용 반경

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline; //스플라인 컴포넌트

	void AutoRun();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};

