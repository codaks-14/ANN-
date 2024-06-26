#ifndef DATAVECTOR_H
#define DATAVECTOR_H

#include <vector>

class DataVector
{
    // Vector to store the elements of the data vector
    std::vector<double> v;

public:
    // Constructor with an optional dimension parameter
    DataVector(int dimension = 0);

    // Destructor
    ~DataVector();

    // Copy constructor
    DataVector(const DataVector& other);

    // Copy assignment operator
    DataVector& operator=(const DataVector& other);

    // Set the dimension of the vector
    void setDimension(int dimension = 0);

    // Set the value of an element at a specific index
    void setElement(int index, double value);

    // Get the value of an element at a specific index
    double getDimension(int index);

    // Get the dimension of the vector
    long long getDimension();

    // Overloaded equality operator for vector comparison
    bool operator==(const DataVector& other);

    // Overloaded addition operator for vector addition
    DataVector operator+(const DataVector& other);

    // Overloaded subtraction operator for vector subtraction
    DataVector operator-(const DataVector& other);

    // Overloaded multiplication operator for dot product
    double operator*(const DataVector& other);

    // Calculate the Euclidean norm of the vector
    double norm();

    // Calculate the Euclidean distance to another vector
    double dist(const DataVector& other);
};

#endif
