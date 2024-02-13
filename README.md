# Trading Strategies Analysis and Comparison

In this document, we analyze and compare various trading strategies implemented for stock market analysis and decision-making.

## Strategies Overview

### 1. Basic Strategy
- Observes price series for the past `n` days.
- Buys 1 share if the price has been monotonically increasing for the last `n` days.
- Sells 1 share if the price has been monotonically decreasing for the last `n` days.

### 2. n-Day Moving Average (DMA)
- Extends basic strategy by using the average price of the last `n` days (DMA).
- Buys if current price > DMA by ≥ `p` standard deviations, sells if current price < DMA by ≥ `p` standard deviations.

### 3. Improved DMA with Stop-Loss and Adaptive Moving Average (AMA)
- Incorporates stop-loss and adaptive moving average.
- Stop-loss closes positions after max hold days if unable to close otherwise.
- Adaptive moving average (AMA) dynamically adjusts based on efficiency ratio.
- Buys when price > AMA by ≥ `p`%, sells when price < AMA by ≥ `p`%.

### 4. MACD (Moving Average Convergence Divergence)
- Uses exponential weighted means to calculate MACD and Signal Line.
- Buys if MACD > Signal, sells if MACD < Signal.

### 5. Relative Strength Index (RSI)
- Calculates RSI based on average gain and loss over the last `n` days.
- Generates buy signal if RSI crosses below oversold_threshold, sell signal if RSI crosses above overbought_threshold.

### 6. ADX (Average Directional Index)
- Calculates ADX based on true range, directional movements, and DX.
- Generates buy signal if ADX > adx_threshold, sell signal if ADX < adx_threshold.

### 7. Linear Regression for Stock Price Prediction
- Predicts stock price using linear regression.
- Buys if predicted price > actual price by ≥ `p`%, sells if predicted price < actual price by ≥ `p`%.

### 8. Mean-Reverting Pairs Trading Strategy
- Uses spread between prices of a pair of stocks.
- Buys spread if z-score < -threshold, sells spread if z-score > threshold.

### 9. Stop-Loss in Pairs Trading Strategy
- Implements loss-based stop-loss in pairs trading strategy.
- Closes position when z-score crosses stop_loss_threshold.
## Graphical Analysis
Each of the following images displays the performance of different trading strategies over time.

- **X-axis:** Time (in years from the corresponding title date)
- **Y-axis:** Final PNL (Profit and Loss) by each method, with parameters similar to the "best_of_all" strategy.

We have created different curves with varying end dates. During our analysis, we found that the step where we square off at the last trading date plays a significant role. This is motivated by the fact that in some cases in daily_cash_flow, the last entry was positive, but the value in final_pnl.txt was negative, thus implying that the last day's price can change profit to loss and loss to profit.

Note: In some curves, all lines become straight after a specific time. This occurs because the jugaad-data library is unable to provide data over that period.

- **SBIN**:

| 01-01-2019              | 01-01-2020              |
| ---------------------- | ---------------------- |
| ![im1](images/SBIN_01-01-2019.png) | ![im2](images/SBIN_01-01-2020.png) |


| 01-01-2021              | 01-01-2022              |
| ---------------------- | ---------------------- |
| ![im1](images/SBIN_01-01-2021.png) | ![im2](images/SBIN_01-01-2022.png) |


| 01-01-2023              | 01-01-2024              |
| ---------------------- | ---------------------- |
| ![im1](images/SBIN_01-01-2023.png) | ![im2](images/SBIN_01-01-2024.png) |

- **CIPLA**:

| 01-01-2019              | 01-01-2020              |
| ---------------------- | ---------------------- |
| ![im1](images/CIPLA_01-01-2019.png) | ![im2](images/CIPLA_01-01-2020.png) |


| 01-01-2021              | 01-01-2022              |
| ---------------------- | ---------------------- |
| ![im1](images/CIPLA_01-01-2021.png) | ![im2](images/CIPLA_01-01-2022.png) |


| 01-01-2023              | 01-01-2024              |
| ---------------------- | ---------------------- |
| ![im1](images/CIPLA_01-01-2023.png) | ![im2](images/CIPLA_01-01-2024.png) |

- **ITC**:

