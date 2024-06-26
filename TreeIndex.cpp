#include <iostream>
#include <vector>
#include <math.h>
#include <random>
#include "TreeIndex.h"
#include<time.h>
#include "VectorDataSet.h"
#include <algorithm>
#include <queue>

using namespace std;

// constructor of Tree Index
TreeIndex::TreeIndex()
{
    dataset_ = {};
}

// destructor of Tree Index
TreeIndex::~TreeIndex()
{
}

// get instance of the tree index
TreeIndex &TreeIndex::GetInstance()
{
    static TreeIndex instance;
    return instance;
}

// search for the k-nearest neighbors and return the result
VectorDataset KDTreeIndex::Search(DataVector &query, int k)
{
    VectorDataset result;
    priority_queue<pair<double, int>> neighbours;
    SearchSubTree(root_, query, k, neighbours);
    // Filling neighbours in the result
    while (!neighbours.empty())
    {
        result.add_vector(dataset_.getVector(neighbours.top().second));
        cout << neighbours.top().second << " ";
        neighbours.pop();
    }

    return result;
}

// search for the k-nearest neighbors in the subtree
void KDTreeIndex::SearchSubTree(Node *node, DataVector &query, int k, priority_queue<pair<double, int>> &neighbours)
{
    if (node == NULL)
    {
        return;
    }
    // if dataset size is gretaer than or equal to k and less than 2*k then search in the dataset
    if (node->dataset.size() >= k && node->dataset.size() < 2 * k)
    {
        for (int i = 0; i < node->dataset.size(); i++)
        {
            DataVector point = dataset_.getVector(node->dataset[i]);
            double dist = query.dist(point);
            neighbours.push(make_pair(dist, node->dataset[i]));
            if (neighbours.size() > k)
            {
                neighbours.pop();
            }
        }
        return;
    }
    int splitDimension = node->depth % dataset_.getVector(0).getDimension();
    double plane_dist = abs(query.getDimension(splitDimension) - node->median);
    if (query.getDimension(splitDimension) <= node->median)
    {
        SearchSubTree(node->left, query, k, neighbours);
        if (plane_dist <= neighbours.top().first || neighbours.size() < k)
        {
            SearchSubTree(node->right, query, k, neighbours);
        }
    }
    else
    {
        SearchSubTree(node->right, query, k, neighbours);
        if (plane_dist <= neighbours.top().first || neighbours.size() < k)
        {
            SearchSubTree(node->left, query, k, neighbours);
        }
    }
    return;
}

// constructor of KD Tree Index
KDTreeIndex::KDTreeIndex()
{
    // initialize the KD tree index
    root_ = NULL;
}

// destructor of KD Tree Index

KDTreeIndex::~KDTreeIndex()
{
    // clear the KD tree index
    delete root_;
}

// get instance of the KD tree index
KDTreeIndex &KDTreeIndex::GetInstance()
{
    static KDTreeIndex instance;
    return instance;
}

// add data to the KD tree index
void KDTreeIndex::AddData(DataVector &data)
{
    if (data.getDimension() != dataset_.getVector(0).getDimension())
    {
        cout << "Dimension mismatch" << endl;
        return;
    }
    dataset_.add_vector(data);
    MakeTree();
}

// remove data from the KD tree index
void KDTreeIndex::RemoveData(DataVector &data)
{
    if (data.getDimension() != dataset_.getVector(0).getDimension())
    {
        cout << "Dimension mismatch" << endl;
        return;
    }
    for (int i = 0; i < dataset_.size(); i++)
    {
        if (dataset_.getVector(i) == data)
        {
            dataset_.remove_vector(i);
            break;
        }
    }
    MakeTree();
}

// make the KD tree
void KDTreeIndex::MakeTree()
{
    delete root_;
    root_ = NULL;
    vector<int> data;
    // cout << "Dataset size: " << dataset_.size() << endl;
    for (int i = 0; i < dataset_.size(); i++)
    {
        data.push_back(i);
    }
    root_ = BuildKDTree(data, 0);
}

