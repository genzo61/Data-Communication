#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server;
    int num1, num2, result;
    char operation;

    // Winsock baþlatma
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    // Soket oluþturma
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Sunucu adresi
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // Yerel makine
    server.sin_port = htons(8080);

    // Sunucuya baðlanma
    if (connect(client_socket, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Connection failed. Error Code: %d\n", WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    printf("Connected to server.\n");

    while(1){
    	// Kullanýcýdan veri al
    printf("Enter first number: ");
    scanf("%d", &num1);
    printf("Enter second number: ");
    scanf("%d", &num2);
    printf("Enter operation (+, -, *, /): ");
    scanf(" %c", &operation);

    // Verileri sunucuya gönder
    num1 = htonl(num1);
    num2 = htonl(num2);
    send(client_socket, (char*)&num1, sizeof(num1), 0);
    send(client_socket, (char*)&num2, sizeof(num2), 0);
    send(client_socket, &operation, sizeof(operation), 0);

    // Sunucudan sonucu al
    recv(client_socket, (char*)&result, sizeof(result), 0);
    result = ntohl(result);
    printf("Result: %d\n", result);
	}

    // Soketi kapatma
    closesocket(client_socket);
    WSACleanup();

    return 0;
}

