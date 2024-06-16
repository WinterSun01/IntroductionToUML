#include <Windows.h>
#include <iostream>
#include <conio.h>
using namespace std;

#define MIN_TANK_VOLUME 20
#define MAX_TANK_VOLUME 150

class Tank
{
    const int VOLUME;
    double fuel_level;
public:

    int get_VOLUME() const
    {
        return VOLUME;
    }

    double get_fuel_level() const
    {
        return fuel_level;
    }

    void fill(double amount)
    {
        if (amount < 0)
            return;
        fuel_level += amount;
        if (fuel_level > VOLUME)
            fuel_level = VOLUME;
    }

    double give_fuel(double amount)
    {
        double fuel_given = (fuel_level >= amount) ? amount : fuel_level;
        fuel_level -= fuel_given;
        return fuel_given;
    }

    Tank(int volume) : VOLUME(
        volume < MIN_TANK_VOLUME ? MIN_TANK_VOLUME :
        volume > MAX_TANK_VOLUME ? MAX_TANK_VOLUME :
        volume)
    {
        fuel_level = 0;
        cout << "Tank is ready " << this << endl;
    }

    ~Tank()
    {
        cout << "Tank is over " << this << endl;
    }

    void info() const
    {
        cout << "Volume:   " << VOLUME << " liters;\n";
        cout << "Fuel level: " << get_fuel_level() << " liters;\n";
    }
};

class Engine
{
    const double VOLUME;
    const double CONSUMPTION;
    double consumption_per_second;
    bool is_started;

public:

    Engine(double volume, double consumption)
        : VOLUME(volume), CONSUMPTION(consumption), is_started(false)
    {
        consumption_per_second = CONSUMPTION / 100.0 / 3600.0;
        cout << "Engine is ready " << this << endl;
    }

    ~Engine()
    {
        cout << "Engine is over " << this << endl;
    }

    void start()
    {
        is_started = true;
        cout << "Engine started" << endl;
    }

    void stop()
    {
        is_started = false;
        cout << "Engine stopped" << endl;
    }

    bool started() const
    {
        return is_started;
    }

    double get_consumption_per_second() const
    {
        return consumption_per_second;
    }
};

class Car
{
    Tank tank;
    Engine engine;
    unsigned int speed;
    const unsigned int MAX_SPEED;
    const unsigned int WEIGHT;
    const unsigned int hp;
    bool driver_inside;
public:

    Car(int tank_volume, double engine_volume, double engine_consumption, unsigned int max_speed, unsigned int weight, unsigned int hp)
        : tank(tank_volume), engine(engine_volume, engine_consumption), speed(0), MAX_SPEED(max_speed), WEIGHT(weight), hp(hp), driver_inside(false)
    {
        cout << "Car is ready " << this << endl;
    }

    ~Car()
    {
        cout << "Car is over " << this << endl;
    }

    void fill(double amount)
    {
        tank.fill(amount);
    }

    void get_in()
    {
        driver_inside = true;
        cout << "Driver is inside" << endl;
    }

    void get_out()
    {
        driver_inside = false;
        cout << "Driver is outside" << endl;
    }

    void start_engine()
    {
        if (driver_inside)
        {
            if (tank.get_fuel_level() > 0)
            {
                engine.start();
            }
            else
            {
                cout << "Cannot start engine: tank is empty" << endl;
            }
        }
        else
        {
            cout << "Driver is not inside" << endl;
        }
    }

    void stop_engine()
    {
        engine.stop();
    }

    void accelerate()
    {
        if (engine.started())
        {
            if (speed < MAX_SPEED)
            {
                if (tank.get_fuel_level() > 0)
                {
                    speed += 10;
                    double fuel_consumed = engine.get_consumption_per_second() * speed; // Простая модель потребления топлива
                    tank.give_fuel(fuel_consumed);
                    cout << "Accelerating. Speed: " << speed << " km/h" << endl;
                    cout << "Fuel consumed: " << fuel_consumed << " liters" << endl;
                }
                else
                {
                    cout << "Cannot accelerate: tank is empty" << endl;
                    stop_engine();
                }
            }
            else
            {
                cout << "Already at max speed" << endl;
            }
        }
        else
        {
            cout << "Engine is not started" << endl;
        }
    }

    void slow_down()
    {
        if (speed > 0)
        {
            speed -= 10;
            cout << "Slowing down. Speed: " << speed << " km/h" << endl;
        }
        else
        {
            cout << "Car is already stopped" << endl;
        }
    }

    void info() const
    {
        cout << "Car info: \n";
        cout << "Speed: " << speed << " km/h\n";
        cout << "Max speed: " << MAX_SPEED << " km/h\n";
        cout << "Weight: " << WEIGHT << " kg\n";
        cout << "HP: " << hp << " hp\n";
        cout << "Driver inside: " << (driver_inside ? "Yes" : "No") << "\n";
        tank.info();
    }
};

void main()
{
    setlocale(LC_ALL, "");

    Car car(150, 2.0, 8.0, 200, 1500, 150);

    int choice;
    double amount;
    while (true)
    {
        cout << "\n1. Fill the tank\n";
        cout << "2. Get in the car\n";
        cout << "3. Get out of the car\n";
        cout << "4. Start the engine\n";
        cout << "5. Stop the engine\n";
        cout << "6. Accelerate\n";
        cout << "7. Slow down\n";
        cout << "8. Show car info\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "Enter amount of fuel to fill: ";
            cin >> amount;
            car.fill(amount);
            break;
        case 2:
            car.get_in();
            break;
        case 3:
            car.get_out();
            break;
        case 4:
            car.start_engine();
            break;
        case 5:
            car.stop_engine();
            break;
        case 6:
            car.accelerate();
            break;
        case 7:
            car.slow_down();
            break;
        case 8:
            car.info();
            break;
        case 9:
            return;
        default:
            cout << "Invalid choice!" << endl;
        }
    }
}
