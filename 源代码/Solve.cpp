#include "Global.h"

extern char FileName[200];
extern question qq;
extern clause* root;

void RecoverBegin(){//���س�ʼ״̬ 
	memset(qq.fre_lit, 0, sizeof(qq.fre_lit));
	qq.clanum = 0;
	for(clause* cla = root; cla; cla = cla->nextcla){
		cla->flag = 0;							//�ָ��Ӿ�ڵ�ı�־�� 
		cla->literal_num = 0;					//�����Ӿ�ڵ��м�¼�������� 
		for(node* lit = cla->first; lit; lit = lit->nextnode){
			lit->flag = 0;						//�ָ������ֽڵ�ı�־�� 
			qq.fre_lit[abs(lit->value)]++;		//�ָ������ֳ��ֵĴ��� 
			cla->literal_num++;					//�ָ��Ӿ�ڵ��м�¼�������� 
		}
		qq.clanum++; 
	}
}

status FindSingleClause(){//Ѱ��һ�����Ӿ� 
	int sin_cla = 0, judge = 0;
	for(clause* p1 = root; p1; p1 = p1->nextcla){
		if(p1->literal_num == 1 && !p1->flag){					//����Ҫ����Ӿ���δ��ɾ��������ֻ��һ����Ȼ��Ҫ����Ӿ�û�б�ɾ�� 
			for(node* p2 = p1->first; p2; p2 = p2->nextnode){	//�Ѿ��ҵ��˶�Ӧ���Ӿ䣬����Ҫ�ҵ������Ǹ��Ӿ��е��ĸ�Ԫ�� 
				if(!p2->flag){									//��û�б�ɾ����Ԫ�ؾ���Ŀ��Ԫ�� 
					sin_cla = p2->value;
					judge = 1;									//���ټ����� 
					break;
				}
			}
			if(judge == 1)	break;								//���ټ����� 
		}
	}
	return sin_cla;												//û�ҵ����Ӿ�ʱ����0 
}

status DeleteTarget(int depth, int single){//ɾ��Ŀ���Ӿ������ 

	for(clause* cla = root; cla; cla = cla->nextcla){
		if(cla->flag)	continue;								//���Ӿ��Ѿ�ɾ�����ж���һ�Ӿ� 
		node* lit = cla->first;
		while(lit){
			if(lit->flag){										//�������Ѿ�ɾ�����ж���һ���� 
				lit = lit->nextnode;
				continue;
			}					
			if(lit->value == single){							
				cla->flag = depth;								//ɾ���Ӿ�
				node* lit2 = cla->first;						 
				while(lit2){									//���ٴ�ͷ����һ�鿴��������Щ���֣���Ӧ��������fre_lit���飩��һ 
					if(!lit2->flag){							//���Ӿ�����֮ǰ�Ĳ�����δ��ɾ���Ĳ�����
						qq.fre_lit[abs(lit2->value)]--; 
						lit2 = lit2->nextnode;
						continue;
					}
					lit2 = lit2->nextnode;									
				}
				qq.clanum--;									//���Ӿ���Ҫ��һ 
				break;
			}
			else if(lit->value == -single){						//��ɾ�������� 
				lit->flag = depth;
				qq.fre_lit[abs(single)]--;
				cla->literal_num--;								//���Ӿ�������������һ 
				if(!cla->literal_num)	return ERROR;			//˵��ɾ��������˿��Ӿ� 
				lit = lit->nextnode;							
				continue;							
			}
			lit = lit->nextnode;
		}
	}
	
	return OK;
} 

status FindShortestLastLiteral(){//Ѱ������Ӿ������һ������ 
	int len = MAX_VARNUM;
	int maxx = 0, res = 0;
	for(clause* cla = root; cla; cla = cla->nextcla){
		if(cla->flag)	continue;
		if(cla->literal_num < len){		//Ѱ������Ӿ� 
			len = cla->literal_num;
			for(node* lit = cla->first; lit; lit = lit->nextnode){
				if(!lit->flag && maxx < qq.fre_lit[abs(lit->value)])	res = lit->value;
			}
		}
		
	}
	return res;
}

