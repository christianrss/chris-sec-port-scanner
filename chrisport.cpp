#include <iostream>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib");
using namespace std;

void initWinsock() {
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Failed to initialized Winsock";
        exit(1);
    }
    cout << "Winsock initialiazed successfully!" << endl;

}

bool isOpen(const string &ip, int port) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock ==  INVALID_SOCKET) {
        return false;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip.c_str());

    int result = connect(sock, (struct sockaddr*)&address, sizeof(address));
    closesocket(sock);

    return result != SOCKET_ERROR;
}

int main() {
    initWinsock();

    string targetIP;
    int port;

    cout << "Enter target IP: ";
    cin >> targetIP;
    cout << "Enter port: ";
    cin >> port;

    if (isOpen(targetIP, port)) {
        cout << "Port " << port << " is open" << endl;
    } else {
        cout << "Port " << port << " is closed" << endl;
    }

    return 0;
}