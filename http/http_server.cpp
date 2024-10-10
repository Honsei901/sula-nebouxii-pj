#include <iostream>
#include <cstdio>
#include <string.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

int main()
{
  /**
   * Initialize Winsock.
   */
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
  {
    std::cerr << "WSAStartup failed." << std::endl;
    return 1;
  }

  /**
   * Open a file to serve.
   */
  FILE *html_data;
  html_data = fopen("./src/index.html", "r");

  /**
   * Set a http file.
   */
  char response_data[1024];
  if (fgets(response_data, 1024, html_data) == NULL)
  {
    std::cerr << "Failed to open the file." << std::endl;
    fclose(html_data);
    return 1;
  }
  fclose(html_data);

  char http_header[2048];
  strcpy(http_header, "HTTP/1.1 200 OK\r\n\n");

  strcat(http_header, response_data);

  /**
   * Create a socket.
   */
  int server_socket;
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == INVALID_SOCKET)
  {
    std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
    WSACleanup();
    return 1;
  }
  std::cout << "Socket created successfully!" << std::endl;

  /**
   * Define the address.
   */
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(8001);
  server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

  bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
  listen(server_socket, 5);

  int client_socket;
  while (1)
  {
    client_socket = accept(server_socket, NULL, NULL);
    send(client_socket, http_header, sizeof(http_header), 0);
    closesocket(client_socket);
  }

  return 0;
}