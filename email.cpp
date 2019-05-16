/*
  Lab: Final Project
  Programmer: Joshua Ruelas
  Editor used: Sublime Text 3
  Compiler used: MinGW
  Description: This program will prompt user for filename and print the inputs
*/

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <deque>

using namespace std;

class toLower {public: char operator()(char c) const{return tolower(c);}};

bool validEmailChar(char c) {
	
	// Check for valid characters
	if(c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c >= '0' && c <= '9' || c == '.' || c == '-' || c== '_' || c == '+')
		return true;
	else return false;
}

int main() {

	// Initialize variables
	deque<string> email;
	deque<string> lEmail;
	bool hasDot = false;
	string inFile;
	string outFile;
	string dInputFile = "fileContainingEmails.txt";
	string dOutputFile = "copyPasteMyEmails.txt";

	// Prompt user for input filename
	cout << "Enter input file name [default: fileContainingEmails.txt]: ";
	getline(cin, inFile);

	// Check for input filename
	if(inFile.length() <= 0)
		inFile = dInputFile;
	else if(inFile.length() > 0)
		dOutputFile = inFile;

	// Open input file
	fstream fin;
	fin.open(inFile.c_str());
	if(!fin.good()) throw "I/O Error!";

	// Prompt user for output filename
	cout << "Enter output file name [default: " << dOutputFile << "]: ";
	getline(cin, outFile);

	// Check for output filename
	if(outFile.length() <= 0)
		outFile = dOutputFile;

	// Loop to read file
	while(!fin.eof()) {

		// Initialize variable
		string file;

		// Read file
		fin >> file;

		// Check for @ symbol
		int a = -1;
		for(int i = 0; i < file.length(); i++) {
			if(file[i] == '@') {
				a = i;

				// Check for start of email
				bool valid = true;
				int s = -1;
				for(int j = a - 1; valid; j--) {
					valid = validEmailChar(file[j]);
					if(!valid) {
						s = j;
					}
				}

				// Check for end of email
				valid = true;
				int e = -1;
				for(int k = a + 1; valid; k++) {
					valid = validEmailChar(file[k]);
					if(!valid) {
						e = k - s - 1;
					}
				}
				
				// Check for valid email
				bool check = validEmailChar(file[s+1]);
				if(check) {
					// Check for period in email
					bool hasDot = false;
					string aEmail = file.substr(s + 1, e);
					for(int l = 0; l < aEmail.length(); l++) {
						if(aEmail[l] == '.')
							hasDot = true;
					}

					// Save email
					if(hasDot)
						email.push_back(aEmail);

					// Check for duplicates
					bool exist = false;
					string lcEmail = aEmail;
					transform(lcEmail.begin(), lcEmail.end(), lcEmail.begin(), toLower());
					for(int m = 0; m < email.size(); m++)
						if(lEmail[m] == lcEmail) {
							exist = true;
							email.pop_back(); // remove if already exists
						}

						// Save email in duplicate check list
						if(hasDot && !exist)
							lEmail.push_back(lcEmail);
					}
				}
			}
		}

		if(!email.empty()) {
			
			// Open and write to output file
			fstream fout;
			fout.open(outFile.c_str());
			if(!fout.good()) throw "I/O error!";
			for(int i = 0; i < email.size(); i++) {
				fout << email[i] << "; ";
				if(i == email.size() - 1)
					fout << email[i];
			}
			for(int j = 0; j < email.size(); j++)
				cout << email[j] << endl;
			cout << endl << email.size() << " emails found in " << inFile  << " and copied to " << outFile << endl;
			cout << endl << "To use the emails, open the output file, copy and paste them in to";
			cout << endl << "the \'bcc\' field to avoid displaying all of the email addresses in the email sent.";
			cout << endl << "Respecting personal privacy is highly encouraged and appreciated." << endl;

			// Close output file
			fout.close();
			fout.clear();
		}

		else
			cout << "No valid emails found in " << inFile << endl;

		// Close input file
		fin.close();
		fin.clear();

		// Display file names
		cout << endl << "Input filename: " << inFile;
		cout << endl << "Output filename: " << outFile;
	}