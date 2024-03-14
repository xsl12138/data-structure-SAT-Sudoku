#include "Global.h"


extern char FileName[200];
extern question qq;
extern clause* root;

void SATQuestion(){
	int op = 1;
	while(op){
		fflush(stdin);
		system("cls");	printf("\n\n");
		printf("                      Please make your options:                         \n");
		printf("------------------------------------------------------------------------\n");
		printf("                  1.读入cnt文件			2.输出文件内容                  \n");
		printf("                  3.DPLL求解			4.输出求解结果                  \n");	//选项3同时输出时间性能和保存文件 
		printf("                  5.检查答案		                                    \n");
		printf("                  0.退出  		                                        \n");
		printf("------------------------------------------------------------------------\n");
		cin >> op; 
		if(op == 1){
			fflush(stdin);
			printf("请输入文件名：");
			scanf("%s",FileName);
			ReadFile(FileName);
			printf("该文件有%d个变量，%d个子句\n",qq.vexnum,qq.clanum);
			printf("已成功读取文件！\n");
			getchar();getchar();
		} 
		else if(op == 2){
			fflush(stdin);
			PrintFile();
			
		}
		else if(op == 3){
			fflush(stdin);
			DPLL();	
		}
		else if(op == 4){
			fflush(stdin);
			PrintAnswer();
		}
		else if(op == 5){
			fflush(stdin);
			CheckAnswer();
			
		}
		else if(op != 0){
			fflush(stdin);
			printf("输入错误！请重新输入！");
			getchar();
		}
	}
}

void init(){
	clause* cla_tem = root;
	while(cla_tem != NULL){
		node* liter_tem = cla_tem->first;
		while(liter_tem != NULL){
			node* rel_liter = liter_tem;	//release_literal
			liter_tem = liter_tem->nextnode;
			free(rel_liter);
		}
		clause* rel_cla = cla_tem;			//release_clause
		cla_tem = cla_tem->nextcla;
		free(rel_cla);
	} 
	root = NULL;
	memset(qq.ans,0,sizeof(qq.ans));
	memset(qq.fre_lit,0,sizeof(qq.fre_lit));
	qq.clanum = 0;	qq.vexnum = 0;
}

void ReadFile(char filename[200]){
	init();				//初始化 
	FILE* fp = NULL;
	if(!(fp = fopen(filename, "r"))){
		printf("无法打开文件！");
		getchar();
	}
	else{
		char c;
		char str[5000] = {'\0'};
//		printf("文件注释内容如下：\n\n"); 
		while((c = fgetc(fp)) == 'c'){
//			cout << c << ' '; 
			fgets(str, 5000, fp);
//			cout << str;
		}
//		cout << endl;
		fgetc(fp);								//读空格
		fscanf(fp,"%s%d%d",str,&qq.vexnum,&qq.clanum); 
		root = (clause*)malloc(sizeof(clause));
		root->literal_num = 0;
		clause* tem1 = root;					//指向子句 
		node* tem2_r = NULL, *tem2_l = NULL;	//指向文字，采用尾插法插入 
		int num_cur;
		int cnt = 0;							//计数 
		for(int i = 1; i <= qq.clanum; i++){
			if(i != 1){							//第一次的tem1->nextcla就是root指向的位置，故不用再开辟空间 
				tem1->nextcla = (clause*)malloc(sizeof(clause));
				tem1 = tem1->nextcla;
				tem1->literal_num = 0;
			}
			tem1->flag = 0;
			tem2_l = NULL;	tem2_r = NULL;
			
			fscanf(fp,"%d",&num_cur);
			while(num_cur != 0){
				tem1->literal_num++; 
				tem2_r = (node*)malloc(sizeof(node));	
				tem2_r->value = num_cur;
				qq.fre_lit[abs(num_cur)]++;
				tem2_r->flag = 0; 
				if(!cnt){
					tem1->first = tem2_r;
					cnt++;
				}
				else{
					tem2_l->nextnode = tem2_r;
				}
				tem2_l = tem2_r;
				fscanf(fp,"%d",&num_cur);
			}
			tem2_l->nextnode = NULL;			//结尾 
			cnt = 0;
		}
		tem1->nextcla = NULL;					//结尾 
		
	}
	fclose(fp);
}

