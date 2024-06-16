#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <thread>
#include <iomanip>

using namespace std;

#define Escape 27
#define Enter 13

#define MAX_TANK_VOLUME 60
#define IDLE_CONSUMPTION 0.0003  //расход топлива на холостом ходу (л/сек)

class Tank
{
    const double MAX_VOLUME;
    double fuel_level;

public:
    double get_MAX_VOLUME() const
    {
        return MAX_VOLUME;
    }

    double get_fuel_level() const
    {
        return fuel_level;
    }

    void fill(double amount)
    {
        if (amount < 0) return;
        fuel_level += amount;
        if (fuel_level > MAX_VOLUME) fuel_level = MAX_VOLUME;
    }

    double give_fuel(double amount)
    {
        double fuel_given = (fuel_level >= amount) ? amount : fuel_level;
        fuel_level -= fuel_given;
        return fuel_given;
    }

    bool in_reserve() const
    {
        return fuel_level < 5;
    }

    Tank(double volume) : MAX_VOLUME(MAX_TANK_VOLUME), fuel_level(volume)
    {
        if (fuel_level > MAX_VOLUME) fuel_level = MAX_VOLUME;
        cout << "Tank is ready " << this << endl;
    }

    ~Tank()
    {
        cout << "Tank is over " << this << endl;
    }

    void info() const
    {
        cout << "Fuel level: " << fuel_level << " liters\n";
        if (in_reserve()) cout << "Fuel is in reserve!\n";
    }
};

class Engine
{
    bool is_started;

public:
    void start()
    {
        is_started = true;
    }

    void stop()
    {
        is_started = false;
    }

    bool started() const
    {
        return is_started;
    }

    Engine() : is_started(false)
    {
        cout << "Engine is ready: " << this << endl;
    }

    ~Engine()
    {
        cout << "Engine is over: " << this << endl;
    }

    void info() const
    {
        cout << "Engine is " << (is_started ? "started\n" : "stopped\n");
    }
};

class Car
{
    Engine engine;
    Tank tank;
    bool driver_inside;

    struct
    {
        std::thread panel_thread;
    } control_threads;

    void panel()
    {
        while (driver_inside)
        {
            system("CLS");
            cout << "Engine is " << (engine.started() ? "started\n" : "stopped\n");
            tank.info();
            Sleep(100);
        }
    }

public:
    Car(double fuel) : tank(fuel), driver_inside(false)
    {
        cout << "Your car is ready: " << this << endl;
        cout << "Press Enter to get in: " << endl;
    }

    ~Car()
    {
        if (control_threads.panel_thread.joinable())
        {
            control_threads.panel_thread.join();
        }
        cout << "Car is over: " << this << endl;
    }

    void control()
    {
        char key;
        do
        {
            key = _getch();
            switch (key)
            {
            case Enter: driver_inside ? get_out() : get_in(); break;
            case 'a': if (driver_inside) engine.start(); break; //нажать 'a' для запуска двигателя
            case 'd': if (driver_inside) engine.stop(); break; //нажать 'd' для остановки двигателя
            case Escape: get_out(); break;
            case ' ': if (driver_inside) start_fuel_consumption(); break;
            }
        } while (key != Escape);
    }

    void get_in()
    {
        driver_inside = true;
        control_threads.panel_thread = std::thread(&Car::panel, this);
    }

    void get_out()
    {
        driver_inside = false;
        if (control_threads.panel_thread.joinable()) control_threads.panel_thread.join();
        system("CLS");
        cout << "You have exited the car\n";
    }

    void start_fuel_consumption()
    {
        while (driver_inside && engine.started() && tank.get_fuel_level() > 0)
        {
            double fuel_used = IDLE_CONSUMPTION;
            if (tank.give_fuel(fuel_used) < fuel_used)
            {
                engine.stop();
                cout << "Engine stopped due to low fuel. Tank is empty.\n";
            }
            Sleep(5);
        }
    }

    void run()
    {
        while (driver_inside)
        {
            if (engine.started())
            {
                double fuel_used = IDLE_CONSUMPTION;
                if (tank.give_fuel(fuel_used) < fuel_used)
                {
                    engine.stop();
                    cout << "Engine stopped due to low fuel. Tank is empty.\n";
                }
            }
        }
    }
};

int main()
{
    setlocale(LC_ALL, "");

    Car car(6);
    std::thread car_thread(&Car::run, &car);
    car.control();
    car_thread.join();

    return 0;
}
