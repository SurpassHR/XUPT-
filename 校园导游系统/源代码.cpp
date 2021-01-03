#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

#define MAXSIZE 66              //数组长度的最大值 
#define MAXVEX  20             	//最大顶点个数 
#define INFINITY 32767			//表示极大值             	

//以下全局变量用于DFS中 
int count1 = 0;// DFSALL ,路径计数器，全局变量，直接++，如果局部变量，递归到上一层值还是没变 
int way1[MAXSIZE] = {0}; //路径数组 
int count2 = 0;// DFSALL ,层数 
int way2[MAXSIZE] = {0}; //路径数组 
int minc = MAXVEX; //中转最少顶点数最值 
int minway[MAXSIZE] = {0};//路径数组 

//顶点类型的定义
typedef struct{
    char  name[19];		//地点的名称 
	char  info[66];    //地点的介绍 
	int visited;        //访问标志 
}Vextype;

//邻接矩阵的数据类型 
typedef struct{
	int arcs[MAXVEX][MAXVEX]; //边(或弧)信息 
	Vextype vex[MAXVEX];    //顶点信息，顶点类型根据实际情况自行定义 
    int vexnum;		//顶点数目 
	int arcnum;     //边(或弧)数目 
	int visited[MAXVEX];   //访问标志数组，标志是否被访问过 
}AdjMatrix;

//存入地图文件 
void MapFile(AdjMatrix G){
	char filename[30] = "map.txt";
	FILE *fp;
	fp = fopen(filename,"wt");
	if(fp == NULL)
	{
		printf("\n不能打开!");
		exit(1);
	}
   	fprintf(fp,"|------------------------------------------------------------------------------------------------|\n");
    fprintf(fp,"|                                ☆西安邮电大学校园地点一览 ☆                                   |\n");
    fprintf(fp,"|------------------------------------------------------------------------------------------------|\n");
    fprintf(fp,"|                                                                                                |\n"); 
	fprintf(fp,"|                   西区正门                                                                     |\n"); 
	fprintf(fp,"|                      |                                                                         |\n");     
	fprintf(fp,"|                      |                                                                         |\n");
	fprintf(fp,"|                      |                                   东区正门==================逸夫楼      |\n");                          
	fprintf(fp,"|                      |                                                     |         |         |\n");     
	fprintf(fp,"|            |===水煮鸽子                                                    |         |         |\n"); 
	fprintf(fp,"|            |         |                                                     |         |         |\n");     
	fprintf(fp,"|           基础       |                                                     |      安美公寓     |\n");    
	fprintf(fp,"|          教学楼      |                                                     |         |         |\n");    
	fprintf(fp,"|            |         |                                                     |         |         |\n");  
	fprintf(fp,"|            |         |                                                     |      安悦公寓     |\n");  
	fprintf(fp,"|            |         |                                          东升苑 ====|                   |\n");    
	fprintf(fp,"|            |         |                                                     |                   |\n");
	fprintf(fp,"|            |       图书馆                                                  |                   |\n");    
	fprintf(fp,"|       教学实验楼     |                                                     |                   |\n");     
	fprintf(fp,"|            |         |=====================================================|                   |\n");     
	fprintf(fp,"|            |         |                 |||   西邮桥   |||                  |                   |\n");
	fprintf(fp,"|  医务室====|         |=====================================================|                   |\n");
	fprintf(fp,"|                      |                                                                         |\n");    
	fprintf(fp,"|                      |===体育馆                                                                |\n");     
	fprintf(fp,"|                      |                                                                         |\n");    
	fprintf(fp,"|                      |                                                                         |\n");    
	fprintf(fp,"|                    旭日苑                                                                      |\n");     
	fprintf(fp,"|                      |                                                                         |\n");
    fprintf(fp,"|                      |                                                                         |\n");
	fprintf(fp,"|                  西区宿舍楼          	                                                         |\n");      
	fprintf(fp,"|                                                                                                |\n");        
    fprintf(fp,"|------------------------------------------------------------------------------------------------|\n");
	fclose(fp);
}

