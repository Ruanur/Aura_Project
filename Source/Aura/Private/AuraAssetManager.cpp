// Copyright Min Creator


#include "AuraAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "AuraGameplayTags.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
    // TODO: ���⿡ return ���� �����մϴ�.

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
