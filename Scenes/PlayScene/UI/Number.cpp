#include "pch.h"
#include "Number.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/GameData/SpriteCutter.h"

#define MAX_NUM 2
#define SPRITE_WEIGHT 64
#define SPRITE_HEIGHT 64

Number::Number():
	m_num(),
	m_position(),
	m_rage()
{
}

Number::Number(SimpleMath::Vector2 position, SimpleMath::Vector2 rage)
{
	m_num = 0;
	m_position = position;
	m_rage = rage;
}

Number::~Number()
{
}

void Number::SetNumber(int num)
{
	m_num = num;
}

void Number::Render()
{

	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();
	SpriteLoder::GetInstance().GetNumberTexture();

	SimpleMath::Color color(1, 1, 1, 1);

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	for (int i = MAX_NUM; i >= 0; i--)
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

			// 数字描画
			pSB->Draw(SpriteLoder::GetInstance().GetNumberTexture().Get(),
				pos,
				&numRect,
				color,
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

	SimpleMath::Color color(1, 1, 1, 1);

	for (int i = MAX_NUM; i >= 0; i--)
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

			// 数字描画
			pSB->Draw(SpriteLoder::GetInstance().GetNumberTexture().Get(),
				render_pos,
				&numRect,
				color,
				0.0f,
				DirectX::XMFLOAT2(64 / 2, 64 / 2),
				m_rage);

			// 10のi乗を計算
			base *= 10;

		}
	}
}
