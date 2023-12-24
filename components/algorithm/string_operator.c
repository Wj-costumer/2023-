#include "string_operator.h"


/*
	求带空格字符串数组的长度
*/
int length_string(char *s) {
	int i = 0;
	while(*s != '\0') {
		s++;
		i++;
	}
	return i;
}

fp32 abstract_distance(char *s, fp32 *dist)
{
	fp32 num = 0.0;
	if(s[0] == 'd')
	{
		for(int i = 0 ; i < length_string(s); i++)
		{
			if ((*s >= '0')&&(*s <= '9'))
			{
				num = num * 10 + *s - '0';
			}
			s++;
		}
	}	
	num *= 0.001; // mm->m
	if(num != 0)
		*dist = num;
	return num;
}

//反转字符串
char *reverse(char *s)
{
    char temp;
    char *p = s;    //p指向s的头部
    char *q = s;    //q指向s的尾部
    while(*q)
        ++q;
    q--;
    
    //交换移动指针，直到p和q交叉
    while(q > p)
    {
        temp = *p;
        *p++ = *q;
        *q-- = temp;
    }
    return s;
}
 
/*
 * 功能：整数转换为字符串
 * char s[] 的作用是存储整数的每一位
 */
char *my_itoa(int n)
{
    int i = 0,isNegative = 0;
    static char s[100];      //必须为static变量，或者是全局变量
    if((isNegative = n) < 0) //如果是负数，先转为正数
    {
        n = -n;
    }
    do      //从各位开始变为字符，直到最高位，最后应该反转
    {
        s[i++] = n%10 + '0';
        n = n/10;
    }while(n > 0);
    
    if(isNegative < 0)   //如果是负数，补上负号
    {
        s[i++] = '-';
    }
    s[i] = '\n';    //最后加上字符串结束符
    return reverse(s);
}
