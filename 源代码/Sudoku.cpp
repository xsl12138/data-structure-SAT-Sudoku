#include "Global.h"

extern sud sudo;
extern question qq;
extern clause* root;

void Sudoku(){//数独模块 
	int op = 1;
	while(op){
		fflush(stdin);
		system("cls");	printf("\n\n");
		printf("                      Welcome to my Sudoku games!:          	        \n");
		printf("                  Click 1 to start game or 0 to exit					\n");
		printf("------------------------------------------------------------------------\n");
		printf("                  1.生成数独棋盘             0.退出		              	\n");
		printf("------------------------------------------------------------------------\n");
		cin >> op; 	
		if(op == 1){
			fflush(stdin);
			initsudo();//初始化棋盘 
			system("cls");
			printf("正在生成数独，请稍后...\n");
			time_t t1, t2;
			t1 = clock();
			CreateSudoku();//创造数独 
			t2 = clock();
			printf("成功生成数独，用时 %dms\n",t2-t1);
			for(int i = 1; i <= 15; i++){
				for(int j = 1; j <= 15; j++){
					sudo.original[i][j] = sudo.finalans[i][j];
				}
			}
			printf("请选择难度：\n");
			printf("1.简单		2.中等		3.困难\n");
			int diff;
			scanf("%d",&diff);
			printf("正在给数独挖空，请稍后...\n"); 
			time_t t3, t4;
			t3 = clock();
			DigHole(diff);//（根据不同难度）挖洞法生成题面 
			t4 = clock();
			printf("已生成题面，用时 %dms\n",t4-t3);//挖洞用时 
			printf("生成数独总用时： %dms\n",t2-t1+t4-t3);
			printf("按回车查看题面\n");
			getchar();getchar();
			int opp = 1;
			while(opp){
				system("cls");
				PrintSudoku();		//打印当前题面	
				if(sudo.num == 153){//双数独共153个数字 
					printf("你已完成该数独的求解！\n");
					getchar(); 
					break;
				}
				cout << endl;
				printf("1.输入求解		2.来点提示		3.显示答案		4.检查		0.退出\n");
				printf("请输入：");
				scanf("%d",&opp);
				if(opp == 1){
					fflush(stdin);
					InputToSolve();		//输入求解 
				}	
				else if(opp == 2){
					fflush(stdin);
					hint();				//提示 
				} 
				else if(opp == 3){
					fflush(stdin);
					system("cls");
					PrintAnsSudoku();	//打印最终答案 
					printf("下次再努力求解吧\n");
					printf("按任意键退出");
					getchar();
					break; 
				}
				else if(opp == 4){		//检查当前解答是否有误 
					fflush(stdin);
					if(CheckCurrentSolve())	printf("当前求解全部正确！\n");
					else printf("当前解答有误！\n");
					getchar();
				}
				else if(opp != 0){
					fflush(stdin);
					printf("输入错误！请重新输入！");
					getchar();
				}	
			}
		} 
		else if(op != 0){
			fflush(stdin);
			printf("输入错误！请重新输入！");
			getchar();
		}
	}
}

void initsudo(){//数独初始化 
	sudo.num = 0;
	for(int i = 1; i <= 15; i++){
		for(int j = 1; j <= 15; j++){
			if((i <= 6 && j > 9) || (i > 9 && j <= 6)){
				sudo.finalans[i][j] = -1;
				sudo.original[i][j] = -1;
				sudo.flag[i][j] = -1; 
				continue;
			}
			sudo.finalans[i][j] = 0;
			sudo.original[i][j] = 0;
			sudo.flag[i][j] = 0;
		}
	}
}

void CreateSudoku(){			//创造一个数独 
	srand((unsigned)time(NULL));
	int x = rand() % 9 + 1;		//随机抽取上方棋盘的一个位置，填入1~9中的某个数字 
	int y = rand() % 9 + 1;
	int v = rand() % 9 + 1;
	sudo.original[x][y] = v;
	sudo.num = 1;
	FormCnfFile(1);				//将初始数独盘导入cnf文件，便于后续求解已得到完整数独题面 
	char cnfname[200] = "prepare.cnf";
	ReadFile(cnfname);			//读取刚刚生成的cnf文件，准备DPLL求解以生成数独题面 
	func1(1,0);					//求解生成数独题面（答案存于qq.ans数组中） 
	AnsTransSudo();				//将ans数组中保存的答案，转移到sudo.finalans数组中 
	sudo.num = 153; 
	FormSudoFile();				//将生成的数独题面存入txt文件 
}

void AnsTransSudo(){			//将ans数组中保存的答案，转移到sudo.finalans数组中
	int k, i, j, n;
	for(k = 1; k <= 2; k++){	//第k个数独（1代表左上数独，2代表右下数独） 
		for(i = 1; i <= 9; i++){//第i行 
			for(j = 1; j <= 9; j++){//第j列 
				for(n = 1; n <= 9; n++){//数字n 
					if(qq.ans[VarTrans(k,i,j,n)] == VarTrans(k,i,j,n)){
						if(k == 2){
							sudo.finalans[i+6][j+6] = n;
						}
						else	sudo.finalans[i][j] = n;
					}
				}
			}
		}
	}
}

