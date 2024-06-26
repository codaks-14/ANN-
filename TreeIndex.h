// TreeIndex.h
#pragma once
#include <vector>
#include <iostream>
#include <queue>
#include <iostream>
#include "VectorDataSet.h"
using namespace std;
class Node
{
public:
    Node(vector<int> data, Node *left_ = NULL, Node *right_ = NULL, double median_ = 0, int depth_ = 0) : left(left_), right(right_), depth(depth_), dataset(data), median(median_) {}
    ~Node()
    {
        dataset.clear();
        delete left;
        delete right;
    }
    Node *left;
    Node *right;
    vector<int> dataset;
    int depth;
    double median;
};

class Node1
{
public:
    Node1(vector<int> data, Node1 *left_ = NULL, Node1 *right_ = NULL, double median_value_ = 0, vector<double> split_vector_ = {0}) : left(left_), right(right_), dataset(data), median_value(median_value_), split_vector(split_vector_) {}
    ~Node1()
    {
        dataset.clear();
        delete left;
        delete right;
    }
    Node1 *left;
    Node1 *right;
    vector<int> dataset;
    double median_value;
    vector<double> split_vector;
};

class TreeIndex
{
protected:
    TreeIndex();
    ~TreeIndex();
    VectorDataset dataset_;

public:
    static TreeIndex &GetInstance();
    void read_dataset(const string &filename);
};

class KDTreeIndex : public TreeIndex
{
public:
    static KDTreeIndex &GetInstance();
    void MakeTree();
    pair<vector<bool>, double> ChooseRule(vector<int> &dataset, int depth);
    void printTree();
    VectorDataset Search(DataVector &query, int k);
    void RemoveData(DataVector &data);
    void AddData(DataVector &data);

private:
    KDTreeIndex();
    ~KDTreeIndex();
    Node *root_;
    const int M = 1;
    Node *BuildKDTree(vector<int> &dataset_, int depth);
    void printSubTree(Node *node, int depth);
    void SearchSubTree(Node *node, DataVector &query, int k, priority_queue<pair<double, int>> &neighbours);
};

class RPTreeIndex : public TreeIndex
{
public:
    static RPTreeIndex &GetInstance();
    void MakeTree();
    pair<pair<vector<bool>, vector<double>>, double> ChooseRule(vector<int> &dataset, int depth);
    void printTree();
    VectorDataset Search(DataVector &query, int k);
    void RemoveData(DataVector &data);
    void AddData(DataVector &data);

private:
    RPTreeIndex();
    ~RPTreeIndex();
    Node1 *root_;
    const int M = 1;
    Node1 *BuildRPTree(vector<int> &dataset_, int depth);
    void printSubTree(Node1 *node, int depth);
    void SearchSubTree(Node1 *node, DataVector &query, int k, priority_queue<pair<double, int>> &neighbours);
};
