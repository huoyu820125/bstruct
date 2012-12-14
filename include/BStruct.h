// BStruct.h: interface for the BStruct class.
//
//////////////////////////////////////////////////////////////////////

#ifndef PROTOCL_H
#define PROTOCL_H

#include "Stream.h"
#include <map>
#include <string>

/*
 *  �����ƽṹ
 *	�ǽ����������ݽṹ���ı������������е�ͨ��Э��
 *	��Ա������-ֵ���,����xml���Զ��帴�ӳ�Ա��Ƕ�ף�
 *
 *	�ṹ�Ķ����Ƹ�ʽ=��Ա1~��Աn���ܳ���(unsigned short)+��Ա1+��Ա2+��Ա3+��Ա4+...+��Աn,��Ա֮���޷ָ��
 *	���256����Ա������������Խ����ֳ�Ա����Ϊһ��BStructǶ�׽�ȥ
 *
 *	��Ա��ʽ=��Ա������(unsigned short) + ��Ա��(string) 
 *			+ ֵ����(unsigned short) + ֵ(byte��)
 *
 *	��Ϊÿ����Ա���ͳ�Աֵ����2�������ı�ʾ�����Կ��԰�n����Ա����Ϊһ��ֵ
 *	����ĳ����Աteacher������ֵ�����ǳ�Ա1+��Ա2+...+��Աn
 *
 *	������short int�����ͣ��Ķ����ƴ�����ʽ���ֽ���
 *	���ڼ����ͳ�Ա���Ե�λ��ǰ����λ�ں�
 *		����BStruct msg; msg["prot"] = (short)0xf1f2;������ǵ�λ��ǰ���ڽṹ�о���0xf2f1
 *	���ڱ䳤���ݣ�����struct�е�int �ȳ�Ա���ֽ���Ϊ����Ĭ�ϣ������в���
 *
 *	������float double��Ϊmemcpyת����byte������c/c++client������ת��
 *
 *
 *	����
 *
 *	����ṹ
 *	����1,�򵥳�Ա��
 *	unsigned char buf[256];
 *	BStruct msg;
 *	msg.Bind(buf,256);
 *	msg["ip"] = "192.168.0.1";
 *	msg["port"] = (short)8888;
 *	//����ֵ�п��ܻ�ʧ�ܣ������������if ( !(msg["ip"] = "192.168.0.1") )
 *	//ʧ��ԭ��ֻ��4���Ҷ��ǲ���ȷ��ʹ��BStruct�������Լ�鸳ֵ�����ʵû��ʵ�����壬
 *	//��Ϊ���Ѿ��÷��ʹ����ˣ���ʹ��鲶���˴��󣬱���Ҳ�ǷǷ��ģ�������Ȼ�����˱�����Σ�գ�
 *	//���߼����Ѿ��ǲ����õġ�
 *	//Ψһ������ǣ����Խ׶μ����԰������ִ���©������ʱ����
 *	//1.bufΪNULL�򳤶Ȳ���
 *	//2.��ǰģʽ�ǲ��Ǵ�������û�е���Bind()��Bind()֮���ֵ���Resolve()ģʽ������Ϊ����
 *	//3.�Ѿ�����ͬ����Ա���ҵ�ǰ���ݳ�������д�볤�Ȳ���ͬ���������޸�����
 *	//4.�������³�Ա��û����ɸ�ֵmsg["new_member"];�����ͻᴴ���³�Ա������ֵ
 *	
 *	����2,�������ݽṹ��
 *	�ṹ����3������:�����ļ�������ʱ�䣬���ӵ�ַ���������ӵ�ַ��һ���������ݽṹ������ip port2������
 *	unsigned char buf[256];
 *	BStruct msg;
 *	msg.Bind(buf,256);
 *	if ( !msg["cfg_file"].IsValid() )//���cfg_file�Ƿ����
 *	//�����Ƿ������²�������ݺϷ��ԣ�ʵ��ʹ��ʱ���鶼���
 *	msg["cfg_file"] = "etc\ser.cfg";//���������ļ�
 *	msg["heart_time"] = (char)60;//��������ʱ��
 *	BStruct adr;
 *	adr.Bind(msg.PreBuffer("ser"),msg.PreSize());//�󶨵�bufĩβ
 *	adr["ip"] = "192.168.0.1";
 *	adr["port"] = (short)8888;
 *	msg["connect_adr"] = adr;//�������ӵ�ַ
 *
 *	����3,�������ݽṹ��䷽ʽ2��ֻ������c/c++��
 *	�ṹ����ͬ��
 *	unsigned char buf[256];
 *	BStruct msg;
 *	msg.Bind(buf,256);
 *	msg["cfg_file"] = "etc\ser.cfg";//���������ļ�
 *	msg["heart_time"] = (char)60;//��������ʱ��
 *	struct adr
 *	{
 *		char ip[21];
 *		short port;
 *	}
 *	adr connect_adr;
 *	memcpy( connect_adr.ip, "192.168.0.1", strlen("192.168.0.1") );
 *	connect_adr.prot = 8888;
 *	msg["connect_adr"].SetValue(&connect_adr,sizeof(adr));//�������ӵ�ַ
 *
 *	�����ṹ
 *
 *	����1���������췶��1�нṹ��
 *	BStruct msg;
 *	unsigned buf;//���ⷽʽ������\�ļ�\�ڴ棩�õ���һ�������Ľṹ
 *	unsigned short size;//�ṹ����
 *	msg.Resolve(buf,size);//�ṹ���ڴ�
 *	string str = msg["ip"];//ȡ��192.168.0.1
 *	short port = msg["port"];//ȡ��8888
 *	
 *	����2���������췶��2�нṹ��
 *	BStruct msg;
 *	unsigned buf;//���ⷽʽ������\�ļ�\�ڴ棩�õ���һ�������Ľṹ
 *	unsigned short size;//�ṹ����
 *	msg.Resolve(buf,size);//�ṹ���ڴ�
 *	string cfg = msg["cfg_file"];//ȡ��etc\ser.cfg
 *	char ht = msg["heart_time"];//ȡ��60;
 *	BStruct adr = msg["connect_adr"];//ȡ�ø��ӳ�Ա
 *	string ip = adr["ip"];//ȡ��192.168.0.1
 *	short port = adr["port"];//ȡ��8888
 *	
 *	����3���������췶��3�нṹ��
 *	BStruct msg;
 *	unsigned buf;//���ⷽʽ������\�ļ�\�ڴ棩�õ���һ�������Ľṹ
 *	unsigned short size;//�ṹ����
 *	msg.Resolve(buf,size);//�ṹ���ڴ�
 *	string cfg = msg["cfg_file"];//ȡ��etc\ser.cfg
 *	char ht = msg["heart_time"];//ȡ��60;
 *	struct adr
 *	{
 *		char ip[21];
 *		short port;
 *	}
 *	adr *connect_adr = msg["connect_adr"].m_data;//ȡ�ø������ݽṹ��ַ
 *	connect_adr->ip;//192.168.0.1
 *	connect_adr->port;//ȡ��8888
*/
namespace bsp
{

class BStruct;
/*
 *	��Աֵ
 */
struct M_VALUE
{
	char *m_data;//��Ա��ֵ�ĵ�ַ������struct��byte���ȶ�����Ƕ����ַ
	unsigned short m_size;//��Ա���ȣ�����struct��byte���ȶ�����Ƕ����С
public:
	M_VALUE(){}
	~M_VALUE(){}

