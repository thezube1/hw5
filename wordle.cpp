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

    buildWords(in, working, 0, floatingCount, floating.size(), dict, results);
    return results;
    

}

// Define any helper functions here

void buildWords(
    const std::string& templateIn,
    std::string& working,
    size_t index,
    std::map<char, int>& floatingCount,
    int floatingRemaining,
    const std::set<std::string>& dict,
    std::set<std::string>& out)
{
    // count remaining black positions... we're pruning early
    int blankPositionsLeft = 0;
    for (size_t i = index; i < templateIn.size(); ++i) {
        if (templateIn[i] == '-') {
            blankPositionsLeft++;
        }
    }
    
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
        buildWords(templateIn, working, index+1, floatingCount, floatingRemaining, dict, out);
        return;
    }

    // case 2 try a-z for blank positions
    for (char c = 'a'; c <= 'z'; ++c) {
        working[index] = c;
        
        auto it = floatingCount.find(c);
        // check letter is in the floating letters and if we have unused instances of it
        if (it != floatingCount.end() && it->second > 0) {
            it->second--;
            buildWords(templateIn, working, index+1, floatingCount, floatingRemaining - 1, dict, out);
            it->second++;
            // backtrack
        } else {
            buildWords(templateIn, working, index+1, floatingCount, floatingRemaining, dict, out);
        }
    }
}

