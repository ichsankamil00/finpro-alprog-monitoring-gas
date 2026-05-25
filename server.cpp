#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "sensor_data.h"

using namespace std;

vector<string> splitData(const string& data, char delimiter) {
    vector<string> hasil;
    string item;
    stringstream ss(data);

    while (getline(ss, item, delimiter)) {
        hasil.push_back(item);
    }

    return hasil;
}

void simpanSemuaData(const EnvironmentRecord& data) {
    ofstream file("data_semua.txt", ios::app);

    if (!file.is_open()) {
        cout << "Gagal membuka data_semua.txt\n";
        return;
    }

    file << data.toTextLine() << endl;
    file.close();
}

void simpanDataKritisJSON(const EnvironmentRecord& data) {
    if (data.getStatus() != "BAHAYA") {
        return;
    }

    ofstream file("data_kritis.json", ios::app);

    if (!file.is_open()) {
        cout << "Gagal membuka data_kritis.json\n";
        return;
    }

    file << data.toJSON() << endl;
    file.close();
}

void prosesDataMasuk(const string& rawData) {
    vector<string> bagian = splitData(rawData, '|');

    if (bagian.size() != 5) {
        cout << "Format data tidak valid: " << rawData << endl;
        return;
    }

    string sensor_id = bagian[0];
    double temperature = stod(bagian[1]);
    double co_value = stod(bagian[2]);
    double lpg_value = stod(bagian[3]);
    string timestamp = bagian[4];

    EnvironmentRecord data(sensor_id, temperature, co_value, lpg_value, timestamp);

    cout << "\n[DATA MASUK DARI CLIENT]\n";
    data.display();

    if (data.getStatus() == "BAHAYA") {
        cout << "PERINGATAN: Kondisi bahaya terdeteksi.\n";
    } else if (data.getStatus() == "WASPADA") {
        cout << "PERHATIAN: Kondisi mulai meningkat, perlu pemantauan.\n";
    } else {
        cout << "Kondisi lingkungan aman.\n";
    }

    simpanSemuaData(data);
    simpanDataKritisJSON(data);

    cout << "Data disimpan ke data_semua.txt";
    if (data.getStatus() == "BAHAYA") {
        cout << " dan data_kritis.json";
    }
    cout << ".\n";
}

void runServer() {
#ifdef _WIN32
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientSize = sizeof(clientAddr);
    char buffer[1024];

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "Gagal menjalankan Winsock.\n";
        return;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cout << "Gagal membuat socket server.\n";
        WSACleanup();
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9090);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "Gagal bind server ke port 8080.\n";
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        cout << "Gagal listen koneksi.\n";
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    cout << "=== SERVER MONITORING AKTIF ===\n";
    cout << "Menunggu koneksi client di port 9090...\n";

    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);
    if (clientSocket == INVALID_SOCKET) {
        cout << "Gagal menerima client.\n";
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    cout << "Client terhubung.\n";

    while (true) {
        ZeroMemory(buffer, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

        if (bytesReceived <= 0) {
            cout << "\nClient terputus / selesai mengirim data.\n";
            break;
        }

        string rawData(buffer);
        prosesDataMasuk(rawData);
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    cout << "Server selesai.\n";
#else
    cout << "Kode server ini disiapkan untuk Windows Winsock.\n";
#endif
}