status FindShortestFirstLiteral(){//Ѱ������Ӿ��еĵ�һ������ 
	int len = MAX_VARNUM;
	for(clause* cla = root; cla; cla = cla->nextcla){
		if(cla->flag)	continue;
		if(cla->literal_num < len){
			len = cla->literal_num;
			for(node* lit = cla->first; lit; lit = lit->nextnode){
				if(!lit->flag)	return lit->value;
			}
		}
		
	}
	return 0;
} 

status FindShortestMaxLiteral(){//Ѱ������Ӿ��г���Ƶ����ߵ����� 
	int len = MAX_VARNUM;
	int maxx = 0, res = 0;
	for(clause* cla = root; cla; cla = cla->nextcla){
		if(cla->flag)	continue;
		if(cla->literal_num <= len){		//Ѱ������Ӿ� 
			len = cla->literal_num;
			for(node* lit = cla->first; lit; lit = lit->nextnode){
				if(!lit->flag && maxx < qq.fre_lit[abs(lit->value)]){
					maxx = qq.fre_lit[abs(lit->value)];
					res = lit->value;
				}
			}
		}
		
	}
	return res;
}

status FindMaxLiteral(){//Ѱ�����У�δɾ���������У�����Ƶ����ߵ����� 
	int maxx = 0;
	int ans = 0; 
	for(int i = 1; i <= qq.vexnum; i++){
		if(maxx < qq.fre_lit[i]){
			maxx = qq.fre_lit[i];
			ans = i;
		}
	} 
	return ans;
}

status FindFirstLiteral(){//Ѱ�����У�δɾ���������е��׸����� 
	clause* cla = root; 
	while(cla){
		if(cla->flag){
			cla = cla->nextcla;
			continue;
		}
		else{
			node* lit = cla->first;
			while(lit){
				if(lit->flag){
					lit = lit->nextnode;
					continue;
				}
				return lit->value;
			}
		}
	}
	return 0;
}

status func1(int depth, int tar){				//�����Ҫ�������ݹ�˼�� 
	if(qq.clanum == 0)	return OK;				//�����ɣ�ʣ���Ӿ���Ϊ0 
	int single = 0;
	if(!tar)	single = FindSingleClause();	//Ѱ��һ�����Ӿ䣨����õ���single�����ɸ��� 
	else	single = tar; 
	
	while(single){								//������ҵ����Ӿ� 
		qq.ans[abs(single)] = single;			//�洢��� 
		int res = DeleteTarget(depth,single);	//ɾ��Ŀ���Ӿ�������
		if(res == ERROR)	return ERROR;		//ɾ����ʱ������˿��Ӿ䣬������Ҫ��
		single = FindSingleClause(); 
	} 
	int max_lit = FindShortestLastLiteral();	//Ѱ������Ӿ������һ�����֣�ע������õ���max_litһ�������ģ� 
	if(!max_lit)	return OK;					//���û�ҵ���˵���������ֳ��ִ������Ѿ����0����ʱ˵���Ѿ���ȷ��� 
	if(func1(depth+1, max_lit) == OK){			//���Ѿ�û�е��Ӿ��ʱ��Ž��еݹ� 
		return OK;								//����ݹ鷵����OK��˵���ҵ��˽� 
	}
	//��һ��ifû��return˵��û�ҵ���
	RecoverLastLevel(depth+1);					//������һ�����ĸ��ĸ�ԭ 
	if(func1(depth+1, -max_lit) == OK){
		return OK;
	} 
	RecoverLastLevel(depth+1);
	return ERROR;								//�ߵ���һ����˵���Ѿ���������� 
}

