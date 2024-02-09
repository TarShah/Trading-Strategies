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
vector<double> macd;
vector<double> sig;
vector<double> short_ewm;
vector<double> long_ewm;
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
void get_short_ewm(int idx){
	int start = idx - 11;
	double ans = price[start];
	double alph = (double)2/(double)13;
	for(start;start<=idx;start++){
		ans = alph*(price[start] - ans) + ans;
	}
	short_ewm[idx] = ans; 
}
void get_long_ewm(int idx){
	int start = idx - 25;
	double ans = price[start];
	double alph = (double)2/(double)27;
	for(start;start<=idx;start++){
		ans = alph*(price[start] - ans) + ans;
	}
	long_ewm[idx] = ans;
}
void get_macd(int idx){
	get_short_ewm(idx);
	get_long_ewm(idx);
	macd[idx] = short_ewm[idx] - long_ewm[idx]; 
}
void get_sig(int idx){
	int start = idx - 8;
	double ans = macd[start];
	double alph = (double)2/(double)10;
	for(start;start<=idx;start++){
		ans = alph*(macd[start] - ans) + ans;
	}
	sig[idx] = ans;
}
void solve(int argc,char* argv[]){
	get_close(string(argv[1]));
	get_dates(string(argv[1]));
	sig.resize(len(dates),0);
	macd.resize(len(dates),0);
	short_ewm.resize(len(dates),0);
	long_ewm.resize(len(dates),0);
	reverse(all(dates));
	reverse(all(price));
	int x = stoi(argv[2]);
	string start_date = argv[3];
	int start = 0;
	for(int i=0;i<len(dates);i++){
		if(dates[i] == start_date){
			start = i;
			break;
		}
	}
	for(int i = start-20;i<start;i++) get_macd(i);
	ll position = 0;
	double cash = 0;
	ofstream order("order_statistics.csv");
	ofstream cashflow("daily_cashflow.csv");
	// Signal for writing into the daily_cashflow file
	for(start;start<len(dates);start++){
		get_macd(start);
		get_sig(start);
		if(macd[start] > sig[start]){
			if(position < x){
				position++;
				cash -= price[start];
				order<<dates[start]<<","<<"BUY"<<","<<1<<","<<price[start]<<endl;
			}
		}
		if(macd[start] < sig[start]){
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
