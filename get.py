import sys
from datetime import datetime, timedelta
from jugaad_data.nse import stock_df

symb = sys.argv[1]
sdt = datetime.strptime(sys.argv[2], "%d/%m/%Y")
edt = datetime.strptime(sys.argv[3], "%d/%m/%Y")
n = int(sys.argv[4])

new_start_date = sdt - timedelta(days=n)
df = stock_df(
    symbol=symb,
    from_date=new_start_date,
    to_date=edt,
    series="EQ",
)
cols = ["DATE", "OPEN", "CLOSE", "HIGH", "LOW", "LTP", "VOLUME", "VALUE", "NO OF TRADES"]
new_df = df[cols].reset_index(drop=True)

csv_name = symb + ".csv"
new_df.to_csv(csv_name, index=False)
print("File Created")
