#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#endif

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <sstream>

using namespace std;

string getTimestamp() {
    time_t now = time(0);
    tm* localTime = localtime(&now);

    char buffer[30];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);

    return string(buffer);
}

double randomDouble(double minVal, double maxVal) {
    double r = (double)rand() / RAND_MAX;
    return minVal + r * (maxVal - minVal);
}

string buatDataSensor(string sensor_id, double temperature, double co_value, double lpg_value, string timestamp) {
    stringstream ss;

    ss << sensor_id << "|"
       << temperature << "|"
       << co_value << "|"
       << lpg_value << "|"
       << timestamp;

    return ss.str();
}

void runClient() {
#ifdef _WIN32
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;

    srand((unsigned)time(0));

    string sensor_id;
    cout << "Masukkan Sensor ID, contoh DAPUR_1: ";
    cin >> sensor_id;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "Gagal menjalankan Winsock.\n";
        return;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cout << "Gagal membuat socket client.\n";
        WSACleanup();
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9090);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    cout << "Menghubungkan ke server...\n";

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "Gagal connect ke server. Pastikan server sudah jalan.\n";
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    cout << "Client terhubung ke server.\n";

    for (int i = 1; i <= 5; i++) {
        double temperature = randomDouble(25, 55);
        double co_value = randomDouble(20, 220);
        double lpg_value = randomDouble(200, 900);
        string timestamp = getTimestamp();

        string data = buatDataSensor(sensor_id, temperature, co_value, lpg_value, timestamp);

        send(clientSocket, data.c_str(), data.length(), 0);

        cout << "\nData ke-" << i << " berhasil dikirim:\n";
        cout << data << endl;

        Sleep(1000);
    }

    closesocket(clientSocket);
    WSACleanup();

    cout << "\nClient selesai mengirim data.\n";
#else
    cout << "Kode client ini disiapkan untuk Windows Winsock.\n";
#endif
}