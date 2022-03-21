#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <array>
#include <set>
#include <map>
#include <cmath>
#include "main.h"
using namespace std;

vector<vector<char>> board;
vector<string> dictionary;
map<char, int> frequencyMap;
set<string> playerSolvedWords;

int main() {
  ifstream inputStream ("dictionary.txt");
  string currentWord;
  while (inputStream >> currentWord) {
    dictionary.push_back(currentWord);
  }
  inputStream.close();

  int boardSize;
  std::cout << "Enter the size of the board: ";
  std::cin >> boardSize;

  bool willCreateCustomBoard;
  string sel;
  std::cout <<"Would you like to enter your own board?: (y/n) ";
  cin >> sel;
  willCreateCustomBoard = (sel == "y" || sel == "yes" || sel == "YES");

  bool inGame = true;
  board = createBoard(boardSize);
  set<string> possibleWords = solveBoogleBoard();

  while (inGame) {
    printBoard();

    bool gotSelection = false;
    bool forfeited = false;
    while (!gotSelection) {
      string selection;
      std::cout << "Would you like to guess or forfeit?: ";
      cin >> selection;

      if (selection == "forfeit") {
        gotSelection = true;
        forfeited = true;
      } else if (selection == "guess") {
        gotSelection = true;
      }

    }

    if (forfeited) {
      inGame = false;
    } else {
      guess();
    }
  }
  // board = createBoardFromString("anwotmfsalihusbv");

  std::cout << "YOU FOUND: " << endl;
  int onLine = 0;
  for (auto itr = playerSolvedWords.begin(); itr != playerSolvedWords.end(); itr++) {
      std::cout << *itr << " ";
      onLine++;
      if (onLine > 10) { std::cout << endl; onLine = 0; }
  }

  std::cout << "POSSIBLE WORDS: " << endl;
  onLine = 0;
  for (auto itr = possibleWords.begin(); itr != possibleWords.end(); itr++) {
      std::cout << *itr << " ";
      onLine++;
      if (onLine > 10) { std::cout << endl; onLine = 0; }
  }

  std::cout << endl;
}

vector<vector<char>> createBoardFromString(string input) {
  vector<vector<char>> board;
  if (!(sqrt(input.length()) == int(sqrt(input.length())))) {
    std::cout << "input not in square format" << endl;
    return board;
  } 
  int size = int(sqrt(input.length()));

  for (int row = 0; row < size; row++) {
    vector<char> boardRow;
    for (int col = 0; col < size; col++) {
      boardRow.push_back(input.at(row * size + col));
    }
    board.push_back(boardRow);
  }

  return board;
}

vector<char> getLetterList() {
  string const letterString = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  // string const lettersByFreq = "eariotnslcudpmhgbfykvxzjq";
  vector<char> letters;
  for (int i = 0; i < letterString.length(); i++) {
    letters.push_back(letterString.at(i));
  }
  return letters;
}

vector<vector<char>> createBoard(int size) {
  vector<char> letters = getLetterList();
  
  srand(time(0));
  vector<vector<char>> board;

  for (int row = 0; row < size; row++) {
    vector<char> boardRow;
    for (int col = 0; col < size; col++) {
      if (letters.size() == 0) {
        letters = getLetterList();
      } 
      int selection = rand() % letters.size();
      boardRow.push_back(letters.at(selection));
      letters.erase(letters.begin() + selection);
    }
    board.push_back(boardRow);
  }

  return board;
}

void printBoard() {
  for (int row = 0; row < board.size(); row++) {
    for (int col = 0; col < board.at(row).size(); col++) {
      std::cout << board.at(row).at(col) << " ";
    }
    std::cout << endl;
  }
}

//gameplay

void guess() {
  string word;
  bool validGuess = false;
  while (!validGuess) {
    std::cout << "Enter your guess: ";
    std::cin >> word;

    if (!isWord(word)) {
      std::cout << word << " is not a word" << endl;
    } else if (!isInDictionary(word)) {
      std::cout << word << " is not a valid word" << endl;
    } else if (word.length() == 0) {
      std::cout << "Please enter a word" << endl;
    } else {
      validGuess = true;
    }
  }

  if (isOnBoard(word)) {
    playerSolvedWords.insert(word);
  }
}

string toUpperCase(string word) {
  string newWord;
  for (int i = 0; i < word.length(); i++) {
    newWord += toupper(word.at(i));
  }
  return newWord;
}

bool isWord(string word) {
  for (int i = 0; i < word.length(); i++) {
    if (!isalpha(word.at(i))) {
      return false;
    }
  }
  return true;
}

bool isOnBoard(string word) {
  word = toUpperCase(word);

  set<pair<int, int>> startingLocations = findCharacterLocations(word.at(0));

  for (auto itr = startingLocations.begin(); itr != startingLocations.end(); itr++) {
    pair<int, int> location = *itr;
    if (searchWordFromLocation(location.first, location.second, word)) {
      return true;
    }
  }
  
  return false;
}

set<pair<int, int>> findCharacterLocations(char character) {
  set<pair<int, int>> locations;
  pair<int, int> const BOARDSIZE = getBoardSize();
  for (int row = 0; row < BOARDSIZE.first; row++) {
    for (int col = 0; col < BOARDSIZE.second; col++) {
      if (board.at(row).at(col) == character) {
        locations.insert(make_pair(row, col));
      }
    }
  }

  return locations;
}

