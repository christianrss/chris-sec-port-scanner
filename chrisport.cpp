#include <iostream>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib");

void initWinsock() {
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialized Winsock";
        exit(1);
    }
    std::cout << "Winsock initiliazed successfully!" << std::endl;

}

int main() {
    initWinsock();
}