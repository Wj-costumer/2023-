#include "string_operator.h"


/*
	����ո��ַ�������ĳ���
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

//��ת�ַ���
char *reverse(char *s)
{
    char temp;
    char *p = s;    //pָ��s��ͷ��
    char *q = s;    //qָ��s��β��
    while(*q)
        ++q;
    q--;
    
    //�����ƶ�ָ�룬ֱ��p��q����
    while(q > p)
    {
        temp = *p;
        *p++ = *q;
        *q-- = temp;
    }
    return s;
}
 
/*
 * ���ܣ�����ת��Ϊ�ַ���
 * char s[] �������Ǵ洢������ÿһλ
 */
char *my_itoa(int n)
{
    int i = 0,isNegative = 0;
    static char s[100];      //����Ϊstatic������������ȫ�ֱ���
    if((isNegative = n) < 0) //����Ǹ�������תΪ����
    {
        n = -n;
    }
    do      //�Ӹ�λ��ʼ��Ϊ�ַ���ֱ�����λ�����Ӧ�÷�ת
    {
        s[i++] = n%10 + '0';
        n = n/10;
    }while(n > 0);
    
    if(isNegative < 0)   //����Ǹ��������ϸ���
    {
        s[i++] = '-';
    }
    s[i] = '\n';    //�������ַ���������
    return reverse(s);
}
