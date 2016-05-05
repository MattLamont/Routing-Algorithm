
routing_table: routing_table.cpp
	g++ -I /usr/include routing_table.cpp -o routing_table -lboost_system -lboost_filesystem -pthread -L /usr/lib64 -L /usr/lib/libboost_thread-mt.so;


