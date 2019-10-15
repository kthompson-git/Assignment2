#include <bits/stdc++.h>  
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <thread>

// check if character in string is duplicated
bool duplicate(int index, std::string &fileIn)
{
  if (index == 0) // check if beginning of string
    return false; // duplicate not possible
  else
  {
    for (int i = 0; i < index; i++) // only checks up to current index
    {
      if (fileIn[index] == fileIn[i]) // if any characters match current index return true
        return true;
    }
    return false;
  }
}

// check frequency of characters in a string
int frequency(char symbol, std::string &fileIn)
{
  int cnt = 0;
  for (int i = 0; i < fileIn.length(); i++)
  {
    if (symbol == fileIn[i])
      cnt++;
  }
  return cnt;
}

// checks character occurences in a string and populates vectors
void getStringData(std::string &fileIn, std::vector<char> &sym, std::vector<int> &cnt)
{  
  for (int i = 0; i < fileIn.length(); i++)
  {
    if (!duplicate(i, fileIn)) // check if character has occurred previously
    {
      sym.push_back(fileIn[i]); // store character in symbol vector
      cnt.push_back(frequency(fileIn[i], fileIn)); // store count of character in count vector
    }   
  }  
  return;
}

// swap the position of two elements in vectors
void vectorSwap(int index1, int index2, std::vector<char> &sym, std::vector<int> &cnt)
{
  int intTemp = cnt[index1];
  char charTemp = sym[index1];

  cnt[index1] = cnt[index2];
  sym[index1] = sym[index2];

  cnt[index2] = intTemp;
  sym[index2] = charTemp;

  return;
}

// sorts vectors from most frequent character to least
// sorts equal frequencies by character low to high ACSII value
void sortVectors(std::vector<char> &sym, std::vector<int> &cnt)
{
  int max = 0, maxIndex;

  for (int i = 0; i < sym.size(); i++)
  {
    for (int j = i; j < sym.size(); j++)
    {
      if (cnt[j] > max)
      {
        maxIndex = j;
        max = cnt[j];
      }
      else if (cnt[j] == max && (int)sym[j] < (int)sym[maxIndex])
        maxIndex = j;
    }
    vectorSwap(i, maxIndex, sym, cnt);
    max = 0;
  }
  return;
}

// creates the thread that will call the server
void createThread(std::string fileIn, int i, std::string *memArray)
{
  
}

// remove specified character from string
void removeChar(char sym, std::string &str)
{
  std::string temp;
  for (int i = 0; i < str.length(); i++)
  {
    if (str[i] != sym)
      temp.push_back(str[i]);
  }
  str.swap(temp);
  return;
}

// compresses symbols to binary
void compression(std::string &fileIn, std::vector<char> &sym, std::string *memArray)
{
  if (fileIn.empty()) // exit function if string is empty
    return;
  for (int i = 0; i < sym.size(); i++)
  {
    createThread(fileIn, i, memArray);
    removeChar(sym[i], fileIn); // remove character that was coded by child process
  }
}

int main(int argc, char *argv[]) 
{
  // check if correct amount of arguments passed
  if (argc != 3)
  {
    fprintf(stderr,"ERROR, insufficient arguments.\nUsage: %s <hostname> <port>\n", argv[0]);
    exit(0);
  }
  
  // string for file input
  std::string fileIn; 
  std::getline(std::cin, fileIn, '\0');

  // vectors for file info
  std::vector<char> symbol;
  std::vector<int> count;

  // populate vectors
  getStringData(fileIn, symbol, count);

  // sort vectors
  sortVectors(symbol, count);

  // create array for thread access
  std::string memArray[(symbol.size()*2)];

  // perform symbol compression
  compression(fileIn, symbol, memArray);

  return 0;
}