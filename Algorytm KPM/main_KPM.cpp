#include <iostream>
#include <string>

using namespace std;

// Print the current state of LPS computation
void printLPSState(const string &pattern, int i, int length, int *lps) {
    cout << "i = " << i+1 << " (P[i] = " << pattern[i] << "), " << "length = " << length << ", LPS: [";
    for (int k = 0; k <= i; k++) {
        cout << lps[k];
        if (k < i) cout << " ";
    }
    cout << "]\n";
}

// Compute the LPS (Longest Prefix Suffix) array step-by-step
void computeLPSArray(const string &pattern, int *lps) {
    int m = (int)pattern.size();
    int length = 0;  // length of the longest prefix suffix
    lps[0] = 0;      // LPS[0] = 0 by definition
    int i = 1;
    
    cout << "Building the LPS array (Π):\n";
    cout << "Pattern: " << pattern << "\n";
    cout << "Indexing i from 1 for clarity as in presentation:\n\n";
    cout << "Initially: i=1, lps[1]=0\n";
    
    while (i < m) {
        if (pattern[i] == pattern[length]) {
            length++;
            lps[i] = length;
            printLPSState(pattern, i, length, lps);
            i++;
        } else {
            if (length != 0) {
                length = lps[length - 1];
            } else {
                lps[i] = 0;
                printLPSState(pattern, i, length, lps);
                i++;
            }
        }
    }
    cout << "\nCompleted LPS array:\n[ ";
    for (int idx = 0; idx < m; idx++)
        cout << lps[idx] << " ";
    cout << "]\n\n";
}

// Print the current matching state as in presentations
void printMatchState(const string &text, const string &pattern, int s, int q) {
    cout << "Text:    ";
    for (int i = 0; i < (int)text.size(); i++) {
        cout << text[i] << " ";
    }
    cout << "\nPattern: ";
    // Print spaces to show shift
    for (int i = 0; i < s; i++) cout << "  ";
    for (int i = 0; i < (int)pattern.size(); i++) {
        cout << pattern[i] << " ";
    }
    cout << "\nCurrent shift s = " << s << ", q = " << q << " (length of current match)\n\n";
}

// KMP search demonstrating internal steps
void KMPSearchSteps(const string &text, const string &pattern, int *lps) {
    int n = (int)text.size();
    int m = (int)pattern.size();

    cout << "KMP Matching Steps:\n";
    cout << "Text: " << text << "\nPattern: " << pattern << "\n\n";
    
    int i = 0; // index in text
    int j = 0; // index in pattern
    
    printMatchState(text, pattern, i-j, j);
    
    while (i < n) {
        if (text[i] == pattern[j]) {
            i++;
            j++;
            printMatchState(text, pattern, i-j, j);
            
            if (j == m) {
                // Found a full match
                cout << "Full match found at position " << (i-j) << "\n\n";
                j = lps[j-1];
                printMatchState(text, pattern, i-j, j);
            }
        } else {
            // mismatch
            if (j != 0) {
                int old_j = j;
                j = lps[j-1]; 
                printMatchState(text, pattern, i-j, j);
            } else {
                i++;
                printMatchState(text, pattern, i-j, j);
            }
        }
    }
}

int main() {
    // Example from the slides:
    // Text:    b a c b a b b a a b a b
    // Pattern: a b a b b a b c a
    string text    = "bacbabbaabab";
    string pattern = "ababbabca";
    
    int m = (int)pattern.size();
    int *lps = new int[m];

    // Compute LPS array step-by-step
    computeLPSArray(pattern, lps);

    // Demonstrate the search process step-by-step
    KMPSearchSteps(text, pattern, lps);

    delete[] lps;
    return 0;
}
