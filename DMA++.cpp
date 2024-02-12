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
double get_SF(double SF,double ER,double c1, double c2){
	double ratio = ((2*ER/(1+c2)) - 1) / ((2*ER/(1+c2)) + 1); 
	ratio -= SF;
	double new_SF = SF + c1*ratio;
	return new_SF;
}
struct ord{
	string type;
	int index;
};
void solve(int argc,char* argv[]){
	get_close(string(argv[1]));
	get_dates(string(argv[1]));
	reverse(all(dates));
	reverse(all(price));
	int n = stoi(argv[2]);
	int x = stoi(argv[3]);
	double p = stod(argv[4]);
	int max_hold_days = stod(argv[5]);
	double c1 = stod(argv[6]);
	double c2 = stod(argv[7]);
	string start_date = argv[8];
	int start = 0;
	
	
	string name = string(argv[9]);
	
	for(int i=0;i<len(dates);i++){
		if(comparable(dates[i]) >= comparable(start_date)){ // to change this.
			start = i;
			break;
		}
	}
	ll position = 0;
	ll prev = start - (n-2);
	double cash = 0;
	double ER;
	double SF = 0.5;
	double AMA = price[start];
	double sum_abs_change = 0;
	for(int i = start - (n-2);i<=start;i++){
		sum_abs_change += abs(price[i] - price[i-1]);
	}
	ofstream order(name+"order_statistics.csv");
	ofstream cashflow(name+"daily_cashflow.csv");
	order<<"Date,Order_dir,Quantity,Price"<<endl;
	cashflow<<"Date,Cashflow"<<endl;
	cashflow<<dates[start]<<','<<cash<<endl;
	queue<ord> q;
	start++;
	for(start;start<len(dates);start++){
		bool buy = false;
		bool sell = false;
		bool held = false;
		bool held_sell = false;
		bool held_buy = false; 
		double price_change = abs(price[start] - price[start-n]);
		sum_abs_change += abs(price[start] - price[start-1]);
		ER = price_change/sum_abs_change;
		SF = get_SF(SF,ER,c1,c2);
		AMA = AMA + SF*(price[start] - AMA);
		//cout<<dates[start]<<" "<<AMA<<" "<<ER<<" "<<SF<<endl;
		if(price[start] >= AMA*((double)(100 + p) / (double)100)) buy = true;
		if(price[start] <= AMA*((double)(100 - p) / (double)100)) sell = true;
		if(!q.empty() and start - q.front().index == max_hold_days){
			held = true;
			if(q.front().type == "SELL") held_sell = true;
			if(q.front().type == "BUY") held_buy = true;
		}
		if(buy){
			if(held_sell){
				if(position == x){
					q.pop();
					cash += price[start];
					position--;
					order<<dates[start]<<","<<"SELL"<<","<<1<<","<<to_string(price[start])<<endl;
				}
				else{
					q.pop();
					q.push(ord{"SELL",start});
				}
			}
			else{
				if(position < x){
					position++;
					cash -= price[start];
					order<<dates[start]<<","<<"BUY"<<","<<1<<","<<to_string(price[start])<<endl;
					if(q.empty() or q.front().type=="SELL"){
						q.push(ord{"SELL",start});
					}
					else{
						q.pop();
					}
				}
			}
		}
		else if(sell){
			if(held_buy){
				if(position == -x){
					q.pop();
					cash -= price[start];
					position++;
					order<<dates[start]<<","<<"BUY"<<","<<1<<","<<to_string(price[start])<<endl;
				}
				else{
					q.pop();
					q.push(ord{"BUY",start});
				}
			}
			else{
				if(position > -x){
					position--;
					cash += price[start];
					order<<dates[start]<<","<<"SELL"<<","<<1<<","<<to_string(price[start])<<endl;
					if(q.empty() or q.front().type=="BUY"){
						q.push(ord{"BUY",start});
					}
					else{
						q.pop();
					}
				}
			}
		}
		else if(held){
			if(held_buy){
				q.pop();
				cash -= price[start];
				position++;
				order<<dates[start]<<","<<"BUY"<<","<<1<<","<<to_string(price[start])<<endl;
			}
			if(held_sell){
				q.pop();
				cash += price[start];
				position--;
				order<<dates[start]<<","<<"SELL"<<","<<1<<","<<to_string(price[start])<<endl;
			}
		}
		sum_abs_change -= abs(price[prev] - price[prev-1]);
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
