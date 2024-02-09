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
void solve(int argc,char* argv[]){
	get_close(string(argv[1]));
	get_dates(string(argv[1]));
	reverse(all(dates));
	reverse(all(price));
	int n = stoi(argv[2]);
	int x = stoi(argv[3]);
	string start_date = argv[4];
	int start = 0;
	for(int i=0;i<len(dates);i++){
		if(dates[i] == start_date){
			start = i;
			break;
		}
	}
	ll position = 0;
	ll trend = 0;
	double cash = 0;
	for(int i = start - (n-1);i<start;i++){
		if(price[i] > price[i-1]){
			trend = max(trend,0LL);
			trend++;
		}
		else if(price[i] < price[i-1]){
			trend = min(trend,0LL);
			trend--;
		}
		else if(price[i] == price[i-1]){
			trend = 0;
		}
	}
	ofstream order("order_statistics.csv");
	ofstream cashflow("daily_cashflow.csv");
	// Signal for writing into the daily_cashflow file
	bool sig = false;
	for(start;start<len(dates);start++){
		// Checking the trend
		if(price[start] > price[start-1]){
			trend = max(trend,0LL);
			trend++;
		}
		else if(price[start] < price[start-1]){
			trend = min(trend,0LL);
			trend--;
		}
		else if(price[start] == price[start-1]){
			trend = 0;
		}
		// Performing required actions 
		if(trend >= n){
			if(position < x){
				position++;
				cash -= price[start];
				order<<dates[start]<<","<<"BUY"<<","<<1<<","<<price[start]<<endl;
			}
		}
		if(trend <= -n){
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
