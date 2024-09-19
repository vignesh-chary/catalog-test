#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <json.hpp> // JSON library
#include <cmath>
#include <iomanip>

using json = nlohmann::json;

// Function to convert value from any base to base 10, using stoll for large numbers
long long convertToBase10(const std::string &value, int base)
{
    return std::stoll(value, nullptr, base); // Converts a string to base 10 integer (long long)
}

// Function to calculate Lagrange Interpolation and find the constant term 'c'
double lagrangeInterpolation(const std::vector<long long> &x_values, const std::vector<long long> &y_values, int k)
{
    double c = 0.0; // This will store the constant term

    // Lagrange interpolation formula
    for (int i = 0; i < k; ++i)
    {
        double term = y_values[i]; // Start with the y_i value

        for (int j = 0; j < k; ++j)
        {
            if (i != j)
            {
                term *= static_cast<double>(0 - x_values[j]) / (x_values[i] - x_values[j]);
            }
        }
        c += term;
    }

    return c;
}

// Function to generate combinations of k elements from a vector of indices
void generateCombinations(int n, int k, std::vector<std::vector<int>> &combinations)
{
    std::vector<bool> v(n);
    std::fill(v.begin(), v.begin() + k, true);

    do
    {
        std::vector<int> combination;
        for (int i = 0; i < n; ++i)
        {
            if (v[i])
                combination.push_back(i);
        }
        combinations.push_back(combination);
    } while (std::prev_permutation(v.begin(), v.end()));
}

int main()
{
    // Open the JSON file
    std::ifstream file("testcase2.json");
    if (!file.is_open())
    {
        std::cerr << "Could not open the file!" << std::endl;
        return 1;
    }

    // Parse the JSON data
    json data;
    file >> data;

    // Extract the number of points (n) and the required number of points (k) for the polynomial
    int n = data["keys"]["n"];
    int k = 7; // Now we are selecting 7 points for each combination

    std::vector<long long> x_values;
    std::vector<long long> y_values;

    // Reading x, y values and converting y to base 10
    for (int i = 1; i <= n; ++i)
    {
        if (data.contains(std::to_string(i)))
        {
            std::string base = data[std::to_string(i)]["base"];
            std::string value = data[std::to_string(i)]["value"];

            // Convert y values to base 10 using stoll for large integers
            long long y = convertToBase10(value, std::stoi(base));
            y_values.push_back(y);
            x_values.push_back(i); // The key (i) serves as x in this case
        }
    }

    // Generate all possible combinations of 7 points from the 9 available points
    std::vector<std::vector<int>> combinations;
    generateCombinations(n, k, combinations);

    // Map to store the constant term (c-value) and its frequency
    std::map<long long, int> c_map;

    // Iterate over each combination and calculate the constant term using Lagrange interpolation
    std::cout << "All constant (c) values obtained:\n";
    for (const auto &combination : combinations)
    {
        std::vector<long long> x_comb, y_comb;

        // Extract x and y values for the current combination
        for (int idx : combination)
        {
            x_comb.push_back(x_values[idx]);
            y_comb.push_back(y_values[idx]);
        }

        // Calculate the constant term (c) using Lagrange interpolation
        double constant_term = lagrangeInterpolation(x_comb, y_comb, k);

        // Round the constant term to the nearest whole number
        long long rounded_c = std::llround(constant_term);

        // Store the rounded constant term in the map and increment its frequency
        c_map[rounded_c]++;

        // Print each rounded c-value
        std::cout << rounded_c << "\n";
    }

    // Find the maximum frequency and the corresponding constant term
    long long max_c_value = 0;
    int max_frequency = 0;

    for (const auto &entry : c_map)
    {
        if (entry.second > max_frequency)
        {
            max_frequency = entry.second;
            max_c_value = entry.first;
        }
    }

    // Output the most frequent constant term and its frequency
    std::cout << "\nMost frequent constant term (c): " << max_c_value << std::endl;
    std::cout << "Frequency: " << max_frequency << std::endl;

    return 0;
}
