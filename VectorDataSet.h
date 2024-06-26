// VectorDataset.h

#ifndef VECTORDATASET_H
#define VECTORDATASET_H
#include<iostream>
#include "DataVector.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class VectorDataset
{
private:
    vector<DataVector> dataset; // Vector to store DataVector objects representing the dataset

public:
    // Default constructor
    VectorDataset() {}

    // Constructor with an initial dataset
    VectorDataset(const vector<DataVector> &data) : dataset(data) {}

    // Destructor
    ~VectorDataset() {dataset.clear();}

    // Get the size of the dataset
    int size() const
    {
        return dataset.size();
    }

    // Get a DataVector from the dataset at a specified index
    DataVector getVector(int index) const
    {
        if (index < dataset.size())
        {
            return dataset[index];
        }
        else
        {
            // Return an empty DataVector if the index is out of bounds
            return DataVector();
        }
    }

    // Read dataset from a CSV file and populate the vector dataset
    void readCSV(const string &filename)
    {
        ifstream file(filename); // Open the CSV file
        if (!file.is_open())
        {
            cout << "Error opening file!\n";
            exit(2); // Exit the program with an error code
        }

        vector<int> labels; // Store image labels
        string line;

        // Skip the first line (header) of the CSV file
        getline(file, line);

        // Read each line from the CSV file
        while (getline(file, line))
        {
            stringstream ss(line);
            vector<double> row;
            int label;
            string value;

            // Parse each value from the line and convert to double
            while (getline(ss, value, ','))
            {
                double pixelValue = stod(value);
                row.push_back(pixelValue);
            }

            // Create a DataVector from the parsed values
            DataVector datavector(row.size());
            for (int i = 0; i < row.size(); i++)
            {
                datavector.setElement(i, row[i]);
            }

            // Add the DataVector to the dataset
            dataset.push_back(datavector);
        }
    }
    void add_vector(DataVector v)
    {
        dataset.push_back(v);
    }

    void remove_vector(int index)
    {
        dataset.erase(dataset.begin() + index);
    }

    
};

#endif // VECTORDATASET_H
