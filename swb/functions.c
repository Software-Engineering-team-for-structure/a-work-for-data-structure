/*
	date: 2021.12.31
	author: shiwenbo
*/

#include "pre.c"

#include "string.h"
#include "stdio.h"
#include "malloc.h"
#include "stdlib.h"
# include "stdbool.h"

//输出地点列表
void narrate()
{
	int sum = G.vex[0].number;
	printf("总共有%d个地点\n", sum);
	for (int i = 0; i < sum; i++)
	{
		printf("%d.%s\n", G.vex[i + 1].number, G.vex[i + 1].sight);
	}
}
// inputCSVfile表示输入的CSV文件的路径，全局变量G.arcs[][]存储输出的邻接矩阵,v顶点数能从文件中读出，a边数也能从文件中读出
int CreateUDN(char *inputCSVfile)
{
	int i, j;
	int k = 0;   //统计矩阵中元素个数
	int count = 0; //统计文件中行数
	int sum_row;   //统计文件中总行数
	int flag = 0;
	int data = 0; //暂存数字
	int arcnum = 0; //暂存边数
	char *temp_str = (char *)malloc(20 * sizeof(char));
	FILE *fp; //文件指针
	FILE *fp1;
	fp = fopen(inputCSVfile, "r"); //打开文件，只读
	// fp1=fopen("int_data.txt","r");
	fp1 = fopen(inputCSVfile, "r");
	if (fp == NULL)
	{
		printf("File cannot open! ");
		// exit;
		return 0;
	}
	while (!feof(fp)) //读文件，直到文件末尾
	{
		flag = fgetc(fp); //将文件内容附给flag
		if (flag == '\n') //如果为\n表示到行尾，便行数加1
			count++;
	}
	sum_row = ++count; //邻接矩阵总行数
	G.vexnum = sum_row - 2; //有两行是信息栏不是矩阵
	// printf("顶点个数为：%d", sum_row-1); // 测试打印文件行数，因为每行只有一个数，所以即统计文件中有多少个数
	printf("\n");
	VertexType sites;
	char temp;
	for (i = 0; i < sum_row; i++) //控制行数
	{
		if (i == 0) //处理第一行,读取sight
		{
			///////////////////////////读取第一行的字符，获得建筑物列表
			int k; //用来记录建筑物序号
			for (k = 0; k < sum_row -1 ; k++)
			{												   //从1开始为地名，0为(adj matrix)可以留下作为哨兵位置
				char *str = (char *)malloc(sizeof(char) * 20); //暂存数组,由于指针的特性，如果不在此处每次都临时创建空间，会导致多个sight都指向同一个空间最终导致所有的地点名都相同
				if (k == sum_row - 2)
				{
					fscanf(fp1, "%s", str);
				}
				else
				{
					fscanf(fp1, "%[^,]%*c", str);
				}
				G.vex[k].sight = (char *)calloc(20, sizeof(char)); //单次只分配一个字符串空间，最终空间由sum_row确定，减少空间浪费
				G.vex[k].sight = str;
				G.vex[k].number = k;
				printf("%s\n",G.vex[k].sight); //测试读取值
			}
			G.vex[0].number = k - 1; //记录建筑物总数
			G.vexnum = k - 1; //结点总数，也就是建筑物总数
			// buiding初始化完成
			printf("\n");
			///////////////////////////////
		}
		else if(1 <= i && i <= sum_row - 2) //1->sum_row都是矩阵元素
		{
			//////////////////////////////读取非第一行的数字
			for (j = 0; j < sum_row - 1; j++) //控制列数 如果是','就不记录，如果是数字就记录,从一开始是为了避开表头的字符串，也因此后面读入的时候都是存入j-1中
			{
				if (j == 0) //第一个为表头，读掉其缓存，str作为暂存表头的变量
				{
					fscanf(fp1, "%[^,]%*c", temp_str); //该temp_str无用，仅仅作为一个参数读掉表头，故不需要为其临时分配空间，只给一个函数内有效域的空间就行
													   // printf("%s",str);  //测试是否读取
				}
				else
				{
					if (j == sum_row - 2) //读到最后一个数字时改变读取方式，不加上，
					{
						fscanf(fp1, "%d", &data);
					}
					else
					{
						fscanf(fp1, "%d,", &data);
					}

					if (data == -1)//用Max代表无穷大
					{
						data = Max;
					}else{
						arcnum++;   //记录已存的数字个数
					} 
					G.arcs[i - 1][j - 1].adj = data;
					
				}
			}
		}
		else //处理最后一行,读取descript
		{
			///////////////////////////读取第一行的字符，获得建筑物列表
			int k; //用来记录建筑物描述序号
			for (k = 0; k < sum_row - 1; k++)
			{												   //从1开始为地名，0为(建筑物信息)可以留下作为哨兵位置
				char *str = (char *)malloc(sizeof(char) * 400); //暂存数组,由于指针的特性，如果不在此处每次都临时创建空间，会导致多个sight都指向同一个空间最终导致所有的地点名都相同
				if (k == sum_row - 2)
				{
					fscanf(fp1, "%s", str);
				}
				else
				{
					fscanf(fp1, "%[^,]%*c", str);
				}
				G.vex[k].description = (char *)calloc(400, sizeof(char)); //单次只分配一个字符串空间，最终空间由sum_row确定，减少空间浪费
				G.vex[k].description = str;
				printf("%s\n",G.vex[k].description); //测试读取值
			}
			G.vex[0].number = k - 1; //记录建筑物总数
			// buiding初始化完成
			printf("\n");
			///////////////////////////////
		}
	}
	G.arcnum = arcnum;
	//G初始化完毕

	// narrate(); //测试已经输入的地点列表
	printf("图共有%d个结点,",G.vexnum);
	printf("共有%d条边\n",G.arcnum);
	printf("\n输入的邻接矩阵为：\n");
	for (int i = 0; i < sum_row - 1 ; i++) //打印表头
	{
		printf("%-10s \t", G.vex[i].sight);
	}
	printf("\n");
	for (int i = 0; i < sum_row - 2; i++) //循环打印保存到数组中的数据,i表行，j表列，num表示数组元素总数，因为有表头的长度，需要对sum_row-1
	{
		printf("%-10s \t", G.vex[i + 1].sight); // building[]从1开始,空格在加\t能保证10位字符后加\t对齐，不然会不会填充到10位
		for (j = 0; j < sum_row - 2; j++)
		{
			if (G.arcs[i][j].adj == Max)
			{
				printf("---           \t"); //邻接矩阵中用Max表示未连通，在用户显示中显示为---
			}
			else
				printf("%-10d \t", G.arcs[i][j].adj); //打印
		}
		printf("\n");
	}
	//数据导入完毕
	//存入图变量完毕
	fclose(fp);	 //关闭文件，释放指向文件的指针
	fclose(fp1); //关闭文件，释放指向文件的指针
	return 1;
}
//查询建筑信息
void search()
{
	int * pt = (int *)calloc(1,sizeof(int));//用来存用户输入的数字
	char ck;
	do
    {
        ck = SearchMenu(pt);    //在SearchMenu中已经写了对于传入pt的数值的合法性判定
		// printf("%d",*pt);   // 测试pt中的值
		if(ck!='e'){
			printf("%s\n",G.vex[*pt].description);
			system("pause");
		}
    } while (ck != 'e');
}

