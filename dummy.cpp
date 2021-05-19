#include <iostream>
#include <ostream>
#include <vector>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sstream>
#include <chrono>
#include <thread>
#include <random> 
#include <queue>

int main(){
    int time1 = std::chrono::system_clock::now().time_since_epoch().count(); 
    sleep(2);
    int time2 = std::chrono::system_clock::now().time_since_epoch().count(); 
    std::cout<<time1 - time2;
}