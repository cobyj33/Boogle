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

std::vector<std::vector<char>> board;
std::vector<std::string> dictionary;
std::map<char, int> frequencyMap;
std::set<std::string> playerSolvedWords;

const std::string DICTIONARY_PATH = "dictionary.txt";

int main() {
  srand(time(NULL));

  std::ifstream inputStream (DICTIONARY_PATH);
  std::string currentWord;
  while (inputStream >> currentWord) {
    dictionary.push_back(currentWord);
  }
  inputStream.close();

  int boardSize;
  std::cout << "Enter the size of the board: ";
  std::cin >> boardSize;

  bool willCreateCustomBoard;
  std::string sel;
  std::cout <<"Would you like to enter your own board?: (y/n) ";
  std::cin >> sel;
  willCreateCustomBoard = (sel == "y" || sel == "yes" || sel == "YES");

  bool inGame = true;
  board = createBoard(boardSize);
  std::set<std::string> possibleWords = solveBoogleBoard();

  while (inGame) {
    printBoard();

    bool gotSelection = false;
    bool forfeited = false;
    while (!gotSelection) {
      std::string selection;
      std::cout << "Would you like to guess or forfeit?: ";
      std::cin >> selection;

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

  std::cout << "YOU FOUND: " << std::endl;
  int onLine = 0;
  for (auto itr = playerSolvedWords.begin(); itr != playerSolvedWords.end(); itr++) {
      std::cout << *itr << " ";
      onLine++;
      if (onLine > 10) { std::cout << std::endl; onLine = 0; }
  }

  std::cout << "POSSIBLE WORDS: " << std::endl;
  onLine = 0;
  for (auto itr = possibleWords.begin(); itr != possibleWords.end(); itr++) {
      std::cout << *itr << " ";
      onLine++;
      if (onLine > 10) { std::cout << std::endl; onLine = 0; }
  }

  std::cout << std::endl;
}

std::vector<std::vector<char>> createBoardFromString(std::string input) {
  std::vector<std::vector<char>> board;
  if (!(sqrt(input.length()) == int(sqrt(input.length())))) {
    std::cout << "input not in square format" << std::endl;
    return board;
  } 
  int size = int(sqrt(input.length()));

  for (int row = 0; row < size; row++) {
    std::vector<char> boardRow;
    for (int col = 0; col < size; col++) {
      boardRow.push_back(input.at(row * size + col));
    }
    board.push_back(boardRow);
  }

  return board;
}

const std::string LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::string LETTERS_BY_FREQUENCY = "EARIOTNSLCUDPMHGBFYKVXZJQ";

std::vector<char> getLetterList() {
  std::vector<char> letters;
  for (int i = 0; i < LETTERS.length(); i++) {
    letters.push_back(LETTERS.at(i));
  }
  return letters;
}

std::vector<std::vector<char>> createBoard(int size) {
  std::vector<char> letters = getLetterList();
  std::vector<std::vector<char>> board;

  for (int row = 0; row < size; row++) {
    std::vector<char> boardRow;
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
    std::cout << std::endl;
  }
}

//gameplay

void guess() {
  std::string word;
  bool validGuess = false;
  while (!validGuess) {
    std::cout << "Enter your guess: ";
    std::cin >> word;

    if (!isWord(word)) {
      std::cout << word << " is not a word" << std::endl;
    } else if (!isInDictionary(word)) {
      std::cout << word << " is not a valid word" << std::endl;
    } else if (word.length() == 0) {
      std::cout << "Please enter a word" << std::endl;
    } else {
      validGuess = true;
    }
  }

  if (isOnBoard(word)) {
    playerSolvedWords.insert(word);
  }
}

std::string toUpperCase(std::string word) {
  std::string newWord;
  for (int i = 0; i < word.length(); i++) {
    newWord += toupper(word.at(i));
  }
  return newWord;
}

bool isWord(std::string word) {
  for (int i = 0; i < word.length(); i++) {
    if (!isalpha(word.at(i))) {
      return false;
    }
  }
  return true;
}

bool isOnBoard(std::string word) {
  word = toUpperCase(word);

  std::set<std::pair<int, int>> startingLocations = findCharacterLocations(word.at(0));

  for (auto itr = startingLocations.begin(); itr != startingLocations.end(); itr++) {
    std::pair<int, int> location = *itr;
    if (searchWordFromLocation(location.first, location.second, word)) {
      return true;
    }
  }
  
  return false;
}

std::set<std::pair<int, int>> findCharacterLocations(char character) {
  std::set<std::pair<int, int>> locations;
  const std::pair<int, int> BOARD_SIZE = getBoardSize();
  for (int row = 0; row < BOARD_SIZE.first; row++) {
    for (int col = 0; col < BOARD_SIZE.second; col++) {
      if (board.at(row).at(col) == character) {
        locations.insert(std::make_pair(row, col));
      }
    }
  }

  return locations;
}

bool searchWordFromLocation(int row, int col, std::string word) {
  int currentRow = row;
  int currentCol = col;
  std::string tracker = "" + word.at(0);

  for (int i = 1; i < word.length(); i++) {
    if (tracker == word) {
      return true;
    }

    char desiredCharacter = word.at(i);
    std::set<std::pair<std::pair<int, int>, char>> adjacentCharacters = getAdjacentCharacters(currentRow, currentCol);

    for (auto itr = adjacentCharacters.begin(); itr != adjacentCharacters.end(); itr++) {
      std::pair<std::pair<int, int>, char> position = *itr;
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

std::set<std::pair<std::pair<int, int>, char>> getAdjacentCharacters(int row, int col) {
  std::set<std::pair<std::pair<int, int>, char>> adjacentCharacters;

  for (int irow = row - 1; irow <= row + 1; irow++) {
    for (int icol = col - 1; icol <= col + 1; icol++) {
      if (isInBounds(irow, icol) && !(irow == row && icol == col)) {
        // std::cout << " adjacent character at " << irow << " " << icol << std::endl;
        std::pair<int, int> location = std::make_pair(irow, icol);
        char character = board.at(irow).at(icol);
        adjacentCharacters.insert(std::make_pair(location, character));
      }
    }
  }

  return adjacentCharacters;
}

std::pair<int, int> getBoardSize() {
  return std::make_pair(board.size(), board.at(0).size());
}

bool isInDictionary(std::string word) {

  int left = 0;
  int right = dictionary.size();
  int mid;

  while (right > left) {
    mid = (right + left) / 2;
    int comparison = word.compare(dictionary.at(mid));
    // std::cout << "Searching word " << dictionary.at(mid) << " against " << word << std::endl;
    
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

bool prefixInDictionary(std::string prefix) {
  int left = 0;
  int right = dictionary.size();
  int mid;

  while (right > left) {
    mid = (right + left) / 2;
    // std::cout << "Prefix: " << prefix << " Word at Middle: " << dictionary.at(mid) << std::endl;
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

bool stringStartsWith(std::string word, std::string prefix) {
  return word.compare(0, prefix.length(), prefix) == 0;
}

bool hasVisited(std::vector<std::pair<int, int>> visited, std::pair<int, int> location) {
  for (int i = 0; i < visited.size(); i++) {
    std::pair<int, int> current = visited.at(i);
    if (current.first == location.first && current.second == location.second) {
      return true;
    }
  }
  return false;
}


std::set<std::string> solve(std::vector<std::pair<int, int>> visited, std::string currentWord) {
  std::set<std::string> found;
  if (isInDictionary(currentWord)) {
    found.insert(currentWord);
  }

  if (!prefixInDictionary(currentWord)) {
    return found;
  }

  int currentRow = visited.at(visited.size() - 1).first;
  int currentCol = visited.at(visited.size() - 1).second;

  std::set<std::pair<std::pair<int, int>, char>> adjacentCharacters = getAdjacentCharacters(currentRow, currentCol);

  for (auto itr = adjacentCharacters.begin(); itr != adjacentCharacters.end(); itr++) {
    std::pair<std::pair<int, int>, char> nextCharacter = *itr;

    if (hasVisited(visited, nextCharacter.first)) { continue; }

    std::vector<std::pair<int, int>> nextVisited(visited);
    nextVisited.push_back(nextCharacter.first);
    std::string nextWord = currentWord + nextCharacter.second;

    std::set<std::string> nextFound = solve(nextVisited, nextWord);
    found.insert(nextFound.begin(), nextFound.end());
  }

  return found;
}

std::set<std::string> solveBoogleBoard() {
  std::set<std::string> solvedWords;
  const std::pair<int, int> BOARD_SIZE = getBoardSize();

  for (int row = 0; row < BOARD_SIZE.first; row++) {
    for (int col = 0; col < BOARD_SIZE.second; col++) {
      std::vector<std::pair<int, int>> visited;
      visited.push_back(std::make_pair(row, col));
      std::string current = std::string(1, board.at(row).at(col));

      std::set<std::string> foundWords = solve(visited, current);

      for (auto itr = foundWords.begin(); itr != foundWords.end(); itr++) {
          solvedWords.insert(*itr);
      }
    }
  }
  
  return solvedWords;
}


// Boggle is a game played on a 4 x 4 grid of letters. The goal is to find as many words as possible that can be formed by a sequence of adjacent letters in the grid, using each cell at most once. Given a game board and a dictionary of valid words, implement a Boggle solver.
