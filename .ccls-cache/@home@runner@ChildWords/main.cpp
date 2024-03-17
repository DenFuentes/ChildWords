#include <iostream>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <algorithm>
using namespace std;

unordered_set<string> loadDictionary(const string& file_path) {
    unordered_set<string> dictionary;
    ifstream file(file_path);
    string word;
    while (getline(file, word)) {
        dictionary.insert(word);
    }
    return dictionary;
}

vector<string> generateChildren(const string& word) {
    vector<string> children;
    for (size_t i = 0; i < word.length(); ++i) {
        string child = word.substr(0, i) + word.substr(i + 1);
        children.push_back(child);
    }
    return children;
}

unordered_set<string> findReducibleWords(const string& word, const unordered_set<string>& dictionary, unordered_map<string, unordered_set<string>>& reducible_words) {
    if (reducible_words.find(word) != reducible_words.end()) {
        return reducible_words[word];
    }

    unordered_set<string> reducible;
    for (const string& child : generateChildren(word)) {
        if (dictionary.find(child) != dictionary.end()) {
            auto child_reducible = findReducibleWords(child, dictionary, reducible_words);
            reducible.insert(child_reducible.begin(), child_reducible.end());
        }
    }
    reducible.insert(word);
    reducible_words[word] = reducible;
    return reducible;
}

vector<string> findLongestReducibleWords(const unordered_set<string>& dictionary) {
    unordered_map<string, unordered_set<string>> reducible_words;
    vector<string> longest_words;
    size_t max_length = 0;

    for (const string& word : dictionary) {
        auto reducible = findReducibleWords(word, dictionary, reducible_words);
        if (word.length() > max_length) {
            max_length = word.length();
            longest_words = {word};
        } else if (word.length() == max_length) {
            longest_words.push_back(word);
        }
    }

    return longest_words;
}

int main() {
    string file_path = "yawl.txt";
    unordered_set<string> dictionary = loadDictionary(file_path);
    dictionary.insert("I");
    dictionary.insert("A");

    vector<string> longest_reducible_words = findLongestReducibleWords(dictionary);

    cout << "Longest reducible word(s): ";
    for (const string& word : longest_reducible_words) {
        cout << word << " ";
    }
    cout << endl;

    return 0;
}