// build the KD tree
Node *KDTreeIndex::BuildKDTree(vector<int> &data, int depth)
{
    if (data.size() == 0)
        return NULL;
    if (data.size() <= M)
    {
        return new Node(data, NULL, NULL, dataset_.getVector(data[0]).getDimension(depth % (dataset_.getVector(data[0]).getDimension())));
    }

    // Choose the rule
    pair<vector<bool>, double> Node_data = ChooseRule(data, depth);
    vector<bool> rule = Node_data.first;
    double medianValue = Node_data.second;
    // Partition dataset based on the rule
    vector<int> leftDataset;
    vector<int> rightDataset;
    for (int i = 0; i < data.size(); i++)
    {
        if (rule[i])
        {
            leftDataset.push_back(data[i]);
        }
        else
        {
            rightDataset.push_back(data[i]);
        }
    }

    // Recursively construct the left and right subtrees
    Node *leftTree = BuildKDTree(leftDataset, depth + 1);
    Node *rightTree = BuildKDTree(rightDataset, depth + 1);
    // Create and return the internal node
    return new Node(data, leftTree, rightTree, medianValue);
}

// choose the rule for the KD tree
pair<vector<bool>, double> KDTreeIndex::ChooseRule(vector<int> &data, int depth)
{
    int dimensions = dataset_.getVector(0).getDimension();
    vector<bool> lessThanMedian(data.size(), false);

    // Choose the dimensions to split based on the current depth
    int splitDimension = depth % dimensions;

    // Calculate median value for the chosen dimension
    vector<double> values;
    for (int i = 0; i < data.size(); i++)
    {
        DataVector point = dataset_.getVector(data[i]);
        values.push_back(point.getDimension(splitDimension));
    }
    sort(values.begin(), values.end());
    double medianValue = values[(values.size() - 1) / 2];

    // Set lessThanMedian based on whether each coordinate is less than the median
    int cnt = (values.size() + 1) / 2;
    for (int i = 0; i < data.size(); ++i)
    {
        DataVector point = dataset_.getVector(data[i]);
        if (point.getDimension(splitDimension) < medianValue)
        {
            lessThanMedian[i] = 1;
            cnt--;
        }
    }
    for (int i = 0; i < data.size(); ++i)
    {
        if (cnt == 0)
            break;
        DataVector point = dataset_.getVector(data[i]);
        if (point.getDimension(splitDimension) == medianValue)
        {
            lessThanMedian[i] = 1;
            cnt--;
        }
    }

    return {lessThanMedian, medianValue};
}

// read the dataset from the CSV file
void TreeIndex::read_dataset(const string &filename)
{
    dataset_.readCSV(filename);
    cout << "Dataset read successfully" << endl;
}

// print the KD tree
void KDTreeIndex::printTree()
{
    printSubTree(root_, 0);
}

// print the KD tree
void KDTreeIndex::printSubTree(Node *node, int depth)
{
    // printing level by level
    if (node == NULL)
    {
        return;
    }
    cout << "Depth: " << depth << "--> ";
    for (int i = 0; i < node->dataset.size(); i++)
    {
        cout << node->dataset[i] << " ";
    }
    // cout<<"Median: "<<node->median<< " ";
    cout << endl;
    printSubTree(node->left, depth + 1);
    printSubTree(node->right, depth + 1);
}

// remove the data from the dataset
void RPTreeIndex::RemoveData(DataVector &data)
{
    if (data.getDimension() != dataset_.getVector(0).getDimension())
    {
        cout << "Dimension mismatch" << endl;
        return;
    }
    for (int i = 0; i < dataset_.size(); i++)
    {
        if (dataset_.getVector(i) == data)
        {
            dataset_.remove_vector(i);
            break;
        }
    }
    MakeTree();
}

// add the data to the dataset
void RPTreeIndex::AddData(DataVector &data)
{
    if (data.getDimension() != dataset_.getVector(0).getDimension())
    {
        cout << "Dimension mismatch" << endl;
        return;
    }
    dataset_.add_vector(data);
    MakeTree();
}

// constructor of RP Tree Index
RPTreeIndex::RPTreeIndex()
{
    // initialize the RP tree index
    root_ = NULL;
}