/* 查询子菜单 */
char SearchMenu(int * pt)
{
	char c;
	int flag;
	int isnumber; // 标记输入的字符串是否是数字
	char * instr = (char *)malloc(10*sizeof(char));
	do
	{
		flag = 1;
		system("cls");
		narrate();
		// printf("\n\t\t\t┏━━━━━━━━━━━━━━━━━━━━━━┑\n");
		// printf(  "\t\t\t┃                      ┃\n");
		// printf(  "\t\t\t┃ 1、查询建筑路径       ┃\n");
		// printf(  "\t\t\t┃ 2、查询建筑信息       ┃\n");
		// printf(  "\t\t\t┃ 3、选择出发点和目的地  ┃\n");
		// printf(  "\t\t\t┃ e、退出               ┃\n");
		// printf(  "\t\t\t┃                      ┃\n");
		// printf(  "\t\t\t┗━━━━━━━━━━━━━━━━━━━━━━┛\n");
		// printf(  "\t\t\t\t 请输入您的选择：");
		printf("\n\t\t\t .━━━━━━━━━━━━━━━━━━━━━━━━━━━. \n");
		printf("\t\t\t┃ .-------------------------. ┃\n");
		printf("\t\t\t┃ |                         | ┃\n");
		printf("\t\t\t┃ |   输入编号查询信息      | ┃\n");
		printf("\t\t\t┃ |   输入e退出             | ┃\n");
		printf("\t\t\t┃ |                         | ┃\n");
		printf("\t\t\t┃ '-------------------------' ┃\n");
		printf("\t\t\t '━━━━━━━━━━━━━━━━━━━━━━━━━━━' \n");
		printf("\t\t\t 请输入：");
		gets(instr);
		////////////输入检查
		if(!strcmp(instr,"e"))//判断一个字符串是否为“e”
		{
			c = 'e';
			flag = 0;
		}else{
			for (int i = 0; i < strlen(instr) ; i++)
			{
				if('0'<=instr[i] && instr [i] <= '9') //判断每个字符都是数字
				{
					isnumber = 1; //随遍给的字符，让该函数范围不是‘e’,代表不退出
				}else{
					isnumber = 0;
					flag = 1;
					printf("输入错误，请重新输入!\n");
					system("pause");
					break;
				}
			}
			if(isnumber)   //满足该条件的到这里已经是数字无疑
			{
				flag = 0 ;
				*pt = atoi(instr);
				if( 1 <= *pt && *pt <= G.vex[0].number)
				{
				}
				else{
					*pt = 0; //该输入无效，将指针清空
					flag = 1;
					printf("不存在该地点,请重新输入!\n");
					system("pause");
				}
			}
		}
	} while (flag);
	return c;
}

