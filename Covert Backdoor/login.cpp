// Imports the relavent libraries.
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <vector>
#include "authlib.h"
#include "openssl/sha.h"
using namespace std;

/*
 * Takes a string input and returns the hashed version of it.
 */
string sha256_string(const string strToHash)
{
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, strToHash.c_str(), strToHash.size());
  SHA256_Final(hash, &sha256);
  stringstream hashedString;

  // Creates hashed string(stream).
  for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
  {
      hashedString << hex << setw(2) << setfill('0') << (int)hash[i];
  }
  return hashedString.str();
}

/*
 * Reads a specified file and returns the contents of that file as a vector.
 */
vector<string> readFromFile(string fileName) {

  string line;
	ifstream file(fileName); // Creates buffer stream.
  vector<string> details; // To store file contents.

	if (file.is_open()) // Checks if the file can be opened.
	{
		getline(file, line); // Reads first line.
		
    while (!file.eof()) // While not at end of the file.
    {     
      // Saves line to vector and then reads the next line.
      details.push_back(line);
      getline(file, line);
    }
		file.close(); // Closes file.
	}
	else
	{
		// Error message when file can't be opened, found or read.
    cerr << "Error@readFromFile(): Can't read " << fileName << endl;
	}
  return details; // Returns the lines in the file.
}

/*
 * Splits the string input separated by a delimiter input and returns a
 * string vector which contains the split strings.
 */
vector<string> split (const string &s, char delim) {
  vector<string> result;
  stringstream ss (s);
  string item;

  // Splits the string into smaller strings separated by delimiter.
  while (getline (ss, item, delim)) {
    result.push_back (item);
  }
  return result; // Returns the vector containing the split strings.
}

/*
 * Takes in the username and the password (hashes it) as strings and
 * checks them against the entries in the file. Returns true if
 * inputs match an entry.
 */
bool loginCheck(string uName, string pWord, string fileName, char delimiter) {

  // Gets and stores each line in the file.
  vector<string> tempFileDetails = readFromFile(fileName);

  // Loops through the string vector.
  for (int i = 0; i < (int)tempFileDetails.size(); i++)
  {
    // Separates line into username and password.
    vector<string> temp = split(tempFileDetails.at(i), delimiter);

    // Compares username input to username entry in vector.
    if (uName.compare(temp.at(0)) == 0) {

      string hashPWord = sha256_string(pWord); // Hashes password input.

      // Conpares hashed password to password entry in vector.
      if (hashPWord.compare(temp.at(1)) == 0) {
        return true; // If password is right.
      }
    }
  }
	return false; // If details are wrong.
}

/*
 * Main Program. Prompts user for username and password and then
 * attempts to authenticates it.
 */
int main() {
  string fileName = "3791570.txt";
  char delimiter = ':';
  string userNameInput, passwordInput;

  // Prompts user to enter username.
  cout << "Enter your username: ";
  getline(cin, userNameInput);

  // Prompts user to enter password.
  cout << "Enter your password: ";
  getline(cin, passwordInput);

  // Attempts to authenticate username and password and then shows message result.
  if (loginCheck(userNameInput, passwordInput, fileName, delimiter))
    authenticated(userNameInput); // Successful
  else rejected(userNameInput); // Not successful.
}