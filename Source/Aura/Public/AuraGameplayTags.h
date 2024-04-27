// Copyright Min Creater

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 * 
 * Native GameplayTags를 포함하는 Singleton
 */
struct FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get()
	{
		return GameplayTags;
	}

protected:

private:
	static FAuraGameplayTags GameplayTags;
};