// int finish(int *S,int n)          //检查该顶点是否已经纳入S[] 
// {
// 	int i; 
//     for(i=0;i<n;i++)
//     {
//         if(!S[i])
//         return 0;
//     }
//     return 1; 
// }

void ShortestPath(int vs)//vs为起点 
{
	vs = vs - 1;
    int i,j,k;
    int min;
    int tmp;
    int flag[NUM];      // 顶点是否已经找到最短路径的判别数组flag 
    // 初始化
    for (i = 0; i < NUM; i++)
    {
        flag[i] = 0;              
        prev[i] = 0;              // 起点的前驱设为0； 
        D[i] = G.arcs[vs][i].adj;
    }

    // 初始化起点 
    flag[vs] = 1;
    D[vs] = 0;

    // 遍历NUM-1次，找到NUM-1个顶点 
    for (i = 1; i < NUM; i++)
    {
        // 寻找当前的最小的路径
        min = Max;
        for (j = 0; j < NUM; j++)
        {
            if (flag[j]==0 && D[j]<min)
            {
                min = D[j];
                k = j;
            }
        }
        // 将该顶点纳入已获取最短路径的定点集 
        flag[k] = 1;

        // 修正当前最短路径和前驱顶点
        for (j = 0; j < NUM; j++)
        {
            tmp = (min + G.arcs[k][j].adj); 
            if (flag[j] == 0 && (tmp< D[j]))
            {
                D[j] = tmp;
                prev[j] = k;
            }
        }
    }
}

