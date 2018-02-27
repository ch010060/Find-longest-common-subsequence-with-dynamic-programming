#include <cstdlib>
#include <iostream>
//#include <string>  //VC 2010
#include <string.h> //gcc 6.3
#include <vector>
#include <algorithm>    // std::sort
#define MaxTestLen 100
#define MaxLCS 200000

using namespace std;

	char string1[MaxTestLen+1]; //字串1
	char string2[MaxTestLen+1]; //字串2
	int table[MaxTestLen+1][MaxTestLen+1] = {{0}}; //DP記錄用表格
	int trace[MaxTestLen+1][MaxTestLen+1] = {{-1}};	//記錄該元素結果的來源方向 0:↖ 1:← 2:↑
	int len_LCS = 0; //LCS的長度
	int LCS_count = 0; //所有LCS的個數
	char Result[MaxTestLen+1]; //當前的LCS結果
	char Print[MaxLCS][MaxTestLen+1]; //存進所有的LCS


void LCS(int i,int j,char Result[],int currlen,char **lookup,int free_flag)
{

	if(currlen == 0) //字串收集到LCS長度就能提早結束了，節省運算
	{	
//如果不需對所有LCS作sort，直接印出即可
			if(free_flag == 1)
			{
			//Free each sub-array
				for(int a = 0; a < i+1; ++a) 
					delete[] lookup[a];   
			//Free the array of pointers
				delete[] lookup;
				free_flag = 0;
			}

		int index;
		for(index=0;index<len_LCS;index++)
			Print[LCS_count][index] = Result[index];

		LCS_count++;
		return;
	}
	else
	{
		if(lookup[i][j]=='1') return; //如果之前走過就提早結束，因為是多餘的走訪
		lookup[i][j] = '1'; //該點仍未走過，把該點記錄成已走過

		if(trace[i][j] == 0) //方向↖
		{
			Result[currlen-1] = string1[i];//記錄matching character

			
			/*在每個matching character的位置建立新的lookup table
			同一個位置分支出去所有路線共用這張新的表
			直到走到下一個matching character才放棄這張表*/
			char** lookup = new char*[i+1];
			for(int a = 0; a < i+1; ++a)
				lookup[a] = new char[j+1];

			//int (*lookup)[MaxTestLen+1] = new int[MaxTestLen+1][MaxTestLen+1];
			for(int a=0;a<i+1;a++)
				for(int b=0;b<j+1;b++)
					lookup[a][b] = 0;

			free_flag = 1;

			LCS(i-1,j-1,Result,currlen-1,lookup,free_flag);
		}
		else if(trace[i][j] == 1) //方向←
			LCS(i,j-1,Result,currlen,lookup,free_flag);
		else if(trace[i][j] == 2)  //方向↑
			LCS(i-1,j,Result,currlen,lookup,free_flag);
		else //(trace[i][j] == 3) //方向←及↑
		{
			LCS(i,j-1,Result,currlen,lookup,free_flag);
			LCS(i-1,j,Result,currlen,lookup,free_flag);
		}

		/*考慮matching點的來源可能是方向←或↑，不一定必走↖，是整個演算法的關鍵!!!
		不擺在上面的if(trace[i][j] == 0)裡面是因為擺這裡才能讓兩種LCS共用到同一個lookup table
		不然同一個LCS會被重複計算到*/
		if(trace[i][j] == 0)
		{
			if(table[i-1][j] == table[i][j])
				LCS(i-1,j,Result,currlen,lookup,free_flag);
			if(table[i][j-1] == table[i][j])
				LCS(i,j-1,Result,currlen,lookup,free_flag);
			
		}
	}
}

