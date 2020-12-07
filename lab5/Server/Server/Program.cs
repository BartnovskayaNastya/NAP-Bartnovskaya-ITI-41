using System;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace Server
{
    class Program
    {
        const double epsilon = 1e-2;
        static double Function(double x)
        {
            return Math.Pow(3, x) + 2 * x;
        }

        static double Rectangle_integral(double a, double b)
        {

            double a_start, b_start, delta, interval_length, x1, x2;
            int iters, function_calcs;
            a_start = a;
            b_start = b;
            delta = epsilon / 2;
            interval_length = b - a;
            iters = 0;
            function_calcs = 0;

            return (a + b) / 2.0;
        }


        static void ThreadProc(Object obj)
        {
            Socket handler = (Socket)obj;
            string data = null;

            // Мы дождались клиента, пытающегося с нами соединиться
            byte[] bytes = new byte[sizeof(double) * 2];
            double[] numbers = new double[2];
            int bytesRec = handler.Receive(bytes);
            data += Encoding.UTF8.GetString(bytes, 0, bytesRec);
            Buffer.BlockCopy(bytes, 0, numbers, 0, bytes.Length);
            double a = Convert.ToDouble(numbers[0]);
            double b = Convert.ToDouble(numbers[1]);

            Console.Write("Подсчет: ");

            double result = Rectangle_integral(a, b);

            Console.WriteLine("ОК");

            // Отправляем ответ клиенту
            string reply = "Результат = " + result;
            byte[] msg = Encoding.UTF8.GetBytes(reply);
            handler.Send(msg);

            handler.Shutdown(SocketShutdown.Both);
            handler.Close();
        }

        static void Main(string[] args)
        {
            // Устанавливаем для сокета локальную конечную точку
            string host = Dns.GetHostName();

            int port = 11000;
            IPEndPoint ipEndPoint = new IPEndPoint(IPAddress.Any, port);

            // Создаем сокет Tcp/Ip
            Socket sListener = new Socket(ipEndPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

            // Назначаем сокет локальной конечной точке и слушаем входящие сокеты
            try
            {
                sListener.Bind(ipEndPoint);
                sListener.Listen(10);

                // Начинаем слушать соединения
                while (true)
                {
                    Console.WriteLine("Имя сервера: {0}", host);
                    Console.WriteLine("Порт: {0}", port);
                    Console.WriteLine("Ожидаем соединение через порт {0}", ipEndPoint);

                    // Программа приостанавливается, ожидая входящее соединение
                    Socket handler = sListener.Accept();

                    ThreadPool.QueueUserWorkItem(new WaitCallback(ThreadProc), handler);



                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }
            finally
            {
                Console.ReadLine();
            }
        }
    }

}