void output(int sight1,int sight2){//输出函数 
	sight1 = sight1 -1 ;
	sight2 = sight2 -1 ;
	printf("最短路径长度为:%d\n最短路径为:",D[sight2]);	
	int i = sight2;
	int temp[NUM];
	int j = 0;
	printf("%s--->",G.vex[sight1+1].sight);
	for(;i>=0;){
		if(prev[i]==0) break;
		temp[j++]=prev[i];
		i = prev[i];
	}
	j--;
	
	for(;j>=0;j--){
	
		printf("%s--->",G.vex[temp[j]+1].sight);
	}
	
	
	printf("%s",G.vex[sight2+1].sight);
}





































// ///////图遍历
// typedef struct Stack{
// 	int data[10];    //定义栈的数组
// 	int top;
// }SeqStack;
// SeqStack* InitStack()
// {
// 	SeqStack *S;
// 	S = (SeqStack*)malloc(sizeof(SeqStack));
// 	if(!S)
// 	exit(1);
// 	S->top = -1;
// 	return S;
// }
// int Empty(SeqStack *S)
// {
// 	if(S->top == -1)
// 	return 1;
// 	else
// 	return 0;
// }
// int Push(SeqStack *S, int data)
// {
// 	if(S->top == G.vexnum - 1)
// 	{
// 		printf("栈满\n"); //测试输出
// 		exit(0);
// 	}
// 	S->data[++S->top] = data;
// 	return 1;
// }
// int Pop(SeqStack *S)
// {
// 	if(!Empty(S))
// 	{
// 		return S->data[S->top--];
// 	}
// 	return 0;
// }
// int Top(SeqStack *S)
// {
// 	if(!Empty(S))
// 	return S->data[S->top];
// 	return 0;
// }
// ////////////////////////////////////////////////////
// int visited(int k, SeqStack *S)
// {
// 	for(int i = S->top ;i >= 0; i--)
// 	{
// 		if( k == S->data[i] )
// 		return 1;
// 	}
// 	return 0;
// }

// void Pathoutput(int *Sq){
// 	int i=0;
// 	for(i=0 ; i<G.vexnum-1 ; i++){
// 		printf("%s--->",G.vex[i+1].sight);
// 	}
// 	printf("%s\n",G.vex[i+1].sight);
// }

// void FindAllPath(MGraph G)
// {
// 	int key;
// 	int start,end;
// 	int sqe = 0; //存序号
// 	printf("\t\t\t输入起点编号:");
// 	scanf("%d", &start);
// 	start = start -1;
// 	int i, w, v, Seqence[10];  //Seqence用来存结果序列
// 	v = start;
// 	for(i = 0; i < G.vexnum; i++) //清空
// 	Seqence[i] = -1;

// 	SeqStack *S = InitStack();
// 	Push(S, start);  //start已经遍历过

// 	Seqence[sqe++] = v;
// 	ArcCell *p;
// 	while(!Empty(S))
// 	{
// 		v = Top(S);  //将S顶给v，就是将最近遍历过的点给v
// 		for(int k =0; k < G.vexnum ; k++ )
// 		{
// 			if(G.arcs[v][k].adj != Max && !visited(k,S)) //没遍历的第一个结点
// 			{
// 				v = k;
// 				key = 1;
// 				break;
// 			}else{
// 				key = 0;  //表示没有遍历到结点，需要退栈
// 			}
// 		}
// 		if(key)
// 		{	
// 			Push(S,v);
// 			Seqence[sqe++] = v;
// 			if(sqe == G.vexnum){  //序列已满证明此路可行
// 				Pathoutput(Seqence);
// 				system("pause");
// 				return;
// 			}
// 		}else{
// 			// Seqence[++sqe] = Top(S);
// 			v = Pop(S);
// 			sqe = v;
// 			// Seqence[sqe++] =  
// 		}
// 		// w = Pop(S);
// 	}
// 	Pathoutput(Seqence);
// 	system("pause");
// 		return;
// }

int  DFS(MGraph G, int v){
	if(v==0)return 0;
	printf("%s-->",G.vex[v+1].sight);
	visited[v] = 1;
	for(int w=0;w<G.vexnum;w++){
		if((G.arcs[v][w].adj!=0)&&(!visited[w]))
			DFS(G,w);
	}
	return 0;
}