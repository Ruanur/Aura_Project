// Copyright Min Creater


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aura/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FAuraAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("AbilityInfo[%s] ���� AbilityTag[%s]�� ã�� �� �����ϴ�. "), *GetNameSafe(this), *AbilityTag.ToString());
	}

	return FAuraAbilityInfo();
}
