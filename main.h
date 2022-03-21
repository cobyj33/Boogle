#pragma
#include <vector>
#include <set>
#include <string>

void createFrequencyMap();
std::vector<char> getLetterList();
std::string toUpperCase(std::string word);
std::vector<std::vector<char>> createBoard(int size);
std::vector<std::vector<char>> createBoardFromString(std::string input);
void printBoard();
bool isInBounds(int row, int col);
std::set<std::pair<std::pair<int, int>, char>> getAdjacentCharacters(int row, int col);
std::pair<int, int> getBoardSize();
std::set<std::string> solveBoogleBoard();
std::set<char> getDesiredCharacters();
bool isInDictionary(std::string word);
bool prefixInDictionary(std::string prefix);
bool stringStartsWith(std::string word, std::string prefix);
bool hasVisited(std::vector<std::pair<int, int>> visited, std::pair<int, int> location);

//gameplay
std::string toUpperCase(std::string word);
bool isOnBoard(std::string word);
std::set<std::pair<int, int>> findCharacterLocations(char character);
bool isWord(std::string word);
bool searchWordFromLocation(int row, int col, std::string word);
void guess();