| 01-01-2019              | 01-01-2020              |
| ---------------------- | ---------------------- |
| ![im1](images/ITC_01-01-2019.png) | ![im2](images/ITC_01-01-2020.png) |


| 01-01-2021              | 01-01-2022              |
| ---------------------- | ---------------------- |
| ![im1](images/ITC_01-01-2021.png) | ![im2](images/ITC_01-01-2022.png) |


| 01-01-2023              | 01-01-2024              |
| ---------------------- | ---------------------- |
| ![im1](images/ITC_01-01-2023.png) | ![im2](images/ITC_01-01-2024.png) |

- **AXISBANK**:

| 01-01-2019              | 01-01-2020              |
| ---------------------- | ---------------------- |
| ![im1](images/AXISBANK_01-01-2019.png) | ![im2](images/AXISBANK_01-01-2020.png) |


| 01-01-2021              | 01-01-2022              |
| ---------------------- | ---------------------- |
| ![im1](images/AXISBANK_01-01-2021.png) | ![im2](images/AXISBANK_01-01-2022.png) |


| 01-01-2023              | 01-01-2024              |
| ---------------------- | ---------------------- |
| ![im1](images/AXISBANK_01-01-2023.png) | ![im2](images/AXISBANK_01-01-2024.png) |
- **UPL**:

| 01-01-2019              | 01-01-2020              |
| ---------------------- | ---------------------- |
| ![im1](images/UPL_01-01-2019.png) | ![im2](images/UPL_01-01-2020.png) |


| 01-01-2021              | 01-01-2022              |
| ---------------------- | ---------------------- |
| ![im1](images/UPL_01-01-2021.png) | ![im2](images/UPL_01-01-2022.png) |


| 01-01-2023              | 01-01-2024              |
| ---------------------- | ---------------------- |
| ![im1](images/UPL_01-01-2023.png) | ![im2](images/UPL_01-01-2024.png) |

## Trends observed
- **Short-Term Analysis:**
  - When observed for a short period of time (e.g., around 2 years), all the algorithms give almost the same result being centered around zero.

- **Long-Term Analysis:**
  - When analyzing over a large duration of time, RSI almost always stands out as the best strategy, outperforming others in most cases.
  - MACD performs poorly in most cases, except for one exception - UPL.

- **Analysis of Trends:**
  - After analyzing the closing price curve along with the trends of the algorithm, we concluded that:
    - For companies experiencing a downturn, MACD performs the best.
    - For companies on the rise, RSI performs the best when analyzed over a large period of time.

## Analysis and Insights of algorithm

- **Performance Evaluation**: 
  - Each strategy's performance can be evaluated based on metrics such as profit/loss, Sharpe ratio, maximum drawdown, etc.
  - Backtesting with historical data can provide insights into the effectiveness of each strategy.
  
- **Risk Management**:
  - Strategies like stop-loss and position constraints help manage risk and limit losses.
  - Incorporating risk-adjusted returns can provide a more comprehensive assessment of strategy performance.
  
- **Market Conditions**:
  - Different strategies may perform better under different market conditions.
  - Strategies relying on trend-following (e.g., DMA, MACD) may perform well in trending markets, while mean-reverting strategies (e.g., RSI, Pairs Trading) may perform better in ranging markets.
  
- **Complexity vs. Robustness**:
  - More complex strategies like linear regression may require more computational resources and data, but may also provide more robust predictions.
  - Simpler strategies like basic DMA may be easier to implement and interpret but may lack sophistication in capturing market dynamics.

## Comparison

- **Performance**: Compare performance metrics (e.g., returns, Sharpe ratio) across different strategies.
- **Risk Management**: Assess the effectiveness of risk management measures in each strategy.
- **Market Adaptability**: Evaluate how well each strategy adapts to different market conditions.
- **Computational Complexity**: Compare computational resources required for implementing and running each strategy.

## Recommendations

- **Diversification**: Consider using a combination of strategies to diversify risk and capture opportunities across different market conditions.
- **Continuous Monitoring**: Regularly monitor and reassess strategy performance to adapt to changing market dynamics.
- **Parameter Tuning**: Optimize strategy parameters based on historical data and performance metrics.
- **Backtesting**: Conduct thorough backtesting before deploying any strategy in live trading to understand its performance and risks.