//打印地图
void MapOutput(){
	printf("|------------------------------------------------------------------------------------------------|\n");
    printf("|                                ☆西安邮电大学校园地点一览 ☆                                   |\n");
    printf("|------------------------------------------------------------------------------------------------|\n");
    printf("|                                                                                                |\n"); 
	printf("|                   西区正门                                                                     |\n"); 
	printf("|                      |                                                                         |\n");     
	printf("|                      |                                                                         |\n");
	printf("|                      |                                   东区正门==================逸夫楼      |\n");                          
	printf("|                      |                                                     |         |         |\n");     
	printf("|            |===水煮鸽子                                                    |         |         |\n"); 
	printf("|            |         |                                                     |         |         |\n");     
	printf("|           基础       |                                                     |      安美公寓     |\n");    
	printf("|          教学楼      |                                                     |         |         |\n");    
	printf("|            |         |                                                     |         |         |\n");  
	printf("|            |         |                                                     |      安悦公寓     |\n");  
	printf("|            |         |                                          东升苑 ====|                   |\n");    
	printf("|            |         |                                                     |                   |\n");
	printf("|            |       图书馆                                                  |                   |\n");    
	printf("|       教学实验楼     |                                                     |                   |\n");     
	printf("|            |         |=====================================================|                   |\n");     
	printf("|            |         |                 |||   西邮桥   |||                  |                   |\n");
	printf("|  医务室====|         |=====================================================|                   |\n");
	printf("|                      |                                                                         |\n");    
	printf("|                      |===体育馆                                                                |\n");     
	printf("|                      |                                                                         |\n");    
	printf("|                      |                                                                         |\n");    
	printf("|                    旭日苑                                                                      |\n");     
	printf("|                      |                                                                         |\n");
    printf("|                      |                                                                         |\n");
	printf("|                  西区宿舍楼          	                                                         |\n");      
	printf("|                                                                                                |\n");        
    printf("|------------------------------------------------------------------------------------------------|\n");    
 } 
 
//创建图,信息从文件中读取 
void Create(AdjMatrix *G){
	int i,j;
	int m,n,weight;
	int vexnum,arcnum;
	char name[19],info[66];
	
	for(i = 1; i <= G->vexnum; i++){   //初始化visited值，刚开始都为0 
		G->vex[i].visited = 0;
    } 
    
	FILE *fp1;
	fp1 = fopen("flag.txt","rt");
	if(fp1 == NULL){
		printf("\n不能打开!");
		exit(1);
	}
	
	FILE *fp2;
	fp2 = fopen("introduce.txt","rt");
	if(fp2 == NULL){
		printf("\n不能打开!");
		exit(1);
	}
	
	FILE *fp3;
	fp3 = fopen("adj.txt","rt");
	if(fp3 == NULL){
		printf("\n不能打开!");
		exit(1);
	}
	
	
	fscanf(fp1,"%d %d",&vexnum,&arcnum);  //从文件中顶点的个数和边的个数 
	G->vexnum = vexnum;
	G->arcnum = arcnum;	
	
	for(i = 1; i <= G->vexnum; i++){
		fscanf(fp2,"%s\n%s",name,info);
		strcpy(G->vex[i].name,name);
		strcpy(G->vex[i].info,info);
	}
	
	for(i = 1; i <= G->vexnum; i++){ 
		for(j = 1;j <= G->vexnum; j++){ 
			G->arcs[i][j] = INFINITY;
		} 
	} 
	
	for(j = 1; j <= G->arcnum; j++){
		fscanf(fp3,"%6d %6d %6d",&m,&n,&weight);
		G->arcs[m][n] = weight;
		G->arcs[n][m] = weight;
	}
	
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
}

//打印地点列表
void PrintPlace(AdjMatrix *G){ 
	int i;
	for(i = 1; i <= G->vexnum; i++ ){
		printf("\t\t\t\t\t\t%d\t%s\n",i,G->vex[i].name); 
    }
}


void IntroducePlace(AdjMatrix G){  
	int i, num,c;
	int flag = 1;
	char n[30];
	
	while(flag == 1 ){
		system( "cls" );
		printf( "\t\t\t\t\t\t——————地点信息查询——————\n" );
		
		printf("\t\t\t\t\t\t请选择需要查询的方式\n");
		printf("\t\t\t\t\t\t1.地点编号查询\n"); 
		printf("\t\t\t\t\t\t2.地点名称查询\n"); 
		printf("\t\t\t\t\t\t3.地点编号名称共同查询\n"); 
		
		scanf("%d",&c);
		
		if(c == 1){
			
			system("cls");

			PrintPlace(&G);
			
			printf( "\t\t\t\t\t\t请输入需要查找的地点编号：" );
			scanf( "%d", &num );
			
			if(num > 0 && num <= G.vexnum ){
				printf( "\n\n");
				printf( "\t\t\t编号:%d\n",num);
				printf( "\t\t\t名称:%s\n", G.vex[num].name );
				printf( "\t\t\t简介:%s\n\n", G.vex[num].info );
			}
			else{
				printf( "\t\t\t\t\t\t信息输入有误!\n" );
		    }
		    
			printf( "\n\t\t\t\t\t\t是否继续查询地点信息？\n" );
			printf( "\t\t\t\t\t\t1:是\n" );
			printf( "\t\t\t\t\t\t0:返回上级菜单\n" );
			scanf( "%d", &flag );
		}
		else if(c == 2){
			system("cls");
			printf("\t\t\t\t\t\t请输入需要查询的地点名称：");
			scanf("%s",n);
			
			for(i = 1; i <= G.vexnum; i++ ){

				if(strcmp(G.vex[i].name,n) == 0){
					
					printf( "\n\n");
					printf( "\t\t\t编号:%d\n",i);
					printf( "\t\t\t名称:%s\n", G.vex[i].name );
					printf( "\t\t\t简介:%s\n\n", G.vex[i].info );
					break;
				}
   			}

			printf( "\n\t\t\t\t\t\t是否继续查询地点信息？\n" );
			printf( "\t\t\t\t\t\t1:是\n" );
			printf( "\t\t\t\t\t\t0:返回上级菜单\n" );
			scanf( "%d", &flag );			
		}
		else if(c == 3){
			system("cls");
			PrintPlace(&G);
			printf( "\t\t\t\t\t\t请输入需要查找的地点编号：" );
			scanf( "%d", &num );
			printf("\t\t\t\t\t\t请输入需要查询的地点名称:");
			scanf("%s",n);
			if(num > 0 && num <= G.vexnum && strcmp(G.vex[num].name,n) == 0){
				printf( "\n\n");
				printf( "\t\t\t编号:%d\n",num);
				printf( "\t\t\t名称:%s\n", G.vex[num].name );
				printf( "\t\t\t简介:%s\n\n", G.vex[num].info );
			}
			
			printf( "\n\t\t\t\t\t\t是否继续查询地点信息？\n" );
			printf( "\t\t\t\t\t\t1:是\n" );
			printf( "\t\t\t\t\t\t0:返回上级菜单\n" );
			scanf( "%d", &flag );
		}
	}
	system("cls");
}

