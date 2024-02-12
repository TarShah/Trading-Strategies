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
string comparable(string date){
	string conv = "";
	conv += date.substr(6,4);
	conv += "-"; 
	conv += date.substr(3,2);
	conv += "-";
	conv += date.substr(0,2);
	return conv;
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
	int x = stoi(argv[2]);
	string start_date = argv[3];
	int start = 0;
	for(int i=0;i<len(dates);i++){
		if(comparable(dates[i]) >= comparable(start_date)){
			start = i;
			break;
		}
	}
	double sig = 0;
	double macd = 0;
	double short_ewm = price[start];
	double long_ewm = price[start];
	ll position = 0;
	double cash = 0;
	ofstream order("order_statistics.csv");
	ofstream cashflow("daily_cashflow.csv");
	order<<"Date,Order_dir,Quantity,Price"<<endl;
	cashflow<<"Date,Cashflow"<<endl;
	for(start;start<len(dates);start++){
		long_ewm = ((double)2/(double)27)*(price[start] - long_ewm) + long_ewm;
		short_ewm = ((double)2/(double)13)*(price[start] - short_ewm) + short_ewm;
		macd = short_ewm - long_ewm;
		sig = ((double)2/(double)10)*(macd - sig) + sig;  
		if(macd > sig){
			if(position < x){
				position++;
				cash -= price[start];
				order<<dates[start]<<","<<"BUY"<<","<<1<<","<<to_string(price[start])<<endl;
			}
		}
		if(macd < sig){
			if(position > -x){
				position--;
				cash += price[start];
				order<<dates[start]<<","<<"SELL"<<","<<1<<","<<to_string(price[start])<<endl;
			}
		}
		cashflow<<dates[start]<<','<<to_string(cash)<<endl;
	}
	// Squaring off 
	cash += price[len(price)-1]*position;
	order.close();
	cashflow.close();
	// Writing the final pnl
	ofstream pnl("final_pnl.txt");
	pnl<<to_string(cash)<<endl;
	pnl.close();
}
int main(int argc, char*  argv[]){
	solve(argc, argv);
	
	return 0;
}