// destructor of RP Tree Index
RPTreeIndex::~RPTreeIndex()
{
    // clear the RP tree index
    delete root_;
}

// get instance of the RP tree index
RPTreeIndex &RPTreeIndex::GetInstance()
{
    static RPTreeIndex instance;
    return instance;
}

// make the RP tree
void RPTreeIndex::MakeTree()
{
    delete root_;
    vector<int> data;

    for (int i = 0; i < dataset_.size(); i++)
    {
        data.push_back(i);
    }
    root_ = BuildRPTree(data, 0);
    cout << "Tree made successfully" << endl;
}

// build the RP tree
Node1 *RPTreeIndex::BuildRPTree(vector<int> &data, int depth)
{
    // cout<<depth<<'\n';
    // Base case: if the size of dataset is less than M, return a leaf node
    if (data.size() == 0)
        return NULL;
    if (data.size() <= 1)
    {
        return new Node1(data);
    }

    // Choose the rule
    pair<pair<vector<bool>, vector<double>>, double> values = ChooseRule(data, depth);
    vector<bool> rule = values.first.first;
    vector<double> split_vector = values.first.second;
    double medianValue = values.second;

    // Partition dataset based on the rule
    std::vector<int> leftDataset;
    std::vector<int> rightDataset;
    for (int i = 0; i < data.size(); i++)
    {
        if (rule[i])
        {
            leftDataset.push_back(data[i]);
        }
        else
        {
            rightDataset.push_back(data[i]);
        }
    }

    // Recursively construct the left and right subtrees
    Node1 *leftTree = BuildRPTree(leftDataset, depth + 1);
    Node1 *rightTree = BuildRPTree(rightDataset, depth + 1);
    // Create and return the internal node
    return new Node1(data, leftTree, rightTree, medianValue, split_vector);
}

// choose the rule for the RP tree
pair<pair<vector<bool>, vector<double>>, double> RPTreeIndex::ChooseRule(vector<int> &data, int depth)
{
    int dimensions = dataset_.getVector(0).getDimension();
    vector<bool> rule(data.size(), false);

    vector<double> v(dimensions);
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(-1, 1);
    double vlengthsq = 0;
    for (int i = 0; i < dimensions; i++)
    {
        v[i] = dis(gen);
        vlengthsq += (v[i] * v[i]);
    }
    double vlength = sqrt(vlengthsq);
    for (int i = 0; i < dimensions; i++)
    {
        v[i] /= vlength;
    }
    uniform_int_distribution<int> dis1(0, data.size() - 1);
    int index = dis1(gen);
    DataVector x = dataset_.getVector(data[index]);
    double maxdist = 0;
    DataVector y;
    for (int i = 0; i < data.size(); i++)
    {
        DataVector z = dataset_.getVector(data[i]);
        double dist = 0;
        dist = x.dist(z);
        if (dist > maxdist)
        {
            maxdist = dist;
            y = z;
        }
    }
    double delta = dis(gen) * maxdist * 6 / sqrt(dimensions);
    // cout<<delta<<endl;
    vector<double> values;
    for (int i = 0; i < data.size(); i++)
    {
        DataVector point = dataset_.getVector(data[i]);
        double dotproduct = 0;
        for (int j = 0; j < dimensions; j++)
        {
            dotproduct += (point.getDimension(j)) * v[j];
        }
        values.push_back(dotproduct);
    }
    sort(values.begin(), values.end());
    double medianValue = values[(values.size() - 1) / 2];
    for (int i = 0; i < data.size(); ++i)
    {
        DataVector point = dataset_.getVector(data[i]);
        double dotproduct = 0;
        for (int j = 0; j < dimensions; j++)
        {
            dotproduct += (point.getDimension(j)) * v[j];
        }
        rule[i] = dotproduct <= medianValue + delta;
    }
    // cout<< accumulate(rule.begin(), rule.end(), 0)<<endl;
    return {{rule, v}, medianValue + delta};
}

// print the RP tree
void RPTreeIndex::printTree()
{
    printSubTree(root_, 0);
}

