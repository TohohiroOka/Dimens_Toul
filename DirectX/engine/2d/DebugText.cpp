#include "DebugText.h"
#include <string>

std::unique_ptr<Sprite> DebugText::spriteDatas[maxCharCount] = {};

DebugText* DebugText::GetInstance()
{
	static DebugText instance;
	return &instance;
}

void DebugText::Initialize()
{
	// 全てのスプライトデータについて
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		// スプライトを生成する
		spriteDatas[i] = Sprite::Create("debugfont");
		spriteDatas[i]->SetAnchorpoint({ 0, 0 });
	}
}

void DebugText::Print(const std::string& _text, float _x, float _y, DirectX::XMFLOAT3 _color, float _size)
{
	SetPos(_x, _y);
	SetSize(_size);
	SetColor(_color);

	NPrint((UINT)_text.size(), _text.c_str());
}

void DebugText::NPrint(int _len, const char* _text)
{
	// 全ての文字について
	for (int i = 0; i < _len; i++)
	{
		// 最大文字数超過
		if (spriteIndex >= maxCharCount) {
			break;
		}

		// 1文字取り出す(※ASCIIコードでしか成り立たない)
		const unsigned char& beeline = _text[i];

		int fontIndex = beeline - 32;
		if (beeline >= 0x7f) {
			fontIndex = 0;
		}

		float fontIndexY = float(fontIndex / fontLineCount);
		float fontIndexX = float(fontIndex % fontLineCount);

		// 座標計算
		spriteDatas[spriteIndex]->SetPosition({ this->posX + fontWidth * this->size * i, this->posY });
		spriteDatas[spriteIndex]->SetTexLeftTop({ fontIndexX * fontWidth, fontIndexY * fontHeight });
		spriteDatas[spriteIndex]->SetTexSize({ float(fontWidth), float(fontHeight) });
		spriteDatas[spriteIndex]->SetSize({ fontWidth * this->size, fontHeight * this->size });
		//更新
		spriteDatas[spriteIndex]->Update();

		spriteDatas[spriteIndex]->SetColor({ color.x,color.y,color.z,1 });

		// 文字を１つ進める
		spriteIndex++;
	}
}

void DebugText::DrawAll()
{
	// 全ての文字のスプライトについて
	for (int i = 0; i < spriteIndex; i++)
	{
		// スプライト描画
		spriteDatas[i]->Draw();
	}

	spriteIndex = 0;
}

void DebugText::Finalize()
{
	for (int i = 0; i < maxCharCount; i++)
	{
		spriteDatas[i].reset();
	}
}