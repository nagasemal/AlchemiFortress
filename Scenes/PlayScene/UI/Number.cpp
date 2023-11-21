#include "pch.h"
#include "Number.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#include "NecromaLib/GameData/Easing.h"

#define MAX_NUM 2
#define SPRITE_WEIGHT 64
#define SPRITE_HEIGHT 64

Number::Number():
	m_num(),
	m_position(),
	m_rage(),
	m_color(0.0f,0.0f,0.0f,1.0f),
	m_animationPosTime(),
	m_prevNum(),
	m_animationFlag()
{
}

Number::Number(SimpleMath::Vector2 position, SimpleMath::Vector2 rage)
{
	m_prevNum = m_num = 0;
	m_position = position;
	m_rage = rage;

	m_position.x += 32 * m_rage.x;

	m_animationFlag = false;
	m_animationPosTime = 0.0f;
}

Number::~Number()
{
}

void Number::SetNumber(int num)
{
	// 前回の値を保存する
	if (m_prevNum != m_num)
	{
		m_prevNum = m_num;
		m_animationPosTime += 0.1f;
		m_animationFlag = true;

	}

	m_num = num;
}

void Number::Render()
{
	if (m_animationFlag)
	{
		m_animationPosTime += DeltaTime::GetInstance().GetDeltaTime() * 2.0f;

		if (m_animationPosTime >= 1.0f) m_animationFlag = false;
	}
	else
	{
		m_animationPosTime -= DeltaTime::GetInstance().GetDeltaTime();
	}

	m_animationPosTime = std::min(std::max(0.0f, m_animationPosTime), 1.0f);

	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();
	SpriteLoder::GetInstance().GetNumberTexture();

	// 桁数取得
	int numDigit = (int)log10(m_num) + 1;

	if (numDigit <= 0) numDigit = 1;

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	for (int i = numDigit; i >= 0; i--)
	{
		// 基数　桁数取得
		int base = 1;

		for (int j = 0; j < i; j++)
		{
			// i桁目の数字を[0, 9]の範囲として取得
			int digit = (m_num / base) % 10;
			RECT numRect = SpriteCutter(64, 64,digit, 0);

			SimpleMath::Vector2 pos = m_position;
			pos.x -= ((64 * m_rage.x) * j);
			pos.y -= Easing::EaseOutCubic(0.0f,5.0f,m_animationPosTime);

			// 数字描画
			pSB->Draw(SpriteLoder::GetInstance().GetNumberTexture().Get(),
				pos,
				&numRect,
				m_color,
				0.0f,
				DirectX::XMFLOAT2(64 / 2, 64 / 2),
				m_rage);

			// 10のi乗を計算
			base *= 10;

		}
	}

	pSB->End();
}

void Number::Render_SelectScene(int first, int next)
{

	Number_Render(first,m_position);

	m_position.x += 32.0f;

	Number_Render(next, m_position);

}

void Number::Number_Render(int num, SimpleMath::Vector2 pos)
{

	pos;
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();


	// 桁数取得
	int numDigit = (int)log10(num) + 1;
	if (numDigit <= 0) numDigit = 1;

	for (int i = numDigit; i >= 0; i--)
	{
		// 基数　桁数取得
		int base = 1;

		for (int j = 0; j < i; j++)
		{
			// i桁目の数字を[0, 9]の範囲として取得
			int digit = (num / base) % 10;
			RECT numRect = SpriteCutter(64, 64, digit, 0);

			SimpleMath::Vector2 render_pos = m_position;
			render_pos.x -= ((64 * (m_rage.x / 2)) * j);
			render_pos.y -= Easing::EaseOutCubic(0.0f, 2.5f, m_animationPosTime);

			// 数字描画
			pSB->Draw(SpriteLoder::GetInstance().GetNumberTexture().Get(),
				render_pos,
				&numRect,
				m_color,
				0.0f,
				DirectX::XMFLOAT2(64 / 2, 64 / 2),
				m_rage);

			// 10のi乗を計算
			base *= 10;

		}
	}
}
