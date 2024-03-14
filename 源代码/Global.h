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

typedef struct Literal{				//文字 
	struct Literal* nextnode;		//下一个文字 
	int value;						//该文字的值（带正负）
	int flag;						//判断是否被删除（0代表未被删除，其余数字代表被删除时的递归层数） 
}node;

typedef struct Clause{				//子句 
	struct Clause* nextcla;			//下一子句
	node* first; 					//该子句的第一个文字
	int literal_num;				//该子句的文字数量 
	int flag;						//判断是否被删除（0代表未被删除，其余数字代表被删除时的递归层数） 
}clause;

typedef struct Question{			//SAT问题 
	int vexnum;						//文字数 
	int clanum;						//子句数
	int fre_lit[MAX_VARNUM+1];		//各文字出现的次数（在无单子句而要选下一个删除关键字时使用）
									//选取出现频率最高的那个关键字作为待删除关键字 
									//frequency_literal 
	int ans[MAX_VARNUM+1];			//（有解时的）解	 
}question;

typedef struct Sudproblem{			//数独问题 
	int original[16][16];			//待解棋盘（随着与用户的交互不断更新） 
	int finalans[16][16];			//棋盘正解 
	int flag[16][16];				//判断棋盘上的位置是否未初始题面所给数字的位置
	//为避免用户在输入求解时，误输入了初始题面所给的位置，故设立flag数组 
	int num;						//非0个数 
}sud;

typedef int status;

//SAT求解CNF文件 
void SATQuestion();							//SAT模块 
void init();								//SAT初始化 
void ReadFile(char filename[200]);			//读取文件 
void PrintFile();							//输出文件内容（仅子句）
void DPLL(); 								//DPLL模块 
void RecoverBegin();						//返回初始状态 
status FindSingleClause();					//寻找单子句 
status DeleteTarget(int depth, int single);	//删除目标子句和文字 
//以下是求解SAT问题的五个关键函数，分别使用了五种不同的方法 
status func1(int depth, int tar);			//方法：寻找最短子句的最后一个文字	
status func2(int depth, int tar);			//方法：寻找最短子句的第一个文字
status func3(int depth, int tar);			//方法：寻找最短子句出现次数最多的文字
status func4(int depth, int tar);			//方法，寻找所有（未被删除的文字中）出现次数最多的文字 
status func5(int depth, int tar); 			//方法：寻找所有（未被删除的文字中）第一个文字 
//以上 
void RecoverLastLevel(int depth);			//（递归时使用）返回上一层递归时的状态 
void PrintAnswer();							//打印答案 
void CheckAnswer();							//检查答案  
//以下是五种求解SAT问题的方法，对应上面的五个func函数 
status FindShortestLastLiteral();			//寻找最短子句的最后一个文字（对应func1） 
status FindShortestFirstLiteral(); 			//寻找最短子句的第一个文字（对应func2）
status FindShortestMaxLiteral();			//寻找最短子句中在问题里出现最多的文字（对应func3）
status FindMaxLiteral();					//寻找一个出现最多的文字（对应func4）
status FindFirstLiteral();					//寻找第一个文字（对应func5）  
//以上 
void FormAnsFile(int res, int time, char filename[200]);//将答案保存在res文件中 

//数独功能 
void Sudoku();								//数独模块
void initsudo();							//数独初始化
void CreateSudoku();						//创造一个数独 
void AnsTransSudo();						//将ans数组中保存的答案，转移到sudo.finalans数组中
status VarTrans(int k, int i, int j, int n);//将数独的1-9数字转化成自然顺序编码 
void FormSudoFile();						//创造数独之后，将数独保存至txt文件中
void FormCnfFile(int flag);					//将数独数字转换为自然语言编码并存入cnf文件 
void DigHole(int diff);						//挖洞法创造数独题面 
status IfCanDig(int x, int y);				//判断当前位置可否挖洞  			
void PrintSudoku();							//打印当前题面 
void PrintAnsSudoku();						//打印数独答案 
void InputToSolve(); 						//输入求解模块 
status CheckCurrentSolve();					//检查当前输入求解的内容是否正确 
void hint();								//数独的提示功能 
