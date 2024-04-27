// Copyright Min Creater

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 * 
 * Native GameplayTags�� �����ϴ� Singleton
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
