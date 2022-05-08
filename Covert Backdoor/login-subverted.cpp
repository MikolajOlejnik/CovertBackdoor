// Imports the relavent libraries.
#include <iostream>
#include <string>
#include <sstream>
#include <atomic>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <vector>
#include <cstdio>
#include "authlib.h"
#include "openssl/sha.h"
#include <regex>
using namespace std;

/* 
 * Returns the hashed version (as string) of the string input.
 */
string sha256_string(const string strToHash)
{
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, strToHash.c_str(), strToHash.size());
  SHA256_Final(hash, &sha256);
  stringstream hashedString;

  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
  {
    hashedString << hex << setw(2) << setfill('0') << (int)hash[i];
  }
  return hashedString.str();
}

/*
 * Parses ciphertext and splits it.
 */
vector<string> split (const string &s, char delim) {
  vector<string> result;
  stringstream ss(s);
  string item;

  // Splits ciphertext.
  while (getline(ss, item, delim)) {
    result.push_back(item);
  }
  return result;
}

/*
 * Matches details to their ciphertext versions to see if they're 
 * the same in the openfile.
*/
bool loginCheck(char* uName, char* pWord) {

  char fileName[] = "3791570.txt";
  ifstream openFile(fileName);
  char delim = ':';

  // Reads file.
	if (openFile.is_open())
	{
		string line;
    getline(openFile, line);

    // Reads and separates.
    do
    {
      vector<string> temp = split(line, delim);

      // Compares plaintext to ciphertext.
      if (strcmp(uName, temp.at(0).c_str()) == 0) {
        if (strcmp(sha256_string(pWord).c_str(), temp.at(1).c_str()) == 0) {
          
          openFile.close();
          return true; // If successful.
        }
      }
      getline(openFile, line); // Reads openfile again.

    } while (!openFile.eof()); 

		openFile.close();
	}
	else
	{
		// Error openFile doesn't open.
    cerr << "Error@loginCheck(): Can't read: " << fileName << endl;
	}
  return false;
}

/*
 * Shows authenticated message.
 */
void authSuccess(char* name) {
  authenticated(name);
}

/*
 * Main program.
 */
int main() {
  char userNameInput[20], passwordInput[20];
  
  // Prompts user to input username.
  printf("Enter your username: ");
  scanf("%[^\n]s", userNameInput);
  
  // Prompts user to enter password.
  printf("Enter your password: ");
  stdin = freopen(NULL,"r",stdin); // Clears input.
  scanf("%[^\n]s", passwordInput);

  // Authenticates.
  if (loginCheck(userNameInput, passwordInput))
    authSuccess(userNameInput); // Correct password grants access.
  else
    rejected(userNameInput); // Incorrect password rejects access.
}