//地点信息，文件 
void IntroduceFile(AdjMatrix G){
	int i;
	char filename[30] = "introduce.txt";
	
	FILE *fp;
		fp = fopen(filename,"wt");
	if(fp == NULL){
		printf("\n不能打开!");
		exit(1);
	}
	
	G.vexnum = 15;
	
	strcpy(G.vex[1].name,"西区正门"); 
	strcpy(G.vex[1].info,"西安邮电大学长安校区西区的正门"); 
	strcpy(G.vex[2].name,"西邮桥"); 
	strcpy(G.vex[2].info,"链接西安邮电大学长安校区西区和东区的桥梁，过马路请走西游桥"); 
	strcpy(G.vex[3].name,"东区正门"); 
	strcpy(G.vex[3].info,"西安邮电大学长安校区东区的正门"); 
	strcpy(G.vex[4].name,"水煮鸽子"); 
	strcpy(G.vex[4].info,"西安邮电大学长安校区西区正门口，由鸽子和喷泉组成"); 
	strcpy(G.vex[5].name,"基础教学楼"); 
	strcpy(G.vex[5].info,"西安邮电大学教学楼，有A楼和B楼，是西安邮电大学学生们上课的地方");
	strcpy(G.vex[6].name,"教学实验楼"); 
	strcpy(G.vex[6].info,"西安邮电大学长安校区西区实验楼");  
	strcpy(G.vex[7].name,"逸夫楼"); 
	strcpy(G.vex[7].info,"西安邮电大学教学楼，是学生们上课的地方，很容易迷路哦");   
	strcpy(G.vex[8].name,"图书馆"); 
	strcpy(G.vex[8].info,"西安邮电大学图书馆，可以借阅书籍和自习"); 
	strcpy(G.vex[9].name,"医务室"); 
	strcpy(G.vex[9].info,"西安邮电大学医务室"); 
	strcpy(G.vex[10].name,"旭日苑"); 
	strcpy(G.vex[10].info,"西安邮电大学西区食堂");
	strcpy(G.vex[11].name,"东升苑"); 
	strcpy(G.vex[11].info,"西安邮电大学东区食堂");
	strcpy(G.vex[12].name,"体育馆"); 
	strcpy(G.vex[12].info,"西安邮电大学体育馆，内有运动场地"); 
	strcpy(G.vex[13].name,"西区宿舍"); 
	strcpy(G.vex[13].info,"分为长思公寓和长智公寓"); 
	strcpy(G.vex[14].name,"安美公寓"); 
	strcpy(G.vex[14].info,"分为安美公寓南楼和北楼"); 
	strcpy(G.vex[15].name,"安悦公寓"); 
	strcpy(G.vex[15].info,"分为安悦公寓南楼和北楼");  
	
	//把信息存到文件中
	for(i = 1; i <= G.vexnum; i++){  
		fprintf(fp,"%s\n%s\n",G.vex[i].name,G.vex[i].info); 
	} 
	
	fclose(fp);
}

