# Default target
.PHONY: all

# Run app.py with specified parameters based on strategy
all:
	python3 get.py $(symbol) "$(start_date)" "$(end_date)" $(n); 
	@if [ "$(strategy)" = "BASIC" ]; then \
		g++ BASIC.cpp -o BASIC; \
		./BASIC $(symbol) $(n) $(x) "$(start_date)"; \
	elif [ "$(strategy)" = "DMA" ]; then \
		 g++ DMA.cpp -o DMA;  \
		 ./DMA $(symbol) $(n) $(x) $(p) "$(start_date)"; \
	elif [ "$(strategy)" = "DMA++" ]; then \
		g++ DMA++.cpp -o DMA++; \
		./DMA++ $(symbol) $(n) $(x) $(p) $(max_hold_days) $(c1) $(c2) "$(start_date)"; \
	elif [ "$(strategy)" = "MACD" ]; then \
		g++ MACD.cpp -o MACD; \
		./MACD $(symbol) $(x) "$(start_date)"; \
	fi
# Clean rule
clean:
	rm -f $(symbol).csv
