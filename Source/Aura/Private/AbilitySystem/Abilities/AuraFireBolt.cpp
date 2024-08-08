// Copyright Min Creator


#include "AbilitySystem/Abilities/AuraFireBolt.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
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
			ScaledDamage);
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
			ScaledDamage);
	}
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
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
		ScaledDamage);
}
