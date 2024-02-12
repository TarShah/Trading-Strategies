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
	int n = stoi(argv[2]);
	int x = stoi(argv[3]);
	double oversold_threshold = stod(argv[4]);
	double overbought_threshold = stod(argv[5]);
	string start_date = argv[6];
	string name = string(argv[7]);
	int start = 0;
	for(int i=0;i<len(dates);i++){
		if(comparable(dates[i]) >= comparable(start_date)){
			start = i;
			break;
		}
	}
	ll position = 0;
	ll prev = start - (n-1);
	double cash = 0;
	double gain = 0;
	double loss = 0;
	for(int i = start-(n-1);i<start;i++){
		gain += max((double)0,price[i] - price[i-1]);
		loss += min((double)0,price[i] - price[i-1]);
	}
	ofstream order(name+"order_statistics.csv");
	ofstream cashflow(name+"daily_cashflow.csv");
	order<<"Date,Order_dir,Quantity,Price"<<endl;
	cashflow<<"Date,Cashflow"<<endl;
	for(start;start<len(dates);start++){
		gain += max((double)0,price[start] - price[start-1]);
		loss += min((double)0,price[start] - price[start-1]);
		double RSI;
		if(loss == 0) RSI = 100;
		else RSI = 100 - (100/(1 + (abs(gain)/abs(loss)))); 
		if(RSI < oversold_threshold){
			if(position < x){
				position++;
				cash -= price[start];
				order<<dates[start]<<","<<"BUY"<<","<<1<<","<<to_string(price[start])<<endl;
			}
		}
		if(RSI > overbought_threshold){
			if(position > -x){
				position--;
				cash += price[start];
				order<<dates[start]<<","<<"SELL"<<","<<1<<","<<to_string(price[start])<<endl;
			}
		}
		//cout<<gain<<" "<<loss<<" "<<RSI<<endl;
		gain -= max((double)0,price[prev] - price[prev-1]);
		loss -= min((double)0,price[prev] - price[prev-1]);
		prev++;
		cashflow<<dates[start]<<','<<to_string(cash)<<endl;
	}
	// Squaring off 
	cash += price[len(price)-1]*position;
	order.close();
	cashflow.close();
	// Writing the final pnl
	ofstream pnl(name+"final_pnl.txt");
	pnl<<to_string(cash)<<endl;
	pnl.close();
}
int main(int argc, char*  argv[]){
	solve(argc, argv);
	
	return 0;
}