int LCS_length(char str1[],char str2[],int len1,int len2)
{
	int i,j;
	for(i=1;i<=len1;i++)
		for(j=1;j<=len2;j++)
			if(string1[i] == string2[j])
			{
				table[i][j] = table[i-1][j-1] + 1; 
				trace[i][j] = 0; //方向↖
			}
			else
			{
				//只找一條LCS時使用的方法
				//table[i][j] = max(table[i][j-1],table[i-1][j]);
				
				if(table[i][j-1] == table[i-1][j]) //優先判斷是否有雙重方向
				{
					table[i][j] = table[i][j-1];
					trace[i][j] = 3; //方向←及↑
				}
				else if(table[i][j-1] > table[i-1][j]) //如果字串1比字串2大(比較ASCII code)
				{					
					table[i][j] = table[i][j-1];
					trace[i][j] = 1; //方向←
				}
				else if (table[i][j-1] < table[i-1][j])
				{
					table[i][j] = table[i-1][j];
					trace[i][j] = 2; //方向↑
				}
			}
		return table[len1][len2];
}

int main(){

	//cin>>string1>>string2;
//測資#1 (5 10)
	sscanf("QHCCDETMFPPWZGIFEFUA", "%s", string1);
	sscanf("GZXPAWWOOCZEMBIPHECM", "%s", string2);
//測資#2 (6 8)
	//sscanf("PUWBVNUCUUESSJUNSVHE", "%s", string1);
	//sscanf("CIHSIIDWCSUTOSWJHTOB", "%s", string2);
//測資#3 (6 6)
	//sscanf("DSCAISAGOODCOURSE", "%s", string1);
	//sscanf("ACGDSACADER", "%s", string2);
//測資#4 (9 6480)
	//sscanf("AAAAADDDDFFCGE", "%s", string1);
	//sscanf("AADDFFFFCCCGGGEE", "%s", string2);
//測資#5 (28 1064)
	//sscanf("VHYREVXQUELESSDWEIULKUXEEEGLMXJGOVYSIVWRAKSTPJLMHKKSPODPBCOZXEWJTEVJCTYVKVJOGAPQYQJVROQLCRDMBWJVIYJG", "%s", string1);
	//sscanf("DBSNGYNOMLTBOYOBWEMZIDVNRUENYWIYNLYVMRHQJEZZWTSDPQUSHZNMEOKQMUQNYZSANLKXDPWLIDHTLJFIJHHYWQHVWRGTPZIO", "%s", string2);
//測資#6 (4 4)
	//sscanf("BDCABA", "%s", string1);
	//sscanf("ABCBDAB", "%s", string2);

	int len1 = strlen(string1);
	int len2 = strlen(string2);
	int i,j;

	int free_flag = 0;

			char** lookup = new char*[len1+1];
			for(int i = 0; i < len1+1; ++i)
				lookup[i] = new char[len2+1];

			for(int a=0;a<len1+1;a++)
				for(int b=0;b<len2+1;b++)
					lookup[a][b] = 0;

//平移array一位，在array[0]塞0，字串從array[1]開始存
	for(i=len1-1;i>=0;i--)
		string1[i+1] = string1[i];
	string1[0] = '0';
	for(j=len2-1;j>=0;j--)
		string2[j+1] = string2[j];
	string2[0] = '0';
//開始畫DP表格，算出LCS的長度
	len_LCS = LCS_length(string1,string2,len1,len2);
//根據DP表格和LCS的長度找出所有的LCS(重複但不同位置的LCS也要算)
	LCS(len1,len2,Result,len_LCS,lookup,free_flag);

	char Array[MaxTestLen+1] = {0} ;
	string tempString = string(Array);
	vector<string> v;
	for(i=0;i<LCS_count;i++)
	{
		for(j=0;j<len_LCS;j++)
		{
			Array[j] = Print[i][j];
		}
		//cout << tempString <<endl;
		v.push_back(string(Array));
	}
	sort(v.begin(), v.end());

//印出 LCS的長度 和 所有LCS的個數
	cout<<len_LCS<<" "<<LCS_count<<endl; 
//印出最後結果
/* 沒排序
	int index;
	for(i=0;i<LCS_count;i++)
	{
		for(index=0;index<len_LCS;index++)
			cout<<Print[i][index];
		cout<<endl;
	}
*/
//有排序
	vector<string>::iterator iter;
	for(iter = v.begin() ; iter != v.end() ; ++iter)
	{
		cout<<*iter<<endl;
	}

	//Free each sub-array
	for(int a = 0; a < len1+1; ++a) 
		delete[] lookup[a];   
	//Free the array of pointers
	delete[] lookup;

	system("pause");
	return 0;
}
