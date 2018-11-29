extern int a;
//extern double a;//错误， a只能有一个类型
extern int a;
extern void fun1(int a, int b);
extern void fun1(int a, int b);
extern void fun1(int a, int b, int c);//函数重载
void fun2();
void fun2(int a);//函数重载
void fun2()
{
}
int main()
{
	a = 100;
	fun1(200, 300);
	fun2();
	fun2(100);
	return 0;
}
void fun2();