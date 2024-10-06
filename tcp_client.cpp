#include <iostream>
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
   * Create a socket.
   *
   * Address family is "AF_INET"  => IPv4 address
   * Socket type is "SOCK_STREAM" => TCP
   * Protocol is 0                => Default protocol
   */
  int network_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (network_socket == INVALID_SOCKET)
  {
    std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
    WSACleanup();
    return 1;
  }
  std::cout << "Socket created successfully!" << std::endl
            << std::endl;

  /**
   * Specify an address for the socket.
   */
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(9002);
  server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

  int connection_status = connect(network_socket, (struct sockaddr *)&server_address, sizeof(server_address));

  /**
   * Check for error with the connection.
   */
  if (connection_status == SOCKET_ERROR)
  {
    std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
    closesocket(network_socket);
    WSACleanup();
    return 1;
  }

  /**
   * Recieve data from the server.
   */
  char server_response[256];
  int bytes_received = recv(network_socket, server_response, sizeof(server_response) - 1, 0);
  if (bytes_received > 0)
  {
    server_response[bytes_received] = '\0';
    std::cout << "The server sent the data: " << server_response << std::endl;
  }
  else if (bytes_received == 0)
  {
    std::cout << "Connection closed by server." << std::endl;
  }
  else
  {
    std::cerr << "Receive failed: " << WSAGetLastError() << std::endl;
  }

  /**
   * Close the socket
   */
  closesocket(network_socket);
  WSACleanup();

  return 0;
}