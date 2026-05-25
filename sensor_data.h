#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

// Abstract class sebagai kontrak dasar data sensor
class SensorRecord {
protected:
    string sensor_id;
    double temperature;
    double co_value;
    double lpg_value;
    string timestamp;

public:
    SensorRecord(string id, double temp, double co, double lpg, string time)
        : sensor_id(id), temperature(temp), co_value(co), lpg_value(lpg), timestamp(time) {}

    virtual bool isDanger() const = 0;
    virtual string getStatus() const = 0;
    virtual void display() const = 0;
    virtual string toJSON() const = 0;

    virtual ~SensorRecord() {}

    string getSensorID() const { return sensor_id; }
    double getTemperature() const { return temperature; }
    double getCOValue() const { return co_value; }
    double getLPGValue() const { return lpg_value; }
    string getTimestamp() const { return timestamp; }
};

// Child class untuk data monitoring lingkungan
class EnvironmentRecord : public SensorRecord {
public:
    EnvironmentRecord(string id, double temp, double co, double lpg, string time)
        : SensorRecord(id, temp, co, lpg, time) {}

    bool isDanger() const override {
        return (temperature > 45 || co_value > 150 || lpg_value > 700);
    }

    string getStatus() const override {
        if (temperature > 45 || co_value > 150 || lpg_value > 700) {
            return "BAHAYA";
        }

        if ((temperature > 35 && temperature <= 45) ||
            (co_value > 50 && co_value <= 150) ||
            (lpg_value > 500 && lpg_value <= 700)) {
            return "WASPADA";
        }

        return "AMAN";
    }

    void display() const override {
        cout << "====================================" << endl;
        cout << "Sensor ID   : " << sensor_id << endl;
        cout << "Suhu        : " << temperature << " C" << endl;
        cout << "CO          : " << co_value << " ppm" << endl;
        cout << "LPG         : " << lpg_value << " / 1023" << endl;
        cout << "Timestamp   : " << timestamp << endl;
        cout << "Status      : " << getStatus() << endl;
        cout << "====================================" << endl;
    }

    string toJSON() const override {
        stringstream ss;

        ss << "{"
           << "\"sensor_id\":\"" << sensor_id << "\","
           << "\"temperature\":" << fixed << setprecision(2) << temperature << ","
           << "\"co_value\":" << fixed << setprecision(2) << co_value << ","
           << "\"lpg_value\":" << fixed << setprecision(2) << lpg_value << ","
           << "\"timestamp\":\"" << timestamp << "\","
           << "\"status\":\"" << getStatus() << "\""
           << "}";

        return ss.str();
    }

    string toTextLine() const {
        stringstream ss;

        ss << sensor_id << "|"
           << fixed << setprecision(2) << temperature << "|"
           << fixed << setprecision(2) << co_value << "|"
           << fixed << setprecision(2) << lpg_value << "|"
           << timestamp << "|"
           << getStatus();

        return ss.str();
    }
};

#endif