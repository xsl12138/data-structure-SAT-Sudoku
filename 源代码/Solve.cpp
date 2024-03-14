#include "Global.h"

extern char FileName[200];
extern question qq;
extern clause* root;

void RecoverBegin(){//返回初始状态 
	memset(qq.fre_lit, 0, sizeof(qq.fre_lit));
	qq.clanum = 0;
	for(clause* cla = root; cla; cla = cla->nextcla){
		cla->flag = 0;							//恢复子句节点的标志域 
		cla->literal_num = 0;					//重置子句节点中记录的文字数 
		for(node* lit = cla->first; lit; lit = lit->nextnode){
			lit->flag = 0;						//恢复各文字节点的标志域 
			qq.fre_lit[abs(lit->value)]++;		//恢复各文字出现的次数 
			cla->literal_num++;					//恢复子句节点中记录的文字数 
		}
		qq.clanum++; 
	}
}

status FindSingleClause(){//寻找一个单子句 
	int sin_cla = 0, judge = 0;
	for(clause* p1 = root; p1; p1 = p1->nextcla){
		if(p1->literal_num == 1 && !p1->flag){					//首先要求该子句中未被删除的文字只有一个，然后要求该子句没有被删除 
			for(node* p2 = p1->first; p2; p2 = p2->nextnode){	//已经找到了对应单子句，现在要找到具体是该子句中的哪个元素 
				if(!p2->flag){									//还没有被删除的元素就是目标元素 
					sin_cla = p2->value;
					judge = 1;									//减少计算量 
					break;
				}
			}
			if(judge == 1)	break;								//减少计算量 
		}
	}
	return sin_cla;												//没找到单子句时返回0 
}

status DeleteTarget(int depth, int single){//删除目标子句和文字 

	for(clause* cla = root; cla; cla = cla->nextcla){
		if(cla->flag)	continue;								//该子句已经删除，判断下一子句 
		node* lit = cla->first;
		while(lit){
			if(lit->flag){										//该文字已经删除，判断下一文字 
				lit = lit->nextnode;
				continue;
			}					
			if(lit->value == single){							
				cla->flag = depth;								//删除子句
				node* lit2 = cla->first;						 
				while(lit2){									//需再从头遍历一遍看里面有哪些文字，对应的数量（fre_lit数组）减一 
					if(!lit2->flag){							//该子句中在之前的操作中未被删除的才算数
						qq.fre_lit[abs(lit2->value)]--; 
						lit2 = lit2->nextnode;
						continue;
					}
					lit2 = lit2->nextnode;									
				}
				qq.clanum--;									//总子句数要减一 
				break;
			}
			else if(lit->value == -single){						//仅删除该文字 
				lit->flag = depth;
				qq.fre_lit[abs(single)]--;
				cla->literal_num--;								//该子句所含文字数减一 
				if(!cla->literal_num)	return ERROR;			//说明删除后出现了空子句 
				lit = lit->nextnode;							
				continue;							
			}
			lit = lit->nextnode;
		}
	}
	
	return OK;
} 

status FindShortestLastLiteral(){//寻找最短子句中最后一个文字 
	int len = MAX_VARNUM;
	int maxx = 0, res = 0;
	for(clause* cla = root; cla; cla = cla->nextcla){
		if(cla->flag)	continue;
		if(cla->literal_num < len){		//寻找最短子句 
			len = cla->literal_num;
			for(node* lit = cla->first; lit; lit = lit->nextnode){
				if(!lit->flag && maxx < qq.fre_lit[abs(lit->value)])	res = lit->value;
			}
		}
		
	}
	return res;
}