//邻接矩阵信息，文件 
void AdjFile(AdjMatrix G){
	int i,j;
	char filename[30] = "adj.txt";
	char filename1[30] = "flag.txt";

	FILE *fp;
		fp = fopen(filename,"wt");
	if(fp == NULL){
		printf("\n不能打开!");
		exit(1);
	}
		
	FILE *fp1;
		fp1 = fopen(filename1,"wt");
	if(fp1 == NULL){
		printf("\n不能打开!");
		exit(1);
	}
	
	
	G.vexnum = 15;
	G.arcnum = 24;   //15个顶点，24个边
	fprintf(fp1,"%d %d\n",G.vexnum,G.arcnum); //存入文件标记
		//初始化矩阵 
	for(i = 0; i <= G.vexnum; i++){   
		for(j = 0; j <= G.vexnum; j++){ 
			G.arcs[i][j] = INFINITY;  //权值全部赋无穷大 
		} 
	}    
	
	//给地点设置权值，即需要走的路程 
	G.arcs[1][3] = G.arcs[3][1] = 800;
	G.arcs[1][4] = G.arcs[4][1] = 120;
	G.arcs[2][3] = G.arcs[3][2] = 300;
	G.arcs[2][8] = G.arcs[8][2] = 450;
	G.arcs[2][9] = G.arcs[9][2] = 600;
	G.arcs[2][11] = G.arcs[11][2] = 100;
	G.arcs[2][12] = G.arcs[12][2] = 500;
	G.arcs[3][7] = G.arcs[7][3] = 150;
	G.arcs[4][5] = G.arcs[5][4] = 140;
	G.arcs[4][8] = G.arcs[8][4] = 200;
	G.arcs[5][6] = G.arcs[6][5] = 111;
	G.arcs[5][8] = G.arcs[8][5] = 222;
	G.arcs[6][8] = G.arcs[8][6] = 300;
	G.arcs[6][9] = G.arcs[9][6] = 417;
	G.arcs[6][10] = G.arcs[10][6] = 360;
	G.arcs[6][12] = G.arcs[12][6] = 250;
	G.arcs[7][14] = G.arcs[14][7] = 120;
	G.arcs[8][12] = G.arcs[12][8] = 345;
	G.arcs[9][10] = G.arcs[10][9] = 404;
	G.arcs[10][12] = G.arcs[12][10] = 200;
	G.arcs[10][13] = G.arcs[13][10] = 100;
	G.arcs[11][15] = G.arcs[15][11] = 150;
	G.arcs[14][15] = G.arcs[15][14] = 50;

	for(i = 1; i <= G.vexnum; ++i){
		for(j = 1; j <= G.vexnum; ++j){
			if(G.arcs[i][j] != INFINITY && i < j){
				fprintf(fp,"%10d %10d %10d\n",i,j,G.arcs[i][j]);
				}
		}
	}
	fclose(fp1);
	fclose(fp);
}

//重置访问数组 
void ClearVisited(AdjMatrix *G) {
	int i;
	for(i = 0; i <= MAXVEX; i++){
		G->vex[i].visited = 0; //全部置0 
	}
}

//深度优先搜索遍历 
void DfsAll(AdjMatrix *G,int s,int e){ 
	int i,j;	
	int shuai = 0;
	G->vex[s].visited = 1;  //置起始点为1 
	way1[count1] = s;    //把起点的序号存在一个数组里，此时count是0 
	
	for(i = 1; i <= G->vexnum; i++){  //遍历顶点		
		if(G->arcs[s][i] != INFINITY && G->vex[i].visited == 0 && i != e){  //权值不等于无穷，而且没有被访问过，并且没有回到起点 
			count1++;  //数组下标递增 
			way1[count1] = i;  //给数组对应的下标附上顶点值 
			DfsAll(G,i,e);  //递归 
			G->vex[i].visited = 0; //递归回来返回到这一层还原这一层现状 
			count1--;
			shuai = 0;  //结束的标志 
			continue;
		}
		if(G->arcs[s][i] != INFINITY && G->vex[i].visited ==0 && i == e && shuai == 0){  //输出 
			count1++;
			way1[count1] = e;
			shuai = 1;
			printf("\n");
			printf("\t%s",G->vex[way1[0]].name);  //起始点 
			for(j = 1;j <= count1; j++){
				printf("-->");
				printf("%s",G->vex[way1[j]].name);		//依次打印 
			}
			count1--;  //让全局变量count变成0，以供其它函数使用 
			return;
		}	
	}		
	return ; 
}

//所有简单路径菜单 
void AllPath(AdjMatrix G,int dist[],int path[][MAXVEX]){  
	int s,e;
	int flag = 1;
	while(flag == 1){
		system("cls");
		printf( "\t\t\t\t\t\t——————所有简单路径查询——————\n" );
		
		PrintPlace(&G);
		
		printf("\n\t\t\t\t\t输入起点编号:");
		scanf("%d",&s);
		printf("\t\t\t\t\t输入终点编号:");
		scanf("%d",&e);
		
		if (s > G.vexnum || s <= 0 || e > G.vexnum || e < 0 || s == e){
			printf( "\t\t\t\t\t\t输入错误!\n\n" );
	    }
		else{
			ClearVisited(&G);  //给标志数组初始化 
			count1=0;
			printf("\n\t\t\t从%s到%s的所有简单路径有:\n", G.vex[s].name, G.vex[e].name);
			
			DfsAll(&G,s,e); //调用函数进行实现功能 
		}
		printf("\n\n\t\t\t\t\t\t是否继续查询所有简单路径?\n" );
		printf("\t\t\t\t\t\t1:是\n" );
		printf("\t\t\t\t\t\t0:返回上级菜单\n" );
		scanf("%d", &flag);
	}
	system("cls");
}

