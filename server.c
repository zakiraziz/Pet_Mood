#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int c;

    WSAStartup(MAKEWORD(2,2), &wsa);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    bind(server_socket, (struct sockaddr *)&server, sizeof(server));
    listen(server_socket, 3);

    printf("Server running on http://localhost:8080\n");

    c = sizeof(struct sockaddr_in);
    client_socket = accept(server_socket, (struct sockaddr *)&client, &c);

    char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<html>"
        "<head><title>My C Website</title></head>"
        "<body>"
        "<h1>Hello Zakir! 🚀</h1>"
        "<p>This website is made using C language!</p>"
        "</body>"
        "</html>";

    send(client_socket, response, strlen(response), 0);

    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
