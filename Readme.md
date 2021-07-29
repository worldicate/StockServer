# Concurrent Stock Server 

![](https://images.velog.io/images/worldicate/post/0570f367-ba85-478c-ae00-684c9d015ff0/%EC%A0%9C%EB%AA%A9%20%EC%97%86%EC%9D%8C.png)

## There are two ways to execute the program.

1) project1 : Event-driven Approach
![](https://images.velog.io/images/worldicate/post/5187853d-0d6f-4ccb-9be9-bf76192cf0f6/%EC%A0%9C%EB%AA%A9%20%EC%97%86%EC%9D%8C3.png)
2) project2 : Thread-Based Approach
![](https://images.velog.io/images/worldicate/post/f851a0d5-40e9-48db-a582-4cf1fc671553/%EC%A0%9C%EB%AA%A9%20%EC%97%86%EC%9D%8C2.png)
## This program contains stockclient.c stockserver.c , multiclient.c
### StockServer.c 
Server can deal with several clients concurrently by Event-driven approach and Process-based approach
### StockClient.c 
Each Client can sell and buy stocks from Stock Server
#### Command
1) show
2) buy [stockID] [ # of stock]
3) sell [stockID] [# of stock]
4) exit
## multiClient.c
You can create the number of client that you want, and each client execute any command.
