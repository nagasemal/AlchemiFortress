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
	//�t�@�C����ǂݎ���p�ŊJ��

	std::string filename("");

	ifs.open(filename, std::ios::in | std::ios::binary);
	//�t�@�C�����J���Ȃ������ꍇ�̓G���[���b�Z�[�W���o��
	if (!ifs)
	{
		return;
	}

	const int xMax = 6;

	std::string s[xMax];				//��s���̃f�[�^��񐔕������z��

	for (int x = 0; x < xMax; x++)
	{
		//��s���̃f�[�^��ǂݍ���
		ifs >> s[x];
		//�ǂݍ��񂾃f�[�^�̋�؂蕶��(,)�𔼊p�X�y�[�X( )�ɒu��������
		std::string tmp = std::regex_replace(s[x], std::regex(","), " ");
		//�������₷�����邽�߂ɃX�g���[���`���ɂ���
		std::istringstream iss(tmp);

		for (int y = 0; y < 5; y++)
		{
			int num = -1;

			//�󔒂܂ł̃f�[�^��ǂݍ���

			iss >> num;

		}
	}

	//�J�����t�@�C�������
	ifs.close();

}