status FindShortestFirstLiteral(){//寻找最短子句中的第一个文字 
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

status FindShortestMaxLiteral(){//寻找最短子句中出现频率最高的文字 
	int len = MAX_VARNUM;
	int maxx = 0, res = 0;
	for(clause* cla = root; cla; cla = cla->nextcla){
		if(cla->flag)	continue;
		if(cla->literal_num <= len){		//寻找最短子句 
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

status FindMaxLiteral(){//寻找所有（未删除）文字中，出现频率最高的文字 
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

status FindFirstLiteral(){//寻找所有（未删除）文字中的首个文字 
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

status func1(int depth, int tar){				//求解主要函数，递归思想 
	if(qq.clanum == 0)	return OK;				//求解完成，剩余子句数为0 
	int single = 0;
	if(!tar)	single = FindSingleClause();	//寻找一个单子句（这里得到的single可正可负） 
	else	single = tar; 
	
	while(single){								//如果能找到单子句 
		qq.ans[abs(single)] = single;			//存储结果 
		int res = DeleteTarget(depth,single);	//删除目标子句与文字
		if(res == ERROR)	return ERROR;		//删除的时候出现了空子句，不满足要求
		single = FindSingleClause(); 
	} 
	int max_lit = FindShortestLastLiteral();	//寻找最短子句中最后一个文字（注意这里得到的max_lit一定是正的） 
	if(!max_lit)	return OK;					//如果没找到，说明所有文字出现次数都已经变成0，此时说明已经正确求解 
	if(func1(depth+1, max_lit) == OK){			//当已经没有单子句的时候才进行递归 
		return OK;								//如果递归返回了OK，说明找到了解 
	}
	//上一个if没有return说明没找到解
	RecoverLastLevel(depth+1);					//把在下一层做的更改复原 
	if(func1(depth+1, -max_lit) == OK){
		return OK;
	} 
	RecoverLastLevel(depth+1);
	return ERROR;								//走到这一步就说明已经不能求解了 
}

status func2(int depth, int tar){				//求解主要函数，递归思想 
	if(qq.clanum == 0)	return OK;				//求解完成，剩余子句数为0 
	int single = 0;
	if(!tar)	single = FindSingleClause();	//寻找一个单子句（这里得到的single可正可负） 
	else	single = tar; 
	
	while(single){								//如果能找到单子句 
		qq.ans[abs(single)] = single;			//存储结果 
		int res = DeleteTarget(depth,single);	//删除目标子句与文字
		if(res == ERROR)	return ERROR;		//删除的时候出现了空子句，不满足要求
		single = FindSingleClause(); 
	} 
	int max_lit = FindShortestFirstLiteral();	//寻找最短子句中第一个文字（注意这里得到的max_lit一定是正的） 
	if(!max_lit)	return OK;					//如果没找到，说明所有文字出现次数都已经变成0，此时说明已经正确求解 
	if(func2(depth+1, max_lit) == OK){			//当已经没有单子句的时候才进行递归 
		return OK;								//如果递归返回了OK，说明找到了解 
	}
	//上一个if没有return说明没找到解
	RecoverLastLevel(depth+1);					//把在下一层做的更改复原 
	if(func2(depth+1, -max_lit) == OK){
		return OK;
	} 
	RecoverLastLevel(depth+1);
	return ERROR;								//走到这一步就说明已经不能求解了 
}

status func3(int depth, int tar){				//求解主要函数，递归思想 
	if(qq.clanum == 0)	return OK;				//求解完成，剩余子句数为0 
	int single = 0;
	if(!tar)	single = FindSingleClause();	//寻找一个单子句（这里得到的single可正可负） 
	else	single = tar; 
	
	while(single){								//如果能找到单子句 
		qq.ans[abs(single)] = single;			//存储结果 
		int res = DeleteTarget(depth,single);	//删除目标子句与文字
		if(res == ERROR)	return ERROR;		//删除的时候出现了空子句，不满足要求
		single = FindSingleClause(); 
	} 
	int max_lit = FindShortestMaxLiteral();		//寻找最短子句中出现频率最多的文字（注意这里得到的max_lit一定是正的） 
	if(!max_lit)	return OK;					//如果没找到，说明所有文字出现次数都已经变成0，此时说明已经正确求解 
	if(func3(depth+1, max_lit) == OK){			//当已经没有单子句的时候才进行递归 
		return OK;								//如果递归返回了OK，说明找到了解 
	}
	//上一个if没有return说明没找到解
	RecoverLastLevel(depth+1);					//把在下一层做的更改复原 
	if(func3(depth+1, -max_lit) == OK){
		return OK;
	} 
	RecoverLastLevel(depth+1);
	return ERROR;								//走到这一步就说明已经不能求解了 
}

status func4(int depth, int tar){				//求解主要函数，递归思想 
	if(qq.clanum == 0)	return OK;				//求解完成，剩余子句数为0 
	int single = 0;
	if(!tar)	single = FindSingleClause();	//寻找一个单子句（这里得到的single可正可负） 
	else	single = tar; 
	
	while(single){								//如果能找到单子句 
		qq.ans[abs(single)] = single;			//存储结果 
		int res = DeleteTarget(depth,single);	//删除目标子句与文字
		if(res == ERROR)	return ERROR;		//删除的时候出现了空子句，不满足要求
		single = FindSingleClause(); 
	} 
	int max_lit = FindMaxLiteral();				//寻找（未被删除的文字中）出现次数最多的文字 
	if(!max_lit)	return OK;					//如果没找到，说明所有文字出现次数都已经变成0，此时说明已经正确求解 
	if(func4(depth+1, max_lit) == OK){			//当已经没有单子句的时候才进行递归 
		return OK;								//如果递归返回了OK，说明找到了解 
	}
	//上一个if没有return说明没找到解
	RecoverLastLevel(depth+1);					//把在下一层做的更改复原 
	if(func4(depth+1, -max_lit) == OK){
		return OK;
	} 
	RecoverLastLevel(depth+1);
	return ERROR;								//走到这一步就说明已经不能求解了 
}

status func5(int depth, int tar){				//求解主要函数，递归思想 
	if(qq.clanum == 0)	return OK;				//求解完成，剩余子句数为0 
	int single = 0;
	if(!tar)	single = FindSingleClause();	//寻找一个单子句（这里得到的single可正可负） 
	else	single = tar; 
	
	while(single){								//如果能找到单子句 
		qq.ans[abs(single)] = single;			//存储结果 
		int res = DeleteTarget(depth,single);	//删除目标子句与文字
		if(res == ERROR)	return ERROR;		//删除的时候出现了空子句，不满足要求
		single = FindSingleClause(); 
	} 
	int max_lit = FindFirstLiteral();			//寻找（未被删除的文字中）首个文字 
	if(!max_lit)	return OK;					//如果没找到，说明所有文字出现次数都已经变成0，此时说明已经正确求解 
	if(func5(depth+1, max_lit) == OK){			//当已经没有单子句的时候才进行递归 
		return OK;								//如果递归返回了OK，说明找到了解 
	}
	//上一个if没有return说明没找到解
	RecoverLastLevel(depth+1);					//把在下一层做的更改复原 
	if(func5(depth+1, -max_lit) == OK){
		return OK;
	} 
	RecoverLastLevel(depth+1);
	return ERROR;								//走到这一步就说明已经不能求解了 	
}

void RecoverLastLevel(int depth){//（递归时使用）返回上一层递归时的状态
	for(clause* cla = root; cla; cla = cla->nextcla){
		if(cla->flag == depth){					//说明该子句在本层被修改（删除操作） 
			cla->flag = 0;						//恢复子句有效性 
			qq.clanum++;						//恢复总子句数 
			for(node*lit = cla->first; lit; lit = lit->nextnode){//寻找这一子句中在该层被改动的文字 
				if(lit->flag == depth){
					lit->flag = 0;				//恢复文字有效性 
					cla->literal_num++;			//恢复子句节点记录的文字数 
					qq.fre_lit[abs(lit->value)]++;//恢复该文字出现的次数 
					continue;
				}
				else if(!lit->flag){			 
					qq.fre_lit[abs(lit->value)]++;
				}
			}
		}
		else{
			node* lit2 = cla->first;			//子句没有被删除，但是其中的文字可能被删除了，要找一找有没有 
			while(lit2){
				if(lit2->flag == depth){		//如果有在该层被删除的文字 
					lit2->flag = 0;				//恢复文字有效性 
					cla->literal_num++;			//恢复子句节点记录的文字数 
					qq.fre_lit[abs(lit2->value)]++;//恢复该文字出现的次数 
					lit2 = lit2->nextnode;
					continue;
				}
				lit2 = lit2->nextnode;
			} 
		}
	}
}
