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

//������
AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true; //���� Ȱ��ȭ

	//CreateDefaultSubobject : USplineComponent�� ���ο� �ν��Ͻ��� �����ϴµ� ���
	//USplineComponent : ���ö��� ����� ��� ��Ÿ���� Ŭ����, �̵� ��θ� ���� ���
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

//�� ������ ȣ��
void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime); //�θ� Ŭ������ PlayerTick �Լ� ȣ��

	CursorTrace(); //Ŀ�� ���� �Լ�
}

//CursorTrace, �÷��̾��� Ŀ�� ��ġ�� �����ϰ� �ش� ��ġ���� � ���Ϳ� �浹�ߴ��� �˻�
//��ȣ�ۿ��� ���� ����
void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	//Ŀ���� ���̴� �� �Ʒ����� �浹 �˻縦 ���� 
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	//��ϵǴ� �浹�� ������ ��ȯ
	if (!CursorHit.bBlockingHit) return;

	//�浹�� ���͸� IEnemyInterface�� ĳ����
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

//Ư�� �Է¿� ���� �׼� ó��
void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//�Է� �±װ� ���콺 ���� ��ư�� ���
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		//ThisActor�� ��ȿ�� ��쿡�� ��ȣ�ۿ� ��� ����
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

//Ư�� �Է��� �������� ���� �׼� ó��
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

//Ư�� �Է��� ��� ������ �ִ� ������ �׼� ó��
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

//�Է� ���� ���ؽ�Ʈ �߰�
void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//check�� ����Ͽ� AuraContext ������ Ȯ��
	//�� ������ �����ϸ� ������ �ߴ�
	check(AuraContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	//Error : EnhancedInputSubsystems.h ����� �� �� ����
	//Solution : Build.cs ���� �� PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	//EnhancedInput �Է�, Binaries ���� ���� �� uproject ��Ŭ�� �� visual studio ������Ʈ ���� 
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	//���콺 Ŀ�� ����
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	//�Է� ��� ����
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);

}

//�Է� ������Ʈ ����
void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//AuraInputComponent ���ε�
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

//�̵� �׼� ó��
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

