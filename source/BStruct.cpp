// BStruct.cpp: implementation of the BStruct class.
//
//////////////////////////////////////////////////////////////////////

#include <cstring>
#include "../include/BStruct.h"
#include "../include/BArray.h"

using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace bsp
{

BStruct::BStruct()
{
	SetName( "BStruct" );
	m_error.m_data = NULL;
	m_error.m_size = 0;
	m_error.m_parent = NULL;
	m_finished = true;
	m_action = BStruct::unknow;
	m_bValid = false;
	m_bEmpty = true;
}

BStruct::BStruct( const char* name )
{
	SetName(name);
	m_error.m_data = NULL;
	m_error.m_size = 0;
	m_error.m_parent = NULL;
	m_finished = true;
	m_action = BStruct::unknow;
	m_bValid = false;
	m_bEmpty = true;
}

BStruct::~BStruct()
{
}

void BStruct::SetName( const char* name )
{
	strcpy(m_name, name);
}

const char* BStruct::Name()
{
	return m_name;
}

void BStruct::Bind(unsigned char *pBuffer, unsigned int uSize)
{
	m_dataMap.clear();
	if ( NULL == pBuffer || 0 >= uSize ) return;
	m_stream.Bind(pBuffer, uSize);
	m_action = BStruct::write;
}

unsigned char* BStruct::GetStream()
{
	return m_stream.GetStream();
}

unsigned int BStruct::GetSize()
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

unsigned int BStruct::PreSize()
{
	return m_stream.GetSize() - sizeof(unsigned short) - m_stream.Pos();
}

M_VALUE BStruct::operator []( string key )
{
	if ( BStruct::unknow == m_action ||  "" == key ) return m_error;
	map<std::string, char*>::iterator it = m_dataMap.find( key );
	M_VALUE data;
	data.m_parent = this;
	data.m_nameSize = 0;
	if ( it == m_dataMap.end() ) 
	{
		if ( BStruct::write != m_action ) return m_error;
		if ( !m_finished ) return m_error;
		data.m_header = (char*)&m_stream.GetStream()[m_stream.GetSize()];
		data.m_name = (data.m_header + sizeof(short));
		data.m_nameSize = memtoi( (unsigned char*)data.m_header, sizeof(short) );
		if ( !m_stream.AddData( (void*)key.c_str(), key.length() ) ) return m_error;
		data.m_data = (char*)(&m_stream.GetStream()[m_stream.Pos()]);//指向字段首地址
		pair<map<std::string, char*>::iterator, bool> ret = 
			m_dataMap.insert(map<std::string, char*>::value_type(key,data.m_header));
		if ( !ret.second ) return m_error;
		data.m_size = 0;
		data.m_data += sizeof(unsigned short);//指向数据首地址
		m_finished = false;
		return data;
	}
	data.m_header = it->second;
	data.m_name = data.m_header + sizeof(short);
	data.m_nameSize = memtoi( (unsigned char*)data.m_header, sizeof(short) );
	data.m_data = data.m_name + data.m_nameSize;
	if ( BStruct::read == m_action || (BStruct::write == m_action && m_finished) )
	{
		if ( NULL != data.m_data ) data.m_size = memtoi((unsigned char*)data.m_data, sizeof(unsigned short));
		else data.m_size = 0;
	}
	else data.m_size = 0;
	if ( 0 != data.m_size ) data.m_data += sizeof(unsigned short);//指向数据首地址
	return data;
}

bool BStruct::IsValid()
{
	return m_bValid;
}

bool BStruct::IsEmpty()
{
	return m_bEmpty;
}

bool BStruct::Resolve(unsigned char *pBuffer, unsigned int uSize)
{
	m_dataMap.clear();
	m_bValid = false;
	m_bEmpty = true;
	if ( NULL == pBuffer || 0 > uSize ) return false;
	m_stream.Bind(pBuffer, uSize);
	m_action = BStruct::read;
	return Resolve();
}

bool BStruct::Resolve()
{
	if ( BStruct::read != m_action ) return false;
	char name[257];//字段名最大256byte+'\0'257byte
	unsigned short namesize;
	namesize = 256;//设置GetData()最大读取256byte
	char *value;
	unsigned short size;
	char *header;
	while ( !m_stream.IsEnd() )
	{
		header = (char*)&m_stream.GetStream()[m_stream.Pos()];
		if ( !m_stream.GetData( name, &namesize ) ) return false;
		value = (char*)m_stream.GetPointer(&size);//取得数据首地址
		if ( NULL == value && 0 != size ) return false;
		if ( NULL != value ) value = value - sizeof(unsigned short);//指向字段首地址
		name[namesize] = 0;
		pair<map<std::string, char*>::iterator, bool> ret = 
			m_dataMap.insert(map<string, char*>::value_type(name,header));
		if ( !ret.second ) return false;//重名成员 
		namesize = 256;//设置GetData()最大读取256byte
		m_bEmpty = false;
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
		m_parent->m_finished = true;
		return true;
	}
	if ( m_size != sizeof(double) ) return false;
	memcpy( m_data, &value, m_size );
	return true;
}

//bool M_VALUE::operator = ( long value )
//{
//	if ( NULL == m_data ) return false;
//	unsigned char buf[sizeof(long)];
//	itomem(buf,value,sizeof(long));
//	if ( 0 >= m_size ) 
//	{
//		m_size = sizeof(long);
//		if ( !m_parent->m_stream.AddData( buf, m_size ) ) return false;
//		m_parent->m_finished = true;
//		return true;
//	}
//	if ( m_size != sizeof(long) ) return false;
//	memcpy( m_data, buf, m_size );
//	return true;
//}
//
bool M_VALUE::operator = ( int32 value )
{
	if ( NULL == m_data ) return false;
	unsigned char buf[sizeof(int32)];
	itomem(buf,value,sizeof(int32));
	if ( 0 >= m_size ) 
	{
		m_size = sizeof(int32);
		if ( !m_parent->m_stream.AddData( buf, m_size ) ) return false;
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
		m_parent->m_finished = true;
		return true;
	}
	if ( m_size != sizeof(int64) ) return false;
	memcpy( m_data, buf, m_size );
	return true;
}

bool M_VALUE::operator = ( BStruct *value )
{
	if ( NULL == m_data ) return false;
	unsigned char *pStream = value->GetStream();
	if ( NULL == pStream ) return false;
	if ( 0 >= m_size ) 
	{
		if ( !m_parent->m_stream.AddData( pStream, value->GetSize() ) ) return false;
		m_size = value->GetSize();
		m_parent->m_finished = true;
		return true;
	}
	if ( m_size != value->GetSize() ) return false;
	memcpy( m_data, pStream, m_size );
	return true;
}

bool M_VALUE::operator = ( BArray *value )
{
	if ( NULL == m_data ) return false;
	unsigned char *pStream = value->GetStream();
	if ( NULL == pStream ) return false;
	if ( 0 >= m_size ) 
	{
		if ( !m_parent->m_stream.AddData( pStream, value->GetSize() ) ) return false;
		m_size = value->GetSize();
		m_parent->m_finished = true;
		return true;
	}
	if ( m_size != value->GetSize() ) return false;
	memcpy( m_data, pStream, m_size );
	return true;
}

bool M_VALUE::SetValue( const void *value, unsigned short uSize )
{
	if ( NULL == m_data ) return false;
	if ( NULL == value || 0 >= uSize ) return false;
	if ( 0 >= m_size ) 
	{
		if ( !m_parent->m_stream.AddData( (unsigned char*)value, uSize ) ) return false;
		m_size = uSize;
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
	if ( NULL == m_parent ) return false;
	return true;
}

M_VALUE::operator char()
{
	if ( NULL == m_data || m_size != sizeof(char) ) 
	{
		char key[256];
		int i = 0;
		key[0] = 0;
		for ( i = 0; i < m_nameSize; i++ ) key[i] = m_name[i];
		key[i] = '\0';
		printf( "%s[%s] size is %d byte cannot convert to char\n", m_parent->Name(), key, m_size );
		return (char)0xff;
	}
	return (char)m_data[0];
}

M_VALUE::operator short()
{
	if ( NULL == m_data || m_size != sizeof(short) )
	{
		char key[256];
		int i = 0;
		key[0] = 0;
		for ( i = 0; i < m_nameSize; i++ ) key[i] = m_name[i];
		key[i] = '\0';
		printf( "%s[%s] size is %d byte cannot convert to short\n", m_parent->Name(), key, m_size );
		return (short)0xffff;
	}
	return (short)memtoi( (unsigned char*)m_data, m_size );
}

M_VALUE::operator float()
{
	if ( NULL == m_data || m_size != sizeof(float) ) 
	{
		char key[256];
		int i = 0;
		key[0] = 0;
		for ( i = 0; i < m_nameSize; i++ ) key[i] = m_name[i];
		key[i] = '\0';
		printf( "%s[%s] size is %d byte cannot convert to float\n", m_parent->Name(), key, m_size );
		return (float)0xffffffff;
	}
	float value;
	memcpy( &value, m_data, m_size );
	return value;
}

M_VALUE::operator double()
{
	if ( NULL == m_data || m_size != sizeof(double) )
	{
		char key[256];
		int i = 0;
		key[0] = 0;
		for ( i = 0; i < m_nameSize; i++ ) key[i] = m_name[i];
		key[i] = '\0';
		printf( "%s[%s] size is %d byte cannot convert to double\n", m_parent->Name(), key, m_size );
		return (double)0xffffffff;
	}
	double value;
	memcpy( &value, m_data, m_size );
	return value;
}

//M_VALUE::operator long()
//{
//	if ( NULL == m_data || m_size != sizeof(long) ) return 0xffffffff;
//	return(long)memtoi( (unsigned char*)m_data, m_size );
//}
//
M_VALUE::operator int32()
{
	if ( NULL == m_data || m_size != sizeof(int32) )
	{
		char key[256];
		int i = 0;
		key[0] = 0;
		for ( i = 0; i < m_nameSize; i++ ) key[i] = m_name[i];
		key[i] = '\0';
		printf( "%s[%s] size is %d byte cannot convert to int32\n", m_parent->Name(), key, m_size );
		return (int32)0xffffffff;
	}
	return (int32)memtoi( (unsigned char*)m_data, m_size );
}

M_VALUE::operator int64()
{
	if ( NULL == m_data || m_size != sizeof(int64) ) 
	{
		char key[256];
		int i = 0;
		key[0] = 0;
		for ( i = 0; i < m_nameSize; i++ ) key[i] = m_name[i];
		key[i] = '\0';
		printf( "%s[%s] size is %d byte cannot convert to int64\n", m_parent->Name(), key, m_size );
		return (int64)0xffffffff;
	}
	return memtoi( (unsigned char*)m_data, m_size );
}

M_VALUE::operator string()
{
	if ( NULL == m_data ) 
	{
		char key[256];
		int i = 0;
		key[0] = 0;
		for ( i = 0; i < m_nameSize; i++ ) key[i] = m_name[i];
		key[i] = '\0';
		printf( "%s[%s] is NULL cannot convert to string\n", m_parent->Name(), key );
		return "";
	}
	string value;
	value.assign(m_data, m_size);
	return value;
}

M_VALUE::operator BStruct()
{
	BStruct value;
	char key[256];
	int i = 0;
	key[0] = 0;
	for ( i = 0; i < m_nameSize; i++ ) key[i] = m_name[i];
	key[i] = '\0';

	char name[256];
	sprintf( name, "%s[%s]", m_parent->Name(), key );
	value.SetName( name );

	if ( !IsValid() ) return value;
	if ( !value.Resolve((unsigned char*)m_data, m_size) )
	{
		printf( "%s cannot convert to BStruct\n", name );
		return value;
	}
	return value;
}

M_VALUE::operator BArray()
{
	BArray value;
	char key[256];
	int i = 0;
	key[0] = 0;
	for ( i = 0; i < m_nameSize; i++ ) key[i] = m_name[i];
	key[i] = '\0';

	char name[256];
	sprintf( name, "%s[%s]", m_parent->Name(), key );
	value.SetName( name );

	if ( !IsValid() ) return value;
	if ( !value.Resolve((unsigned char*)m_data, m_size) )
	{
		printf( "%s cannot convert to BArray\n", name );
		return value;
	}
	return value;
}

}
