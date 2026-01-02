#include <iostream>
#include <vector>
#include <ctime>
#include <thread>
#include <chrono>
#include <windows.h>


typedef void (*SensorCallback)(float);

struct TemperatureSensor {
    float warningthreshold; 
    float threshold;  
    SensorCallback onwarning;            
    SensorCallback onAlert;      
};

void register_alert_handler(TemperatureSensor& sensor, SensorCallback callback_func) {
    sensor.onAlert = callback_func;
    std::cout << "[System] Обробник події успішно зареєстровано." << std::endl;
}

void register_warning_handler(TemperatureSensor& sensor, SensorCallback callback_func) {
    sensor.onwarning = callback_func;
    std::cout << "[System] Обробник події успішно зареєстровано." << std::endl;
}


void soundAlarm(float temp) {
    std::cout << "\n>>> [ACTION] ВКЛЮЧАЮ СИРЕНУ! Поточна температура критична: " << temp << "°C" << std::endl;
}

void logToFile(float temp) {
    std::cout << "\n>>> [ACTION] ТИХИЙ ЗАПИС: Температура " << temp << "°C занесена в базу даних." << std::endl;
}

void activateCooling(float temp) {
    std::cout << "\n>>> [ACTION] Вмикаю охолодження! Температура зараз: " << temp << "°C " << std::endl;
}

void warning(float temp) {
    std::cout << "\n>>> [ACTION] Попередження! Температура зараз: " << temp << "°C " << std::endl;
}



void run_sensor_simulation(TemperatureSensor& sensor) {
    std::cout << "\n--- Початок моніторингу (Поріг: " << sensor.threshold << "°C) ---" << std::endl;

    std::srand(std::time(nullptr)); 

    for (int i = 1; i <= 8; ++i) {
        float currentTemp = 30.0f + static_cast<float>(std::rand() % 80) / 10.0f;

        std::cout << "Замір #" << i << ": " << currentTemp << "°C" << std::endl;

        if (currentTemp >= sensor.warningthreshold && currentTemp < sensor.threshold) {
            if (sensor.onwarning != nullptr) {
                sensor.onwarning(currentTemp);
            }
            else {
                std::cout << " (Подія ігнорується, бо обробник не призначено)" << std::endl;
            }
        }
        else if (currentTemp >= sensor.threshold) {
            if (sensor.onAlert != nullptr) {
                sensor.onAlert(currentTemp);
            }
            else {
                std::cout << " (Подія ігнорується, бо обробник не призначено)" << std::endl;
            }
        }
    
        std::this_thread::sleep_for(std::chrono::milliseconds(700));
    }
}

int main() {
    SetConsoleOutputCP(1251);
    TemperatureSensor mySensor;
    mySensor.warningthreshold = 36.6f;
    mySensor.threshold = 37.5f; 
    mySensor.onAlert = nullptr; 
    mySensor.onwarning = nullptr;
    std::cout << "Система моніторингу першого порогу готова до налаштування.\n";
    std::cout << "1 - Режим попередження\n2 - Режим охолодження\nОберіть варіант: ";

    int mode;
    std::cin >> mode;
    if (mode == 1) {
        register_warning_handler(mySensor, warning);
    }
    else {
        register_warning_handler(mySensor, activateCooling);
    }
    
    std::cout << "Система моніторингу другого порогу готова до налаштування.\n";
    std::cout << "1 - Режим тривоги (Сирена)\n2 - Режим логування (Файл)\nОберіть варіант: ";

    std::cin >> mode;

    if (mode == 1) {
        register_alert_handler(mySensor, soundAlarm);
    }
    else {
        register_alert_handler(mySensor, logToFile);
    }

    run_sensor_simulation(mySensor);

    return 0;
}
