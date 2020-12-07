#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>
#pragma warning(disable: 4996)


const int max_clients = 1;
SOCKET connections[1];
int num_clients = 0;
double e = 0.0001;
double a = -1;
double b = 1;
double sum = 0;

DWORD WINAPI NewClient(SOCKET client_socket);
SOCKET server_socket;

std::chrono::time_point<std::chrono::high_resolution_clock> time_start;
std::chrono::time_point<std::chrono::high_resolution_clock> time_end;
std::chrono::nanoseconds delta_time;
std::chrono::nanoseconds all_time;


HANDLE threads[max_clients];



int main(int argc, char* argv[])
{
    //загрузка версии библиотеки для вызова сетевых функций
    WSAData wsaData;

    //запрашиваемая версия для библиотеки winsock
    WORD DLLVersion = MAKEWORD(2, 1);
    //функция для загрузки библиотеки
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        std::cout <<"Error lib\n";
        exit(1);
    }

    //структура для хранения адреса (для интернет протоколов)
    SOCKADDR_IN server_socket_addres;
    
    //для хранения ip-адреса
    server_socket_addres.sin_addr.s_addr = 0;
    //для хранения порта
    server_socket_addres.sin_port = htons(1111);
    //указание семейства протоколов - af_inet -сокет интернета
    server_socket_addres.sin_family = AF_INET;

    //создаем сокет (параметры: протокол, тип сокета - потоковый
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    //связываем сокет с его локальным адресом
    bind(server_socket, (SOCKADDR*)&server_socket_addres, sizeof(server_socket_addres));

    //прослушиваем порт(какой порт + количество клиентов в очереди)
    listen(server_socket, SOMAXCONN);


    //принемаем соединение, создавая новый сокет для клиента
    SOCKET client_socket;
    SOCKADDR_IN client_socket_addres;
    int sizeof_client_socket_addres = sizeof(server_socket_addres);

    while ((client_socket = accept(server_socket, (sockaddr*)&client_socket_addres, &sizeof_client_socket_addres)))
    {
        if (client_socket == 0) {
            std::cout << "Error with client connection\n";
        }
        else {
            std::cout << "Client connected\n";
            num_clients++;
        }

        // Засекаем время
        time_start = std::chrono::high_resolution_clock::now();

        //запускаем поток
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)NewClient, (LPVOID)client_socket, NULL, NULL);

        time_end = std::chrono::high_resolution_clock::now();

        delta_time = std::chrono::duration_cast<std::chrono::nanoseconds>(time_end - time_start);
        all_time += delta_time;


        if (num_clients == max_clients) {
            break;
        }
    }

    std::chrono::nanoseconds delta_time = std::chrono::duration_cast<std::chrono::nanoseconds>(time_end - time_start);
    std::cout << "Time: " << delta_time.count() * 0.001f << "us" << std::endl;
    closesocket(server_socket);
    printf("Sum from all clients = %f\n", sum);
    

    system("pause");
    WSACleanup();
    return(0);
}


DWORD WINAPI NewClient(SOCKET client_socket)
{
    double start, end;

    double segment_size, size, result_sum;
    size = b - a;
    segment_size = size / max_clients;

    
    start = a + segment_size * (num_clients-1);
    end = start + segment_size;


   // printf("for %d clienta Start = %f\n", index, start);
   // printf("for %d clienta Start = %f\n", index, end);
    

    std::stringstream s;
    s << start;
    s << " ";
    s << end;
    s << " ";
    s << e;
    std::string str = s.str();
    const char *buff = str.c_str();
    send(client_socket, buff, std::strlen(buff)+1, 0);



    char buff2[2048];
    recv(client_socket, buff2, sizeof(buff2)+1, 0);
    result_sum = strtod(buff2, NULL);
    sum += result_sum;


    return 0;
}
