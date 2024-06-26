#include <iostream>
#include <cmath>
#include "DataVector.h"

using namespace std;

// Constructor with an optional dimension parameter
DataVector::DataVector(int dimension)
{
    setDimension(dimension);
}

// Destructor
DataVector::~DataVector()
{
    // No explicit memory deallocation needed for std::vector
}

// Copy constructor
DataVector::DataVector(const DataVector &other) : v(other.v) {}

// Copy assignment operator
DataVector &DataVector::operator=(const DataVector &other)
{
    if (this != &other)
    {
        v = other.v;
    }
    return *this;
}

// Set the dimension of the vector
void DataVector::setDimension(int dimension)
{
    // Clear existing vector and resize with zeros
    v.clear();
    v.resize(dimension, 0.0);
}

// Overloaded equality operator for vector comparison
bool DataVector::operator==(const DataVector &other)
{
    // Check if dimensions are the same
    if (v.size() != other.v.size())
    {
        return false;
    }

    if(v == other.v)
    {
        return true;
    }
    return false;

}

// Overloaded addition operator for vector addition
DataVector DataVector::operator+(const DataVector &other)
{
    // Check if dimensions are the same
    if (v.size() != other.v.size())
    {
        cout << "Error!! The dimensions of the two vectors are not the same, hence the operation is not valid";
        return DataVector();
    }

    // Perform vector addition
    DataVector result(v.size());
    for (size_t i = 0; i < v.size(); ++i)
    {
        result.v[i] = v[i] + other.v[i];
    }
    return result;
}

// Overloaded subtraction operator for vector subtraction
DataVector DataVector::operator-(const DataVector &other)
{
    // Check if dimensions are the same
    if (v.size() != other.v.size())
    {
        cout << "Error!! The dimensions of the two vectors are not the same, hence the operation is not valid";
        return DataVector();
    }

    // Perform vector subtraction
    DataVector result(v.size());
    for (size_t i = 0; i < v.size(); ++i)
    {
        result.v[i] = v[i] - other.v[i];
    }
    return result;
}

// Overloaded multiplication operator for dot product
double DataVector::operator*(const DataVector &other)
{
    // Calculate dot product
    double result = 0.0;
    for (size_t i = 0; i < v.size(); ++i)
    {
        result += v[i] * other.v[i];
    }
    return result;
}

// Calculate the Euclidean norm of the vector
double DataVector::norm()
{
    return sqrt(*this * *this);
}

// Calculate the Euclidean distance to another vector
double DataVector::dist(const DataVector &other)
{
    DataVector diff = *this - other;
    return diff.norm();
}

// Set the value of an element at a specific index
void DataVector::setElement(int index, double value)
{
    if (index >= 0 && index < v.size())
    {
        v[index] = value;
    }
    else
    {
        cout << "The given index is out of bounds\n";
    }
}

// Get the dimension of the vector
long long DataVector::getDimension()
{
    return v.size();
}

// Get the value of an element at a specific index
double DataVector::getDimension(int index)
{
    if (index >= 0 && index < v.size())
        return v[index];
    else
    {
        throw out_of_range("This index is out of range.\n");
    }
}