// print the RP tree
void RPTreeIndex::printSubTree(Node1 *node, int depth)
{
    // printing level by level
    if (node == NULL)
    {
        return;
    }
    cout << "Depth: " << depth << " Dataset: ";
    for (int i = 0; i < node->dataset.size(); i++)
    {
        cout << node->dataset[i] << " ";
    }
    cout << endl;
    printSubTree(node->left, depth + 1);
    printSubTree(node->right, depth + 1);
}

// search for the k-nearest neighbors in the RP tree
VectorDataset RPTreeIndex::Search(DataVector &query, int k)
{
    VectorDataset result;
    priority_queue<pair<double, int>> neighbours;
    SearchSubTree(root_, query, k, neighbours);
    while (!neighbours.empty())
    {
        result.add_vector(dataset_.getVector(neighbours.top().second));
        cout << neighbours.top().second << " ";
        neighbours.pop();
    }
    return result;
}

// search for the k-nearest neighbors in the subtree
void RPTreeIndex::SearchSubTree(Node1 *node, DataVector &query, int k, priority_queue<pair<double, int>> &neighbours)
{
    // cout<<neighbours.size()<<endl;
    if (node == NULL)
    {
        return;
    }
    if (node->left == NULL && node->right == NULL)
    {
        for (int i = 0; i < node->dataset.size(); i++)
        {
            DataVector point = dataset_.getVector(node->dataset[i]);
            double dist = query.dist(point);
            neighbours.push(make_pair(dist, node->dataset[i]));
            if (neighbours.size() > k)
            {
                neighbours.pop();
            }
        }
        return;
    }
    double projection = 0;
    for (int i = 0; i < query.getDimension(); i++)
    {
        projection += query.getDimension(i) * node->split_vector[i];
    }
    if (projection <= node->median_value)
    {

        SearchSubTree(node->left, query, k, neighbours);
        if ((neighbours.size() > 0 && projection <= neighbours.top().first) || node->left == NULL)
        {
            SearchSubTree(node->right, query, k, neighbours);
        }
    }
    else
    {
        SearchSubTree(node->right, query, k, neighbours);
        if (neighbours.size() > 0 && projection <= neighbours.top().first || node->right == NULL)
        {
            SearchSubTree(node->left, query, k, neighbours);
        }
    }
    return;
}

int main()
{
    RPTreeIndex &rptree = RPTreeIndex::GetInstance();
    VectorDataset test;
    clock_t start = clock();
    test.readCSV("fmnist-test.csv");
    rptree.read_dataset("fmnist-train.csv");
    clock_t end = clock();
    cout << "Time taken to read the data is : " << fixed << double(end - start) / CLOCKS_PER_SEC << '\n';

    start = clock();
    rptree.MakeTree();
    end = clock();
    cout << "Time taken to make the tree is : " << fixed << double(end - start) / CLOCKS_PER_SEC << '\n';
    // rptree.printTree();
    DataVector query;
    VectorDataset result;
    start = clock();
    for (int i = 0; i <test.size(); i++)
    {
        query = test.getVector(i);
        result = rptree.Search(query, 2);
    }
    end = clock();
    cout << "Time taken to search the tree is : " << fixed << double(end - start) / CLOCKS_PER_SEC << '\n';

    KDTreeIndex &kdtree = KDTreeIndex::GetInstance();
    VectorDataset test1;
    start = clock();
    test1.readCSV("fmnist-test.csv");
    kdtree.read_dataset("fmnist-train.csv");
    end = clock();
    cout << "Time taken to read the data is : " << fixed << double(end - start) / CLOCKS_PER_SEC << '\n';
    start = clock();
    kdtree.MakeTree();
    end = clock();
    cout << "Time taken to make the tree is : " << fixed << double(end - start) / CLOCKS_PER_SEC << '\n';
    DataVector query1;
    VectorDataset result1;
    start = clock();
    for (int i = 0; i <test1.size(); i++)
    {
        query1 = test.getVector(i);
        result1 = kdtree.Search(query1, 2);
    }
    end = clock();
    cout << "Time taken to search the tree is : " << fixed << double(end - start) / CLOCKS_PER_SEC << '\n';

    return 0;
}
