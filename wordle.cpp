#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include <vector>
#include <map>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void buildWords(
    const std::string& templateIn,
    std::string& working,
    size_t index,
    std::map<char, int>& floatingCount,
    const std::set<std::string>& dict,
    std::set<std::string>& out);
bool inDict(const std::set<std::string>& dict, const std::string& s);


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    //std::vector<char> floatingChars = toCharArray(floating);
    //std::vector<char> fixedChars = toCharArray(in);
    // -i--
    // dn
    // Add your code here

    std::set<std::string> results;

    std::string working = in;
    std::map<char, int> floatingCount;
    // initialize counts of floating words
    for (char letter: floating) floatingCount[letter]++;

    buildWords(in, working, 0, floatingCount, dict, results);
    return results;
    

}

// Define any helper functions here

void buildWords(
    const std::string& templateIn,
    std::string& working,
    size_t index,
    std::map<char, int>& floatingCount,
    const std::set<std::string>& dict,
    std::set<std::string>& out)
{
    if (index == working.size()) {
        // we have filled all positions, add the word to results
        for (auto& charCount: floatingCount) {
            // lol nevermind case
            if (charCount.second > 0) return;
        }
        // check if word is a real word
        if (dict.find(working) != dict.end()) {
            out.insert(working);
        }
        return;
    }

    if (templateIn[index] != '-') {
        // a fixed letter, increase index to move to next character
        working[index] = templateIn[index];
        buildWords(templateIn, working, index+1, floatingCount, dict, out);
        return;
    }

    // try remaining floating letters
    for (auto& floatingChar: floatingCount) {
        char letter = floatingChar.first;
        int& letterCount = floatingChar.second;
        if (letterCount == 0) continue;
        working[index] = letter;
        letterCount--;
        buildWords(templateIn, working, index+1, floatingCount, dict, out);
        letterCount++; // backtrack
    }

    // otherwise try letter from a-z
    for (char c = 'a'; c <= 'z'; ++c) {
        auto it = floatingCount.find(c);
        if (it != floatingCount.end() && it->second > 0) continue; // avoid duplicating choices handled above
        working[index] = c;
        buildWords(templateIn, working, index+1, floatingCount, dict, out);
    }

}

bool inDict(const std::set<std::string>& dict, const std::string& s) {
    return dict.find(s) != dict.end();
}