// Copyright Min Creater

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "AuraInputConfig.h"
#include "AuraInputComponent.generated.h"

/*
 * 
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	//템플릿을 사용하여 함수를 바인딩하는 함수를 정의하는 코드 스니펫
	//여기서 함수는 다음과 같은 인수를 취한다.
	/*
	* i. InputConfig : UAuraInputConfig 형식의 포인터, 이는 바인딩할 입력 구성을 나타냄
	* ii. Object : 사용자 클래스(UserClass)의 객체에 대한 포인터, 바인딩된 함수가 호출될 때, 이 객체를 사용한다.
	* iii. PressedFunc : 눌림 이벤트가 발생했을 때 호출될 함수의 형식을 나타냄.
	* iv. ReleasedFunc : 릴리스 이벤트가 발생했을 때, 호출될 함수의 형식을 나타냄. 
	* v. HeldFunc : 버튼이 눌린 상태로 유지되고 있는 동안 호출될 함수의 형식을 나타냄. 
	*/
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAuraInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	//입력 구성이 유효한지 체크
	check(InputConfig);

	//입력 구성 객체에 저장된 각 입력 액션에 대해 루프를 실행
	for (const FAuraInputAction& Action : InputConfig->AbilityInputActions)
	{
		//입력 액션이 존재하고, 유효한 태그가 있는지 확인.
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			//누름(PrssedFunc) 함수가 존재하는 경우, 해당 액션과 Started 이벤트에 대해 함수를 바인딩 함.
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}

			//뗌(ReleasedFunc) 함수가 존재하는 경후 해당 액션과 Completed 이벤트에 대해 함수를 바인딩 함.
			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}

			//유지(Held) 함수가 존재하는 경우 해당 액션과 Triggered 이벤트에 대해 함수를 바인딩 함.
			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}

			
		}
	}
}
