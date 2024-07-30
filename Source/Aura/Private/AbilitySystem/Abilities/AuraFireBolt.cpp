// Copyright Min Creator


#include "AbilitySystem/Abilities/AuraFireBolt.h"
#include "Aura/Public/AuraGameplayTags.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 Damage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			//�ɷ� �̸�
			"<Title>Fire Bolt</>\n\n"

			//�ɷ� ����
			"<Small>Level: </><Level>%d</>\n"

			//�ɷ� ���� �Ҹ�
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"

			//��Ÿ��
			"<Small>Cooldown: </><Cooldown>: %.1f</>\n\n"

			//����
			"<Default>ȭ�� ��ü�� �߻��Ͽ� " 

			//���ط�
			"</><Damage>%d</> ���ظ� �����ϴ�."
			"<Default> ���� Ȯ���� ȭ�� ���ظ� �Խ��ϴ�.</>"),

			//��
			Level,
			ManaCost,
			Cooldown,
			Damage);
	}
	else
	{
		return FString::Printf(TEXT(
			//�ɷ� �̸�
			"<Title>Fire Bolt</>\n\n"

			//�ɷ� ����
			"<Small>Level: </><Level>%d</>\n"

			//�ɷ� ���� �Ҹ�
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"

			//��Ÿ��
			"<Small>Cooldown: </><Cooldown>: %.1f</>\n\n"

			//ȭ�� ��ü ����
			"<Default>ȭ�� ��ü�� %d�� �߻��Ͽ� "

			//���ط�
			"</><Damage>%d</> ���ظ� �����ϴ�."
			"<Default> ���� Ȯ���� ȭ�� ���ظ� �Խ��ϴ�.</>"),

			//��
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, NumProjectiles),
			Damage);
	}
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 Damage = DamageTypes[FAuraGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		//�ɷ� �̸�
		"<Title>Fire Bolt</>\n\n"

		//�ɷ� ����
		"<Small>Level: </><Level>%d</>\n"

		//�ɷ� ���� �Ҹ�
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"

		//��Ÿ��
		"<Small>Cooldown: </><Cooldown>: %.1f</>\n\n"

		//ȭ�� ��ü ����
		"<Default>ȭ�� ��ü�� %d�� �߻��Ͽ� "

		//���ط�
		"</><Damage>%d</> ���ظ� �����ϴ�."
		"<Default> ���� Ȯ���� ȭ�� ���ظ� �Խ��ϴ�.</>"),

		//��
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, NumProjectiles),
		Damage);
}
