#include <iostream>
#include "server.h"
#include "client.h"

using namespace std;

int main() {
    int pilihan;

    do {
        cout << "\n=== SISTEM MONITORING SUHU, CO, DAN LPG ===\n";
        cout << "1. Jalankan Server Monitoring\n";
        cout << "2. Jalankan Client Dummy Sensor\n";
        cout << "3. Keluar\n";
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
                cout << "Program selesai.\n";
                break;

            default:
                cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 3);

    return 0;
}