//中转最少 ,也可以用广搜，顶点A到顶点B所含边的数目最少的路线，从A出发广度优先遍历，遇到顶点B就可以停止了 
void DfsLitter(AdjMatrix *G,int start,int end){
	int shuai = 0;
	int i,j;
	G->vex[start].visited = 1;
	way2[count2] = start;
	for(i = 1; i <= G->vexnum; i++){		
		if(G->arcs[start][i] != INFINITY && G->vex[i].visited == 0 && i != end){
			count2++;
			way2[count2] = i;
			DfsLitter(G,i,end); //递归 
			G->vex[i].visited = 0;
			count2--;
			shuai = 0;
			continue;
		}  //此时搜到了所有路径 
		
		
		if(G->arcs[start][i] != INFINITY && G->vex[i].visited == 0 && i == end && shuai == 0){
			count2++;
			way2[count2] = end;
			shuai = 1;
		
			if(count2 < minc){   //如果count2比最大顶点数小 
				minc = count2;   //就赋值 
				for(i = 0; i <= count2; i++){
					minway[i]=way2[i];   //将记录的way2值存到minway中 
			    }
			}
			
			count2--;
			return;
		}	
	}		
}

//中转次数最少简单路径
void LitterPath(AdjMatrix G){
	int i,j,s,e;
	int flag = 1;
	
	while (flag == 1){
		int i;
		system("cls");
		printf("\t\t\t\t\t\t——————中转次数最少路径查询——————\n");
		
		PrintPlace(&G);
		
		printf("\n\t\t\t\t\t输入起点编号:");
		scanf( "%d", &s);
		printf("\t\t\t\t\t输入终点编号:");
		scanf("%d", &e);
		if (s > G.vexnum || s <= 0 || e > G.vexnum || e < 0 || s == e)
			printf("\t\t\t\t\t\t输入错误!\n\n");
	
		else{
			ClearVisited(&G);
		
			minc = MAXVEX;
			
			count2 = 0;
		
			printf("\n\t\t\t从%s到%s的中转最少简单路径为:\n", G.vex[s].name, G.vex[e].name);
			DfsLitter(&G,s,e);  //深搜寻找 
		
			printf("\t\t\t%5s ",G.vex[minway[0]].name);
		
			for(i = 1; i <= minc; i++){    //mic即为count2 
				printf("-->");
				printf("%5s ",G.vex[minway[i]].name);
			}
			printf("\n\n\t\t\t\t\t\t是否继续查询中转最少简单路径?\n");
			printf("\t\t\t\t\t\t1:是\n");
			printf("\t\t\t\t\t\t0:返回上级菜单\n");
			scanf("%d", &flag);
			}
		system( "cls" );
	}
}

//最短路径,dist数组记录各条最短路径长度，path数组记录对应路径上的各顶点 ，依最短路径递增次序求得各条路径 
void Dijkstra(AdjMatrix *G,int s,int e,int dist[MAXVEX],int path[][MAXVEX]){  
	int mindist,i,j,k,t = 1;  //mindist 路径的权值(长度) 
	
	for(i = 1; i <= G->vexnum; i++){  //给dist数组初始化， 
		dist[i] = G->arcs[s][i]; //将邻接数组每一行所对应的列的值填入dist数组中 
		
		if(G->arcs[s][i] != INFINITY){  //如果权值不等于无穷大，则说明有路
			path[i][1] = s;   //将起点记录,path[i]记录从源点到Vi最短路径上的各个顶点
	    } 
	}	
	
	path[s][0] = 1;   //置1标志，说明源点为start 
	
	for(i = 2; i <= G->vexnum; i++){   //寻找各条最短路径 
		
		mindist = INFINITY;  //将最小初值设置为无穷大 
		
		for(j = 1; j <= G->vexnum; j++){//选择最小权值的路径 
		
			if(!path[j][0] && dist[j] < mindist){//j未加入集合S  //path数组记录某定点是否加到S中 
				k = j;  //当前起点到顶点k权值最小
				mindist = dist[j];
			}
	    }
			if(mindist == INFINITY){
				return;             //不存在start到其他顶点的路径 
		    }
		    
			path[k][0] = 1;  // 将顶点k加入到集合S中，说明该顶点已经被考察 
			
			
			//看有没有更小的，进而更新修改路径 
			for(j = 1;j <= G->vexnum; j++){   //修改路径
				if(!path[j][0] && G->arcs[k][j]<INFINITY && dist[k]+G->arcs[k][j] < dist[j]){  //第k行有路且未被选中过的顶点 &&到k的最小权值+k到j的权值<当前到j的最小权值路径 
					dist[j] = dist[k]+G->arcs[k][j];
					t = 1;
					while(path[k][t] != 0){   //记录最新的最短路径 
						path[j][t] = path[k][t];
						t++;
					}
					path[j][t] = k;
					path[j][t+1] = 0;
				}
			 } 	
	}
}

