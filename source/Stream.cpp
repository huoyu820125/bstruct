#include <cstring>
#include "../include/Stream.h"

namespace bsp
{

//��һ�����������յ��ֽ���ǰ�����ֽ��ں��˳�򱣴浽buf
void itomem( unsigned char *buf, uint64 value, int size )
{
	int move = (size-1) * 8;
	int del = (size-1) * 8;
	int i = 0;
	for ( i = 0; i < size; i++ )
	{
		buf[i] = (char)((value << move) >> del);
		move -= 8;
	}
}

//��buf�ֽڰ��յ��ֽ���ǰ�����ֽ��ں��˳����ϳ�һ������
uint64 memtoi( unsigned char *buf, int size )
{
	uint64 value = 0;
	int i = 0;
	value += buf[size - 1];
	for ( i = size - 2; 0 <= i; i-- )
	{
		value <<= 8;
		value += buf[i];
	}
	return value;
}

Stream::Stream()
:m_pMsgBuffer(NULL), m_uSize(0), m_uPos(0)
{
}

Stream::~Stream()
{
}

void Stream::Bind(unsigned char *pBuffer, unsigned uSize)
{
	m_pMsgBuffer = pBuffer;
	m_uSize = uSize;
	m_uPos = 0;
}

//ȡ����װ�õ���Ϣ
unsigned char* Stream::GetStream()
{
	return this->m_pMsgBuffer;
}
//ȡ����װ�õ���Ϣ�ĳ���
unsigned short Stream::GetSize()
{
	return this->m_uPos;
}

unsigned short Stream::GetBufferSize()
{
	return this->m_uSize;
}

bool Stream::AddData( char value )
{
	if ( m_uPos + sizeof(char) > m_uSize ) return false;
	m_pMsgBuffer[m_uPos] = value;
	m_uPos += sizeof(char);
	
	return true;
}

bool Stream::AddData( short value )
{
	if ( m_uPos + sizeof(short) > m_uSize ) return false;
	itomem(&m_pMsgBuffer[m_uPos], value, sizeof(short));
	m_uPos += sizeof(short);
	
	return true;
}

bool Stream::AddData(float value)
{
	if ( m_uPos + sizeof(float) > m_uSize ) return false;
	memcpy( &m_pMsgBuffer[m_uPos], &value, sizeof(float) );
	m_uPos += sizeof(float);
	
	return true;
}

bool Stream::AddData(double value)
{
	if ( m_uPos + sizeof(double) > m_uSize ) return false;
	memcpy( &m_pMsgBuffer[m_uPos], &value, sizeof(double) );
	m_uPos += sizeof(double);
	
	return true;
}

bool Stream::AddData( long value )
{
	if ( m_uPos + sizeof(long) > m_uSize ) return false;
	itomem(&m_pMsgBuffer[m_uPos], value, sizeof(long));
	m_uPos += sizeof(long);
	
	return true;
}

bool Stream::AddData( int32 value )
{
	if ( m_uPos + sizeof(int32) > m_uSize ) return false;
	itomem(&m_pMsgBuffer[m_uPos], value, sizeof(int32));
	m_uPos += sizeof(int32);
	
	return true;
}

bool Stream::AddData( int64 value )
{
	if ( m_uPos + sizeof(int64) > m_uSize ) return false;
	itomem(&m_pMsgBuffer[m_uPos], value, sizeof(int64));
	m_uPos += sizeof(int64);
	
	return true;
}

bool Stream::AddData( const void* pStruct, unsigned short uSize )
{
	if ( m_uPos + uSize + 2 > m_uSize ) return false;
	if ( !AddData( uSize ) ) return false;
	//Ƕ���ֶΣ������û���ʱ�Ѿ����ƺ����ݣ������ظ�����
	if ( pStruct != &m_pMsgBuffer[m_uPos] )
	{
		memcpy( &m_pMsgBuffer[m_uPos], pStruct, uSize );
	}
	m_uPos += uSize;
	
	return true;
}

bool Stream::IsEnd()
{
	return m_uPos == m_uSize;
}

bool Stream::GetData( char *value )
{
	if ( m_uPos + sizeof(char) > m_uSize ) return false;
	memcpy( value, &m_pMsgBuffer[m_uPos], sizeof(char) );
	m_uPos += sizeof(char);

	return true;
}

bool Stream::GetData( short *value )
{
	if ( m_uPos + sizeof(short) > m_uSize ) return false;
	*value = (short)memtoi(&m_pMsgBuffer[m_uPos], sizeof(short));
	m_uPos += sizeof(short);
	
	return true;
}

bool Stream::GetData(float *value)
{
	if ( m_uPos + sizeof(float) > m_uSize ) return false;
	memcpy( value, &m_pMsgBuffer[m_uPos], sizeof(float) );
	m_uPos += sizeof(float);
	
	return true;
}

bool Stream::GetData(double *value)
{
	if ( m_uPos + sizeof(double) > m_uSize ) return false;
	memcpy( value, &m_pMsgBuffer[m_uPos], sizeof(double) );
	m_uPos += sizeof(double);
	
	return true;
}

bool Stream::GetData( long *value )
{
	if ( m_uPos + sizeof(long) > m_uSize ) return false;
	*value = (long)memtoi(&m_pMsgBuffer[m_uPos], sizeof(long));
	m_uPos += sizeof(long);
	
	return true;
}

bool Stream::GetData( int32 *value )
{
	if ( m_uPos + sizeof(int32) > m_uSize ) return false;
	*value = (int32)memtoi(&m_pMsgBuffer[m_uPos], sizeof(int32));
	m_uPos += sizeof(int32);
	
	return true;
}

bool Stream::GetData( int64 *value )
{
	if ( m_uPos + sizeof(int64) > m_uSize ) return false;
	*value = memtoi(&m_pMsgBuffer[m_uPos], sizeof(int64));
	m_uPos += sizeof(int64);
	
	return true;
}

bool Stream::GetData(void* pStruct, short *uSize)
{
	unsigned short maxSize = *uSize;
	*uSize = 0;
	if ( NULL == pStruct || maxSize <= 0 ) return false;
	if ( !GetData( uSize ) ) return false;
	if ( m_uPos + (*uSize) > m_uSize || (*uSize) > maxSize ) 
	{
		*uSize = 0;
		return false;
	}
	memcpy( pStruct, &m_pMsgBuffer[m_uPos], (*uSize) );
	m_uPos += (*uSize);
	
	return true;
}

unsigned char* Stream::GetPointer( short *uSize )
{
	*uSize = 0;
	if ( !GetData( uSize ) ) return NULL;
	if ( m_uPos + (*uSize) > m_uSize ) 
	{
		*uSize = 0;
		return NULL;
	}
	unsigned char *pStruct = &m_pMsgBuffer[m_uPos];
	m_uPos += (*uSize);
	
	return pStruct;
}

}
