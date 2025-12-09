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
    int floatingRemaining,
    int blankPositionsLeft,
    const std::set<std::string>& dict,
    std::set<std::string>& out);



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

    // Count blank positions once
    int blankCount = 0;
    for (char c : in) {
        if (c == '-') blankCount++;
    }

    buildWords(in, working, 0, floatingCount, floating.size(), blankCount, dict, results);
    return results;
    

}

// Define any helper functions here

void buildWords(
    const std::string& templateIn,
    std::string& working,
    size_t index,
    std::map<char, int>& floatingCount,
    int floatingRemaining,
    int blankPositionsLeft,
    const std::set<std::string>& dict,
    std::set<std::string>& out)
{
    
    if (floatingRemaining > blankPositionsLeft) {
        return;  // can't place all floating letters
    }

    // base case: all positions are filled
    if (index == working.size()) {
        if (floatingRemaining != 0) return;
        // check if it's a real word
        if (dict.find(working) != dict.end()) out.insert(working);
        return;
    }

    // case 1 that we have a fixed word
    if (templateIn[index] != '-') {
        buildWords(templateIn, working, index+1, floatingCount, floatingRemaining, blankPositionsLeft, dict, out);
        return;
    }

    // if floating letters equals blank positions we have to use only floating letters
    if (floatingRemaining == blankPositionsLeft) {
        for (auto& pair : floatingCount) {
            if (pair.second > 0) {
                working[index] = pair.first;
                pair.second--;
                buildWords(templateIn, working, index+1, floatingCount, floatingRemaining - 1, blankPositionsLeft - 1, dict, out);
                pair.second++;
            }
        }
        // skip other letters
        return; 
    }

    // try floating letters first
    for (auto& pair : floatingCount) {
        if (pair.second > 0) {
            char c = pair.first;
            working[index] = c;
            pair.second--;
            buildWords(templateIn, working, index+1, floatingCount, floatingRemaining - 1, blankPositionsLeft - 1, dict, out);
            pair.second++;
        }
    }

    // otherwise try non-floating letters if we can afford to
    // case 2 try a-z for blank positions
    for (char c = 'a'; c <= 'z'; ++c) {
       if (floatingCount.find(c) == floatingCount.end()) {
            working[index] = c;
            buildWords(templateIn, working, index+1, floatingCount, floatingRemaining, blankPositionsLeft - 1, dict, out);
        }
    }
}

