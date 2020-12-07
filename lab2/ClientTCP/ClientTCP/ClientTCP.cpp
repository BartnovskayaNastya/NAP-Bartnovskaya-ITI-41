#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <sstream>
#pragma warning(disable: 4996)



double function(double x) {
    return pow(3, x) * cos(x);
}

double rectangle_integral(double a, double b, int n) {

    double h;
    double fx;
    fx = function(a);
    h = (b - a) / n;
    int i;
    for (i = 1; i < n - 1; i++) {
        fx += function(h*i + a);
    }
    return fx * h;
}

int main(int argc, char* argv[])
{
    //загрузка версии библиотеки для вызова сетевых функций
    WSAData wsaData;

    //запрашиваемая версия для библиотеки winsock
    WORD DLLVersion = MAKEWORD(2, 1);
    //фуекция для загрузки библиотеки
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        std::cout << "Error lib\n";
        exit(1);
    }

    char ip_server[15];

    std::cin >> ip_server;

    //std::vector<int> size_segment(2);


    //структура для хранения адреса (для интернет протоколов)
    SOCKADDR_IN socket_addres;
    int sizeof_socket_addres = sizeof(socket_addres);
    //для хранения ip-адреса
    socket_addres.sin_addr.s_addr = inet_addr(ip_server);
    //для хранения порта
    socket_addres.sin_port = htons(1111);
    //указание семейства протоколов - af_inet -сокет интернета
    socket_addres.sin_family = AF_INET;

    //создаем сокет (параметры: протокол, тип сокета - потоковый или асинхронный
    SOCKET client_Socket = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(client_Socket, (SOCKADDR*)&socket_addres, sizeof(socket_addres)) != 0) {
        std::cout << "Error connection to Server\n";
        return 1;
    }

    std::cout << "Connected to Server\n";


    double a, b, e;
    int n = 1;
    double result, s2;
    char buff[2048];

    char* a_end;
    char* b_end;
    recv(client_Socket, buff, sizeof(double)*3+sizeof(char)*2+1, 0);
    a = strtod(buff, &a_end);
    b = strtod(a_end, &b_end);
    e = strtod(b_end, NULL);

    


    result = rectangle_integral(a, b, n);
    do
    {
        n = n * 2;
        s2 = result;
        result = rectangle_integral(a, b, n);
    } while (fabs(result - s2) > e);


    std::stringstream s;
    s << fabs(result);
    std::string str = s.str();
    const char* buff2 = str.c_str();
    send(client_Socket, buff2, std::strlen(buff2) + 1, 0);


    printf("a = %f\n", a);
    printf("b = %f\n", b);
    printf("sum = %f\n", fabs(result));



    system("pause");
    closesocket(client_Socket);
    WSACleanup();

    return(0);
}




