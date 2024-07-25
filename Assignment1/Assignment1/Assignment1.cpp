/*
Authors:
1. Gevorg Markarov (40273717)
2. Alec Kirakossian  ()
3. Shahe Bannis (40286754)
4. Hrag Bankian (40245363)
*/
#include <iostream>
#include <cmath>
#include <limits>
#include <cctype>

using namespace std;

/**
* Function to create an array of integers of size n
* @param n the size of the array
* @return the pointer to the array
 */
int* createArray(int n) {
    int* arr = new int[n];
    return arr;
}

/**
* Function to initialize the array with values from 0 to n-1
* @param arr the array to initialize
* @param n the size of the array
 */

void initializeArray(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i;
    }
}

/**
* Function to print the array
* @param arr the array to print
 */
void printArray(int* arr, int n) {
    cout << "Array: [";
    for (int i = 0; i < n; i++) {
        cout << arr[i];
        if (i < n - 1) {
            cout << ", ";
        }
    }
    cout << "]\n";
}

/**
* Function to delete the array
* @param arr the array to delete
 */
void deleteArray(int* arr) {
    delete[] arr;
}

// Point class definition
class Point {
private:
    int x, y, z;

public:
    Point(int x = 0, int y = 0, int z = 0) : x(x), y(y), z(z) {}

    ~Point() {
        cout << "Point (" << x << ", " << y << ", " << z << ") destroyed.\n";
    }

    /** 
    * Translate the point along the specified axis by the given distance.
    * @param d the distance to translate
    * @param axis the axis to translate along ('x', 'y', or 'z')
    * @return 0 if successful, -1 if invalid axis, -2 if distance is not finite
     */
    int translate(int d, char axis) {
        if (!std::isfinite(static_cast<double>(d))) {
            return -2;
        }
        switch (axis) {
        case 'x':
            x += d;
            break;
        case 'y':
            y += d;
            break;
        case 'z':
            z += d;
            break;
        default:
            return -1;
        }
        return 0;
    }

    /** 
    * Display the point in the format (x, y, z).
     */
    void display() const {
        cout << "(" << x << ", " << y << ", " << z << ")";
    }

    friend class Triangle;
};

// Triangle class definition
class Triangle {
private:
    Point* vertex_1, * vertex_2, * vertex_3;

public:
    Triangle() : vertex_1(nullptr), vertex_2(nullptr), vertex_3(nullptr) {}

    Triangle(Point* p1, Point* p2, Point* p3) : vertex_1(p1), vertex_2(p2), vertex_3(p3) {}

    ~Triangle() {
        delete vertex_1;
        delete vertex_2;
        delete vertex_3;
        cout << "Triangle destroyed.\n";
    }

    /**
    * Translate the triangle along the specified axis by the given distance.
    * @param d the distance to translate
    * @param axis the axis to translate along ('x', 'y', or 'z')
     */
    void translate(int d, char axis) {
        if (vertex_1) vertex_1->translate(d, axis);
        if (vertex_2) vertex_2->translate(d, axis);
        if (vertex_3) vertex_3->translate(d, axis);
    }

    /**
    * Calculate the area of the triangle.
    * @return the area of the triangle
     */
    double calcArea() const {
        double a = sqrt(pow(vertex_2->x - vertex_1->x, 2) + pow(vertex_2->y - vertex_1->y, 2) + pow(vertex_2->z - vertex_1->z, 2));
        double b = sqrt(pow(vertex_3->x - vertex_2->x, 2) + pow(vertex_3->y - vertex_2->y, 2) + pow(vertex_3->z - vertex_2->z, 2));
        double c = sqrt(pow(vertex_1->x - vertex_3->x, 2) + pow(vertex_1->y - vertex_3->y, 2) + pow(vertex_1->z - vertex_3->z, 2));
        double s = (a + b + c) / 2;
        return sqrt(s * (s - a) * (s - b) * (s - c));
    }

    /**
    * Display the triangle vertices.
     */
    void display() const {
        cout << "Triangle vertices:\n";
        if (vertex_1) vertex_1->display(); else cout << "(null)";
        cout << "\n";
        if (vertex_2) vertex_2->display(); else cout << "(null)";
        cout << "\n";
        if (vertex_3) vertex_3->display(); else cout << "(null)";
        cout << "\n";
    }
};


/**
* Get a valid integer input from the user.
* @return the valid integer input
 */
int getValidInt() {
    int value;
    while (true) {
        cin >> value;
        if (cin.fail()) {
            cin.clear(); // clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
            cout << "Invalid input. Please enter a valid integer: ";
        }
        else {
            return value;
        }
    }
}

/**
* Menu function to create and manipulate a triangle.
 */
void menu() {
    Triangle* triangle = nullptr;
    char choice = 'L';

    do {
        cout << "\nMenu:\n";
        cout << "1. Create Triangle\n";
        cout << "2. Translate Triangle\n";
        cout << "3. Display Triangle\n";
        cout << "4. Calculate Area\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        string choices;
        cin >> choices;
        if (choices.size() == 1) {
            choice = choices[0];
            switch (choice) {
            case '1': {
                int x1, y1, z1, x2, y2, z2, x3, y3, z3;
                cout << "Enter coordinates of vertex 1: \n";
                cout << "x: ";
                x1 = getValidInt();
                cout << "y: ";
                y1 = getValidInt();
                cout << "z: ";
                z1 = getValidInt();
                cout << "Enter coordinates of vertex 2: \n";
                cout << "x: ";
                x2 = getValidInt();
                cout << "y: ";
                y2 = getValidInt();
                cout << "z: ";
                z2 = getValidInt();
                cout << "Enter coordinates of vertex 3: \n";
                cout << "x: ";
                x3 = getValidInt();
                cout << "y: ";
                y3 = getValidInt();
                cout << "z: ";
                z3 = getValidInt();
                triangle = new Triangle(new Point(x1, y1, z1), new Point(x2, y2, z2), new Point(x3, y3, z3));
                break;
            }
            case '2': {
                if (triangle) {
                    int d;
                    char axis;
                    cout << "Enter distance and axis\n";
                    cout << "Distance of Translation: ";
                    d = getValidInt();
                    cout << "Axis that is being transformed (x, y or z): ";
                    cin >> axis;
                    if (!cin.fail() && (axis == 'x' || axis == 'y' || axis == 'z')) {
                        triangle->translate(d, axis);
                    }
                    else {
                        cout << "Invalid input.\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                }
                else {
                    cout << "No triangle created.\n";
                }
                break;
            }
            case '3': {
                if (triangle) {
                    triangle->display();
                }
                else {
                    cout << "No triangle created.\n";
                }
                break;
            }
            case '4': {
                if (triangle) {
                    cout << "Area: " << triangle->calcArea() << "\n";
                }
                else {
                    cout << "No triangle created.\n";
                }
                break;
            }
            case '5':
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice.\n";
            }
        }
        else {
            cout << "Invalid choice.\n";
        }
    } while (choice != '5');
    
    

    delete triangle;
}

// Main function
int main() {
    // Test array functions
    int* arr = createArray(10);
    printArray(arr, 10);
    initializeArray(arr, 10);
    printArray(arr, 10);
    deleteArray(arr);


    // Start menu
    menu();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
