import sys
from datetime import datetime, timedelta
from jugaad_data.nse import stock_df

symb = sys.argv[1]
sdt = datetime.strptime(sys.argv[2], "%d/%m/%Y") - timedelta(days=10)
edt = datetime.strptime(sys.argv[3], "%d/%m/%Y")
name = sys.argv[4]
df = stock_df(
    symbol=symb,
    from_date=sdt,
    to_date=edt,
    series="EQ",
)
cols = ["DATE", "CLOSE", "OPEN", "VWAP", "LOW", "HIGH", "NO OF TRADES"]
new_df = df[cols].reset_index(drop=True)

csv_name = name + ".csv"
new_df.to_csv(csv_name, index=False)
print("File Created")
