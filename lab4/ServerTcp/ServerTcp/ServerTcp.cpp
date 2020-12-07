#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <windows.h>
#include "ThreadPool.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>
#pragma warning(disable: 4996)

const double epsilon = 1e-2;

//прототип функции 
DWORD WINAPI ThreadFunc(LPVOID client_socket);
SOCKET server_socket;


double function(double x) {
    return pow(3, x) + 2 * x;
}

double rectangle_integral(double a, double b) {

    double a_start, b_start, delta, interval_length, x1, x2;
    int iters, function_calcs;
    a_start = a;
    b_start = b;
    delta = epsilon / 2;
    interval_length = b - a;
    iters = 0;
    function_calcs = 0;

    while (interval_length > epsilon)
    {
        x1 = (a + b - delta) / 2;
        x2 = (a + b + delta) / 2;
        if (function(x1) > function(x2)) {
            a_start = x1;
        }
        else {
            b_start = x2;
        }

        interval_length = b_start - a_start;
        function_calcs += 2;
        iters += 1;
    }
    return (a + b) / 2.0;
}

DWORD WINAPI ThreadFunc(LPVOID client_socket)
{
    double result, a, b;

    SOCKET mySock;
    mySock = ((SOCKET*)client_socket)[0];

    //HANDLE hEvent = (HANDLE)context;
    //SetEvent(hEvent);

    char buff2[2048];
    char* a_end;
    char* b_end;
    recv(mySock, buff2, sizeof(double) * 3 + sizeof(char) * 2 + 1, 0);
    a = strtod(buff2, &a_end);
    b = strtod(a_end, &b_end);

    result = rectangle_integral(a, b);

    std::stringstream s;
    s << result;
    std::string str = s.str();
    const char* buff = str.c_str();
    send(mySock, buff, std::strlen(buff) + 1, 0);

    return 0;
}




int main(int argc, char* argv[])
{
    //загрузка версии библиотеки для вызова сетевых функций
    WSAData wsaData;

    //запрашиваемая версия для библиотеки winsock
    WORD DLLVersion = MAKEWORD(2, 1);
    //функция для загрузки библиотеки
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        std::cout << "Error lib\n";
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
        }

        QueueUserWorkItem(ThreadFunc, &client_socket, WT_EXECUTEDEFAULT);

        //if (num_clients == max_clients) {
          //  break;
        //}
    }

    closesocket(server_socket);
    system("pause");
    WSACleanup();
    return(0);
}


