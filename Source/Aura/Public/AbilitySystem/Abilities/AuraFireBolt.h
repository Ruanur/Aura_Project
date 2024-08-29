// Copyright Min Creator

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()
	
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Firebolt")
	float ProjectileSpread = 90.f;

	UPROPERTY(EditDefaultsOnly, Category = "Firebolt")
	float MaxNumProjectiles = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Firebolt")
	float HomingAcclerationMin= 1600.f;

	UPROPERTY(EditDefaultsOnly, Category = "Firebolt")
	float HomingAcclerationMax = 3200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Firebolt")
	bool bLaunchHomingProjectiles = true;
};
