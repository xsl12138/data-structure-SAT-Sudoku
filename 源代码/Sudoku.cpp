#include "Global.h"

extern sud sudo;
extern question qq;
extern clause* root;

void Sudoku(){//����ģ�� 
	int op = 1;
	while(op){
		fflush(stdin);
		system("cls");	printf("\n\n");
		printf("                      Welcome to my Sudoku games!:          	        \n");
		printf("                  Click 1 to start game or 0 to exit					\n");
		printf("------------------------------------------------------------------------\n");
		printf("                  1.������������             0.�˳�		              	\n");
		printf("------------------------------------------------------------------------\n");
		cin >> op; 	
		if(op == 1){
			fflush(stdin);
			initsudo();//��ʼ������ 
			system("cls");
			printf("�����������������Ժ�...\n");
			time_t t1, t2;
			t1 = clock();
			CreateSudoku();//�������� 
			t2 = clock();
			printf("�ɹ�������������ʱ %dms\n",t2-t1);
			for(int i = 1; i <= 15; i++){
				for(int j = 1; j <= 15; j++){
					sudo.original[i][j] = sudo.finalans[i][j];
				}
			}
			printf("��ѡ���Ѷȣ�\n");
			printf("1.��		2.�е�		3.����\n");
			int diff;
			scanf("%d",&diff);
			printf("���ڸ������ڿգ����Ժ�...\n"); 
			time_t t3, t4;
			t3 = clock();
			DigHole(diff);//�����ݲ�ͬ�Ѷȣ��ڶ����������� 
			t4 = clock();
			printf("���������棬��ʱ %dms\n",t4-t3);//�ڶ���ʱ 
			printf("������������ʱ�� %dms\n",t2-t1+t4-t3);
			printf("���س��鿴����\n");
			getchar();getchar();
			int opp = 1;
			while(opp){
				system("cls");
				PrintSudoku();		//��ӡ��ǰ����	
				if(sudo.num == 153){//˫������153������ 
					printf("������ɸ���������⣡\n");
					getchar(); 
					break;
				}
				cout << endl;
				printf("1.�������		2.������ʾ		3.��ʾ��		4.���		0.�˳�\n");
				printf("�����룺");
				scanf("%d",&opp);
				if(opp == 1){
					fflush(stdin);
					InputToSolve();		//������� 
				}	
				else if(opp == 2){
					fflush(stdin);
					hint();				//��ʾ 
				} 
				else if(opp == 3){
					fflush(stdin);
					system("cls");
					PrintAnsSudoku();	//��ӡ���մ� 
					printf("�´���Ŭ������\n");
					printf("��������˳�");
					getchar();
					break; 
				}
				else if(opp == 4){		//��鵱ǰ����Ƿ����� 
					fflush(stdin);
					if(CheckCurrentSolve())	printf("��ǰ���ȫ����ȷ��\n");
					else printf("��ǰ�������\n");
					getchar();
				}
				else if(opp != 0){
					fflush(stdin);
					printf("����������������룡");
					getchar();
				}	
			}
		} 
		else if(op != 0){
			fflush(stdin);
			printf("����������������룡");
			getchar();
		}
	}
}

