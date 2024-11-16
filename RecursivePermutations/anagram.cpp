// This performs all these tasks RECURSIVELY and WITHOUT loops
// Finds all permutations of a given string
// Finds if given permutation is a valid word
// Stores valid words in an array
// Prints valid words from the array
// You may have to increase your stack size to run this program with larger dictionary files.

#include <iostream>
#include <fstream>
#include <istream>
#include <string>
using namespace std;


// YOU MAY WANT TO CHANGE THESE VALUES FOR LARGER DICTIONARY FILES
const int MAXRESULTS = 20;    
const int MAXDICTWORDS = 30  ; 


int auxLoadR(istream& dictFile, string dict[], int index, bool gotLine) {
    if (gotLine || index >= MAXDICTWORDS) {
        return 0;
    }
        
    getline(dictFile, dict[index]);

    gotLine = dictFile.eof();

    return 1 + auxLoadR(dictFile, dict, index + 1, gotLine);

}


int loadR(istream& dictfile, string dict[]) {
    int dictSize = auxLoadR(dictfile, dict, 0, false);
    return dictSize;
}

bool isDuplicate(const string &word, string results[], int index, int max) {
    if (index >= max)
        return false;
    else
        if (results[index] == word)
            return true;
    return isDuplicate(word, results, index + 1, max);
}

bool inDict(string word, const string dict[], const int dictSize, string results[], int i) {

    if (i >= dictSize)  //Base case, you can't index PAST the end of the dictionary
        return false;
    
    if (dict[i] == word && !(isDuplicate(word, results, 0, i))) //Check if its actually a word AND if its not a duplicate
        return true;

    return inDict(word, dict, dictSize, results, i + 1); //If we're not at the end of the dictionary 
                                                         //NOR have we found a matching word iterate through recursively

}


void auxPermute(string prefix, string rest, const string dict[], const int dictSize, string results[], int& resultsIndex, int i) {

    if (rest.size() == 0) { // Base Case
        if (inDict(prefix, dict, dictSize, results,0)) {
            results[resultsIndex] = prefix; // Store the generated permutation
            resultsIndex++; // Increment index
        }
        return;
    }
   
    if (i >= rest.size()) { // Base Case
        return;
    }
   
   
    string newPrefix = prefix + rest[i]; // Update prefix
    string newRest = rest.substr(0, i) + rest.substr(i + 1); // Exclude character `rest[i]` from rest

    auxPermute(newPrefix, newRest, dict, dictSize, results, resultsIndex, 0); //explore permutation with rest[i] in prefix
    auxPermute(prefix, rest, dict, dictSize, results, resultsIndex, i + 1); // explore permutation with the rest[i+1] NEXT letter at the end of prfix
   
}


void permute(string prefix, string rest, const string dict[], const int dictSize, string results[], int& resultsIndex) {

    auxPermute(prefix, rest, dict, dictSize, results, resultsIndex, 0);

}

int anagramR(string word, const string dict[], int size, string results[])
{
    int num = 0;
    permute("", word, dict, size, results, num);

    return num;
}

void auxPrintR(string results[], int size, int index) {
    if (index >= size)
        return;

    cout << results[index] << endl;
    auxPrintR(results, size, index+1);
}

void printR(string results[], int size) {
    auxPrintR(results, size, 0);
}

int main() {
    string dictionary = "";
    ifstream inFile;
    
    cout << "Please enter the dictionary file name: ";
    cin >> dictionary;
    cout << endl;

    inFile.open(dictionary);

    if (inFile.fail()) {
        cout << "Failed to read inFile";
        return 0;
    }

    string dict[MAXDICTWORDS];
    int numDictSize = loadR(inFile, dict);
    
    string str;
    string results[MAXRESULTS];
    cout << "Please enter a string to find its valid permutations: ";
    cin >> str;
    cout << endl;
   

    // Call permute and pass an initial resultsIndex of 0
    int numPermutations = anagramR(str, dict, 6, results);

    // Print results
    cout << "There are a total of " << numPermutations << " valid permutation(s) in the given string: " << str << endl;
    cout << "These are its valid permutation(s): " << endl;
    printR(results, numPermutations);

    inFile.close();

    return 0;
}