//打印两点间的最短路径
void DijkstraPrint(int start, int end ,AdjMatrix G,int path[][MAXVEX]){
	int i = 2;
	int length = 0;
	printf( "\n\t从%s到%s的最短路径是:", G.vex[start].name, G.vex[end].name );
	printf( "  %s", G.vex[start].name );
	
	while(path[end][i]){   //记录的路径 
		printf("-->%s ",G.vex[path[end][i]].name);
		length += G.arcs[path[end][i-1]][path[end][i]];	
		i++;
	}
	printf( "-->%s", G.vex[end].name );
	length += G.arcs[path[end][i-1]][end];	
	printf( "，长度为%dM", length);
	printf( "\n");
 } 

//分菜单 最短路径 
void ShortPath(AdjMatrix G,int dist[],int path[][MAXVEX]){ 
	int i,j,s,e;
	int flag = 1;
	while (flag == 1){
		system("cls");
		printf("\t\t\t\t\t\t——————最短路径查询——————\n");
		PrintPlace(&G);
		printf("\n\t\t\t\t\t输入起点编号:");
		scanf("%d",&s);
		printf("\t\t\t\t\t输入终点编号:");
		scanf("%d",&e);
		if (s > G.vexnum || s <= 0 || e > G.vexnum || e < 0 || s == e){
			printf("\t\t\t\t\t\t输入错误!\n\n");
	    }
		else{
			ClearVisited(&G);   //菜单 
			
			Dijkstra(&G,s,e,dist,path); //算法 
			DijkstraPrint(s,e,G,path);//打印 
	
		}
		printf("\n\t\t\t\t\t\t是否继续查询最短路径?\n");
		printf("\t\t\t\t\t\t1:是\n");
		printf("\t\t\t\t\t\t0:返回上级菜单\n");
		scanf("%d",&flag);
	}
	system("cls");
}

//路径二级主菜单 
void SearchPath(AdjMatrix G,int dist[],int path[][MAXVEX]){ 
	int x,flag = 1;
	while(flag == 1){  //二级菜单 
		printf("\t\t\t1. 所有简单路径\n");  //各顶点均不重复 
		printf("\t\t\t2. 最短的简单路径(中转最少)\n");
		printf("\t\t\t3. 最佳访问路径(最短路径长度)\n");
		printf("\t\t\t0. 返回主菜单\n");
		printf("\t\t\t请选择您需要的操作:");
		scanf("%d",&x);

		switch(x){
			case 1: system("cls");AllPath(G,dist,path);break;
			case 2: system("cls");LitterPath(G);break;
			case 3: system("cls");ShortPath(G,dist,path);break;
			case 0: flag = 0; break;
			default:printf( "\t\t\t\t\t\t\t输入信息错误,请重新输入!\n" ); break;
		}
	system("cls");
   }
}

//注册信息
void enroll(){
	char a[100];                   //注册用户名
	char b[100];                   //注册密码
	char s[100];                   //再次确定密码
	int  len;
	
	
   	printf("请输入您的用户名：");
	scanf("%s",a);
	
	printf("请设置您的密码：");
			
	reset: scanf("%s",b);     //用到了if goto语句 

	    len = strlen(b);    //读取密码长度 
	   
	    if(len > 9){
		  printf("密码长度过长，请重新设置：");
	      goto reset;    //if goto 语句  
	    }
   	    printf("请再次输入您设置的密码：");
     	scanf("%s",s);
	
	    if(strcmp(b,s) == 0){       //字符串比较函数 
		    FILE *fp;
		
			fp=fopen("register.txt","at");     //选用追加方式，可以存多个信息
		
			if(fp == NULL){
				printf("文件不存在或者请先注册!");
				exit(1);                           
	     	}
		
			fprintf(fp,"%s %s\n",a,b);                    //字符串写入函数进行写入操作
        	system("cls");
	    	printf("\n\t------注册成功------\n");
			fclose(fp);
		}
		else if(strcmp(b,s) != 0){
			printf("您两次输入的密码不一致，请重新设置密码！\n");
			goto reset;          //if goto 语句 
		}
}

