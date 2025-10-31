#include <bits/stdc++.h>
using namespace std;



struct Prediction {
    double nextValue;
    string patternType;
    double confidence;
};

Prediction predictNext(const vector<double>& seq) {
    int n = seq.size();
    if (n < 3) return {0, "Insufficient data", 0.0};

    vector<double> diff, ratio;
    for (int i = 1; i < n; i++) {
        diff.push_back(seq[i] - seq[i - 1]);
        if (seq[i - 1] != 0)
            ratio.push_back(seq[i] / seq[i - 1]);
    }

    // 🧮 Check arithmetic pattern
    bool isArithmetic = all_of(diff.begin() + 1, diff.end(),
        [&](double d){ return fabs(d - diff[0]) < 1e-6; });

    // 🧠 Check geometric pattern
    bool isGeometric = all_of(ratio.begin() + 1, ratio.end(),
        [&](double r){ return fabs(r - ratio[0]) < 1e-6; });

    // 🔁 Check alternating pattern
    bool isAlternating = true;
    if (n >= 4) {
        for (int i = 2; i < n; i++)
            if (fabs(seq[i] - seq[i-2]) > 1e-6)
                isAlternating = false;
    } else isAlternating = false;

    // 🌀 Check recursive difference pattern (like Fibonacci)
    bool isRecursive = true;
    if (n >= 3) {
        for (int i = 2; i < n; i++)
            if (fabs(seq[i] - (seq[i - 1] + seq[i - 2])) > 1e-6)
                isRecursive = false;
    } else isRecursive = false;

    // 🎯 Decide prediction
    if (isArithmetic) {
        return {seq.back() + diff[0], "Arithmetic Progression", 0.95};
    } else if (isGeometric) {
        return {seq.back() * ratio[0], "Geometric Progression", 0.95};
    } else if (isAlternating) {
        return {seq[n - 2], "Alternating Sequence", 0.85};
    } else if (isRecursive) {
        return {seq[n - 1] + seq[n - 2], "Recursive (Fibonacci-like)", 0.9};
    }

    // 🧩 Fallback: approximate trend by regression
    double sumX = 0, sumY = 0, sumXY = 0, sumXX = 0;
    for (int i = 0; i < n; i++) {
        sumX += i;
        sumY += seq[i];
        sumXY += i * seq[i];
        sumXX += i * i;
    }

    double slope = (n * sumXY - sumX * sumY) / (n * sumXX - sumX * sumX);
    double intercept = (sumY - slope * sumX) / n;
    double nextValue = slope * n + intercept;

    return {nextValue, "Linear Regression Trend", 0.75};
}

int main() {
    vector<vector<double>> testCases = {
        {2, 4, 6, 8, 10},
        {3, 9, 27, 81},
        {5, 2, 5, 2, 5, 2},
        {1, 1, 2, 3, 5, 8},
        {10, 12, 15, 19, 24}
    };

    for (auto seq : testCases) {
        auto result = predictNext(seq);
        cout << "Sequence: ";
        for (auto v : seq) cout << v << " ";
        cout << "\n→ Predicted Next: " << result.nextValue
             << " (" << result.patternType
             << ", Confidence: " << result.confidence * 100 << "%)\n\n";
    }

    return 0;
}