status VarTrans(int k, int i, int j, int n){//将数独的1-9数字转化成自然顺序编码 
	return (k-1)*729 + (i-1)*81 + (j-1)*9 + n;//即指导文档中的公式 
}


void FormSudoFile(){//创造数独之后，将数独保存至txt文件中 
	FILE* fp = NULL;
	if(!(fp = fopen("SudoProblem.txt", "w"))){
		printf("无法打开文件！");
		getchar(); 
	}
	else{
		for(int i = 1; i <= 15; i++){
			for(int j = 1; j <= 15; j++){
				if(sudo.finalans[i][j] == -1)	fprintf(fp, "   ");
				else	fprintf(fp, "%2d", sudo.finalans[i][j]);
			}
			fprintf(fp, "\n");
		}
	}
	fclose(fp);
}

void FormCnfFile(int flag){//将数独数字转换为自然语言编码并存入cnf文件 
//flag仅仅是用于判断此函数被哪个函数调用了，以确定生成的文件名是什么样的
	char cnfname[200] = {'\0'};
	if(flag == 1)	strcpy(cnfname, "prepare.cnf");
	else if(flag == 2)	strcpy(cnfname, "dighole.cnf"); 
	FILE* fp = NULL;
	int dis[9][2] = { {0,0},{0,1},{0,2},{1,0},{1,1},{1,2},{2,0},{2,1},{2,2} }; 
	if(!(fp = fopen(cnfname, "w"))){
		printf("无法打开文件\n");
		getchar(); 
	}
	else{
		fprintf(fp,"p cnf %d %d\n",729*2, 11988*2+162+sudo.num);
		int k, i, j, count, n;
		{//将特有规则写入文件（总共sudo.num条语句） 
		//避免两个数独的重叠区重复录入，故分区块写入特有规则 
			for(i = 1; i <= 9; i++){
				for(j = 1; j <= 9; j++){
					if(sudo.original[i][j])	fprintf(fp, "%d 0\n", VarTrans(1,i,j,sudo.original[i][j]));
				}
			} 
			for(i = 7; i <= 9; i++){
				for(j = 10; j <= 15; j++){
					if(sudo.original[i][j])	fprintf(fp, "%d 0\n", VarTrans(2,i,j,sudo.finalans[i][j]));
				}
			}
			for(i = 10; i <= 15; i++){
				for(j = 7; j<= 15; j++){
					if(sudo.original[i][j])	fprintf(fp, "%d 0\n", VarTrans(2,i,j,sudo.finalans[i][j]));
				}
			}	
		}
		
		{//将基础规则写入文件（单数独有11988条，共两个数独）
			for(k = 1; k <= 2; k++){
				for(i = 1; i <= 9; i++){						//行约束，每行有9+36*9条规则，共9行 
					for(n = 1; n <= 9; n++){					//9条规则 
						for(j = 1; j <= 9; j++){
							fprintf(fp, "%d ", VarTrans(k,i,j,n));
						}
						fprintf(fp,"0\n");
					}
					for(n = 1; n <= 9; n++){					//9*36条规则 
						for(count = 1; count <= 9; count++){	//36条规则 
							for(j = count+1; j <= 9; j++){
								fprintf(fp, "%d %d 0\n", -VarTrans(k,i,count,n), -VarTrans(k,i,j,n));
							}
						}
					}
				}
				
				for(j = 1; j <= 9; j++){						//列约束，每行有9+36*9条规则，共9行 
					for(n = 1; n <= 9; n++){					//9条规则 
						for(i = 1; i <= 9; i++){
							fprintf(fp, "%d ", VarTrans(k,i,j,n));
						}
						fprintf(fp,"0\n");
					}
					for(n = 1; n <= 9; n++){					//9*36条规则 
						for(count = 1; count <= 9; count++){	//36条规则 
							for(i = count+1; i <= 9; i++){
								fprintf(fp, "%d %d 0\n", -VarTrans(k,count,j,n), -VarTrans(k,i,j,n));
							}
						}
					}
				}
				
				for(count = 0; count < 9; count++){		//九宫格约束，每个九宫格9+36*9条规则，共9个九宫格 
					int x = 1 + dis[count][0] * 3;
					int y = 1 + dis[count][1] * 3;
					//[x][y]是每个九宫格左上角格子的位置 
					for(int n = 1; n <= 9; n++){		//9条规则 
						for(int m = 0; m < 9; m++)
							fprintf(fp, "%d ", VarTrans(k, x+dis[m][0], y+dis[m][1], n));
						fprintf(fp, "0\n");	
					}
					for(int n = 1; n <= 9; n++){		//9*36条规则 
						for(int p = 0; p < 9; p++){		//36条规则 
							for(int q = p+1; q < 9; q++){
								fprintf(fp, "%d %d 0\n",-VarTrans(k, x+dis[p][0], y+dis[p][1], n), -VarTrans(k, x+dis[q][0], y+dis[q][1], n));
							}
						}
					} 
				}
				
				for(i = 1; i <= 9; i++){				//81个格子，每个格子有1+36条规则 
					for(j = 1; j <= 9; j++){
						for(n = 1; n <= 9; n++){
							fprintf(fp, "%d ", VarTrans(k,i,j,n));
						}
						fprintf(fp, "0\n");
						
						for(count = 1; count <= 9; count++){//36条规则 
							for(n = count+1; n <= 9; n++){
								fprintf(fp, "%d %d 0\n", -VarTrans(k,i,j,count), -VarTrans(k,i,j,n));
							}
						} 
					}
				} 
			}	
		}	
		
		{//将重叠部分的数独规则写入文件（共9*18 = 162条） 
			for(int x1 = 7, x2 = 1; x1 <= 9 && x2 <= 3; x1++,x2++){
				for(int y1 = 7, y2 = 1; y1 <= 9 && y2 <= 3; y1++,y2++){
					for(n = 1; n <= 9; n++){
						fprintf(fp, "%d %d 0\n", -VarTrans(1,x1,y1,n), VarTrans(2,x2,y2,n));
						fprintf(fp, "%d %d 0\n", VarTrans(1,x1,y1,n), -VarTrans(2,x2,y2,n));
					}
				}
			}
		}
	}
	fclose(fp); 
	
} 