status func2(int depth, int tar){				//�����Ҫ�������ݹ�˼�� 
	if(qq.clanum == 0)	return OK;				//�����ɣ�ʣ���Ӿ���Ϊ0 
	int single = 0;
	if(!tar)	single = FindSingleClause();	//Ѱ��һ�����Ӿ䣨����õ���single�����ɸ��� 
	else	single = tar; 
	
	while(single){								//������ҵ����Ӿ� 
		qq.ans[abs(single)] = single;			//�洢��� 
		int res = DeleteTarget(depth,single);	//ɾ��Ŀ���Ӿ�������
		if(res == ERROR)	return ERROR;		//ɾ����ʱ������˿��Ӿ䣬������Ҫ��
		single = FindSingleClause(); 
	} 
	int max_lit = FindShortestFirstLiteral();	//Ѱ������Ӿ��е�һ�����֣�ע������õ���max_litһ�������ģ� 
	if(!max_lit)	return OK;					//���û�ҵ���˵���������ֳ��ִ������Ѿ����0����ʱ˵���Ѿ���ȷ��� 
	if(func2(depth+1, max_lit) == OK){			//���Ѿ�û�е��Ӿ��ʱ��Ž��еݹ� 
		return OK;								//����ݹ鷵����OK��˵���ҵ��˽� 
	}
	//��һ��ifû��return˵��û�ҵ���
	RecoverLastLevel(depth+1);					//������һ�����ĸ��ĸ�ԭ 
	if(func2(depth+1, -max_lit) == OK){
		return OK;
	} 
	RecoverLastLevel(depth+1);
	return ERROR;								//�ߵ���һ����˵���Ѿ���������� 
}

status func3(int depth, int tar){				//�����Ҫ�������ݹ�˼�� 
	if(qq.clanum == 0)	return OK;				//�����ɣ�ʣ���Ӿ���Ϊ0 
	int single = 0;
	if(!tar)	single = FindSingleClause();	//Ѱ��һ�����Ӿ䣨����õ���single�����ɸ��� 
	else	single = tar; 
	
	while(single){								//������ҵ����Ӿ� 
		qq.ans[abs(single)] = single;			//�洢��� 
		int res = DeleteTarget(depth,single);	//ɾ��Ŀ���Ӿ�������
		if(res == ERROR)	return ERROR;		//ɾ����ʱ������˿��Ӿ䣬������Ҫ��
		single = FindSingleClause(); 
	} 
	int max_lit = FindShortestMaxLiteral();		//Ѱ������Ӿ��г���Ƶ���������֣�ע������õ���max_litһ�������ģ� 
	if(!max_lit)	return OK;					//���û�ҵ���˵���������ֳ��ִ������Ѿ����0����ʱ˵���Ѿ���ȷ��� 
	if(func3(depth+1, max_lit) == OK){			//���Ѿ�û�е��Ӿ��ʱ��Ž��еݹ� 
		return OK;								//����ݹ鷵����OK��˵���ҵ��˽� 
	}
	//��һ��ifû��return˵��û�ҵ���
	RecoverLastLevel(depth+1);					//������һ�����ĸ��ĸ�ԭ 
	if(func3(depth+1, -max_lit) == OK){
		return OK;
	} 
	RecoverLastLevel(depth+1);
	return ERROR;								//�ߵ���һ����˵���Ѿ���������� 
}

status func4(int depth, int tar){				//�����Ҫ�������ݹ�˼�� 
	if(qq.clanum == 0)	return OK;				//�����ɣ�ʣ���Ӿ���Ϊ0 
	int single = 0;
	if(!tar)	single = FindSingleClause();	//Ѱ��һ�����Ӿ䣨����õ���single�����ɸ��� 
	else	single = tar; 
	
	while(single){								//������ҵ����Ӿ� 
		qq.ans[abs(single)] = single;			//�洢��� 
		int res = DeleteTarget(depth,single);	//ɾ��Ŀ���Ӿ�������
		if(res == ERROR)	return ERROR;		//ɾ����ʱ������˿��Ӿ䣬������Ҫ��
		single = FindSingleClause(); 
	} 
	int max_lit = FindMaxLiteral();				//Ѱ�ң�δ��ɾ���������У����ִ����������� 
	if(!max_lit)	return OK;					//���û�ҵ���˵���������ֳ��ִ������Ѿ����0����ʱ˵���Ѿ���ȷ��� 
	if(func4(depth+1, max_lit) == OK){			//���Ѿ�û�е��Ӿ��ʱ��Ž��еݹ� 
		return OK;								//����ݹ鷵����OK��˵���ҵ��˽� 
	}
	//��һ��ifû��return˵��û�ҵ���
	RecoverLastLevel(depth+1);					//������һ�����ĸ��ĸ�ԭ 
	if(func4(depth+1, -max_lit) == OK){
		return OK;
	} 
	RecoverLastLevel(depth+1);
	return ERROR;								//�ߵ���һ����˵���Ѿ���������� 
}