void initsudo(){//������ʼ�� 
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

void CreateSudoku(){			//����һ������ 
	srand((unsigned)time(NULL));
	int x = rand() % 9 + 1;		//�����ȡ�Ϸ����̵�һ��λ�ã�����1~9�е�ĳ������ 
	int y = rand() % 9 + 1;
	int v = rand() % 9 + 1;
	sudo.original[x][y] = v;
	sudo.num = 1;
	FormCnfFile(1);				//����ʼ�����̵���cnf�ļ������ں�������ѵõ������������� 
	char cnfname[200] = "prepare.cnf";
	ReadFile(cnfname);			//��ȡ�ո����ɵ�cnf�ļ���׼��DPLL����������������� 
	func1(1,0);					//��������������棨�𰸴���qq.ans�����У� 
	AnsTransSudo();				//��ans�����б���Ĵ𰸣�ת�Ƶ�sudo.finalans������ 
	sudo.num = 153; 
	FormSudoFile();				//�����ɵ������������txt�ļ� 
}

void AnsTransSudo(){			//��ans�����б���Ĵ𰸣�ת�Ƶ�sudo.finalans������
	int k, i, j, n;
	for(k = 1; k <= 2; k++){	//��k��������1��������������2�������������� 
		for(i = 1; i <= 9; i++){//��i�� 
			for(j = 1; j <= 9; j++){//��j�� 
				for(n = 1; n <= 9; n++){//����n 
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

status VarTrans(int k, int i, int j, int n){//��������1-9����ת������Ȼ˳����� 
	return (k-1)*729 + (i-1)*81 + (j-1)*9 + n;//��ָ���ĵ��еĹ�ʽ 
}


void FormSudoFile(){//��������֮�󣬽�����������txt�ļ��� 
	FILE* fp = NULL;
	if(!(fp = fopen("SudoProblem.txt", "w"))){
		printf("�޷����ļ���");
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

void FormCnfFile(int flag){//����������ת��Ϊ��Ȼ���Ա��벢����cnf�ļ� 
//flag�����������жϴ˺������ĸ����������ˣ���ȷ�����ɵ��ļ�����ʲô����
	char cnfname[200] = {'\0'};
	if(flag == 1)	strcpy(cnfname, "prepare.cnf");
	else if(flag == 2)	strcpy(cnfname, "dighole.cnf"); 
	FILE* fp = NULL;
	int dis[9][2] = { {0,0},{0,1},{0,2},{1,0},{1,1},{1,2},{2,0},{2,1},{2,2} }; 
	if(!(fp = fopen(cnfname, "w"))){
		printf("�޷����ļ�\n");
		getchar(); 
	}
	else{
		fprintf(fp,"p cnf %d %d\n",729*2, 11988*2+162+sudo.num);
		int k, i, j, count, n;
		{//�����й���д���ļ����ܹ�sudo.num����䣩 
		//���������������ص����ظ�¼�룬�ʷ�����д�����й��� 
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
		
		{//����������д���ļ�����������11988����������������
			for(k = 1; k <= 2; k++){
				for(i = 1; i <= 9; i++){						//��Լ����ÿ����9+36*9�����򣬹�9�� 
					for(n = 1; n <= 9; n++){					//9������ 
						for(j = 1; j <= 9; j++){
							fprintf(fp, "%d ", VarTrans(k,i,j,n));
						}
						fprintf(fp,"0\n");
					}
					for(n = 1; n <= 9; n++){					//9*36������ 
						for(count = 1; count <= 9; count++){	//36������ 
							for(j = count+1; j <= 9; j++){
								fprintf(fp, "%d %d 0\n", -VarTrans(k,i,count,n), -VarTrans(k,i,j,n));
							}
						}
					}
				}
				
				for(j = 1; j <= 9; j++){						//��Լ����ÿ����9+36*9�����򣬹�9�� 
					for(n = 1; n <= 9; n++){					//9������ 
						for(i = 1; i <= 9; i++){
							fprintf(fp, "%d ", VarTrans(k,i,j,n));
						}
						fprintf(fp,"0\n");
					}
					for(n = 1; n <= 9; n++){					//9*36������ 
						for(count = 1; count <= 9; count++){	//36������ 
							for(i = count+1; i <= 9; i++){
								fprintf(fp, "%d %d 0\n", -VarTrans(k,count,j,n), -VarTrans(k,i,j,n));
							}
						}
					}
				}
				
				for(count = 0; count < 9; count++){		//�Ź���Լ����ÿ���Ź���9+36*9�����򣬹�9���Ź��� 
					int x = 1 + dis[count][0] * 3;
					int y = 1 + dis[count][1] * 3;
					//[x][y]��ÿ���Ź������ϽǸ��ӵ�λ�� 
					for(int n = 1; n <= 9; n++){		//9������ 
						for(int m = 0; m < 9; m++)
							fprintf(fp, "%d ", VarTrans(k, x+dis[m][0], y+dis[m][1], n));
						fprintf(fp, "0\n");	
					}
					for(int n = 1; n <= 9; n++){		//9*36������ 
						for(int p = 0; p < 9; p++){		//36������ 
							for(int q = p+1; q < 9; q++){
								fprintf(fp, "%d %d 0\n",-VarTrans(k, x+dis[p][0], y+dis[p][1], n), -VarTrans(k, x+dis[q][0], y+dis[q][1], n));
							}
						}
					} 
				}
				
				for(i = 1; i <= 9; i++){				//81�����ӣ�ÿ��������1+36������ 
					for(j = 1; j <= 9; j++){
						for(n = 1; n <= 9; n++){
							fprintf(fp, "%d ", VarTrans(k,i,j,n));
						}
						fprintf(fp, "0\n");
						
						for(count = 1; count <= 9; count++){//36������ 
							for(n = count+1; n <= 9; n++){
								fprintf(fp, "%d %d 0\n", -VarTrans(k,i,j,count), -VarTrans(k,i,j,n));
							}
						} 
					}
				} 
			}	
		}	
		
		{//���ص����ֵ���������д���ļ�����9*18 = 162���� 
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

void DigHole(int diff){	//�ڶ��������������� 
	int count_del = 0;	//��¼�Ѿ�ɾ�����ڶ����ĸ��� 
	int target = 0;		//Ŀ���ڶ��� 
	if(diff == 1)	target = 50;
	else if(diff == 2)	target = 70;
	else if(diff == 3)	target = 90;
	while(1){
		if(count_del >= target)	break; 
		int x = rand() % 15 + 1;
		int y = rand() % 15 + 1;
		if((x <= 6 && y > 9) || (x > 9 && y <= 6))	continue;
		if(IfCanDig(x,y) == OK){
			sudo.flag[x][y] = 1;			//���������ʱ�õ���flagΪ1˵�������������֣�flagΪ0˵���������ʼ���֣����ܸ��� 
			count_del++;
		}
	}
} 

status IfCanDig(int x, int y){//�жϵ�ǰλ�ÿɷ��ڶ� 
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
				if(res == OK){		//˵���ⲻΨһ����õ㲻��ɾ�� 
					sudo.original[x][y] = sudo.finalans[x][y];	//�ָ� 
					return ERROR;
				}
				else{
					sudo.original[x][y] = 0;	//�����ڶ�
					sudo.num--; 
					return OK;
				}
				
			}
		}
	}
	
}

void PrintSudoku(){//��ӡ��ǰ���� 
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

void PrintAnsSudoku(){//��ӡ������ 
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

void InputToSolve(){//�������ģ�� 
	printf("�����������С����Լ��������֣�\n");
	int x, y, v;
	cin >> x >> y >> v;
	if((x < 1 || x > 15) || (y < 1 || y > 15) || (v < 0 || v > 9)){
		printf("������Ϸ���λ���Լ��������֣�\n");
		getchar();getchar();
		return;
	}
	else if(sudo.flag[x][y] == 0){
		printf("�������λ������Ŀ�������ֵ�λ�ã���λ�����ֲ��ɸ��ģ�\n");
		getchar();getchar();
		return; 
	}
	sudo.original[x][y] = v;
	
}

status CheckCurrentSolve(){//��鵱ǰ����Ľ��Ƿ����� 
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
