#include "Global.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
int choice = 1;
char FileName[200] = {'\0'};
question qq;  
sud sudo; 
clause * root = NULL;

int main(void){
	while(choice){
		fflush(stdin);
		system("cls");	printf("\n\n");
		printf("                    Menu for SAT Solver based on DPLL                   \n");
		printf("------------------------------------------------------------------------\n");
		printf("                     1.SAT     2.Sudoku    0.Exit                       \n");
		printf("------------------------------------------------------------------------\n");
		cin >> choice;
		if(choice == 1){		//SAT��� 
			fflush(stdin);
			SATQuestion();		//����SATģ�� 
		}
		else if(choice == 2){	//�������� 
			fflush(stdin);
			Sudoku();			//��������ģ�� 
		}
		else if(choice != 0){
			fflush(stdin);
			printf("�����������������1��2��0��"); 
			getchar();
		}
		
	}
	free(root);
	return 0;
}
