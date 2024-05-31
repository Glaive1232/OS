#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <string>
#include <thread>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include<random>
#include<chrono>

#include "json/json.hpp"
#define PORT 5555


#define SA struct sockaddr_in
#define PORT 5555

std::random_device rt;    
std::mt19937 mt(rt());
std::uniform_int_distribution<int> dist(1, 6 );

bool encounter();
int main()
{
    int sock = 0;
    struct sockaddr_in client;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        throw std::runtime_error("error creating socket");
    }
     memset(&client, '\0', sizeof(client));
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = inet_addr("127.0.0.1");
    client.sin_port = htons(PORT);  

   

    int res_connect = connect(sock, (struct sockaddr *)&client, sizeof(client));
        if (res_connect == -1) {
            perror("Connection failed... \n");
            exit(EXIT_FAILURE);
        }
        std::cout<<"connection established\n";

        
        char buffer[1024];

    while(true) {   
        memset(buffer, 0, 1024);
        ssize_t received_bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);  // Leave space for null-terminator
        if (received_bytes == -1) 
        {
            perror("recv failed");
            close(sock);
            exit(EXIT_FAILURE);
        }
        buffer[received_bytes] = '\0';  //stringi verjy
        if(!encounter())
            send(sock, "regroupping", 11, 0);
        else
            break;
    }


     nlohmann::json js;
    // Parse JSON
    try {
       js = nlohmann::json::parse(buffer);
        std::cout << std::setw(4) << js << std::endl; 
    } catch (nlohmann::json::parse_error& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }
    std::cout<<"i AM here \n";
    sleep(2);
    if(!std::strcmp(std::string(js["destination"]).c_str(),"Port Knot City") )
    {
        std::cout<<"i am in port knot city \n";
        sleep(3);
        js["carried_materials"] = {};
        js["carried_materials"]["ceramics"]["medium"] = 2;
        js["carried_materials"]["ceramics"]["large"] = 1;
        js["carried_materials"]["metals"]["small"] = 1;
        js["carried_materials"]["metals"]["medium"] = 1;
        js["carried_materials"]["resins"]["medium"] = 1;
        js["carried_materials"]["resins"]["large"] = 1;
        js["destination"] = "Lake Knot City";
        std::cout<<" the resources gived to sender \n";
        std::cout<< " \n\n\n ";
        std::cout<<std::setw(4)<<js<<std::endl;
        std::string fsend = js.dump();
        send(sock, fsend.c_str(), fsend.length(), 0);
        
        sleep(10);  
    }
    else if (!std::strcmp(std::string(js["destination"]).c_str(),"Lake Knot City"))
    {
        std::cout<<"i am in Lake knot city \n";
         js["carried_materials"] = {};
        js["carried_materials"]["Crystals"] = 300;
        js["carried_materials"]["metals"]["small"] = 2;
        js["carried_materials"]["metals"]["medium"] = 1;
        js["carried_materials"]["metals"]["large"] = 2;
        js["destination"] = "South Knot City";

        std::cout<<" the resources gived to sender \n";
        std::cout<< " \n\n\n ";
        std::cout<<std::setw(4)<<js<<std::endl;
        std::string fsend = js.dump();
        send(sock, fsend.c_str(), fsend.length(), 0);
        
        sleep(3);

    } 
    else if (!std::strcmp(std::string(js["destination"]).c_str(),"South Knot City"))
    {
        std::cout<<"i am in South knot city \n";
         js["carried_materials"] = {};
        js["carried_materials"]["Crystals"] = 50;
        js["carried_materials"]["resins"]["small"] = 1;
        js["carried_materials"]["chemicals"]["medium"] = 5;
        js["carried_materials"]["metals"]["large"] = 2;
        js["destination"] = "Mountain Knot City";

        std::cout<<" the resources gived to sender \n";
        std::cout<< " \n\n\n ";
        std::cout<<std::setw(4)<<js<<std::endl;
        std::string fsend = js.dump();
        send(sock, fsend.c_str(), fsend.length(), 0);
        
        sleep(3);

    }  
    else
    {
        std::cout<<"the final delivery delivered successfully \n \n \n";
        std::cout<<"Thank you for your hard work!   "; 
        std::cout<< js["name"] <<" "<< js["surname"] << std::endl;;
    }
    close(sock);
    return 0;
}


bool encounter()
{
    std::cout<<"You have encountered Beached Things, roll the dice to see the outcome of the encounte\n \n";
    std::string roll;
    std::cin>>roll;
    while(roll.compare("/roll"))
    {
        std::cout<<"stop kidding, roll quickly \n";
        std::cin>>roll;
    }
    int first = dist(mt);
    int second = dist(mt);
    std::cout<<"first cube: " << first << " second: " << second<<std::endl;
    if(first + second >= 8)
    {
        std::cout<<"you succesfully  killed the Beached things!! you can continue your delivery!! \n \n";
        return  true;
    }
    std::cout<<"you was defeated and losed all your stuff: return to your base \n \n ";
    return false;
}