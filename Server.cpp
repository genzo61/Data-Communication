#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int client_size, num1, num2, result;
    char operation;

    // Winsock ba�latma
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    // Soket olu�turma
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Adres ve port ba�lama
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Dinleme
    listen(server_socket, 3);
    printf("Waiting for incoming connections...\n");

    client_size = sizeof(struct sockaddr_in);
    client_socket = accept(server_socket, (struct sockaddr*)&client, &client_size);
    if (client_socket == INVALID_SOCKET) {
        printf("Accept failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Connection accepted.\n");
	while(1){
		 // �stemciden veri al
    recv(client_socket, (char*)&num1, sizeof(num1), 0);
    recv(client_socket, (char*)&num2, sizeof(num2), 0);
    recv(client_socket, &operation, sizeof(operation), 0);

    // Verileri ��z ve i�le
    num1 = ntohl(num1);
    num2 = ntohl(num2);

    switch (operation) {
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case '*': result = num1 * num2; break;
        case '/': 
            if (num2 != 0) 
                result = num1 / num2; 
            else 
                result = 0; // Hata durumunda 0 d�nd�r
            break;
        default: result = 0; // Ge�ersiz i�lem
    }

    // Sonucu istemciye g�nder
    result = htonl(result);
    send(client_socket, (char*)&result, sizeof(result), 0);
	}
   

    // Soketleri kapatma
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}