	bool IsValid();//��Ч����true,��Ч����false
	//���ñ䳤���ݵ���Ա�У�ȡ�䳤����ֱֵ�ӷ���m_data m_size
	bool SetValue( const void *value, unsigned short uSize );
	operator std::string();
	bool operator = ( char *value );
	bool operator = ( const char *value )
	{
		return *this = (char*)value;
	}
	operator char();
	operator unsigned char()
	{
		return (char)*this;
	}
	bool operator = ( char value );
	bool operator = ( unsigned char value )
	{
		return *this = (char)value;
	}
	operator short();
	operator unsigned short()
	{
		return (short)*this;
	}
	bool operator = ( short value );
	bool operator = ( unsigned short value )
	{
		return *this = (short)value;
	}
	operator float();
	bool operator = ( float value );
	operator double();
	bool operator = ( double value );
	operator long();
	operator unsigned long()
	{
		return (long)*this;
	}
	bool operator = ( long value );
	bool operator = ( unsigned long value )
	{
		return *this = (long)value;
	}
	operator int32();
	operator uint32()
	{
		return (int32)*this;
	}
	bool operator = ( int32 value );
	bool operator = ( uint32 value )
	{
		return *this = (int32)value;
	}
	operator int64();
	operator uint64()
	{
		return (int64)*this;
	}
	bool operator = ( int64 value );
	bool operator = ( uint64 value )
	{
		return *this = (int64)value;
	}

