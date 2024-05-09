// Copyright Min Creater


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Interaction/EnemyInterface.h"

//생성자
AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true; //복제 활성화

	//CreateDefaultSubobject : USplineComponent의 새로운 인스턴스를 생성하는데 사용
	//USplineComponent : 스플라인 기반의 곡선을 나타내는 클래스, 이동 경로를 위해 사용
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

//매 프레임 호출
void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime); //부모 클래스의 PlayerTick 함수 호출

	CursorTrace(); //커서 추적 함수
}

//CursorTrace, 플레이어의 커서 위치를 추적하고 해당 위치에서 어떤 액터에 충돌했는지 검사
//상호작용힐 액터 결정
void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	//커서가 보이는 것 아래에서 충돌 검사를 실행 
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	//블록되는 충돌이 없으면 반환
	if (!CursorHit.bBlockingHit) return;

	//충돌한 액터를 IEnemyInterface로 캐스팅
	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	
	/*
	* Line trace from cursor. There are sereval scenarios;
	* A. LastActor is null && ThisActor is null;
	*	- Do nothing.
	* B. LastActor is null && ThisActor is valid
	*	- Highlight ThisActor
	* C. LastActor is valid && ThisActor is null
	*	- UnHighlight LastActor
	* D. Both actors are valid, but LastActor != ThisActor 
	*	- UnHighlight LastActor, and Highlight ThisActor
	* E. Both actors are valid, and are the same actor
	*	- Do nothing
	*/

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			// Case B
			ThisActor->HighlightActor();
		}
		else
		{
			// Cast A - both are null, do nothing 

		}
	}
	else // LastActor is vaild 
	{
		if (ThisActor == nullptr)
		{
			// Case C 
			LastActor->UnHighlightActor();
		}
		else // both actors are valid 
		{
			if (LastActor != ThisActor)
			{
				// Case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				// Case E - Do nothing.
			}
		}
	}
}

//특정 입력에 대한 액션 처리
void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//입력 태그가 마우스 왼쪽 버튼인 경우
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		//ThisActor가 유효한 경우에반 상호작용 대상 설정
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

//특정 입력의 릴리스에 대한 액션 처리
void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
		return;
	}

	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
	}
	else
	{
		APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
				}
				bAutoRunning = true;
			}

		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

//특정 입력을 계속 누르고 있는 동안의 액션 처리
void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}

	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		FHitResult Hit;
		if (GetHitResultUnderCursor(ECC_Visibility, false, Hit))
		{
			CachedDestination = Hit.ImpactPoint;
		}

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}

//입력 매핑 컨텍스트 추가
void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//check를 사용하여 AuraContext 포인터 확인
	//이 조건이 실패하면 실행을 중단
	check(AuraContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	//Error : EnhancedInputSubsystems.h 헤더를 열 수 없음
	//Solution : Build.cs 파일 내 PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	//EnhancedInput 입력, Binaries 폴더 삭제 후 uproject 우클릭 후 visual studio 프로젝트 생성 
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	//마우스 커서 설정
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	//입력 모드 설정
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);

}

//입력 컴포넌트 설정
void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//AuraInputComponent 바인딩
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

//이동 액션 처리
void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

