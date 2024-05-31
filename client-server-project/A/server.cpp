#include <iostream>
#include <vector>
#include <string>
#include <sys/socket.h>
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


std::random_device rt;    
std::mt19937 mt(rt());
std::uniform_int_distribution<int> dist(1,4 );

pthread_mutex_t lakemute, southmute, mountainmute;
int sock;

nlohmann::json outer_json;
int loadingweight(nlohmann::json & js);// qashna hashvelu
void checkingstamina(int &stamina);
void* PortKnotCity(void* arg) {
    int *index = (int*)arg;

 
    struct sockaddr_in cli;
    socklen_t length = sizeof(cli);
    int conn_sock = accept(sock, (struct sockaddr*)(&cli), &length);
    if (conn_sock == -1) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    if(*index == 0){    

        std::cout << "Port Knot City entered the Chiral network" << std::endl;
        std::ifstream out("../out.json");
        nlohmann::json js = nlohmann::json::parse(out);
       
        js["carried_materials"]["resins"]["small"] = 1;
        js["carried_materials"]["resins"]["medium"] = 2;
        js["carried_materials"]["resins"]["large"] = 1;
        js["carried_materials"]["metals"]["small"] = 2;
        js["carried_materials"]["resins"]["medium"] = 2;
        int weight = loadingweight(js); ;
        js["load_weight"] = weight;
        js["destination"] = "Port Knot City";
        char buffer[1024];
        
        while(true)
        {
                if(!std::strcmp(buffer, "regroupping"))
                {
                    std::cout<<"i need to rest after encouter with Things \n \n";
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                    std::cout<<"I am ready to new delivery \n";
                }
                int stamina = js["stamina"];
                checkingstamina(stamina);
                stamina -= dist(mt) * weight / 5;
                js["stamina"] = stamina;
                std::string str = js.dump();
                memset(buffer, 0, 1024);
                send(conn_sock, str.c_str(), str.length(), 0 );
                std::cout<<"the delivery guy is went \n \n";
                int received_bytes = recv(conn_sock, buffer, 1023, 0);
                if (received_bytes < 0) {
                    perror("Error reading");
                    close(conn_sock);
                    pthread_exit(nullptr); // or return nullptr;
                }

                buffer[received_bytes] = '\0';
                std::cout<<buffer<<std::endl;
                if(std::strcmp(buffer, "regroupping"))
                    break;

        }
            
                outer_json = nlohmann::json::parse(buffer);
                std::cout<<"Going to next city \n \n ";
                pthread_mutex_unlock(&lakemute);
                close(conn_sock);
                pthread_exit(nullptr);
            

        }
    else  {
        if(*index == 1 )
            {
            std::cout << "Lake Knot City entered the Chiral network" << std::endl;
            pthread_mutex_lock(&lakemute);
            }
        else if(*index == 2 )
        {
            std::cout << "South Knot City entered the Chiral network" << std::endl;
            pthread_mutex_lock(&southmute);  
        }
        else 
        {
            std::cout << "Mountain Knot City entered the Chiral network" << std::endl;
            pthread_mutex_lock(&mountainmute);
        }
        char buffer[1024];
        while(true){
             if(!std::strcmp(buffer, "regroupping"))
                {
                    std::cout<<"i need to rest after encouter with Things \n \n";
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                    std::cout<<"I am ready to new delivery \n";
                }
            int weight = loadingweight(outer_json);
            outer_json["load_weight"] = weight;
            int stamina = outer_json["stamina"];
            checkingstamina(stamina);
            stamina -= dist(mt) * weight / 5;
            outer_json["stamina"] = stamina;
            std::string str = outer_json.dump();
            std::cout<<"delivering next things \n \n"<< outer_json.dump(4) << std::endl;
            send(conn_sock, str.c_str(), str.length(), 0 );
           
            memset(buffer, 0, 1024);
            int received_bytes = recv(conn_sock, buffer, 1023, 0);
            if (received_bytes < 0) {
                perror("Error reading");
                close(conn_sock);
                pthread_exit(nullptr); // or return nullptr;
            }
            buffer[received_bytes] = '\0';
            if(std::strcmp(buffer, "regroupping"))
                break;
        }

         if(*index == 3)
            {
                std::cout<<"all deliveries are successfully delivered"<<std::endl;
                close(conn_sock);
                pthread_exit(nullptr);
            }
        
         outer_json = nlohmann::json::parse(buffer);
         outer_json["load_weight"] = loadingweight(outer_json);
        std::cout << std::setw(4) << outer_json << std::endl;
        if(*index == 1)
            pthread_mutex_unlock(&southmute);
        else if (*index == 2)
            pthread_mutex_unlock(&mountainmute);
         close(conn_sock);
         pthread_exit(nullptr);

    }
   
}



void creating_threads(pthread_t* &cities,  int *indexes) {
    if (pthread_create(&cities[0], nullptr, &PortKnotCity, indexes )!= 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&cities[1], nullptr, &PortKnotCity, &indexes[1]) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&cities[2], nullptr, &PortKnotCity, &indexes[2]) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&cities[3], nullptr, &PortKnotCity, &indexes[3] )!= 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
}

void joining_threads(pthread_t* &cities);
int main() {
    pthread_mutex_init(&lakemute, nullptr);
    pthread_mutex_init(&southmute, nullptr);
    pthread_mutex_init(&mountainmute, nullptr);
// tarbe mutexner em ogtagorcelu, vor chihst hertakanutyamb ashxaten

    int * indexes= new int(4);
    indexes[0] = 0;
    indexes[1] = 1;
    indexes[2] = 2;
    indexes[3] = 3;
    pthread_mutex_lock(&lakemute);
    pthread_mutex_lock(&southmute);
    pthread_mutex_lock(&mountainmute);
    struct sockaddr_in sockaddr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int optval = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockaddr.sin_port = htons(PORT);
    if (bind(sock, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(sock, 4) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    pthread_t* cities = new pthread_t[4];
    creating_threads(cities, indexes);

    pthread_mutex_destroy(&lakemute);
    pthread_mutex_destroy(&southmute);
    pthread_mutex_destroy(&mountainmute);

    joining_threads(cities);

    delete[] cities;

    close(sock);

    return 0;
}

void joining_threads(pthread_t* &cities)
{
    if (pthread_join(cities[0],NULL )!= 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }
    if (pthread_join(cities[1], NULL) != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }
    if (pthread_join(cities[2], NULL) != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }
    if (pthread_join(cities[3], NULL )!= 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    return;
}

int loadingweight(nlohmann::json & js)
{
     auto carriediteral = js.find("carried_materials");
    int weight = 0;

    //qashi hashvelna dzerov che, ayl algoritmov
    for(auto &it : carriediteral->items() )
        if(it.value().is_object())
            for(auto &ti : it.value().items() )
                weight += (int)js["weights"][it.key()][ti.key()]  * (int)ti.value();
        else
            weight += (float)js["weights"][it.key()] * (int)it.value();
    
    return weight;
}

void checkingstamina(int &stamina)
{
    if(stamina>= 50)
        return;
    std::cout<<"I need to recover some strengh. \n \n ";
    stamina  = (stamina+50>= 100)? 100 : stamina + 50;
    std::this_thread::sleep_for(std::chrono::seconds(15));
    return;
     
}