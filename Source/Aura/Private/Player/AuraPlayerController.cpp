// Copyright Min Creater


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

//������
AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true; //���� Ȱ��ȭ

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
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
}

//Ư�� �Է��� �������� ���� �׼� ó��
void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Blue, *InputTag.ToString());
}

//Ư�� �Է��� ��� ������ �ִ� ������ �׼� ó��
void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Green, *InputTag.ToString());
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

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);

}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

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

