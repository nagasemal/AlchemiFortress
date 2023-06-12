#include "pch.h"
#include "SCVLoder.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <regex>

SCVLoder::SCVLoder()
{
}

SCVLoder::~SCVLoder()
{
}

void SCVLoder::LoadEnemyData()
{

	std::ifstream ifs;
	//ファイルを読み取り専用で開く

	std::string filename("");

	ifs.open(filename, std::ios::in | std::ios::binary);
	//ファイルが開けなかった場合はエラーメッセージを出す
	if (!ifs)
	{
		return;
	}

	const int xMax = 6;

	std::string s[xMax];				//一行分のデータを列数分入れる配列

	for (int x = 0; x < xMax; x++)
	{
		//一行分のデータを読み込む
		ifs >> s[x];
		//読み込んだデータの区切り文字(,)を半角スペース( )に置き換える
		std::string tmp = std::regex_replace(s[x], std::regex(","), " ");
		//処理しやすくするためにストリーム形式にする
		std::istringstream iss(tmp);

		for (int y = 0; y < 5; y++)
		{
			int num = -1;

			//空白までのデータを読み込む

			iss >> num;

		}
	}

	//開いたファイルを閉じる
	ifs.close();

}