void PrintFile(){//输出文件内容（仅子句）
	if(qq.clanum == 0 && qq.vexnum == 0){
		printf("尚未读取文件！\n");
		getchar();
		return; 
	}
	printf("该文件所有子句如下：\n"); 
	clause* tem1 = root;
	node* tem2;
	while(tem1 != NULL){
		tem2 = tem1->first;
		cout << tem1->literal_num <<": ";
		while(tem2 != NULL){
			printf("%5d",tem2->value);
			tem2 = tem2->nextnode;
		}
		cout << endl;
		tem1 = tem1->nextcla;
	}
	printf("---------------------------------------------\n"); 
	getchar();
}

void DPLL(){//DPLL模块 
	if(qq.clanum == 0 && qq.vexnum == 0){
		printf("尚未读取文件！\n");
		getchar();
		return; 
	}
	RecoverBegin(); 		//将存储数据的链表恢复到初始状态，以便再进行一次求解
	printf("1.func1(last)		2.func2(first)		3.func3(shortmax)		4.func4(max)		5.func5(simple)\n");
	printf("请选择你要使用的方法：");
	int opt, res;			//res记录求解结果（有解还是无解） 
	scanf("%d",&opt); 
	time_t t1, t2;
	t1 = clock();
	if(opt == 1)	res = func1(1,0);
	else if(opt == 2)	res = func2(1,0);
	else if(opt == 3)	res = func3(1,0);
	else if(opt == 4)	res = func4(1,0);
	else if(opt == 5)	res = func5(1,0);
	else{
		printf("请输入1~5中的数字！\n");
		getchar(); 
	}
	t2 = clock();
	printf("此方法用时：%dms\n",t2-t1);
	if(res == OK)	printf("已成功求解！\n");
	else	printf("此题无解！\n");
		
	FormAnsFile(res, t2-t1, FileName);			//将答案保存在res文件中
	getchar();getchar();
}

void PrintAnswer(){//打印答案 
	cout << "答案如下：" << endl;
	for(int i = 1; i <= qq.vexnum; i++){
		if(!qq.ans[i])	printf("%5d",i);		//说明是无关变量，取正值即可 
		else	printf("%5d",qq.ans[i]);
		if(i%10 == 0)	cout << endl;
	}
	getchar();
}

void CheckAnswer(){//检查答案 
	int res = 0, flag = 0;
	for(clause* cla = root; cla; cla = cla->nextcla){
		flag = 0;
		for(node* lit = cla->first; lit; lit = lit->nextnode){
			if(lit->value == qq.ans[abs(lit->value)]){
			//一个子句中只要有一个是真的，那么子句就是真的，说明此解是正确的 
				flag = 1;
				break; 
			}
		}
		if(!flag)	break; 						//flag非1即0，0说明求得的解是错误的 
		//能到这里说明一个子句中没有一个文字与求得的解相同
	}
	
	if(flag)	printf("所求解正确！\n");
	else printf("所求解错误！\n");
	getchar(); 
}

void FormAnsFile(int res, int time, char filename[200]){//将答案保存在res文件中 
	FILE* fp;
	int len = strlen(filename);
	filename[len - 3] = 'r';					//文件后缀名 
	filename[len - 2] = 'e';
	filename[len - 1] = 's';
	fp = fopen(filename, "w+");
	fprintf(fp, "s %d", res);
	fprintf(fp,"\nv");
	if(res != 1);								//不能求解 
	else{
		for (int i = 1;i <= qq.vexnum;i++){		//写入答案 
			fprintf(fp, " %d", qq.ans[i]);
		}
	}
	fprintf(fp, "\nt %d", time);				//写入求解时间 
	fclose(fp);
}
