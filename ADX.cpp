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
		if(comparable(dates[i]) >= comparable(start_date)){
			start = i;
			break;
		}
	}
	ll position = 0;
	double cash = 0;
	double alph  = (double)2/(double)(n+1); 
	ofstream order("order_statistics.csv");
	ofstream cashflow("daily_cashflow.csv");
	order<<"Date,Order_dir,Quantity,Price"<<endl;
	cashflow<<"Date,Cashflow"<<endl;
	double TR = max({high[start]-low[start],abs(high[start]-price[start]),abs(low[start]-price[start])});
	double DM_plus = max((double)0,high[start]-high[start-1]);
	double DM_minus = max((double)0,low[start]-low[start-1]);
	double ATR = TR;
	double DI_plus = DM_plus/ATR;
	double DI_minus = DM_minus/ATR;
	double DX = ((DI_plus - DI_minus) / (DI_plus + DI_minus))*(double)100;
	double ADX = DX;
	for(start;start<len(dates);start++){
		TR = max({high[start]-low[start],abs(high[start]-price[start-1]),abs(low[start]-price[start-1])});
		DM_plus = max((double)0,high[start]-high[start-1]);
		DM_minus = max((double)0,low[start]-low[start-1]);
		ATR = alph*(TR - ATR) + ATR;
		DI_plus = alph*((DM_plus/ATR) - DI_plus) + DI_plus;
		DI_minus = alph*((DM_minus/ATR) - DI_minus) + DI_minus; 
		DX = ((DI_plus - DI_minus) / (DI_plus + DI_minus))*(double)100;
		ADX = alph*(DX - ADX) + ADX;
		if(ADX > adx_threshold){
			if(position < x){
				position++;
				cash -= price[start];
				order<<dates[start]<<","<<"BUY"<<","<<1<<","<<to_string(price[start])<<endl;
			}
		}
		if(ADX < adx_threshold){
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
