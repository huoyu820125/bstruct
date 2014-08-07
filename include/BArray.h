// BArray.h: interface for the BArray class.
//
//////////////////////////////////////////////////////////////////////

#ifndef BARRAY_H
#define BARRAY_H

#include "Stream.h"
#include <string>
#include <vector>

/*
 *  �����ƶ�������
 *	Ԫ�ؿ�������ͨ��char int float��Ҳ������BStruct�����顢byte[]
 *	�����ʽ��Ԫ�ش�С(unsigned short,0��ʾԪ�ش�С�䳤)+Ԫ��1+...+Ԫ��n
 *	Ԫ�ر����ʽ�����Ԫ�ش�С��0������Ԫ�أ���ֵ
 *	Ԫ�ر����ʽ�����Ԫ�ش�СΪ0���䳤Ԫ�أ���ֵ����(unsigned short) + ֵ	
 *
 *
 *	������short int�����ͣ��Ķ����ƴ�����ʽ���ֽ���
 *	���ڼ����ͳ�Ա���Ե�λ��ǰ����λ�ں�
 *		����BArray msg; msg["prot"] = (short)0xf1f2;������ǵ�λ��ǰ���ڽṹ�о���0xf2f1
 *	���ڱ䳤���ݣ�����struct�е�int �ȳ�Ա���ֽ���Ϊ����Ĭ�ϣ������в���
 *
 *	������float double��Ϊmemcpyת����byte������c/c++client������ת��
 *
 *
 *	����
 *
 *	����ṹ
 *	unsigned char buf[256];
 *	BArray msg;
 *	msg.Bind(buf,256);
 *	msg[1] = "192.168.0.1";
 *	msg[2] = (short)8888;
 *
 *	�����ṹ
 *	BArray msg;
 *	unsigned buf;//���ⷽʽ��BStruct��Ա\����\�ļ�\�ڴ棩�õ���һ������������
 *	unsigned short size;//���������������
 *	msg.Resolve(buf,size);//�ṹ���ڴ�
 *	msg.GetCount();//Ԫ�ظ���
 *	string str = msg[1];//ȡ��192.168.0.1
 *	short port = msg[2];//ȡ��8888
*/


namespace bsp
{
	
class BArray;
class BStruct;
/*
 *	��Աֵ
 */
struct E_VALUE
{
	int m_index;//��Ա��������λ��
	char *m_data;//��Ա��ֵ�ĵ�ַ������struct��byte���ȶ�����Ƕ����ַ
	unsigned short m_size;//��Ա���ȣ�����struct��byte���ȶ�����Ƕ����С
public:
	E_VALUE(){}
	~E_VALUE(){}

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
//	operator long();
//	operator unsigned long()
//	{
//		return (long)*this;
//	}
//	bool operator = ( long value );
//	bool operator = ( unsigned long value )
//	{
//		return *this = (long)value;
//	}
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

	operator BArray();
	bool operator = ( BArray *value );
	operator BStruct();
	bool operator = ( BStruct *value );
		
private:
	BArray *m_parent;
	friend class BArray;
};

class BArray  
{
private:
	enum action
	{
		unknow = 0,
		write = 1,
		read = 2
	};
	friend struct E_VALUE;
public: 
	BArray();
	BArray( const char *name );
	virtual ~BArray();

public:
	void SetName( const char* name );
	const char* Name();
	/*
		����Ԫ�س��ȣ�������Ĭ��Ϊ�䳤Ԫ�أ�������Ϊ����Ԫ��
		��������Bind()ǰ���ã�Bind()�Ὣ����Ϣ����������У�Bind()�����þ���Ч��
		�����ڽ�����SetElementSize()��Ч��Resolve()������еõ�����Ԫ�������Ƕ������Ǳ䳤��
		����SetElementSize()���
	*/
	void SetElementSize(unsigned short size);
	void Bind(unsigned char *pBuffer, unsigned int uSize);//�󶨻���,�û����
	//������������������ʹ��[]������ֱ��ȡ����Ӧ��Ա���
	bool Resolve(unsigned char *pBuffer, unsigned int uSize);
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
	E_VALUE operator []( int index );
	unsigned char* GetStream();//ȡ��������
	unsigned int GetSize();//ȡ������������
	unsigned short GetElementSize();//ȡ��Ԫ�س���
	unsigned int GetCount();//ȡ�������С
   /*
		Ϊ����ĳ���䳤�������׼�����壬���ػ����׵�ַ
		��PreSize()һ��ʹ�ã��õ���׼���Ļ��������
		���������Ϻ󣬱���ʹ��ʹ��[]�������������󱣴浽�ṹ�У�
		�����ٴε���PreBufferΪ�³�Ա׼������
		����
		unsigned char buf[1024]//��Ϣ����
		BArray msg(buf,1024,BArray::write);//����һ���ṹ��ʹ��buf��Ϊ����
		//����һ��client��Ա��ʹ��msgĩβ��buf��Ϊitem�Ļ���
		//����Ա����client���浽msg�Ļ��壨buf����ĩβ��
		//���Բ�����������PreBuffer��������ɺ�������
		BArray item(msg.PreBuffer("client"),msg.PreSize(),BArray::write);
		���item��������䣬�������Զ�����
		msg["client"] = item;//��item���ӵ�msg��������ĩβ
	*/
	unsigned char* PreBuffer();
	unsigned int PreSize();//׼���Ļ��������
	bool IsValid();//��Ч����true,��Ч����false
	bool IsEmpty();//�շ���true
private:
	bool Resolve();//�����󶨵�������
private:
	char m_name[256];
	Stream m_stream;
	std::vector<char*> m_data;
	E_VALUE m_error;//����ʧ��ʱ���ش�������
	bool m_finished;
	action m_action;
	bool m_bValid;
	bool m_bEmpty;
	unsigned short m_elementSize;
};

}
#endif // !defined(BARRAY_H)
