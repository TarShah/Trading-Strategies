# Default target
.PHONY: all

# Run app.py with specified parameters based on strategy
all:
	@if [ "$(strategy)" = "BASIC" ]; then \
		python3 get.py $(symbol) "$(start_date)" "$(end_date)"  $(shell echo $$(($(n)*2 + 10))); \
		g++ BASIC.cpp -o BASIC; \
		./BASIC $(symbol) $(n) $(x) "$(start_date)"; \
	elif [ "$(strategy)" = "DMA" ]; then \
		python3 get.py $(symbol) "$(start_date)" "$(end_date)" $(shell echo $$(($(n)*2 + 10))); \
		g++ DMA.cpp -o DMA; \
		./DMA $(symbol) $(n) $(x) $(p) "$(start_date)"; \
	elif [ "$(strategy)" = "DMA++" ]; then \
		python3 get.py $(symbol) "$(start_date)" "$(end_date)" $(shell echo $$(($(n)*2 + 10))); \
		g++ "DMA++.cpp" -o "DMA++"; \
		./"DMA++" $(symbol) $(n) $(x) $(p) $(max_hold_days) $(c1) $(c2) "$(start_date)"; \
	elif [ "$(strategy)" = "MACD" ]; then \
		python3 get.py $(symbol) "$(start_date)" "$(end_date)" 100; \
		g++ MACD.cpp -o MACD; \
		./MACD $(symbol) $(x) "$(start_date)"; \
	elif [ "$(strategy)" = "RSI" ]; then \
		python3 get.py $(symbol) "$(start_date)" "$(end_date)" $(shell echo $$(($(n)*2 + 10))); \
		g++ RSI.cpp -o RSI; \
		./RSI $(symbol) $(n) $(x) $(oversold_threshold) $(overbought_threshold) "$(start_date)"; \
	elif [ "$(strategy)" = "ADX" ]; then \
		python3 get.py $(symbol) "$(start_date)" "$(end_date)" $(shell echo $$(($(n)*3 + 10))); \
		g++ ADX.cpp -o ADX; \
		./ADX $(symbol) $(n) $(x) $(adx_threshold) "$(start_date)"; \
	elif [ "$(strategy)" = "LINEAR_REGRESSION" ]; then \
		python3 linear.py $(symbol) "$(train_start_date)" "$(train_end_date)" "train"; \
		python3 linear.py $(symbol) "$(start_date)" "$(end_date)" $(symbol); \
		g++ LINEAR_REGRESSION.cpp -o LINEAR_REGRESSION; \
		./LINEAR_REGRESSION $(symbol) $(x) $(p) "$(train_start_date)" "$(start_date)"; \
	elif [ "$(strategy)" = "PAIRS" ]; then \
		python3 get.py $(symbol1) "$(start_date)" "$(end_date)" $(shell echo $$(($(n)*3 + 10))); \
		python3 get.py $(symbol2) "$(start_date)" "$(end_date)" $(shell echo $$(($(n)*3 + 10))); \
		g++ PAIRS.cpp -o PAIRS; \
		./PAIRS $(symbol1) $(symbol2) "$(start_date)" "$(end_date)" $(n) $(x) $(threshold) $(stop_loss_threshold) ; \
	elif [ "$(strategy)" = "BEST_OF_ALL" ]; then \
		g++ BEST_OF_ALL.cpp -o BEST_OF_ALL; \
		./BEST_OF_ALL $(symbol) "$(start_date)" "$(end_date)"; \
	fi

# Clean rule
clean:
	rm -f $(symbol).csv
