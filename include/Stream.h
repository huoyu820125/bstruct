#ifndef NET_MSG_H
#define NET_MSG_H

#ifdef WIN32
#else
#include <sys/types.h>
#endif

typedef int					int32;
typedef unsigned int		uint32;
#ifdef WIN32
typedef __int64				int64;
typedef unsigned __int64	uint64;
#else
typedef int64_t				int64;
typedef u_int64_t			uint64;
#endif

namespace bsp
{

/**
* ����
* ����/������Ϣ��
* ������Ϣ�����Ȱ󶨱�����Ϣ�Ļ��壬Ȼ�����AddData�������������
* ������Ϣ�����Ȱ�Ҫ��������Ϣ���壬Ȼ�����GetDataȡ�ò���
*/
class Stream
{
public:
	Stream();
	virtual ~Stream();

public:
	void Bind(unsigned char *pBuffer, unsigned uSize);//��һ�����������ڴ�֮����ת��
	//ȡ�û�������
	unsigned short GetBufferSize();
//////////////////////////////////////////////////////////////////////////
//��װ��Ϣ�����������
	//ȡ����װ�õ�������,����ͷ���������ݳ���
	unsigned char* GetStream();
	//ȡ����װ�õ���Ϣ�ĳ���
	unsigned short GetSize();
	//��װ����������������
	/**
	 * ����1����������Ϣ��,char��unsigned char��int�ȸ������͵�����
	 * �ɹ�����true��ʧ�ܷ���false
	 * ��ֱ�Ӵ��ݳ��������Զ�ƥ�䵽4~nbyte����С������
	 * ����AddData( 1 )����Ϊ�ǲ���һ��int��ռ4byte�����ϣ��ֻռ1byte����ʹ��AddData((char)1)
	 * ����AddData( 0xffffffff )����Ϊ�ǲ���һ��unsigned int��ռ4byte
	 * ����AddData( 0xffffffff ff)����Ϊ�ǲ���һ��int64��ռ8byte
	*/
	bool AddData(char value);
	inline bool AddData( unsigned char value )
	{
		return AddData( (char)value );
	}
	bool AddData(short value);
	inline bool AddData( unsigned short value )
	{
		return AddData( (short)value );
	}
	bool AddData(float value);
	bool AddData(double value);
	bool AddData(long value);
	inline bool AddData( unsigned long value )
	{
		return AddData( (long)value );
	}
	bool AddData(int32 value);
	inline bool AddData( uint32 value )
	{
		return AddData( (int32)value );
	}
	bool AddData(int64 value);
	inline bool AddData( uint64 value )
	{
		return AddData( (int64)value );
	}
	
	/**
	 * ����1���ṹ�Ͳ�������Ϣ��
	 * void*��ʾ�ṹ��ַ
	 * unsigned short��ʾ�ṹ��С��
	 * byte�����ַ�����Ϊ��������ݽṹ
	 * 
	 * �ɹ�����true��ʧ�ܷ���false
	 */
	bool AddData(const void* pStruct, unsigned short uSize);
	
//////////////////////////////////////////////////////////////////////////
//�����󶨵���������ȡ����Ϣ����
	//�Ѷ���ĩβ
	bool IsEnd();
	/**
	* ������ȡ��һ������char��unsigned char��int�ȸ������͵�����
	* �ɹ�����true��ʧ�ܷ���false
	*/
	bool GetData(char *value);
	inline bool GetData(unsigned char *value)
	{
		return GetData((char*) value);
	}
	bool GetData(short *value);
	inline bool GetData(unsigned short *value)
	{
		return GetData((short*) value);
	}
	bool GetData(float *value);
	bool GetData(double *value);
	bool GetData(long *value);
	inline bool GetData(unsigned long *value)
	{
		return GetData((long*) value);
	}
	bool GetData(int32 *value);
	inline bool GetData(uint32 *value)
	{
		return GetData((int32*) value);
	}
	bool GetData(int64 *value);
	inline bool GetData(uint64 *value)
	{
		return GetData((int64*) value);
	}
	/**
	 * ����Ϣ��ȡ�ñ䳤����
	 * �ɹ�����true��ʧ�ܷ���false
	 * 
	 * pStruct�������Ļ���
	 * uSize���뻺������������ʵ�ʶ�����С
	 * 
	 */
	bool GetData(void* pStruct, short *uSize);
	bool GetData(void* pStruct, unsigned short *uSize)
	{
		return GetData( pStruct, (short*)uSize );
	}
	/**
	 * ����Ϣ��ȡ�ñ䳤�����ָ��
	 * �ɹ����ض���ָ�룬ʧ�ܷ���NULL
	 * uSize���ص�ַ��С
	*/
	unsigned char* GetPointer( short *uSize );
	unsigned char* GetPointer( unsigned short *uSize )
	{
		return GetPointer( (short*)uSize );
	}
protected:
	//�󶨵Ļ�����
	unsigned char* m_pMsgBuffer;
	//�����С/��������Ϣ����
	unsigned short m_uSize;
	//��Ϣ���/��������λ��
	unsigned short m_uPos;
	
};

//��buf�ֽڰ��յ��ֽ���ǰ�����ֽ��ں��˳����ϳ�һ������
uint64 memtoi( unsigned char *buf, int size );
//��һ�����������յ��ֽ���ǰ�����ֽ��ں��˳�򱣴浽buf
void itomem( unsigned char *buf, uint64 value, int size );
}

#endif //NET_MSG_H
