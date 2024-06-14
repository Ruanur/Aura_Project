// Copyright Min Creator


#include "AuraAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "AuraGameplayTags.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
    // TODO: 여기에 return 문을 삽입합니다.

    check(GEngine);

    UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
    return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
    Super::StartInitialLoading();
    FAuraGameplayTags::InitializeNativeGameplayTags();

    //This is required to use Target Data
    UAbilitySystemGlobals::Get().InitGlobalData();
}