	operator BStruct();
	bool operator = ( BStruct value );
	
private:
	BStruct *m_parent;
	friend class BStruct;
};

class BStruct  
{
private:
	enum action
	{
		unknow = 0,
		write = 1,
		read = 2
	};
	friend struct M_VALUE;
public:
	BStruct();
	virtual ~BStruct();

public:
	void Bind(unsigned char *pBuffer, unsigned short uSize);//�󶨻���,�û����
	//������������������ʹ��[]������ֱ��ȡ����Ӧ��Ա���
	bool Resolve(unsigned char *pBuffer, unsigned short uSize);
	/*
	 *	ȡ�ó�Աname����ȡֵ/��ֵ����
	 *	��ֵ��
	 *		����ֱ�ӽ�char short int...�������ͱ��浽�ṹ��
	 *		�ɹ�����trueʧ�ܷ���false��һ���ǳ��Ȳ���
	 *		�������������������鸳ֵ�����Ƿ�ɹ�
	 *		if ( !(msg["port"] = 8080) ) ...;
	 *		�������ݽṹ������ʹ��SetValue���и�ֵ������
	 *		if ( !msg["ip"].SetValue( "127.0.0.1", 9 ) ) ...;
	 *
	 *		��1�����ֱ�Ӵ��ݳ��������Զ�ƥ�䵽4~nbyte����С������
	 *		����msg["port"] = 1;����Ϊ�ǲ���һ��int��ռ4byte�����ϣ��ֻռ1byte����msg["port"] = (char)1;
	 *		����msg["port"] = 0xffffffff����Ϊ�ǲ���һ��unsigned int��ռ4byte
	 *		����msg["port"] = 0xffffffff ff����Ϊ�ǲ���һ��int64��ռ8byte
	 *
	 *		��2����Ա��һ�α���ֵ�󣬳��Ⱦ�ȷ���ˣ��Ժ�ֻ�����޸�Ϊ��ͬ��С��ֵ��
	 *		���򴥷�assert����
	 *
	 *		��3��û�е���Bind()�������Ը��ƣ����򴥷�assert����
	 */
	M_VALUE& operator []( std::string name );
	unsigned char* GetStream();//ȡ��������
	unsigned short GetSize();//ȡ������������
	/*
		Ϊ����ĳ���䳤�������׼�����壬���ػ����׵�ַ
		��PreSize()һ��ʹ�ã��õ���׼���Ļ��������
		���������Ϻ󣬱���ʹ��ʹ��[]�������������󱣴浽�ṹ�У�
		�����ٴε���PreBufferΪ�³�Ա׼������
		����
		unsigned char buf[1024]//��Ϣ����
		BStruct msg(buf,1024,BStruct::write);//����һ���ṹ��ʹ��buf��Ϊ����
		//����һ��client��Ա��ʹ��msgĩβ��buf��Ϊitem�Ļ���
		//����Ա����client���浽msg�Ļ��壨buf����ĩβ��
		//���Բ�����������PreBuffer��������ɺ�������
		BStruct item(msg.PreBuffer("client"),msg.PreSize(),BStruct::write);
		���item��������䣬�������Զ�����
		msg["client"] = item;//��item���ӵ�msg��������ĩβ
	*/
	unsigned char* PreBuffer( char *key );
	unsigned short PreSize();//׼���Ļ��������
	bool IsValid();//��Ч����true,��Ч����false
private:
	bool Resolve();//�����󶨵�������
private:
	Stream m_stream;
	M_VALUE m_dataList[256];//�������֧��256����Ա
	M_VALUE m_error;//����ʧ��ʱ���ش�������
	int m_pos;
	std::map<std::string, M_VALUE*> m_dataMap;
	bool m_finished;
	action m_action;
	bool m_bValid;
};

}
#endif // !defined(PROTOCL_H)
