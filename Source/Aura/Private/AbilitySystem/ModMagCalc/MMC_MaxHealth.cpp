//Copyright Min Creator


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

// UMMC_MaxHealth Ŭ���� ������ ����
UMMC_MaxHealth::UMMC_MaxHealth()
{
	//Vigor �Ӽ� ĸó ����
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	//ĸó�� ���� �Ӽ� �߰�
	RelevantAttributesToCapture.Add(VigorDef);
}

//�⺻ ũ�� ��� �Լ� ����
float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// �ҽ��� Ÿ���� �±� ����
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0.f;
	//Vigor �Ӽ� ĸó ũ�� ��������
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max<float>(Vigor, 0.f);

	int32 PlayerLevel = 1;
	//�÷��̾� ���� ��������
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}

	//�⺻ ü�� ���
	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
}
