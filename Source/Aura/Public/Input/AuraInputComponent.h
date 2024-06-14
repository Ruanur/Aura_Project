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
	//���ø��� ����Ͽ� �Լ��� ���ε��ϴ� �Լ��� �����ϴ� �ڵ� ������
	//���⼭ �Լ��� ������ ���� �μ��� ���Ѵ�.
	/*
	* i. InputConfig : UAuraInputConfig ������ ������, �̴� ���ε��� �Է� ������ ��Ÿ��
	* ii. Object : ����� Ŭ����(UserClass)�� ��ü�� ���� ������, ���ε��� �Լ��� ȣ��� ��, �� ��ü�� ����Ѵ�.
	* iii. PressedFunc : ���� �̺�Ʈ�� �߻����� �� ȣ��� �Լ��� ������ ��Ÿ��.
	* iv. ReleasedFunc : ������ �̺�Ʈ�� �߻����� ��, ȣ��� �Լ��� ������ ��Ÿ��. 
	* v. HeldFunc : ��ư�� ���� ���·� �����ǰ� �ִ� ���� ȣ��� �Լ��� ������ ��Ÿ��. 
	*/
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAuraInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	//�Է� ������ ��ȿ���� üũ
	check(InputConfig);

	//�Է� ���� ��ü�� ����� �� �Է� �׼ǿ� ���� ������ ����
	for (const FAuraInputAction& Action : InputConfig->AbilityInputActions)
	{
		//�Է� �׼��� �����ϰ�, ��ȿ�� �±װ� �ִ��� Ȯ��.
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			//����(PrssedFunc) �Լ��� �����ϴ� ���, �ش� �׼ǰ� Started �̺�Ʈ�� ���� �Լ��� ���ε� ��.
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}

			//��(ReleasedFunc) �Լ��� �����ϴ� ���� �ش� �׼ǰ� Completed �̺�Ʈ�� ���� �Լ��� ���ε� ��.
			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}

			//����(Held) �Լ��� �����ϴ� ��� �ش� �׼ǰ� Triggered �̺�Ʈ�� ���� �Լ��� ���ε� ��.
			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}

			
		}
	}
}
