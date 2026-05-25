#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "server.h"
#include "client.h"
#include "sensor_data.h"

using namespace std;

vector<string> splitLine(const string& line, char delimiter) {
    vector<string> hasil;
    string item;
    stringstream ss(line);

    while (getline(ss, item, delimiter)) {
        hasil.push_back(item);
    }

    return hasil;
}

vector<EnvironmentRecord> bacaDataSemua() {
    vector<EnvironmentRecord> dataList;
    ifstream file("data_semua.txt");

    if (!file.is_open()) {
        cout << "File data_semua.txt belum ditemukan atau belum ada data.\n";
        return dataList;
    }

    string line;

    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        vector<string> bagian = splitLine(line, '|');

        // Format:
        // sensor_id|temperature|co_value|lpg_value|timestamp|status
        if (bagian.size() < 6) {
            continue;
        }

        string sensor_id = bagian[0];
        double temperature = stod(bagian[1]);
        double co_value = stod(bagian[2]);
        double lpg_value = stod(bagian[3]);
        string timestamp = bagian[4];

        EnvironmentRecord data(sensor_id, temperature, co_value, lpg_value, timestamp);
        dataList.push_back(data);
    }

    file.close();
    return dataList;
}

void tampilkanHeaderTabel() {
    cout << "=================================================================================================================\n";
    cout << "| No | Sensor ID     | Suhu (C) | CO (ppm) | LPG (/1023) | Timestamp           | Status   |\n";
    cout << "=================================================================================================================\n";
}

void tampilkanBarisData(const EnvironmentRecord& data, int nomor) {
    cout << "| " << nomor << "  "
         << "| " << data.getSensorID() << "        "
         << "| " << data.getTemperature() << "     "
         << "| " << data.getCOValue() << "     "
         << "| " << data.getLPGValue() << "       "
         << "| " << data.getTimestamp() << " "
         << "| " << data.getStatus() << " |\n";
}

void tampilkanSemuaData() {
    vector<EnvironmentRecord> dataList = bacaDataSemua();

    if (dataList.empty()) {
        cout << "Belum ada data untuk ditampilkan.\n";
        return;
    }

    cout << "\n=== SEMUA DATA MONITORING ===\n";
    tampilkanHeaderTabel();

    for (int i = 0; i < dataList.size(); i++) {
        tampilkanBarisData(dataList[i], i + 1);
    }

    cout << "=================================================================================================================\n";
}

void cariBerdasarkanSensorID() {
    vector<EnvironmentRecord> dataList = bacaDataSemua();

    if (dataList.empty()) {
        cout << "Belum ada data untuk dicari.\n";
        return;
    }

    string target;
    cout << "Masukkan Sensor ID yang dicari, contoh DAPUR_1: ";
    cin >> target;

    bool ditemukan = false;

    cout << "\n=== HASIL PENCARIAN SENSOR ID: " << target << " ===\n";
    tampilkanHeaderTabel();

    // Linear Search manual
    for (int i = 0; i < dataList.size(); i++) {
        if (dataList[i].getSensorID() == target) {
            tampilkanBarisData(dataList[i], i + 1);
            ditemukan = true;
        }
    }

    cout << "=================================================================================================================\n";

    if (!ditemukan) {
        cout << "Data dengan Sensor ID " << target << " tidak ditemukan.\n";
    }
}

void bubbleSortByTemperature(vector<EnvironmentRecord>& dataList) {
    int n = dataList.size();

    // Bubble Sort manual, descending
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (dataList[j].getTemperature() < dataList[j + 1].getTemperature()) {
                EnvironmentRecord temp = dataList[j];
                dataList[j] = dataList[j + 1];
                dataList[j + 1] = temp;
            }
        }
    }
}

void bubbleSortByCO(vector<EnvironmentRecord>& dataList) {
    int n = dataList.size();

    // Bubble Sort manual, descending
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (dataList[j].getCOValue() < dataList[j + 1].getCOValue()) {
                EnvironmentRecord temp = dataList[j];
                dataList[j] = dataList[j + 1];
                dataList[j + 1] = temp;
            }
        }
    }
}

void bubbleSortByLPG(vector<EnvironmentRecord>& dataList) {
    int n = dataList.size();

    // Bubble Sort manual, descending
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (dataList[j].getLPGValue() < dataList[j + 1].getLPGValue()) {
                EnvironmentRecord temp = dataList[j];
                dataList[j] = dataList[j + 1];
                dataList[j + 1] = temp;
            }
        }
    }
}

void tampilkanDataTerurut(int mode) {
    vector<EnvironmentRecord> dataList = bacaDataSemua();

    if (dataList.empty()) {
        cout << "Belum ada data untuk diurutkan.\n";
        return;
    }

    if (mode == 1) {
        bubbleSortByTemperature(dataList);
        cout << "\nData berhasil diurutkan berdasarkan suhu tertinggi.\n";
    } else if (mode == 2) {
        bubbleSortByCO(dataList);
        cout << "\nData berhasil diurutkan berdasarkan CO tertinggi.\n";
    } else if (mode == 3) {
        bubbleSortByLPG(dataList);
        cout << "\nData berhasil diurutkan berdasarkan LPG tertinggi.\n";
    }

    tampilkanHeaderTabel();

    for (int i = 0; i < dataList.size(); i++) {
        tampilkanBarisData(dataList[i], i + 1);
    }

    cout << "=================================================================================================================\n";
}

int main() {
    int pilihan;

    do {
        cout << "\n=== SISTEM MONITORING SUHU, CO, DAN LPG ===\n";
        cout << "1. Jalankan Server Monitoring\n";
        cout << "2. Jalankan Client Dummy Sensor\n";
        cout << "3. Tampilkan semua data\n";
        cout << "4. Cari data berdasarkan Sensor ID\n";
        cout << "5. Urutkan berdasarkan suhu tertinggi\n";
        cout << "6. Urutkan berdasarkan CO tertinggi\n";
        cout << "7. Urutkan berdasarkan LPG tertinggi\n";
        cout << "8. Keluar\n";
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                runServer();
                break;

            case 2:
                runClient();
                break;

            case 3:
                tampilkanSemuaData();
                break;

            case 4:
                cariBerdasarkanSensorID();
                break;

            case 5:
                tampilkanDataTerurut(1);
                break;

            case 6:
                tampilkanDataTerurut(2);
                break;

            case 7:
                tampilkanDataTerurut(3);
                break;

            case 8:
                cout << "Program selesai.\n";
                break;

            default:
                cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 8);

    return 0;
}