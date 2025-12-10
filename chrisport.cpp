#include <iostream>
#include <WinSock2.h>
#include <vector>
#include <thread>
#include <string>
#include <mutex>
#include <iomanip>
#pragma comment(lib, "ws2_32.lib");
using namespace std;

mutex printMutex;

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

string getServiceName(int port) {
    struct servent *service = getservbyport(htons(port), "tcp");
    return service ? service->s_name : "Unknown";
}

void scanPorts(const string &ip, int startPort, int endPort) {
    for (int port = startPort; port <= endPort; port++) {
        if (isOpen(ip, port)) {
            string serviceName = getServiceName(port);
            lock_guard<mutex> lock(printMutex);

            cout << "\033[32m"
                "Port " << setw(5) << port
                << " | Protocol: TCP"
                << " | Service: " << serviceName
                << "\033[0m" << endl;
        }
    }
}

int main() {
    initWinsock();

    string targetIP;
    int startPort, endPort, numThreads;

    cout << "Enter target IP: ";
    cin >> targetIP;
    cout << "Enter start port: ";
    cin >> startPort;
    cout << "Enter end port: ";
    cin >> endPort;
    cout << "Enter number of threads: ";
    cin >> numThreads;

    int range = (endPort - startPort + 1) / numThreads;
    vector<thread> threads;

    for (int i = 0; i < numThreads; i++) {
        int rangeStart = startPort + i * range;
        int rangeEnd = (i == numThreads - 1)
            ? endPort
            : rangeStart + range - 1;

        threads.emplace_back(scanPorts, targetIP, rangeStart, rangeEnd);
    }

    for (auto &t : threads) {
        t.join();
    }

    cout << "Scan completed.";

    return 0;
}