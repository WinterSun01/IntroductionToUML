#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

#define Number_Pi 3.14159265358979323846
#define MIN_SIZE 3 // Минимальный размер для всех фигур

using namespace std;

class Shape
{
public:
    virtual void primaryProperties() const = 0;
    virtual void secondaryProperties() const = 0;
    virtual void draw() const = 0;
    virtual ~Shape() = default;
};

class Square : public Shape
{
    double side;

public:
    Square(double s) : side(s) {}

    void primaryProperties() const override
    {
        cout << "Square: side = " << side << endl;
    }

    void secondaryProperties() const override
    {
        double diagonal = side * sqrt(2);
        double area = side * side;
        double perimeter = 4 * side;
        cout << "Diagonal = " << diagonal << ", Area = " << area << ", Perimeter = " << perimeter << endl;
    }

    void draw() const override
    {
        cout << "Drawing a square:" << endl;
        for (int i = 0; i < side; i++)
        {
            for (int j = 0; j < side; j++)
            {
                cout << "* ";
            }
            cout << endl;
        }
    }
};

class Rectangle : public Shape
{
    double width, height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}

    void primaryProperties() const override
    {
        cout << "Rectangle: width = " << width << ", height = " << height << endl;
    }

    void secondaryProperties() const override
    {
        double diagonal = sqrt(width * width + height * height);
        double area = width * height;
        double perimeter = 2 * (width + height);
        cout << "Diagonal = " << diagonal << ", Area = " << area << ", Perimeter = " << perimeter << endl;
    }

    void draw() const override
    {
        cout << "Drawing a rectangle:" << endl;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                cout << "* ";
            }
            cout << endl;
        }
    }
};

class Circle : public Shape
{
    double radius;

public:
    Circle(double r) : radius(r) {}

    void primaryProperties() const override
    {
        cout << "Circle: radius = " << radius << endl;
    }

    void secondaryProperties() const override
    {
        double circumference = 2 * Number_Pi * radius;
        double area = Number_Pi * radius * radius;
        cout << "Circumference = " << circumference << ", Area = " << area << endl;
    }

    void draw() const override
    {
        cout << "Drawing a circle:" << endl;
        const int diameter = 2 * radius;

        for (int i = 0; i <= diameter; ++i)
        {
            for (int j = 0; j <= diameter; ++j)
            {
                double distance = sqrt((i - radius) * (i - radius) + (j - radius) * (j - radius));
                if (distance <= radius)
                {
                    cout << "* ";
                }
                else
                {
                    cout << "  ";
                }
            }
            cout << endl;
        }
    }
};

class Triangle : public Shape
{
protected:
    double a, b, c;

public:
    Triangle(double side1, double side2, double side3) : a(side1), b(side2), c(side3)
    {
        if (a + b <= c || a + c <= b || b + c <= a)
        {
            throw invalid_argument("Invalid triangle sides");
        }
    }

    void primaryProperties() const override
    {
        cout << "Triangle: sides = " << a << ", " << b << ", " << c << endl;
    }

    void secondaryProperties() const override
    {
        double s = (a + b + c) / 2;
        double area = sqrt(s * (s - a) * (s - b) * (s - c));
        double perimeter = a + b + c;
        cout << "Area = " << area << ", Perimeter = " << perimeter << endl;
    }

    void draw() const override
    {
        cout << "Drawing a triangle:" << endl;
        int height = static_cast<int>(a);

        for (int i = 1; i <= height; i++)
        {
            for (int j = 1; j <= i; j++)
            {
                cout << "* ";
            }
            cout << endl;
        }
    }
};

class EquilateralTriangle : public Triangle
{
public:
    EquilateralTriangle(double side) : Triangle(side, side, side) {}

    void primaryProperties() const override
    {
        cout << "Equilateral Triangle: side = " << a << endl;
    }
};

class RightTriangle : public Triangle
{
public:
    RightTriangle(double base, double height) : Triangle(base, height, sqrt(base* base + height * height)) {}

    void primaryProperties() const override
    {
        cout << "Right Triangle: base = " << a << ", height = " << b << endl;
    }
};

class ScaleneTriangle : public Triangle
{
public:
    ScaleneTriangle(double side1, double side2, double side3) : Triangle(side1, side2, side3) {}

    void primaryProperties() const override
    {
        cout << "Scalene Triangle: sides = " << a << ", " << b << ", " << c << endl;
    }
};

class ShapeFactory
{
public:
    static Shape* createRandomShape()
    {
        int shapeType = rand() % 7; // Increased to accommodate new triangle types

        switch (shapeType)
        {
        case 0: return new Square(rand() % (10 - MIN_SIZE + 1) + MIN_SIZE);
        case 1: return new Rectangle(rand() % (10 - MIN_SIZE + 1) + MIN_SIZE, rand() % (10 - MIN_SIZE + 1) + MIN_SIZE);
        case 2: return new Circle(rand() % (10 - MIN_SIZE + 1) + MIN_SIZE);
        case 3: return createRandomTriangle();
        case 4: return new EquilateralTriangle(rand() % (10 - MIN_SIZE + 1) + MIN_SIZE);
        case 5: return new RightTriangle(rand() % (10 - MIN_SIZE + 1) + MIN_SIZE, rand() % (10 - MIN_SIZE + 1) + MIN_SIZE);
        case 6: return new ScaleneTriangle(rand() % (10 - MIN_SIZE + 1) + MIN_SIZE, rand() % (10 - MIN_SIZE + 1) + MIN_SIZE, rand() % (10 - MIN_SIZE + 1) + MIN_SIZE);
        default: return nullptr;
        }
    }

    static Shape* createRandomTriangle()
    {
        double a, b, c;

        do
        {
            a = rand() % (10 - MIN_SIZE + 1) + MIN_SIZE;
            b = rand() % (10 - MIN_SIZE + 1) + MIN_SIZE;
            c = rand() % (10 - MIN_SIZE + 1) + MIN_SIZE;
        } while (a + b <= c || a + c <= b || b + c <= a);
        return new Triangle(a, b, c);
    }
};

int main() {
    srand(time(0));

    const int numShapes = 10;
    Shape* shapes[numShapes];

    for (int i = 0; i < numShapes; ++i)
    {
        shapes[i] = ShapeFactory::createRandomShape();
    }

    for (int i = 0; i < numShapes; ++i)
    {
        shapes[i]->primaryProperties();
        shapes[i]->secondaryProperties();
        shapes[i]->draw();
        cout << endl;
    }

    for (int i = 0; i < numShapes; ++i)
    {
        delete shapes[i];
    }

    return 0;
}
