# Default target
.PHONY: all

# Run app.py with specified parameters based on strategy
all:
	@if [ "$(strategy)" = "BASIC" ]; then \
		python3 get.py $(symbol) "$(start_date)" "$(end_date)" $(n); \
		g++ BASIC.cpp -o BASIC; \
		./BASIC $(symbol) $(n) $(x) "$(start_date)"; \
	elif [ "$(strategy)" = "DMA" ]; then \
		python3 get.py $(symbol) "$(start_date)" "$(end_date)" $(n); \
		g++ DMA.cpp -o DMA; \
		./DMA $(symbol) $(n) $(x) $(p) "$(start_date)"; \
	elif [ "$(strategy)" = "DMA++" ]; then \
		python3 get.py $(symbol) "$(start_date)" "$(end_date)" $(n); \
		g++ "DMA++.cpp" -o "DMA++"; \
		./"DMA++" $(symbol) $(n) $(x) $(p) $(max_hold_days) $(c1) $(c2) "$(start_date)"; \
	elif [ "$(strategy)" = "MACD" ]; then \
		python3 get.py $(symbol) "$(start_date)" "$(end_date)" 30; \
		g++ MACD.cpp -o MACD; \
		./MACD $(symbol) $(x) "$(start_date)"; \
	elif [ "$(strategy)" = "RSI" ]; then \
		python3 get.py $(symbol) "$(start_date)" "$(end_date)" $(n); \
		g++ RSI.cpp -o RSI; \
		./RSI $(symbol) $(n) $(x) $(oversold_threshold) $(overbought_threshold) "$(start_date)"; \
	fi

# Clean rule
clean:
	rm -f $(symbol).csv
