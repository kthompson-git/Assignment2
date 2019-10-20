#include <bits/stdc++.h>  
#include <iostream>
#include <unistd.h>
#include <stdio.h>
// #include <pthread.h>

struct Data
{
	public:
    int index;
    int flag;
    char sym;
    std::string message;
    std::string code;
};

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

std::string generateCode (std::string fileIn, char sym)
{
  std::string code;
  for (int i = 0; i < fileIn.length(); i++)
  {
    if (fileIn[i] == sym)
      code.append("1");
    else
      code.append("0");
  }
  return code;
}

// changes all newline characters to <EOL>
std::string newlineToEOL(std::string &fileIn)
{
  std::string temp;
  for (int i = 0; i < fileIn.length(); i++)
  {
    if (fileIn[i] == '\n')
      temp = temp + "<EOL>";
    else
      temp.push_back(fileIn[i]);
  }
  return temp;
}

// send message to server and receive code
void *serverCall(void *listRef)
{
  struct Data *list = (struct Data *) listRef;
  // std::cout << "index:\t" << list->index << std::endl;
  // std::cout << "flag:\t" << list->flag << std::endl;
  // std::cout << "char:\t" << list->sym << std::endl;
  // std::cout << "msg:\t" << list->message << std::endl;
  // list->code = generateCode(list->message, list->sym);
  // std::cout << "code:\t" << list->code << std::endl;


	return NULL;
}

// creates threads to compress symbols to binary
void compression(std::string &fileIn, std::vector<char> &sym, Data *list)
{
  if (fileIn.empty()) // exit function if string is empty
    return;
  // pthread_t tid[sym.size()];
  for (int i = 0; i < sym.size(); i++)
  {
    list[i].index = i;
    list[i].flag = 0;
    list[i].sym = sym[i];
    list[i].message = fileIn;
    list[i].code = fileIn;
    
    // serverCall((void *)&list[i]);
    
    // create thread
    if (pthread_create(&tid[i], NULL, serverCall, (void *) &list[i]))
    {
      fprintf(stderr, "Error creating thread.\n");
      exit(1);
    }

      
    	
    removeChar(sym[i], fileIn); // remove character that was coded by child process
  }
  pthread_exit(NULL);
}

// prints the vectors
void printVectors(std::vector<char> &sym, std::vector<int> &cnt)
{
  for (int i = 0; i < sym.size(); i++)
  {
    if (sym[i] == '\n') // special case for newline character
      std::cout << "<EOL> frequency = " << cnt[i] << std::endl;
    else
      std::cout << sym[i] << " frequency = " << cnt[i] << std::endl;
  }
  return;
}

// print message and code from linked list
void printList(std::vector<char> sym, Data *list)
{
  Data *node = NULL;
  for (int i = 0; i < sym.size(); i++)
  {     
    if (i == 0)
      std::cout << "Original Message:\t" << list[i].message << std::endl;
    else
      std::cout << "Remaining Message:\t" << list[i].message << std::endl;

    if (sym[i] == '\n')
      std::cout << "Symbol <EOL> code:\t" << list[i].code << std::endl;
    else
      std::cout << "Symbol " << sym[i] << " code:\t\t" << list[i].code << std::endl;
  }
  return;
}

// calls printing functions
void printCompression(std::vector<char> &sym, std::vector<int> &cnt, Data *list)
{
  printVectors(sym, cnt);
  printList(sym, list);
  return;
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

  // start linked list for data
  // static struct Data *head = NULL;
  // std::cout << "Head address 1:\t" << &head << std::endl;
  // std::cout << "Head address 1:\t" << head << std::endl;
  struct Data list[symbol.size()];

  // perform symbol compression
  std::cout << "compression" << std::endl;
  compression(fileIn, symbol, list);

  // print compression steps
  std::cout << "print compression" << std::endl;
  printCompression(symbol, count, list);


  return 0;
}