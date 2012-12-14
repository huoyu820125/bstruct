// BStruct.cpp: implementation of the BStruct class.
//
//////////////////////////////////////////////////////////////////////

#include "../include/BStruct.h"
using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace bsp
{

BStruct::BStruct()
{
	m_error.m_data = NULL;
	m_error.m_size = 0;
	m_error.m_parent = NULL;

	int i = 0;
	for ( i = 0; i < 256; i++ )
	{
		m_dataList[i].m_parent = this;
		m_dataList[i].m_size = 0;
		m_dataList[i].m_data = NULL;
	}
	m_pos = 0;
	m_finished = true;
	m_action = BStruct::unknow;
	m_bValid = false;
}

BStruct::~BStruct()
{
}

void BStruct::Bind(unsigned char *pBuffer, unsigned short uSize)
{
	m_pos = 0;
	m_dataMap.clear();
	if ( NULL == pBuffer || 0 >= uSize ) return;
	m_stream.Bind(pBuffer, uSize);
	m_stream.AddData( uSize );
	m_action = BStruct::write;
}

unsigned char* BStruct::GetStream()
{
	if ( NULL == m_stream.GetStream() ) return NULL;
	itomem(m_stream.GetStream(), m_stream.Pos()-2, sizeof(short));//将总长度保存到头部
	return m_stream.GetStream();
}

unsigned short BStruct::GetSize()
{
	if ( BStruct::write == m_action ) return m_stream.Pos();
	else if ( BStruct::read == m_action ) return m_stream.GetSize();
	else return 0;
}

unsigned char* BStruct::PreBuffer( char *key )
{
	(*this)[key];
	return &(GetStream()[m_stream.Pos()+sizeof(unsigned short)]);
}

unsigned short BStruct::PreSize()
{
	return m_stream.GetSize() - sizeof(unsigned short) - m_stream.Pos();
}

M_VALUE& BStruct::operator []( string key )
{
	if ( BStruct::unknow == m_action ||  "" == key ) return m_error;
	map<std::string, M_VALUE*>::iterator it = m_dataMap.find( key );
	if ( it == m_dataMap.end() ) 
	{
		if ( BStruct::write != m_action ) return m_error;
		if ( !m_finished ) return m_error;
		if ( !m_stream.AddData( (void*)key.c_str(), key.length() ) ) return m_error;
		pair<map<std::string, M_VALUE*>::iterator, bool> ret = m_dataMap.insert(map<std::string, M_VALUE*>::value_type(key,&m_dataList[m_pos]));
		if ( !ret.second ) return m_error;
		m_dataList[m_pos].m_size = 0;
		m_dataList[m_pos].m_data = (char*)(&m_stream.GetStream()[m_stream.Pos()]);
		m_finished = false;
		m_pos++;
		return *(ret.first->second);
	}
	return *(it->second);
}

bool BStruct::IsValid()
{
	return m_bValid;
}

bool BStruct::Resolve(unsigned char *pBuffer, unsigned short uSize)
{
	m_pos = 0;
	m_dataMap.clear();
	m_bValid = false;
	if ( NULL == pBuffer || 0 >= uSize ) return false;
	m_stream.Bind(pBuffer, uSize);
	m_action = BStruct::read;
	return Resolve();
}

bool BStruct::Resolve()
{
	if ( BStruct::read != m_action ) return false;
	char name[257];//字段名最大256byte+'\0'257byte
	unsigned short namesize;
	if ( !m_stream.GetData(&namesize) ) return false;
	namesize = 256;//设置GetData()最大读取256byte
	while ( !m_stream.IsEnd() )
	{
		if ( !m_stream.GetData( name, &namesize ) ) return false;
		m_dataList[m_pos].m_data = (char*)m_stream.GetPointer(&m_dataList[m_pos].m_size);
		if ( NULL == m_dataList[m_pos].m_data ) return false;
		name[namesize] = 0;
		pair<map<std::string, M_VALUE*>::iterator, bool> ret = 
			m_dataMap.insert(map<string, M_VALUE*>::value_type(name,&m_dataList[m_pos]));
		if ( !ret.second )//重名成员 
		{
			m_dataList[m_pos].m_size = 0;
			return false;
		}
		
		m_pos++;
		namesize = 256;//设置GetData()最大读取256byte
	}
	m_bValid = true;
	return true;
}

//////////////////////////////////////////////////////////////////////////
//赋值操作
bool M_VALUE::operator = ( char* value )
{
	if ( NULL == m_data ) return false;
	unsigned short vLen = strlen(value);
	if ( 0 >= m_size ) 
	{
		if ( !m_parent->m_stream.AddData( value, vLen ) ) return false;
		m_size = vLen;
		m_data = &m_data[2];
		m_parent->m_finished = true;
		return true;
	}
	if ( m_size != vLen ) return false;
	memcpy( m_data, value, m_size );
	return true;
}

bool M_VALUE::operator = ( char value )
{	
	if ( NULL == m_data ) return false;
	if ( 0 >= m_size ) 
	{
		if ( !m_parent->m_stream.AddData( &value, sizeof(char) ) ) return false;
		m_size = sizeof(char);
		m_data = &m_data[2];
		m_parent->m_finished = true;
		return true;
	}
	if( m_size != sizeof(char) ) return false;
	m_data[0] = value;
	return true;
}

bool M_VALUE::operator = ( short value )
{
	if ( NULL == m_data ) return false;
	unsigned char buf[sizeof(short)];
	itomem(buf,value,sizeof(short));
	if ( 0 >= m_size ) 
	{
		m_size = sizeof(short);
		if ( !m_parent->m_stream.AddData( buf, m_size ) ) return false;
		m_data = &m_data[2];
		m_parent->m_finished = true;
		return true;
	}
	if ( m_size != sizeof(short) ) return false;
	memcpy( m_data, buf, m_size );
	return true;
}

bool M_VALUE::operator = ( float value )
{	
	if ( NULL == m_data ) return false;
	if ( 0 >= m_size ) 
	{
		if ( !m_parent->m_stream.AddData( &value, sizeof(float) ) ) return false;
		m_size = sizeof(float);
		m_data = &m_data[2];
		m_parent->m_finished = true;
		return true;
	}
	if ( m_size != sizeof(float) ) return false;
	memcpy( m_data, &value, m_size );
	return true;
}

bool M_VALUE::operator = ( double value )
{
	if ( NULL == m_data ) return false;
	if ( 0 >= m_size ) 
	{
		if ( !m_parent->m_stream.AddData( &value, sizeof(double) ) ) return false;
		m_size = sizeof(double);
		m_data = &m_data[2];
		m_parent->m_finished = true;
		return true;
	}
	if ( m_size != sizeof(double) ) return false;
	memcpy( m_data, &value, m_size );
	return true;
}

bool M_VALUE::operator = ( long value )
{
	if ( NULL == m_data ) return false;
	unsigned char buf[sizeof(long)];
	itomem(buf,value,sizeof(long));
	if ( 0 >= m_size ) 
	{
		m_size = sizeof(long);
		if ( !m_parent->m_stream.AddData( buf, m_size ) ) return false;
		m_data = &m_data[2];
		m_parent->m_finished = true;
		return true;
	}
	if ( m_size != sizeof(long) ) return false;
	memcpy( m_data, buf, m_size );
	return true;
}

bool M_VALUE::operator = ( int32 value )
{
	if ( NULL == m_data ) return false;
	unsigned char buf[sizeof(int32)];
	itomem(buf,value,sizeof(int32));
	if ( 0 >= m_size ) 
	{
		m_size = sizeof(int32);
		if ( !m_parent->m_stream.AddData( buf, m_size ) ) return false;
		m_data = &m_data[2];
		m_parent->m_finished = true;
		return true;
	}
	if ( m_size != sizeof(int32) ) return false;
	memcpy( m_data, buf, m_size );
	return true;
}

bool M_VALUE::operator = ( int64 value )
{	
	if ( NULL == m_data ) return false;
	unsigned char buf[sizeof(int64)];
	itomem(buf,value,sizeof(int64));
	if ( 0 >= m_size ) 
	{
		m_size = sizeof(int64);
		if ( !m_parent->m_stream.AddData( buf, m_size ) ) return false;
		m_data = &m_data[2];
		m_parent->m_finished = true;
		return true;
	}
	if ( m_size != sizeof(int64) ) return false;
	memcpy( m_data, buf, m_size );
	return true;
}

bool M_VALUE::operator = ( BStruct value )
{
	if ( NULL == m_data ) return false;
	unsigned char *pStream = value.GetStream();
	if ( NULL == pStream ) return false;
	if ( 0 >= m_size ) 
	{
		if ( !m_parent->m_stream.AddData( pStream, value.GetSize() ) ) return false;
		m_size = value.GetSize();
		m_data = &m_data[2];
		m_parent->m_finished = true;
		return true;
	}
	if ( m_size != value.GetSize() ) return false;
	memcpy( m_data, pStream, m_size );
	return true;
}

bool M_VALUE::SetValue( const void *value, unsigned short uSize )
{
	if ( NULL == value || 0 >= uSize ) return false;
	if ( 0 >= m_size ) 
	{
		if ( !m_parent->m_stream.AddData( (unsigned char*)value, uSize ) ) return false;
		m_size = uSize;
		m_data = &m_data[2];
		m_parent->m_finished = true;
		return true;
	}
	if( m_size != uSize ) return false;
	memcpy( m_data, (unsigned char*)value, m_size );
	return true;
}

//////////////////////////////////////////////////////////////////////////
//取值操作
bool M_VALUE::IsValid()
{
	return NULL != m_data;
}

M_VALUE::operator char()
{
	if ( NULL == m_data || m_size != sizeof(char) ) return (char)0xff;
	return (char)m_data[0];
}

M_VALUE::operator short()
{
	if ( NULL == m_data || m_size != sizeof(short) ) return (short)0xffff;
	return (short)memtoi( (unsigned char*)m_data, m_size );
}

M_VALUE::operator float()
{
	if ( NULL == m_data || m_size != sizeof(float) ) return (float)0xffffffff;
	float value;
	memcpy( &value, m_data, m_size );
	return value;
}

M_VALUE::operator double()
{
	if ( NULL == m_data || m_size != sizeof(double) ) return 0xffffffff;
	double value;
	memcpy( &value, m_data, m_size );
	return value;
}

M_VALUE::operator long()
{
	if ( NULL == m_data || m_size != sizeof(long) ) return 0xffffffff;
	return(long)memtoi( (unsigned char*)m_data, m_size );
}

M_VALUE::operator int32()
{
	if ( NULL == m_data || m_size != sizeof(int32) ) return 0xffffffff;
	return (int32)memtoi( (unsigned char*)m_data, m_size );
}

M_VALUE::operator int64()
{
	if ( NULL == m_data || m_size != sizeof(int64) ) return 0xffffffff;
	return memtoi( (unsigned char*)m_data, m_size );
}

M_VALUE::operator string()
{
	if ( NULL == m_data ) return "";
	string value;
	value.assign(m_data, m_size);
	return value;
}

M_VALUE::operator BStruct()
{
	BStruct value;
	if ( NULL == m_data ) return value;
	value.Resolve((unsigned char*)m_data, m_size);
	return value;
}

}
