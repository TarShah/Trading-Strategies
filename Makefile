# Default target
.PHONY: all

# Run app.py with specified parameters based on strategy
all:
	python3 get.py $(symbol) "$(start_date)" "$(end_date)"; 
	@if [ "$(strategy)" = "BASIC" ]; then \
		g++ BASIC.cpp -o BASIC; \
		./BASIC $(symbol) $(n) $(x); \
	elif [ "$(strategy)" = "DMA" ]; then \
		 g++ DMA.cpp -o DMA;  \
		 ./DMA $(symbol) $(n) $(x) $(p); \
	elif [ "$(strategy)" = "DMA++" ]; then \
		g++ DMA++.cpp -o DMA++; \
		./DMA++ $(symbol) $(x) $(p) $(n) $(max_hold_days) $(c1) $(c2) ; \
	elif [ "$(strategy)" = "MACD" ]; then \
		g++ MACD.cpp -o MACD; \
		./MACD $(symbol) $(x); \
	fi
# Clean rule
clean:
	rm -f $(symbol).csv

