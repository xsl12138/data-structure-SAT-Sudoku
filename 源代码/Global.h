#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

using namespace std;

#define MAX_VARNUM 30000
#define ERROR 0
#define OK 1

typedef struct Literal{				//���� 
	struct Literal* nextnode;		//��һ������ 
	int value;						//�����ֵ�ֵ����������
	int flag;						//�ж��Ƿ�ɾ����0����δ��ɾ�����������ִ���ɾ��ʱ�ĵݹ������ 
}node;

typedef struct Clause{				//�Ӿ� 
	struct Clause* nextcla;			//��һ�Ӿ�
	node* first; 					//���Ӿ�ĵ�һ������
	int literal_num;				//���Ӿ���������� 
	int flag;						//�ж��Ƿ�ɾ����0����δ��ɾ�����������ִ���ɾ��ʱ�ĵݹ������ 
}clause;

typedef struct Question{			//SAT���� 
	int vexnum;						//������ 
	int clanum;						//�Ӿ���
	int fre_lit[MAX_VARNUM+1];		//�����ֳ��ֵĴ��������޵��Ӿ��Ҫѡ��һ��ɾ���ؼ���ʱʹ�ã�
									//ѡȡ����Ƶ����ߵ��Ǹ��ؼ�����Ϊ��ɾ���ؼ��� 
									//frequency_literal 
	int ans[MAX_VARNUM+1];			//���н�ʱ�ģ���	 
}question;

typedef struct Sudproblem{			//�������� 
	int original[16][16];			//�������̣��������û��Ľ������ϸ��£� 
	int finalans[16][16];			//�������� 
	int flag[16][16];				//�ж������ϵ�λ���Ƿ�δ��ʼ�����������ֵ�λ��
	//Ϊ�����û����������ʱ���������˳�ʼ����������λ�ã�������flag���� 
	int num;						//��0���� 
}sud;

typedef int status;

//SAT���CNF�ļ� 
void SATQuestion();							//SATģ�� 
void init();								//SAT��ʼ�� 
void ReadFile(char filename[200]);			//��ȡ�ļ� 
void PrintFile();							//����ļ����ݣ����Ӿ䣩
void DPLL(); 								//DPLLģ�� 
void RecoverBegin();						//���س�ʼ״̬ 
status FindSingleClause();					//Ѱ�ҵ��Ӿ� 
status DeleteTarget(int depth, int single);	//ɾ��Ŀ���Ӿ������ 
//���������SAT���������ؼ��������ֱ�ʹ�������ֲ�ͬ�ķ��� 
status func1(int depth, int tar);			//������Ѱ������Ӿ�����һ������	
status func2(int depth, int tar);			//������Ѱ������Ӿ�ĵ�һ������
status func3(int depth, int tar);			//������Ѱ������Ӿ���ִ�����������
status func4(int depth, int tar);			//������Ѱ�����У�δ��ɾ���������У����ִ����������� 
status func5(int depth, int tar); 			//������Ѱ�����У�δ��ɾ���������У���һ������ 
//���� 
void RecoverLastLevel(int depth);			//���ݹ�ʱʹ�ã�������һ��ݹ�ʱ��״̬ 
void PrintAnswer();							//��ӡ�� 
void CheckAnswer();							//����  
//�������������SAT����ķ�������Ӧ��������func���� 
status FindShortestLastLiteral();			//Ѱ������Ӿ�����һ�����֣���Ӧfunc1�� 
status FindShortestFirstLiteral(); 			//Ѱ������Ӿ�ĵ�һ�����֣���Ӧfunc2��
status FindShortestMaxLiteral();			//Ѱ������Ӿ���������������������֣���Ӧfunc3��
status FindMaxLiteral();					//Ѱ��һ�������������֣���Ӧfunc4��
status FindFirstLiteral();					//Ѱ�ҵ�һ�����֣���Ӧfunc5��  
//���� 
void FormAnsFile(int res, int time, char filename[200]);//���𰸱�����res�ļ��� 

//�������� 
void Sudoku();								//����ģ��
void initsudo();							//������ʼ��
void CreateSudoku();						//����һ������ 
void AnsTransSudo();						//��ans�����б���Ĵ𰸣�ת�Ƶ�sudo.finalans������
status VarTrans(int k, int i, int j, int n);//��������1-9����ת������Ȼ˳����� 
void FormSudoFile();						//��������֮�󣬽�����������txt�ļ���
void FormCnfFile(int flag);					//����������ת��Ϊ��Ȼ���Ա��벢����cnf�ļ� 
void DigHole(int diff);						//�ڶ��������������� 
status IfCanDig(int x, int y);				//�жϵ�ǰλ�ÿɷ��ڶ�  			
void PrintSudoku();							//��ӡ��ǰ���� 
void PrintAnsSudoku();						//��ӡ������ 
void InputToSolve(); 						//�������ģ�� 
status CheckCurrentSolve();					//��鵱ǰ�������������Ƿ���ȷ 
void hint();								//��������ʾ���� 
