#pragma once
#include <string>
#include "mpirxx.h"
#include <vector>
#include <QDateTime>
#include "ExtensionField.h"
#include "Point.h"

using std::string;

class Q_DECL_EXPORT TBSCertificate
{
public:
	TBSCertificate();
	~TBSCertificate();

	int ver; //������ �����������
	mpz_class serialNumber;
	string AlgorithmIdentifier; //������������� ���������, ������������� �� ��� ������� �����������
	//string Issuer_e_mailAddress;//e_mail �������� �����������
	string Issuer_id_at_countryName; //������ ��������
	string Issuer_id_at_localityName; //�������������� ��������
	string Issuer_id_at_organizationName;//����������� �������� ����������
	string Issuer_id_at_organizationalUnitName; //������������� ��������
	string Issuer_id_at_commonName; //������������ ��� ����������� �������� ����������.
	string Issuer_id_at_streetAddress;//������ ��������
	string Issuer_id_at_emailAddress;


	QDateTime  NotBefore; //������ �������� �����������
	QDateTime NotAfter;
	//������ �������� ������� ������ � �������� ������.
	string Subject_e_mailAddress;//e_mail �������� �����������
	string Subject_id_at_countryName; //������ ��������
	string Subject_id_at_localityName; //�������������� ��������
	string Subject_id_at_organizationName;//����������� �������� ����������
	string Subject_id_at_organizationalUnitName;
	string Subject_id_at_commonName; //������������ ��� ����������� �������� ����������.
	string Subject_id_at_streetAddress;//����� ��������� �����������
	string Subject_id_at_emailAddress;
	 // subjectPublicKeyInfo
	string publicKeyParamSet;
	string digestParamSet;
	string encryptionParamSet;
	//--subjectPublicKey
	mpz_class Qx;
	mpz_class Qy;

	//---Extensions --���������� 
	vector <ExtensionField>  extensions;
	string CerR=""; //������� �����������(������������ � ����� �����������),������ R.
	string CerS=""; //������� �����������(������������ � ����� �����������),������ S.
	string calcR=""; //�������� ������� R � ���������� ���������� ������� �����������.
	string calcS=""; //�������� ������� S � ���������� ���������� ������� �����������.
	string  digest; //�������� ���������.

	//������� ����������� ��������� ������ ����� ������������� ������ -�������
	mpz_class q;
	//������ ���������� x ����� �������� �������(��������� �����)
	void setQx(string str_Qx);	
	//������ ���������� x ����� �������� �������(��������� �����)
	void setQy(string str_Qy);
	//���������� ��������� �������� ����� �������� �������(��������� �����) �������������� ��������  setQx 
	string getQx();
	//���������� ��������� �������� ����� �������� �������(��������� �����) �������������� ��������  setQy
	string getQy();

	//������������� ���������� 2.5.29.15,  ��������� ����, ��� ������� ������������ ����������������� �������� ����.
	void SetkeyUsageExtension();
	//������������� ���������� 2.5.29.19 ��� ���������� ���������, ����� �� ������� ����������� ��� ��
	void SetbasicConstraintsExtension();
	//������������� ���������� ������ ��� �������
	void setUsageExtensionOnlySign();
   
	/*
	��� ���������� �������������� �������� ����. �� ��������� ���������������� ��������� �����, 
	������������ ����� � ��� �� ��������� (��������, ��� ���������� �����).
     ������������� ����� ������ ���� ���������� �� ��������� �� ���� ��������
	 ��������������� ��������, � ������� �� ������������. ��� ���������� ������ �� ��������.
	*/
	void subjectKeyIdentifierExtension();
	//������ ������ ������������
	void verExtension();
	//������������� �������� ����
	void setPubKey(Point Q);
	//��������� �������� ����
	Point GetPubKey();
	//������������� ������ �������� �� ��������� ������ subject. 
	void setIssuerFromSubject(TBSCertificate subject);
	/*
	   ������������� ��������� �����������. �������� ������ �� ��������.
	   ���������� ��������� �����������,��������� �������� ������. 
	*/
	void setAlgoritms(TBSCertificate parent);
		
	void setTbsBin(vector<unsigned char> val);
	void setSignValBin(vector<unsigned char> val);

	void Tr512Sign(); //��������������� ������� � mpz_class S,R.
	//���������� ������ R �������.
	mpz_class GetR();
	//���������� ������ S �������.
	mpz_class GetS();
	//������������� �������� ��������� �����-������� Qx,Qy.
	void Set512PubKey(vector<uint8_t> val);
	//���������� �������� ������������� tbs.
	vector<uint8_t> GetBinTbs();


private:
	//��������� ������������� ��������� �����. ����������� �������� setQx setQy 
	string val_str_Qx;
	string val_str_Qy;

	//�������� ���� ��������� �����������.
	Point pubECkey;

	vector<unsigned char> tbsBin;//�������� ������������� ����� ����������� ������� ��������� �� �����������.
	vector<unsigned char> signValBin; //�������� ������������� ������� ����������� ��������� �� �����������.

	mpz_class S; //������� ������� �����������.
	mpz_class R;
};

