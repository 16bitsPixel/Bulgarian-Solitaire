#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

//function prototypes
void getInput(int& pileTotal, vector<int>& arr);
void playGame(vector<int>& arr, vector<string>& repeats, bool& isRepeat);
void MergeSort(vector<int>& arr, int start, int end);
void Merge(vector<int>& arr, int start, int midpoint, int end);
void printPartition(vector<int>& arr, vector<string>& repeats, bool& isRepeat);
bool checkRepeats(vector<string> repeats, string current);

int main() {
	//variables
	int totalPile;
	vector<int> arrPiles;
	vector<string> repeats;
	bool repeatFound = false;
	
	//gets input from user
	getInput(totalPile, arrPiles);
	
	//prints the current partition, adds a partition to the repeats vector
	printPartition(arrPiles, repeats, repeatFound);
	
	//takes one card from every pile and makes a new pile until a repeated partition has been found
	while (!repeatFound) {
		playGame(arrPiles, repeats, repeatFound);
	}
	
	cout << "The game is over, a repeat has been found";
	
	return 0;
}

/*
Function to get user input, asking for number of piles and values of piles
@param
	pileTotal - reference to the sum of all piles
	arr - reference to the array for all piles
@return
	nothing
*/
void getInput(int& pileTotal, vector<int>& arr) {
	bool redo;
	do {
		redo = false;
		cout << "Enter a number for the sum of all piles: ";
		cin >> pileTotal;
		if (pileTotal < 0 || cin.fail()) {
			cout << "Invalid option. (Sum can't be negative or a string). Please try again.\n\n";
			cin.clear();
			cin.ignore(256, '\n');
			redo = true;
		}
	}
	while(redo);
	cout << endl;
	
	int tempTotal = pileTotal; //used for while loop to fill array
	int tempPileNum; //used to get the value for each pile
	while (tempTotal != 0) {
		do {
			redo = false;
			cout << "Enter how many cards are in a pile: ";
			cin >> tempPileNum;
			if (tempPileNum <= 0 || cin.fail()) {
				cout << "Invalid option. (Number must be greater than 0, can't be a string). Please try again.\n\n";
				cin.clear();
				cin.ignore(256, '\n');
				redo = true;
			}
		}
		while(redo);
		
		if (tempTotal - tempPileNum < 0) {
			arr.push_back(tempTotal);
			tempTotal = 0;
		}
		else {
			arr.push_back(tempPileNum);
			tempTotal -= tempPileNum;
		}
		cout << "There are now " << tempTotal << " cards left" << endl;
	}
	cout << endl;
}

/*
Function that plays bulgarian solitaire until a repeat happens
@param
	arr - reference to the piles of cards
	repeats - reference to vector of already found partitions
	isRepeat - reference to bool of if a repeat has been found
@return
	nothing
*/
void playGame(vector<int>& arr, vector<string>& repeats, bool& isRepeat) {
	int newPile = 0;	//used to count cards in new pile
	int zeroCount = 0;	//used to count how many piles are discarded
	
	//takes one card from each pile and increments the newPile variable
	for (int i = 0; i < arr.size(); i++) {
		//if a pile only has one card in it, zeroCount is incremented
		if (arr.at(i) == 1) {
			zeroCount++;
		}
		arr.at(i)--;
		newPile++;
	}
	
	//takes out the zeroes from the pile
	for (int i = 0; i < zeroCount; i++) {
		arr.pop_back();
	}
	
	//adds the newest pile
	arr.push_back(newPile);
	
	//prints the current partition
	printPartition(arr, repeats, isRepeat);
}

/*
MergeSort sorts the array by recursively splitting the array in half and then merging
Pre:
	arr - array that will be sorted
	start - the starting index of the array
	end - the last index of the array
Post:
	splits the array in half and calls the merge function once everything is split
Return:
	nothing
*/
void MergeSort(vector<int>& arr, int start, int end) {
	int midpoint = 0;
	
	if (start < end) {
		//finds the midpoint of the array
		midpoint = (start + end) / 2;
		
		//splits the array into two halves
		MergeSort(arr, start, midpoint);
		MergeSort(arr, midpoint + 1, end);
	
		//merges the split arrays in sorted order
		Merge(arr, start, midpoint, end);
	}
}

/*
Merges the arrays in greatest to least order
Pre:
	arr - reference to vector to be sorted
	start - the starting index of the array
	midpoint - the middle value of array
	end - the last index of the array
Post:
	merges the array given into sorted order
Return:
	nothing
*/
void Merge(vector<int>& arr, int start, int midpoint, int end) {
	/*
	Variables
		merged size - size of partition
		mergePos - index of next sorted value
		leftPos - index of left partition
		rightPos - index of right position
		mergedNumbers - temporary array to add sorted values to
	*/
	int mergedSize = end - start + 1;
	int mergePos = 0;
	int leftPos = start;
	int rightPos = midpoint + 1;
	int mergedNumbers[mergedSize];
	
	//adds an element from the left/right partition to the temp array in greatest to least order
	while (leftPos <= midpoint && rightPos <= end) {
		if (arr.at(rightPos) < arr.at(leftPos)) {
			mergedNumbers[mergePos] = arr.at(leftPos);
			++leftPos;
		}
		else {
			mergedNumbers[mergePos] = arr.at(rightPos);
			++rightPos;
		}
		++mergePos;
	}
	
	//if the left partition is empty, add the rest of the right partition
	while (leftPos <= midpoint) {
		mergedNumbers[mergePos] = arr.at(leftPos);
		++leftPos;
		++mergePos;
	}
	
	//if the right partition is empty, add the rest of the left partition
	while (rightPos <= end) {
		mergedNumbers[mergePos] = arr.at(rightPos);
		++rightPos;
		++mergePos;
	}
	
	//copies the values of the temporary array to the original array
	for (mergePos = 0; mergePos < mergedSize; ++mergePos) {
		arr.at(start + mergePos) = mergedNumbers[mergePos];
	}
}

/*
function to print the current partition, update the repeats vector and isRepeat
@param
	arr - reference to current piles
	repeats - reference to all past partitions
	isRepeat - reference to if a repeat has been found
*/
void printPartition(vector<int>& arr, vector<string>& repeats, bool& isRepeat) {
	//sorts the partition in descending order
	MergeSort(arr, 0, arr.size() - 1);
	
	stringstream output;
	cout << "Current Partition:" << endl;
	for (int a : arr) {
		output << a << " ";
	}
	repeats.push_back(output.str());
	cout << output.str() << endl << endl;
	
	//checks if a repeation has been found and handles it
	isRepeat = checkRepeats(repeats, output.str());
	if (isRepeat) {
		cout << "This is a repeated partition" << endl;
	}
}

/*
checks to see if there is a repeated partition
@param
	repeats - vector of past partitions
	current - the current partition to be checked
@return
	true/false if the current partition has been found before
*/
bool checkRepeats(vector<string> repeats, string current) {
	for (int i = 0; i < repeats.size() - 1; i++) {
		if (repeats.at(i) == current) {
			return true;
		}
	}
	return false;
}
