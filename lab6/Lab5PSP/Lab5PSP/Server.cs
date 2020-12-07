using System;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Net.Security;

namespace Lab5PSP
{
    class Server
    {
        const double epsilon = 1e-2;
        public Server()
        {
            string host = Dns.GetHostName();

            int port = 80;
            IPEndPoint ipEndPoint = new IPEndPoint(IPAddress.Any, port);

            Socket sListener = new Socket(ipEndPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            sListener.Bind(ipEndPoint);
            sListener.Listen(10);

            // Начинаем слушать соединения
            while (true)
            {
                Socket handler = sListener.Accept();
                ThreadPool.QueueUserWorkItem(new WaitCallback(ThreadProc), handler);
            }
        }

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
            Socket Client = (Socket)obj;


            //запрос клиента
            string Request = "";

            byte[] Buffer = new byte[1024];
            int bytes;

            do
            {
                bytes = Client.Receive(Buffer);
                Request += Encoding.ASCII.GetString(Buffer, 0, bytes);

            }
            while (Client.Available > 0);

            //парсинг сроки запроса
            Match ReqMatch = Regex.Match(Request, @"^\w+\s+([^\s]+)[^\s]*\s+HTTP/.*|");
            // Получаем строку запроса
            string RequestUri = ReqMatch.Groups[1].Value;

            RequestUri = Uri.UnescapeDataString(RequestUri);

            string Headers;
            //для записи заголовков
            byte[] HeadersBuffer;

            // Если строка запроса оканчивается на "/", то добавим к ней index.html
           if (RequestUri.EndsWith("/") ||
                RequestUri.EndsWith(".css") ||
                RequestUri.EndsWith(".html") ||
                RequestUri.EndsWith(".js") ||
                RequestUri.EndsWith(".htm"))
           {
                if (RequestUri.EndsWith("/"))
                {
                    RequestUri += "index.html";
                }

                string FilePath = "Resources/" + RequestUri;


                // Получаем расширение файла из строки запроса
                string Extension = RequestUri.Substring(RequestUri.LastIndexOf('.'));

                // Тип содержимого
                string ContentType = "";

                // Пытаемся определить тип содержимого по расширению файла
                switch (Extension)
                {
                    case ".htm":
                    case ".html":
                        ContentType = "text/html";
                        break;
                    case ".css":
                        ContentType = "text/stylesheet";
                        break;
                    case ".js":
                        ContentType = "text/javascript";
                        break;
                    case ".jpg":
                        ContentType = "image/jpeg";
                        break;
                    case ".jpeg":
                    case ".png":
                    case ".gif":
                        ContentType = "image/" + Extension.Substring(1);
                        break;
                    default:
                        if (Extension.Length > 1)
                        {
                            ContentType = "application/" + Extension.Substring(1);
                        }
                        else
                        {
                            ContentType = "application/unknown";
                        }
                        break;
                }

                // Открываем файл, страхуясь на случай ошибки
                FileStream FS;
                
                FS = new FileStream(FilePath, FileMode.Open, FileAccess.Read, FileShare.Read);
               

                // Посылаем заголовки
                Headers = "HTTP/1.1 200 OK\n" +
                    "Content-Type: " + ContentType +
                    "\nContent-Length: " + FS.Length +
                    "\n\n";
                HeadersBuffer = Encoding.ASCII.GetBytes(Headers);
                Client.Send(HeadersBuffer);

                // Пока не достигнут конец файла
                while (FS.Position < FS.Length)
                {
                    // Читаем данные из файла
                    bytes = FS.Read(Buffer, 0, Buffer.Length);
                    // И передаем их клиенту
                    Client.Send(Buffer);
                }

                // Закроем файл и соединение
                FS.Close();
                Client.Close();
           }
            else 
            
            if (RequestUri.Length > 2)
            {
                double a, b, result = 0;
                string all_numbers = RequestUri.Remove(0, 1);
                string[] numbers = all_numbers.Split(' ');

                if (numbers.Length == 2)
                {
                    a = Convert.ToDouble(numbers[0]);
                    b = Convert.ToDouble(numbers[1]);

                    result = Rectangle_integral(a, b);
                }
                var correctStr = result.ToString();
                byte[] message = Encoding.ASCII.GetBytes(correctStr);
                Headers = "HTTP/1.1 200 OK\n" +
                    "Content-Type: " + "text" + "\n" +
                    "Content-Length: " +
                    message.Length + "\n\n";
                HeadersBuffer = Encoding.ASCII.GetBytes(Headers);
                Client.Send(HeadersBuffer);
                Client.Send(message);

                Client.Close();
            }
        }
    }
}
