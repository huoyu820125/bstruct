// BStruct.cpp: implementation of the BStruct class.
//
//////////////////////////////////////////////////////////////////////

#include "../include/BStruct.h"
using namespace std;
#include <assert.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace bsp
{

BStruct::BStruct()
{
	m_pos = 0;
	m_finished = true;
	m_action = BStruct::unknow;
}

BStruct::~BStruct()
{
}

void BStruct::Bind(unsigned char *pBuffer, unsigned short uSize)
{
	m_pos = 0;
	m_dataMap.clear();
	m_stream.Bind(pBuffer, uSize);
	m_stream.AddData( uSize );
	m_action = BStruct::write;
}

bool BStruct::Resolve(unsigned char *pBuffer, unsigned short uSize)
{
	m_pos = 0;
	m_dataMap.clear();
	m_stream.Bind(pBuffer, uSize);
	m_action = BStruct::read;
	return Resolve();
}

bool BStruct::Resolve()
{
	if ( BStruct::read != m_action ) return true;
	char name[256];
	unsigned short namesize;
	m_stream.GetData(&namesize);//跳过长度
	while ( m_stream.GetData( name, &namesize ) )
	{
		name[namesize] = 0;
		m_dataList[m_pos].m_data = (char*)m_stream.GetPointer(&m_dataList[m_pos].m_size);
		pair<map<std::string, M_VALUE*>::iterator, bool> ret = 
			m_dataMap.insert(map<std::string, M_VALUE*>::value_type(name,&m_dataList[m_pos]));
		m_pos++;
	}
	return true;
}

unsigned char* BStruct::GetStream()
{
	//将总长度保存到头部
	itomem(m_stream.GetStream(), m_stream.GetSize()-2, sizeof(short));
	return m_stream.GetStream();
}

unsigned short BStruct::GetSize()
{
	return m_stream.GetSize();
}

unsigned char* BStruct::PreBuffer( char *key )
{
	(*this)[key];
	return &(GetStream()[GetSize()+sizeof(unsigned short)]);
}

unsigned short BStruct::PreSize()
{
	return m_stream.GetBufferSize() - sizeof(unsigned short) - GetSize();
}

M_VALUE& BStruct::operator []( string key )
{
	assert( "" != key );
	map<std::string, M_VALUE*>::iterator it = m_dataMap.find( key );
	if ( it == m_dataMap.end() ) 
	{
		assert( BStruct::write == m_action );
		assert( m_finished );
		pair<map<std::string, M_VALUE*>::iterator, bool> ret = m_dataMap.insert(map<std::string, M_VALUE*>::value_type(key,&m_dataList[m_pos]));
		assert( ret.second );
		m_dataList[m_pos].m_parent = this;
		assert( m_stream.AddData( (void*)key.c_str(), key.length() ) );
		m_finished = false;
		m_dataList[m_pos].m_data = (char*)(&m_stream.GetStream()[m_stream.GetSize()]);
		m_dataList[m_pos].m_size = 0;
		m_pos++;
		return *(ret.first->second);
	}
	return *(it->second);
}

M_VALUE::operator string()
{
	string value;
	value.assign(m_data, m_size);
	return value;
}

M_VALUE& M_VALUE::operator = ( char* value )
{
	unsigned short vLen = strlen(value);
	if ( 0 >= m_size ) 
	{
		assert( m_parent->m_stream.AddData( value, vLen ) );
		m_size = vLen;
		m_data = &m_data[2];
		m_parent->m_finished = true;
		return *this;
	}
	assert( m_size == vLen );
	memcpy( m_data, value, m_size );
	return *this;
}

M_VALUE::operator char()
{
	assert( m_size == sizeof(char) );
	return (char)m_data[0];
}

M_VALUE& M_VALUE::operator = ( char value )
{	
	if ( 0 >= m_size ) 
	{
		assert( m_parent->m_stream.AddData( &value, sizeof(char) ) );
		m_size = sizeof(char);
		m_data = &m_data[2];
		m_parent->m_finished = true;
		return *this;
	}
	assert( m_size == sizeof(char) );
	m_data[0] = value;
	return *this;
}

M_VALUE::operator short()
{
	assert( m_size == sizeof(short) );
	short value = memtoi( (unsigned char*)m_data, m_size );
	return value;
}

M_VALUE& M_VALUE::operator = ( short value )
{	
	unsigned char buf[sizeof(short)];
	itomem(buf,value,sizeof(short));
	if ( 0 >= m_size ) 
	{
		m_size = sizeof(short);
		assert( m_parent->m_stream.AddData( buf, m_size ) );
		m_data = &m_data[2];
		m_parent->m_finished = true;
		return *this;
	}
	assert( m_size == sizeof(short) );
	memcpy( m_data, buf, m_size );
	return *this;
}

M_VALUE::operator float()
{
	assert( m_size == sizeof(float) );
	float value;
	memcpy( &value, m_data, m_size );
	return value;
}

M_VALUE& M_VALUE::operator = ( float value )
{	
	if ( 0 >= m_size ) 
	{
		assert( m_parent->m_stream.AddData( &value, sizeof(float) ) );
		m_size = sizeof(float);
		m_data = &m_data[2];
		m_parent->m_finished = true;
		return *this;
	}
	assert( m_size == sizeof(float) );
	memcpy( m_data, &value, m_size );
	return *this;
}

M_VALUE::operator double()
{
	assert( m_size == sizeof(double) );
	double value;
	memcpy( &value, m_data, m_size );
	return value;
}

M_VALUE& M_VALUE::operator = ( double value )
{
	if ( 0 >= m_size ) 
	{
		assert( m_parent->m_stream.AddData( &value, sizeof(double) ) );
		m_size = sizeof(double);
		m_data = &m_data[2];
		m_parent->m_finished = true;
		return *this;
	}
	assert( m_size == sizeof(double) );
	memcpy( m_data, &value, m_size );
	return *this;
}

M_VALUE::operator long()
{
	assert( m_size == sizeof(long) );
	long value = memtoi( (unsigned char*)m_data, m_size );
	return value;
}

M_VALUE& M_VALUE::operator = ( long value )
{
	unsigned char buf[sizeof(long)];
	itomem(buf,value,sizeof(long));
	if ( 0 >= m_size ) 
	{
		m_size = sizeof(long);
		assert( m_parent->m_stream.AddData( buf, m_size ) );
		m_data = &m_data[2];
		m_parent->m_finished = true;
		return *this;
	}
	assert( m_size == sizeof(long) );
	memcpy( m_data, buf, m_size );
	return *this;
}

M_VALUE::operator int32()
{
	assert( m_size == sizeof(int32) );
	int32 value = memtoi( (unsigned char*)m_data, m_size );
	return value;
}

M_VALUE& M_VALUE::operator = ( int32 value )
{	
	unsigned char buf[sizeof(int32)];
	itomem(buf,value,sizeof(int32));
	if ( 0 >= m_size ) 
	{
		m_size = sizeof(int32);
		assert( m_parent->m_stream.AddData( buf, m_size ) );
		m_data = &m_data[2];
		m_parent->m_finished = true;
		return *this;
	}
	assert( m_size == sizeof(int32) );
	memcpy( m_data, buf, m_size );
	return *this;
}

M_VALUE::operator int64()
{
	assert( m_size == sizeof(int64) );
	int64 value = memtoi( (unsigned char*)m_data, m_size );
	return value;
}

M_VALUE& M_VALUE::operator = ( int64 value )
{	
	unsigned char buf[sizeof(int64)];
	itomem(buf,value,sizeof(int64));
	if ( 0 >= m_size ) 
	{
		m_size = sizeof(int64);
		assert( m_parent->m_stream.AddData( buf, m_size ) );
		m_data = &m_data[2];
		m_parent->m_finished = true;
		return *this;
	}
	assert( m_size == sizeof(int64) );
	memcpy( m_data, buf, m_size );
	return *this;
}

M_VALUE::operator BStruct()
{
	BStruct value;
	value.Resolve((unsigned char*)m_data, m_size);
	return value;
}

M_VALUE& M_VALUE::operator = ( BStruct value )
{
	if ( 0 >= m_size ) 
	{
		assert( m_parent->m_stream.AddData( value.GetStream(), value.GetSize() ) );
		m_size = value.GetSize();
		m_data = &m_data[2];
		m_parent->m_finished = true;
		return *this;
	}
	assert( m_size == sizeof(int64) );
	memcpy( m_data, value.GetStream(), m_size );
	return *this;
}

bool M_VALUE::SetValue( const void *value, unsigned short uSize )
{
	if ( 0 >= m_size ) 
	{
		if ( !m_parent->m_stream.AddData( (unsigned char*)value, uSize ) ) return false;
		m_size = uSize;
		m_data = &m_data[2];
		m_parent->m_finished = true;
		return true;
	}
	if( m_size != sizeof(int64) ) return false;
	memcpy( m_data, (unsigned char*)value, m_size );
	return true;
}

}
