// Copyright Min Creater


#include "AuraAssetManager.h"
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
}
