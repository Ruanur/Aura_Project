// Copyright Min Creater


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//ActivateAbility의 기본 클래스 구현 호출
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//참고: 여기에서 능력을 활성화하는 추가 기능을 추가할 수 있다.
}

void UAuraProjectileSpell::SpawnProjectile()
{
	//서버가 투사체를 생성할 권한이 있는지 확인 
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	//서버가 아니면 함수 종료
	if (!bIsServer) return;

	//CombatInterface를 캐스팅하여 가져옴
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		//전투 소켓 위치를 가져옴
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

		//스폰 트랜스폼 설정
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		//TODO : 투사체 회전 설정

		//투사체를 지연 생성
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		//TODO : 투사체에 피해를 입히기 위한 게임플레이 효과 스펙 부여

		//투사체 스폰 완료
		Projectile->FinishSpawning(SpawnTransform);
	}
}
