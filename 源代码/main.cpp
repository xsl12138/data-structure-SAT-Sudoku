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
		if(choice == 1){		//SAT求解 
			fflush(stdin);
			SATQuestion();		//进入SAT模块 
		}
		else if(choice == 2){	//数独问题 
			fflush(stdin);
			Sudoku();			//进入数独模块 
		}
		else if(choice != 0){
			fflush(stdin);
			printf("输入错误！请重新输入1或2或0！"); 
			getchar();
		}
		
	}
	free(root);
	return 0;
}
