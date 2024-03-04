// Copyright Min Creater


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

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
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);

}
