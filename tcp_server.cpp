#include <iostream>
#include <WinSock2.h>

int main()
{
  /**
   * Set a message.
   */
  char server_message[256] = "A MESSAGE FROM SERVER";

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
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == INVALID_SOCKET)
  {
    std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
    WSACleanup();
    return 1;
  }

  std::cout << "Socket created successfully!" << std::endl;

  /**
   * Specify an address for the socket.
   */
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(9002);
  server_address.sin_addr.s_addr = INADDR_ANY;

  bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
  listen(server_socket, 5);

  int client_socket = accept(server_socket, NULL, NULL);
  send(client_socket, server_message, sizeof(server_message), 0);

  /**
   * Close the socket
   */
  closesocket(server_socket);
  WSACleanup();

  return 0;
}