status func5(int depth, int tar){				//�����Ҫ�������ݹ�˼�� 
	if(qq.clanum == 0)	return OK;				//�����ɣ�ʣ���Ӿ���Ϊ0 
	int single = 0;
	if(!tar)	single = FindSingleClause();	//Ѱ��һ�����Ӿ䣨����õ���single�����ɸ��� 
	else	single = tar; 
	
	while(single){								//������ҵ����Ӿ� 
		qq.ans[abs(single)] = single;			//�洢��� 
		int res = DeleteTarget(depth,single);	//ɾ��Ŀ���Ӿ�������
		if(res == ERROR)	return ERROR;		//ɾ����ʱ������˿��Ӿ䣬������Ҫ��
		single = FindSingleClause(); 
	} 
	int max_lit = FindFirstLiteral();			//Ѱ�ң�δ��ɾ���������У��׸����� 
	if(!max_lit)	return OK;					//���û�ҵ���˵���������ֳ��ִ������Ѿ����0����ʱ˵���Ѿ���ȷ��� 
	if(func5(depth+1, max_lit) == OK){			//���Ѿ�û�е��Ӿ��ʱ��Ž��еݹ� 
		return OK;								//����ݹ鷵����OK��˵���ҵ��˽� 
	}
	//��һ��ifû��return˵��û�ҵ���
	RecoverLastLevel(depth+1);					//������һ�����ĸ��ĸ�ԭ 
	if(func5(depth+1, -max_lit) == OK){
		return OK;
	} 
	RecoverLastLevel(depth+1);
	return ERROR;								//�ߵ���һ����˵���Ѿ���������� 	
}

void RecoverLastLevel(int depth){//���ݹ�ʱʹ�ã�������һ��ݹ�ʱ��״̬
	for(clause* cla = root; cla; cla = cla->nextcla){
		if(cla->flag == depth){					//˵�����Ӿ��ڱ��㱻�޸ģ�ɾ�������� 
			cla->flag = 0;						//�ָ��Ӿ���Ч�� 
			qq.clanum++;						//�ָ����Ӿ��� 
			for(node*lit = cla->first; lit; lit = lit->nextnode){//Ѱ����һ�Ӿ����ڸò㱻�Ķ������� 
				if(lit->flag == depth){
					lit->flag = 0;				//�ָ�������Ч�� 
					cla->literal_num++;			//�ָ��Ӿ�ڵ��¼�������� 
					qq.fre_lit[abs(lit->value)]++;//�ָ������ֳ��ֵĴ��� 
					continue;
				}
				else if(!lit->flag){			 
					qq.fre_lit[abs(lit->value)]++;
				}
			}
		}
		else{
			node* lit2 = cla->first;			//�Ӿ�û�б�ɾ�����������е����ֿ��ܱ�ɾ���ˣ�Ҫ��һ����û�� 
			while(lit2){
				if(lit2->flag == depth){		//������ڸò㱻ɾ�������� 
					lit2->flag = 0;				//�ָ�������Ч�� 
					cla->literal_num++;			//�ָ��Ӿ�ڵ��¼�������� 
					qq.fre_lit[abs(lit2->value)]++;//�ָ������ֳ��ֵĴ��� 
					lit2 = lit2->nextnode;
					continue;
				}
				lit2 = lit2->nextnode;
			} 
		}
	}
}
