#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define pb push_back
#define len(v) (int)v.size() 
#define all(v) v.begin(),v.end()
#define rall(v) v.rbegin(),v.rend()
#ifdef Raptor
#include "debug.h"
#else
#define debug(...) 1
#define imie(...) 1;
#endif
vector<string> dates;
vector<double> price;
vector<double> high;
vector<double> low;
// Indicator Variables 
vector<double> TR;
vector<double> ATR;
vector<double> DX;
vector<double> ADX;
vector<double> DM_plus;
vector<double> DM_minus;
vector<double> DI_plus;
vector<double> DI_minus;
string change_date_rep(string date){
	string new_date = "";
	new_date += date.substr(8,2);
	new_date += "/";
	new_date += date.substr(5,2);
	new_date += "/";
	new_date += date.substr(0,4); 
	return new_date;
}
void get_dates(string symb){
	string line;
    ifstream file(symb + ".csv"); 
    int targetColumnIndex = -1;
    string targetColumnName = "DATE"; 
	while(std::getline(file, line)){ 
		if(targetColumnIndex == -1){
			stringstream ss(line);
			string columnName;
			int columnIndex = 0;
			while(std::getline(ss, columnName, ',')){
				if(columnName == targetColumnName){
					targetColumnIndex = columnIndex;
					break;
				}
				columnIndex++;
			}
			assert(targetColumnIndex != -1);
        }
        else{
			stringstream ss(line);
			string cell;
			int currentColumn = 0;
			while(std::getline(ss, cell, ',')){ 
				if(currentColumn == targetColumnIndex){  
					dates.push_back(change_date_rep(cell)); 
					break; 
				}
				currentColumn++;
			}
		}
	}
}
void get_close(string symb){ 
    string line;
    ifstream file(symb + ".csv"); 
    int targetColumnIndex = -1;
    string targetColumnName = "CLOSE"; 
	while(std::getline(file, line)){ 
		if(targetColumnIndex == -1){
			stringstream ss(line);
			string columnName;
			int columnIndex = 0;
			while(std::getline(ss, columnName, ',')){
				if(columnName == targetColumnName){
					targetColumnIndex = columnIndex;
					break;
				}
				columnIndex++;
			}
			assert(targetColumnIndex != -1);
        }
        else{
			stringstream ss(line);
			string cell;
			int currentColumn = 0;
			while(std::getline(ss, cell, ',')){ 
				if(currentColumn == targetColumnIndex){  
					price.push_back(stod(cell)); 
					break; 
				}
				currentColumn++;
			}
		}
	}
}
void get_high(string symb){ 
    string line;
    ifstream file(symb + ".csv"); 
    int targetColumnIndex = -1;
    string targetColumnName = "HIGH"; 
	while(std::getline(file, line)){ 
		if(targetColumnIndex == -1){
			stringstream ss(line);
			string columnName;
			int columnIndex = 0;
			while(std::getline(ss, columnName, ',')){
				if(columnName == targetColumnName){
					targetColumnIndex = columnIndex;
					break;
				}
				columnIndex++;
			}
			assert(targetColumnIndex != -1);
        }
        else{
			stringstream ss(line);
			string cell;
			int currentColumn = 0;
			while(std::getline(ss, cell, ',')){ 
				if(currentColumn == targetColumnIndex){  
					high.push_back(stod(cell)); 
					break; 
				}
				currentColumn++;
			}
		}
	}
}
void get_low(string symb){ 
    string line;
    ifstream file(symb + ".csv"); 
    int targetColumnIndex = -1;
    string targetColumnName = "LOW"; 
	while(std::getline(file, line)){ 
		if(targetColumnIndex == -1){
			stringstream ss(line);
			string columnName;
			int columnIndex = 0;
			while(std::getline(ss, columnName, ',')){
				if(columnName == targetColumnName){
					targetColumnIndex = columnIndex;
					break;
				}
				columnIndex++;
			}
			assert(targetColumnIndex != -1);
        }
        else{
			stringstream ss(line);
			string cell;
			int currentColumn = 0;
			while(std::getline(ss, cell, ',')){ 
				if(currentColumn == targetColumnIndex){  
					low.push_back(stod(cell)); 
					break; 
				}
				currentColumn++;
			}
		}
	}
}
void get_TR(int idx){
	TR[idx] = max({high[idx]-low[idx],abs(high[idx]-price[idx-1]),abs(low[idx]-price[idx-1])});
}
void get_DM(int idx){
	DM_plus[idx] = max((double)0,high[idx]-high[idx-1]); 
	DM_minus[idx] = max((double)0,low[idx]-low[idx-1]);
}
void get_ATR(int idx, int n){
	int start = idx - (n-1);
	double alph  = (double)2/(double)(n+1); 
	double ans = TR[start];
	for(start;start<=idx;start++){
		ans = alph*(TR[start] - ans) + ans;
	}
	ATR[idx] = ans; 
}
void get_DI(int idx, int n){
	int start = idx - (n-1);
	double alph  = (double)2/(double)(n+1); 
	double ans1 = (DM_plus[start]/ATR[start]);
	double ans2 = (DM_minus[start]/ATR[start]); 
	for(start;start<=idx;start++){
		ans1 = alph*((DM_plus[start]/ATR[start]) - ans1) + ans1;
		ans2 = alph*((DM_minus[start]/ATR[start]) - ans2) + ans2;
	}
	DI_plus[idx] = ans1;
	DI_minus[idx] = ans2; 
}
void get_DX(int idx){
	DX[idx] = ((DI_plus[idx] - DI_minus[idx])/(DI_plus[idx] + DI_minus[idx]))*100; 
}
void get_ADX(int idx, int n){
	int start = idx - (n-1);
	double alph  = (double)2/(double)(n+1); 
	double ans = DX[start];
	for(start;start<=idx;start++){
		ans = alph*(DX[start] - ans) + ans;
	}
	ATR[idx] = ans; 
}
void solve(int argc,char* argv[]){
	get_close(string(argv[1]));
	get_dates(string(argv[1]));
	get_high(string(argv[1]));
	get_low(string(argv[1]));
	reverse(all(dates));
	reverse(all(price));
	reverse(all(high));
	reverse(all(low));
	int n = stoi(argv[2]);
	int x = stoi(argv[3]);
	double adx_threshold = stod(argv[4]); 
	string start_date = argv[5];
	int start = 0;
	for(int i=0;i<len(dates);i++){
		if(dates[i] == start_date){
			start = i;
			break;
		}
	}
	ll position = 0;
	double cash = 0;
	for(int i = 0;i<start;i++){
		get_TR(i);
		get_DM(i);
	}
	for(int i = n-1;i<start;i++){
		get_ATR(start,n);
	}
	for(int i = 2*(n-1);i<start;i++){
		get_DI(start,n);
		get_DX(start);
	}
	ofstream order("order_statistics.csv");
	ofstream cashflow("daily_cashflow.csv");
	for(start;start<len(dates);start++){
		get_TR(start);
		get_DM(start);
		get_ATR(start,n);
		get_DI(start,n);
		get_DX(start);
		get_ADX(start,n);
		if(ADX[start] > adx_threshold){
			if(position < x){
				position++;
				cash -= price[start];
				order<<dates[start]<<","<<"BUY"<<","<<1<<","<<price[start]<<endl;
			}
		}
		if(ADX[start] < adx_threshold){
			if(position > -x){
				position--;
				cash += price[start];
				order<<dates[start]<<","<<"SELL"<<","<<1<<","<<price[start]<<endl;
			}
		}
		cashflow<<dates[start]<<','<<cash<<endl;
	}
	// Squaring off 
	cash += price[len(price)-1]*position;
	order.close();
	cashflow.close();
	// Writing the final pnl
	ofstream pnl("final_pnl.txt");
	pnl<<cash<<endl;
	pnl.close();
}
int main(int argc, char*  argv[]){
	solve(argc, argv);
	
	return 0;
}
