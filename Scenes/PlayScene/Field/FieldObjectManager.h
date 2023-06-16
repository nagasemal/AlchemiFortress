//--------------------------------------------------------------------------------------
// File: FieldObjectManager.h
//
//　フィールド上に設置される回転しないオブジェクトの管理
//
//  
// Date: 2023.6.14
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------
#pragma once
#include "Scenes/PlayScene/Field/Field.h"
#include "Scenes/PlayScene/Field/Crystal/Crystal.h"
#include <list>

class FieldObjectManager
{
public:
	FieldObjectManager();
	~FieldObjectManager();

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

public:

	// 結晶の生成
	void CrystalSpawn(int num);

	Field* GetFieldObject()													{ return m_field.get();}
	std::list<Crystal>* GetCrystalObject()									{ return m_crystals.get();}

private:

	// 床モデル
	std::unique_ptr<DirectX::Model> m_floorModel;
	// 結晶モデル
	std::unique_ptr<DirectX::Model> m_crystalModel;

	//フィールドオブジェクト
	std::unique_ptr<Field> m_field;

	// 結晶配列
	std::unique_ptr<std::list<Crystal>> m_crystals;

};