//登陆页面
int land(){
	int  i = 0;      //i是为了判断密码长度 并加密 
	char a[10];                 
	char b[10];                   

	char user_name[10];           
	char user_passwords[10];      

	printf("请输入您的用户名：");
	scanf("%s",user_name);

    printf("请输入您的密码：");

	while(i < 9 && (user_passwords[i] = getch()) && user_passwords[i] != '\r'){   //如果输入超限 或者 遇到换行符就跳出循环
		printf("*");   //掩饰密码 
		i++;
	}
	
	user_passwords[i] = 0;   //字符串结束标志 '/0' 
	
	FILE *fp;
	fp=fopen("register.txt","rt");       //又打开文件 
	
	if(fp == NULL){
		printf("文件不存在!");
		exit(1);
	}
	
	while(fscanf(fp,"%s %s",a,b) != EOF){    //读文件判断账号密码是否正确     
		if(strcmp(a,user_name) == 0  &&  strcmp(b,user_passwords) == 0){   //字符串比较函数，看是否正确 
		 	system("cls");
			printf("\n\t\t\t--------登陆成功--------\n");
			return 0;
		 } 
	}
	if(1){
		system("cls");
		printf("\n信息输入错误！(请检查注册文件信息)\n");
	    land();
	}
	fclose(fp);
}

//增加新地点和信息 
void AddPlace(AdjMatrix *G){
	int s,e;
	char filename[30] = "introduce.txt";
	char filename1[30] = "flag.txt";
	
	
	int flag = 1;
	char p[20];
	char i[66];
	
	while (flag == 1){
		//增加地点的地名
		//增加地点的介绍 	
		
		printf("请输入新地点的地名：");
		scanf("%s",p);	
		printf("请输入新地点的简介：");
		scanf("%s",i);	
	
		G->vexnum += 1;  //顶点数加1 
	 
		strcpy(G->vex[G->vexnum].name,p); 
		strcpy(G->vex[G->vexnum].info,i); 	
	
		FILE *fp;
		fp = fopen(filename,"at");
		if(fp == NULL){
			printf("\n不能打开!");
			exit(1);
	}
	
	fprintf(fp,"%s\n%s\n",G->vex[G->vexnum].name,G->vex[G->vexnum].info); 
	fclose(fp);

	FILE *fp1;
	fp1 = fopen(filename1,"wt");
	if(fp1 == NULL){
		printf("\n不能打开!");
		exit(1);
	}
	
	fprintf(fp1,"%d %d\n",G->vexnum,G->arcnum); 
	fclose(fp1);

	printf("\t\t添加地点成功，可打开\"introduce文件\"查看！");  //转义字符 
	printf( "\n\t\t\t\t\t\t是否继续添加地点？\n" );
	printf( "\t\t\t\t\t\t1:是\n" );
	printf( "\t\t\t\t\t\t0:返回上级菜单\n" );
	scanf( "%d", &flag );
	
	}
	system("cls"); 
}

//增加新路线	
void AddRoad(AdjMatrix *G){
	int s,e,weight;
	char filename[30] = "adj.txt";
	char filename1[30] = "flag.txt";
	int flag = 1;
	
	while(flag == 1){
		PrintPlace(G);  //打印地点的序号 
	
   	    printf("请输入增加路线的起点序号：");
		scanf("%d",&s);
		printf("请输入增加路线的终点序号：");
		scanf("%d",&e);
	
		printf("请输入两条路线之间的距离：");
		scanf("%d",&weight);		
		G->arcs[s][e] = G->arcs[e][s] = weight;
		
		FILE *fp;
		fp = fopen(filename,"at");
		if(fp == NULL){
			printf("\n不能打开!");
			exit(1);
		}
		fprintf(fp,"%10d %10d %10d\n",s,e,G->arcs[s][e]);
		fclose(fp);
		
		FILE *fp1;
		fp1 = fopen(filename1,"at");
		if(fp1 == NULL){
			printf("\n不能打开!");
			exit(1);
		}
		
		G->arcnum += 1; //边数+1
		fprintf(fp1,"%d %d\n",G->vexnum,G->arcnum); 
		fclose(fp1); 

		system("cls");
		printf("\t\t添加路线成功，可打开\"adj文件\"查看！");  //转义字符 
		printf( "\n\t\t\t\t\t\t是否继续添加路线？\n" );
		printf( "\t\t\t\t\t\t1:是\n" );
		printf( "\t\t\t\t\t\t0:返回上级菜单\n" );
		scanf( "%d", &flag );
	}
	system("cls"); 
}
	