void DigHole(int diff){	//挖洞法创造数独题面 
	int count_del = 0;	//记录已经删除（挖洞）的个数 
	int target = 0;		//目标挖洞数 
	if(diff == 1)	target = 50;
	else if(diff == 2)	target = 70;
	else if(diff == 3)	target = 90;
	while(1){
		if(count_del >= target)	break; 
		int x = rand() % 15 + 1;
		int y = rand() % 15 + 1;
		if((x <= 6 && y > 9) || (x > 9 && y <= 6))	continue;
		if(IfCanDig(x,y) == OK){
			sudo.flag[x][y] = 1;			//在输入求解时用到，flag为1说明可以输入数字，flag为0说明是题面初始数字，不能更改 
			count_del++;
		}
	}
} 

status IfCanDig(int x, int y){//判断当前位置可否挖洞 
	if(sudo.original[x][y] == 0 || sudo.finalans[x][y] == -1)	return ERROR;
	else{
		for(int i = 1; i <= 9; i++){
			if(i == sudo.original[x][y])	continue;
			else{
				sudo.original[x][y] = i;
				FormCnfFile(2);
				char cnfname[200] = "dighole.cnf";
				ReadFile(cnfname);
				int res = func1(1,0);
				if(res == OK){		//说明解不唯一，则该点不能删除 
					sudo.original[x][y] = sudo.finalans[x][y];	//恢复 
					return ERROR;
				}
				else{
					sudo.original[x][y] = 0;	//可以挖洞
					sudo.num--; 
					return OK;
				}
				
			}
		}
	}
	
}

void PrintSudoku(){//打印当前题面 
	for(int i = 1; i <= 15; i++){
		for(int j = 1; j <= 15; j++){
			if(sudo.original[i][j] == -1){
				printf("  ");
				continue;
			}	
			printf("%d ",sudo.original[i][j]);
		}
		cout << endl;
	}
}

void PrintAnsSudoku(){//打印数独答案 
	for(int i = 1; i <= 15; i++){
		for(int j = 1; j <= 15; j++){
			if(sudo.finalans[i][j] == -1){
				printf("  ");
				continue;
			}
			printf("%d ",sudo.finalans[i][j]);
		}
		cout << endl;
	}
}

void InputToSolve(){//输入求解模块 
	printf("请依次输入行、列以及填充的数字：\n");
	int x, y, v;
	cin >> x >> y >> v;
	if((x < 1 || x > 15) || (y < 1 || y > 15) || (v < 0 || v > 9)){
		printf("请输入合法的位置以及填充的数字！\n");
		getchar();getchar();
		return;
	}
	else if(sudo.flag[x][y] == 0){
		printf("您输入的位置是题目所给数字的位置，此位置数字不可更改！\n");
		getchar();getchar();
		return; 
	}
	sudo.original[x][y] = v;
	
}

status CheckCurrentSolve(){//检查当前输入的解是否有误 
	for(int i = 1; i <= 15; i++){
		for(int j = 1; j <= 15; j++){
			if(sudo.finalans[i][j] == -1)	continue;
			if(sudo.original[i][j] != sudo.finalans[i][j] && sudo.original[i][j] != 0)	return ERROR;
		}
	}
	return OK;
}
 

void hint(){
	while(1){
		int x = rand() % 15 + 1;
		int y = rand() % 15 + 1;
		if((x <= 6 && y > 9) || (x > 9 && y <= 6))	continue;
		sudo.original[x][y] = sudo.finalans[x][y];
		break; 
	} 
}
