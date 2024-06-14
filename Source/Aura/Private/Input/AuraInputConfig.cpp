// Copyright Min Creater


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FAuraInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("InputTag에 대한 AbilityInputAction을 찾을 수 없음. [%s], InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this))
	}

	return nullptr;
}
