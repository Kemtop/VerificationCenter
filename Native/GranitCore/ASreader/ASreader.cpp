#include "ASreader.h"
#include "ASblockData.h"

ASreader::ASreader()
{
	begin_sign_block=0;
}


ASreader::~ASreader()
{
}

//������ ���� ���������� ������� �������;
int ASreader::ReadSignData(QByteArray SignBlock)
{
	uint8_t title[5]; //��������� 5 ���� [���][�����]
	int block_len = 0; //����� ����� ������.

	int f_len = SignBlock.length();//����� �����
	int cur_pos = 0;//������� �������.

	while (cur_pos < f_len)
	{
		QByteArray tl = SignBlock.mid(cur_pos,5); //������� 5 ����.
		std::memcpy(title,tl.constData(),5); //������� ���� � title.
		cur_pos += 5; //�������� ������� ���������.

		if ( !((title[0]==116)||(title[0] == 117) ) ) return 1; //���� �� �������� ������ � �������.

		ASblockData B;
		B.type = title[0]; //���
		
		DecodeLen(title, block_len); //������� ����� ����� ������
		B.len = block_len; //����� ����� ������
		B.data = SignBlock.mid(cur_pos, block_len); //������ ����� ������.       
		cur_pos += block_len;
		//�� ���������� ������� ���� ������ ���������� �������
		if (block_len != B.data.size()) return 2;

		Blocks.append(B);
	}

	return 0;
}										   
bool ASreader::Read(QFile & fileIn, qint64 f_len)
{
	//����������� ����� ������������� �����
	/*
	[7 ���� ��� �� ����� ���� ������][��������� 47][��� ���� 34.11 ��������� ������� �� ��� 64����][������ ����� ����������� ������ 8����]
	7+47+64=118

	*/

	//������������ ������� �������(����������� ����) �� ������ ����������� ������� ����� ������
	if (!fileIn.seek(118)) return 0;

	//���������� ������� ����� ������
	uint8_t arr_len[8];
	char F_block[32];

	if (fileIn.read(F_block, 8) != 8)	return 0;
	for (uint8_t i = 0;i < 8;i++)  arr_len[i] = static_cast<uint8_t>(F_block[i]);

	//�������������� ������ � �����
	quint64 data_len = 0;
	quint64 tmp = 0;

	for (uint8_t i = 0;i < 8;i++)
	{
		tmp = 0;
		tmp = tmp | arr_len[i];
		tmp = tmp << i * 8;
		data_len = data_len | tmp;
	}


	//�������� ������� � ������� ������������� ����������� ������
	data_len = data_len + 118 + 8; //������� ����� ����� ������

	 //������������ ������� �������(����������� ����) �� ������ ����� ������
	if (!fileIn.seek(data_len)) return 0;
	//���� �� ���� ������ ���� �� �������� ����� �����
	quint64 pos = data_len;

	uint8_t title[5]; //��������� 5 ���� [���][�����]
	int block_len=0;

	quint64 begin_pos = 0; //������� ������ �����
	while (fileIn.pos() < f_len)
	{
		begin_pos = fileIn.pos(); //�������� ������� ������ ����� ������, ����� ��� ��������� ������ ����� �������
		fileIn.read(reinterpret_cast<char*>(title), 5);
		ASblockData B;
		B.type =title[0]; //���
		
		//���� ��������� ��������� ���� �������
		if (title[0] == 116) begin_sign_block = begin_pos;

		DecodeLen(title, block_len); //������� ����� ����� ������
		B.len = block_len; //����� ����� ������
		B.data=fileIn.read(block_len);
		//�� ���������� ������� ���� ������ ���������� �������
		if (block_len != B.data.size()) return 0;

		Blocks.append(B);
		
	}

	return 1;
}

void ASreader::DecodeLen(uint8_t *t, int & len)
{
	//�������������� ������ � �����
	len = 0;
	int tmp = 0;

	for (uint8_t i = 1;i < 5;i++)
	{
		tmp = 0;
		tmp =static_cast<int>(t[i]);
		tmp = tmp << (i-1)* 8;
	    len = len | tmp;
	}
}


bool ASreader::GetSignVector(QByteArray & r, QByteArray & s)
{		
	//���� �� ���� ������ 
	bool found = 0;
	//����� r
	for (int i = 0;i < Blocks.count();i++)
	{
		//���� ������ ���� ���������� ������ r �������
		if (Blocks[i].type == 116)
		{
			r = Blocks[i].data;
			
			found = 1;
			break;
		}
	}
	//���� �� ������
	if (found == 0) return 0;
	
	//����� s
	found = 0; //����� �����
	for (int i = 0;i < Blocks.count();i++)
	{
		//���� ������ ���� ���������� ������ r �������
		if (Blocks[i].type == 117)
		{
			s = Blocks[i].data;
			found = 1;
			break;
		}
	}
	//���� �� ������
	if (found == 0) return 0;

	return true;
}

bool ASreader::GetSignKeyIndex(string & k_index)
{
	
	//���� �� ���� ������ 
	bool found = 0;
	for (int i = 0;i <Blocks.count();i++)
	{
		//���� ������ ���� ���������� ����������� ��������� ����
		if (Blocks[i].type == 118)
		{
			//�������������� � ������
			//string t1(Blocks[i].data.data(), Blocks[i].data.size());
			k_index = Blocks[i].data.toHex().toStdString();
//			k_index = t1;
			found = 1;
			break;
		}
	}
	//���� �� ������
	if (found == 0) return false;

	return true;
}
