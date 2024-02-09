import sys
from datetime import datetime
from jugaad_data.nse import stock_df

#to change year to current year -1 
def convert_to_earlier_year(date_str):
    date_obj = datetime.strptime(date_str, '%d/%m/%Y')
    earlier_year_date = date_obj - timedelta(days=366)  # Not perfectly accurate but a simple approximation
    return earlier_year_date.strftime('%d/%m/%Y')

symb = sys.argv[1]
sdt = convert_to_earlier_year(sys.argv[2])
edt = sys.argv[3]
df = stock_df(symbol=symb, from_date=datetime.strptime(sdt,"%d/%m/%Y"),to_date=datetime.strptime(edt,"%d/%m/%Y"), series="EQ",)
cols = ["DATE","OPEN","CLOSE","HIGH","LOW","LTP","VOLUME","VALUE","NO OF TRADES"]
new_df = df[cols].reset_index(drop=True)

csv_name = symb + ".csv"
new_df.to_csv(csv_name,index=False)
print("File Created")

