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
  FILE *html_data = fopen("./src/index.html", "rb");
  if (html_data == NULL)
  {
    std::cerr << "Failed to open the file." << std::endl;
    WSACleanup();
    return 1;
  }

  /**
   * Get a file size.
   */
  fseek(html_data, 0, SEEK_END);
  long file_size = ftell(html_data);
  rewind(html_data);

  /**
   * Secure buffer.
   */
  char *response_data = new char[file_size + 1];
  if (response_data == NULL)
  {
    std::cerr << "Memory allocation failed." << std::endl;
    fclose(html_data);
    WSACleanup();
    return 1;
  }

  /**
   * Read the file.
   */
  size_t read_size = fread(response_data, 1, file_size, html_data);
  if (read_size != file_size)
  {
    std::cerr << "Failed to read the file." << std::endl;
    delete[] response_data;
    fclose(html_data);
    WSACleanup();
    return 1;
  }
  response_data[file_size] = '\0';
  fclose(html_data);

  /**
   * Set a header.
   */
  char http_header[2048];
  sprintf(http_header,
          "HTTP/1.1 200 OK\r\n"
          "Content-Type: text/html\r\n"
          "Content-Length: %ld\r\n"
          "\r\n",
          file_size);

  // Combine the header with a body.
  char *http_response = new char[strlen(http_header) + file_size + 1];
  strcpy(http_response, http_header);
  memcpy(http_response + strlen(http_header), response_data, file_size);
  http_response[strlen(http_header) + file_size] = '\0';

  /**
   * Create a socket.
   */
  SOCKET server_socket;
  server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (server_socket == INVALID_SOCKET)
  {
    std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
    delete[] response_data;
    delete[] http_response;
    WSACleanup();
    return 1;
  }
  std::cout << "Socket created successfully!" << std::endl;

  /**
   * Define the address.
   */
  sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(5050);
  server_address.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR)
  {
    std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
    closesocket(server_socket);
    delete[] response_data;
    delete[] http_response;
    WSACleanup();
    return 1;
  };

  if (listen(server_socket, 5) == SOCKET_ERROR)
  {
    std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
    closesocket(server_socket);
    delete[] response_data;
    delete[] http_response;
    WSACleanup();
    return 1;
  }

  SOCKET client_socket;
  while (true)
  {
    client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == INVALID_SOCKET)
    {
      std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
      continue;
    }

    int send_result = send(client_socket, http_response, strlen(http_response), 0);
    if (send_result == SOCKET_ERROR)
    {
      std::cerr << "Failed to send a data: " << WSAGetLastError() << std::endl;
    }
    else
    {
      std::cout << "Data sent to client." << std::endl;
    }

    closesocket(client_socket);
  }

  closesocket(client_socket);
  delete[] response_data;
  delete[] http_response;
  WSACleanup();

  return 0;
}