bool searchWordFromLocation(int row, int col, string word) {
  int currentRow = row;
  int currentCol = col;
  string tracker = "" + word.at(0);

  for (int i = 1; i < word.length(); i++) {
    if (tracker == word) {
      return true;
    }

    char desiredCharacter = word.at(i);
    set<pair<pair<int, int>, char>> adjacentCharacters = getAdjacentCharacters(currentRow, currentCol);

    for (auto itr = adjacentCharacters.begin(); itr != adjacentCharacters.end(); itr++) {
      pair<pair<int, int>, char> position = *itr;
      if (position.second == desiredCharacter) {
        tracker += desiredCharacter;
        currentRow = position.first.first;
        currentCol = position.first.second;
        break;
      }
    }
  }

  return tracker == word;
}




//Solver

bool isInBounds(int row, int col) {
  return row >= 0 && row < board.size() && col >= 0 && col < board.at(row).size();
}

set<pair<pair<int, int>, char>> getAdjacentCharacters(int row, int col) {
  set<pair<pair<int, int>, char>> adjacentCharacters;

  for (int irow = row - 1; irow <= row + 1; irow++) {
    for (int icol = col - 1; icol <= col + 1; icol++) {
      if (isInBounds(irow, icol) && !(irow == row && icol == col)) {
        // std::cout << " adjacent character at " << irow << " " << icol << endl;
        pair<int, int> location = make_pair(irow, icol);
        char character = board.at(irow).at(icol);
        adjacentCharacters.insert(make_pair(location, character));
      }
    }
  }

  return adjacentCharacters;
}

pair<int, int> getBoardSize() {
  return make_pair(board.size(), board.at(0).size());
}

bool isInDictionary(string word) {

  int left = 0;
  int right = dictionary.size();
  int mid;

  while (right > left) {
    mid = (right + left) / 2;
    int comparison = word.compare(dictionary.at(mid));
    // std::cout << "Searching word " << dictionary.at(mid) << " against " << word << endl;
    
    if (comparison == 0) {
      return true;
    } else if (comparison > 0) {
      left = mid + 1;
    } else if (comparison < 0) {
      right = mid - 1;
    }
  }

  return false;

}

bool prefixInDictionary(string prefix) {
  int left = 0;
  int right = dictionary.size();
  int mid;

  while (right > left) {
    mid = (right + left) / 2;
    // std::cout << "Prefix: " << prefix << " Word at Middle: " << dictionary.at(mid) << endl;
    int comparison = dictionary.at(mid).compare(0, prefix.length(), prefix);

    if (comparison == 0) {
      return true;
    } else if (comparison < 0) {
      left = mid + 1;
    } else if (comparison > 0) {
      right = mid - 1;
    }
  }

  return false;
}

bool stringStartsWith(string word, string prefix) {
  return word.compare(0, prefix.length(), prefix) == 0;
}

bool hasVisited(vector<pair<int, int>> visited, pair<int, int> location) {
  for (int i = 0; i < visited.size(); i++) {
    pair<int, int> current = visited.at(i);
    if (current.first == location.first && current.second == location.second) {
      return true;
    }
  }
  return false;
}


set<string> solve(vector<pair<int, int>> visited, string currentWord) {
  set<string> found;
  if (isInDictionary(currentWord)) {
    found.insert(currentWord);
  }

  if (!prefixInDictionary(currentWord)) {
    return found;
  }

  int currentRow = visited.at(visited.size() - 1).first;
  int currentCol = visited.at(visited.size() - 1).second;

  set<pair<pair<int, int>, char>> adjacentCharacters = getAdjacentCharacters(currentRow, currentCol);

  for (auto itr = adjacentCharacters.begin(); itr != adjacentCharacters.end(); itr++) {
    pair<pair<int, int>, char> nextCharacter = *itr;

    if (hasVisited(visited, nextCharacter.first)) { continue; }

    vector<pair<int, int>> nextVisited(visited);
    nextVisited.push_back(nextCharacter.first);
    string nextWord = currentWord + nextCharacter.second;

    set<string> nextFound = solve(nextVisited, nextWord);
    found.insert(nextFound.begin(), nextFound.end());
  }

  return found;
}

set<string> solveBoogleBoard() {
  set<string> solvedWords;
  const pair<int, int> BOARD_SIZE = getBoardSize();

  for (int row = 0; row < BOARD_SIZE.first; row++) {
    for (int col = 0; col < BOARD_SIZE.second; col++) {
      vector<pair<int, int>> visited;
      visited.push_back(make_pair(row, col));
      string current = string(1, board.at(row).at(col));

      set<string> foundWords = solve(visited, current);

      for (auto itr = foundWords.begin(); itr != foundWords.end(); itr++) {
          solvedWords.insert(*itr);
      }
    }
  }
  
  return solvedWords;
}


// Boggle is a game played on a 4 x 4 grid of letters. The goal is to find as many words as possible that can be formed by a sequence of adjacent letters in the grid, using each cell at most once. Given a game board and a dictionary of valid words, implement a Boggle solver.
