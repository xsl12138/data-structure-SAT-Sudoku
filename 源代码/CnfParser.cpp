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
		printf("                  1.����cnt�ļ�			2.����ļ�����                  \n");
		printf("                  3.DPLL���			4.��������                  \n");	//ѡ��3ͬʱ���ʱ�����ܺͱ����ļ� 
		printf("                  5.����		                                    \n");
		printf("                  0.�˳�  		                                        \n");
		printf("------------------------------------------------------------------------\n");
		cin >> op; 
		if(op == 1){
			fflush(stdin);
			printf("�������ļ�����");
			scanf("%s",FileName);
			ReadFile(FileName);
			printf("���ļ���%d��������%d���Ӿ�\n",qq.vexnum,qq.clanum);
			printf("�ѳɹ���ȡ�ļ���\n");
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
			printf("����������������룡");
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
	init();				//��ʼ�� 
	FILE* fp = NULL;
	if(!(fp = fopen(filename, "r"))){
		printf("�޷����ļ���");
		getchar();
	}
	else{
		char c;
		char str[5000] = {'\0'};
//		printf("�ļ�ע���������£�\n\n"); 
		while((c = fgetc(fp)) == 'c'){
//			cout << c << ' '; 
			fgets(str, 5000, fp);
//			cout << str;
		}
//		cout << endl;
		fgetc(fp);								//���ո�
		fscanf(fp,"%s%d%d",str,&qq.vexnum,&qq.clanum); 
		root = (clause*)malloc(sizeof(clause));
		root->literal_num = 0;
		clause* tem1 = root;					//ָ���Ӿ� 
		node* tem2_r = NULL, *tem2_l = NULL;	//ָ�����֣�����β�巨���� 
		int num_cur;
		int cnt = 0;							//���� 
		for(int i = 1; i <= qq.clanum; i++){
			if(i != 1){							//��һ�ε�tem1->nextcla����rootָ���λ�ã��ʲ����ٿ��ٿռ� 
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
			tem2_l->nextnode = NULL;			//��β 
			cnt = 0;
		}
		tem1->nextcla = NULL;					//��β 
		
	}
	fclose(fp);
}

void PrintFile(){//����ļ����ݣ����Ӿ䣩
	if(qq.clanum == 0 && qq.vexnum == 0){
		printf("��δ��ȡ�ļ���\n");
		getchar();
		return; 
	}
	printf("���ļ������Ӿ����£�\n"); 
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

void DPLL(){//DPLLģ�� 
	if(qq.clanum == 0 && qq.vexnum == 0){
		printf("��δ��ȡ�ļ���\n");
		getchar();
		return; 
	}
	RecoverBegin(); 		//���洢���ݵ�����ָ�����ʼ״̬���Ա��ٽ���һ�����
	printf("1.func1(last)		2.func2(first)		3.func3(shortmax)		4.func4(max)		5.func5(simple)\n");
	printf("��ѡ����Ҫʹ�õķ�����");
	int opt, res;			//res��¼��������н⻹���޽⣩ 
	scanf("%d",&opt); 
	time_t t1, t2;
	t1 = clock();
	if(opt == 1)	res = func1(1,0);
	else if(opt == 2)	res = func2(1,0);
	else if(opt == 3)	res = func3(1,0);
	else if(opt == 4)	res = func4(1,0);
	else if(opt == 5)	res = func5(1,0);
	else{
		printf("������1~5�е����֣�\n");
		getchar(); 
	}
	t2 = clock();
	printf("�˷�����ʱ��%dms\n",t2-t1);
	if(res == OK)	printf("�ѳɹ���⣡\n");
	else	printf("�����޽⣡\n");
		
	FormAnsFile(res, t2-t1, FileName);			//���𰸱�����res�ļ���
	getchar();getchar();
}

void PrintAnswer(){//��ӡ�� 
	cout << "�����£�" << endl;
	for(int i = 1; i <= qq.vexnum; i++){
		if(!qq.ans[i])	printf("%5d",i);		//˵�����޹ر�����ȡ��ֵ���� 
		else	printf("%5d",qq.ans[i]);
		if(i%10 == 0)	cout << endl;
	}
	getchar();
}

void CheckAnswer(){//���� 
	int res = 0, flag = 0;
	for(clause* cla = root; cla; cla = cla->nextcla){
		flag = 0;
		for(node* lit = cla->first; lit; lit = lit->nextnode){
			if(lit->value == qq.ans[abs(lit->value)]){
			//һ���Ӿ���ֻҪ��һ������ģ���ô�Ӿ������ģ�˵���˽�����ȷ�� 
				flag = 1;
				break; 
			}
		}
		if(!flag)	break; 						//flag��1��0��0˵����õĽ��Ǵ���� 
		//�ܵ�����˵��һ���Ӿ���û��һ����������õĽ���ͬ
	}
	
	if(flag)	printf("�������ȷ��\n");
	else printf("��������\n");
	getchar(); 
}

void FormAnsFile(int res, int time, char filename[200]){//���𰸱�����res�ļ��� 
	FILE* fp;
	int len = strlen(filename);
	filename[len - 3] = 'r';					//�ļ���׺�� 
	filename[len - 2] = 'e';
	filename[len - 1] = 's';
	fp = fopen(filename, "w+");
	fprintf(fp, "s %d", res);
	fprintf(fp,"\nv");
	if(res != 1);								//������� 
	else{
		for (int i = 1;i <= qq.vexnum;i++){		//д��� 
			fprintf(fp, " %d", qq.ans[i]);
		}
	}
	fprintf(fp, "\nt %d", time);				//д�����ʱ�� 
	fclose(fp);
}
