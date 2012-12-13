#include <cstdio>
#include "BStruct.h"
using namespace std;

int main()
{
	unsigned char buf[256];
	//����һ���ṹ����Ϣ
	bsp::BStruct p;
	p.Bind(buf,256);//�뻺��󶨣��Ժ����ṹ���������ݣ����ᵽ���ӵ�bufĩβ
	p["1"] = "1";//���ṹ��������
	p["2"] = (unsigned char)2;//���ṹ��������
	p["3"] = (unsigned short)3;//�����ﹹ������
	p["4"] = (unsigned int)4;//���ṹ��������
	p["5"] = (float)5;//���ṹ��������
	p["6"] = (double)6;//���ṹ��������
	p["7"] = (unsigned long)7;//���ṹ��������
	p["8"] = (uint32)8;//���ṹ��������
	struct BSD
	{
		int i;
		short s;
	};
	BSD sdasad;
	sdasad.i = 123;
	sdasad.s = 456;
	//���ṹ����struct��java�Ȳ�֧��strcut�����ԣ������޷�������������
	//����ֻʹ��ͨ���������ݣ�char int �ַ��� byte�����
	p["13"].SetValue(&sdasad,sizeof(BSD));
	p["9"] = (uint64)9;//���ṹ��������
	p["12"].SetValue( "huoyu", 5/*����Ҫ��\0���ƽ�ȥ*/ );//���ṹ�����ַ���
	bsp::BStruct obj;//����һ���ӽṹ
	obj.Bind(p.PreBuffer( "10" ), p.PreSize());
	obj["o1"] = 10;
	obj["o2"] = "11";
	obj["o3"] = 12;
	p["10"] = obj;//���ṹ��Ƕ�׽ṹ
	p["11"] = 13;
	//���ṹ���͵����磬������memcpy���ڴ棬ģ�����緢��/����
	char msg[1024];
	int nLen;
	memcpy( msg, p.GetStream(), p.GetSize() );//���ͳ�ȥ

	//���ղ������ṹ
	nLen = p.GetSize();//���յ��ṹ��С
	bsp::BStruct p1;
	p1.Resolve((unsigned char*)msg,nLen);//����
	//ȡ����
	string str = (string)p1["1"];
	char c = p1["2"];
	short s = p1["3"];
	int i = p1["4"];
	float f = p1["5"];
	double d = p1["6"];
	long l = p1["7"];
	int32 i32 = p1["8"];
	int64 i64 = p1["9"];
	bsp::BStruct po2 = p1["10"];
	printf( "%s\n", str.c_str() );
	printf( "%d\n", c );
	printf( "%d\n", s );
	printf( "%d\n", i );
	printf( "%f\n", f );
	printf( "%f\n", d );
	printf( "%d\n", l );
	printf( "%d\n", i32 );
	printf( "%d\n", i64 );
	str = (string)p1["12"];
	printf( "%s\n", str.c_str() );
	BSD *ds = (BSD*)p1["13"].m_data;
	printf( "%d\n", ds->i );
	printf( "%d\n", ds->s );
	i = po2["o1"];
	str = (string)po2["o2"];
	i32 = po2["o3"];
	int i11 = p1["11"];
	printf( "%d\n", i );
	printf( "%s\n", str.c_str() );
	printf( "%d\n", i32 );
	printf( "%d\n", i11 );
	
	return 0;
}