//删除路线 
void DelRoad(AdjMatrix *G){
	int s,e,weight,i,j;
	char filename[30] = "adj.txt";
	char filename1[30] = "flag.txt";
	int flag = 1;
	
	FILE *fp;
	fp = fopen(filename,"at");
	if(fp == NULL){
		printf("\n不能打开!");
		exit(1);
	}
	
	FILE *fp1;
	fp1 = fopen(filename1,"wt");
	if(fp == NULL){
		printf("\n不能打开!");
		exit(1);
	}
	
	while(flag == 1){
	
		PrintPlace(G);
	
		printf("请输入撤销路线的起点序号：");
		scanf("%d",&s);
		printf("请输入撤销路线的终点序号：");
		scanf("%d",&e);
	
		//遍历adj文件，如果遍历到s和e，那么给它的值赋予无穷大		
   		G->arcs[s][e] = G->arcs[e][s] = INFINITY;
  		fprintf(fp,"%10d %10d %10d\n",s,e,G->arcs[s][e]);
  	
	  	G->arcnum -= 1; //边数-1
		fprintf(fp1,"%d %d\n",G->vexnum,G->arcnum); 
  		fclose(fp); 
   		fclose(fp1);
		system("cls");
		printf("\t\t撤销路线成功，可打开\"adj文件\"查看！");  //转义字符 
		printf( "\n\t\t\t\t\t\t是否继续撤销路线？\n" );
		printf( "\t\t\t\t\t\t1:是\n" );
		printf( "\t\t\t\t\t\t0:返回上级菜单\n" );
		scanf( "%d", &flag );   		    
   } 
	system("cls"); 	
} 

//游客
void User(){
	int x;
	int dist[MAXVEX];// 记录各条最短路径长度 
	int	path[MAXVEX][MAXVEX] = {0};//记录对应路径上的各顶点
	AdjMatrix G;   //邻接矩阵   
	Create(&G); 
    while(1){
		printf("\t\t\t1. 校园平面图\n");
		printf("\t\t\t2. 地点信息查询\n");
		printf("\t\t\t3. 问路查询\n");
		printf("\t\t\t0. 退出\n"); 
		printf("\t\t\t请选择您需要的操作:");
		scanf("%d",&x);
	
		switch(x){
		case 1: system("cls"); MapOutput(); break;
		case 2: system("cls"); IntroducePlace(G); break;
		case 3: system("cls"); SearchPath(G,dist,path); break;
		case 0: printf("\n\t\t\t\t\t\t\t"); exit(0); break;
		default: printf("\n———————————————输入信息错误,请重新输入!!!————————————————————\n"); break;
		}
	} 
}

//管理员菜单 
void OwnerMeau(AdjMatrix G){
	int c;
	int x;
	int dist[MAXVEX];// 记录各条最短路径长度 
	int	path[MAXVEX][MAXVEX] = {0};//记录对应路径上的各顶点
    while(1){
		printf("\t\t\t1. 添加新地点\n");
		printf("\t\t\t2. 添加新路线\n");
		printf("\t\t\t3. 撤销旧路线\n");
		printf("\t\t\t4. 校园平面图\n");
		printf("\t\t\t5. 地点信息查询\n");
		printf("\t\t\t6. 问路查询\n");
		printf("\t\t\t0. 退出\n"); 
		printf("\t\t\t请选择您需要的操作:");
		scanf("%d",&c);
		getchar();
	
		switch(c){
			case 1: system("cls"); AddPlace(&G); break;
			case 2: system("cls"); AddRoad(&G); break;
			case 3: system("cls"); DelRoad(&G); break;
			case 4: system("cls"); MapOutput(); break;
			case 5: system("cls"); IntroducePlace(G); break;
			case 6: system("cls"); SearchPath(G,dist,path); break;
			case 0: printf("\n\t\t\t\t\t\t\t"); exit(0);
			default: printf("\n———————————————输入信息错误,请重新输入!!!————————————————————\n"); break;	
    	}
	}
}

//管理员登录 
void Owner(){
	int x;
	int dist[MAXVEX];// 记录各条最短路径长度 
	int	path[MAXVEX][MAXVEX] = {0};//记录对应路径上的各顶点
	AdjMatrix G;   //领接矩阵 
	MapFile(G); 
	IntroduceFile(G);  //向文件中存入介绍信息 
	AdjFile(G);    //向文件中存入邻接矩阵信息 
	Create(&G);
	
	system("cls");
	printf("\t\t1.登陆\t\t\t2.注册并登录\n");
    printf("请选择序号：");
    
    while(1){
		scanf("%d",&x);
		switch(x){
		 	case 1:  land(); OwnerMeau(G); break;
			case 2:  enroll(); land(); OwnerMeau(G); break;
			default: printf( "\n———————————————输入无效,请重新正确输入!!!————————————————————\n"); break;
		}		
	} 
}

//主函数 
int main(){
 	int flag;
	
    printf("——————————————————————————————————————————————\n"); 
    printf("\n\t\t\t欢迎使用西安邮电大学长安校区导游系统 !\n\n");   
    printf("——————————————————————————————————————————————\n");
	printf("\n\n\n"); 
	
	//游客or管理员 
	printf("\t\t1.我是游客\t\t\t2.我是管理员\n");
	printf("请选择序号：");
	
	while(1){
		scanf("%d",&flag);
		switch(flag){
		 	case 1: system("cls"); User(); break;
			case 2: system("cls"); Owner(); break;
			default: printf("\n———————————————输入无效,请重新正确输入!!!————————————————————\n"); break;
		}
	} 
}
