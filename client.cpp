#include <bits/stdc++.h>  
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <pthread.h>

struct Data
{
	public:
    int index;
    int flag;
    int size;
    char sym;
    char message[1000];
    // std::string message;
    char code[1000];
    // std::string code;
    Data *next;
};

void append(Data** headRef, int i, int flag, int size, char sym, std::string msg)  
{  
    std::cout << "In append\n";
    Data *newNode = new Data(); 
  
    Data *last = *headRef; 
  
    newNode->index = i; 
    newNode->flag = flag;
    newNode->size = size;
    newNode->sym = sym;
    for (int j = 0; j < size; j++)
    {
      newNode->message[j] = msg[j];
      newNode->code[j] = msg[j]; 
    }
    newNode->next = NULL; 

    // std::cout << "index:\t" << newNode->index << std::endl;
    // std::cout << "flag:\t" << newNode->flag << std::endl;
    // std::cout << "size:\t" << newNode->size << std::endl;
    // std::cout << "char:\t" << newNode->sym << std::endl;
    // std::cout << "msg:\t" << newNode->message << std::endl; 
    // std::cout << "code:\t" << newNode->code << std::endl;

    if (*headRef == NULL)  
    {  
      *headRef = newNode; 
      return;  
    }  

    while (last->next != NULL)  
      last = last->next;  
  
    last->next = newNode;  
    return;  
}  

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
void *serverCall(void *headRef)
{
  // std::cout << "\nHead Ref passed to server:\t" << headRef << std::endl;
  struct Data *head = (struct Data *) headRef;
  std::cout << "thread num:\t" << head->index << std::endl;
  while (true)
  {
    if (head->flag == 0)
    {
      // std::cout << "\nHead created:\t" << &head << std::endl;
      // std::cout << "index:\t" << head->index << std::endl;
      // std::cout << "flag:\t" << head->flag << std::endl;
      // std::cout << "size:\t" << head->size << std::endl;
      // std::cout << "char:\t" << head->sym << std::endl;
      // std::cout << "msg:\t" << head->message << std::endl;
      std::string code = generateCode(head->message, head->sym);
      for (int i = 0; i < head->size; i++)
      {
        head->code[i] = code[i];
      }
      // std::cout << "code:\t" << head->code << std::endl;
      head->flag = 1;
      return NULL;
    }
    else if (head->flag == 1)
    {
      if (head->next == NULL)
        return NULL;
      head = head->next;
    }
  }
}

// creates threads to compress symbols to binary
void populateList(std::string &fileIn, std::vector<char> &sym, Data **head)
{
  // std::cout << "\nHead passed to populate:\t" << &head << std::endl;

  if (fileIn.empty()) // exit function if string is empty
    return;
  for (int i = 0; i < sym.size(); i++)
  {
    append(head, i, 0, fileIn.length(), sym[i], fileIn);
    removeChar(sym[i], fileIn); 
  }

  // std::cout << "\nPrinting populate list\n";
  // Data *temp = *head;
  // for (int i = 0; i < sym.size(); i++)
  // {
  //   std::cout << "index:\t" << temp->index << std::endl;
  //   std::cout << "flag:\t" << temp->flag << std::endl;
  //   std::cout << "size:\t" << temp->size << std::endl;
  //   std::cout << "char:\t" << temp->sym << std::endl;
  //   std::cout << "msg:\t" << temp->message << std::endl;
  //   std::cout << "code:\t" << temp->code << std::endl;
  //   temp = temp->next;
  // }
  return;
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
void printList(std::vector<char> sym, Data *head)
{
  Data *node = head;
  for (int i = 0; i < sym.size(); i++)
  { 
    std::string str(node->message);    
    if (i == 0)
      std::cout << "Original Message:\t" << newlineToEOL(str) << std::endl;
    else
      std::cout << "Remaining Message:\t" << newlineToEOL(str) << std::endl;

    if (sym[i] == '\n')
      std::cout << "Symbol <EOL> code:\t" << node->code << std::endl;
    else
      std::cout << "Symbol " << sym[i] << " code:\t\t" << node->code << std::endl;
    node = node->next;
  }
  return;
}

// calls printing functions
void printCompression(std::vector<char> &sym, std::vector<int> &cnt, Data *head)
{
  printVectors(sym, cnt);
  printList(sym, head);
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
  static struct Data *head = NULL;
  std::cout << "\nHead init:\t" << &head << std::endl;
  // std::cout << "Head init:\t" << head << std::endl;
  // std::cout << "Head init next:\t" << &head->next << std::endl;

  // perform symbol compression
  std::cout << "compression" << std::endl;
  populateList(fileIn, symbol, &head);

  // create thread
  pthread_t tid[symbol.size()];
  for (int i = 0; i < symbol.size(); i++)
  {
    if (pthread_create(&tid[i], NULL, serverCall, head))
    {
      fprintf(stderr, "Error creating thread.\n");
      exit(1);
    }
  }
  pthread_exit(NULL);
  wait(1);


  // std::cout << "\nHead after populate:\t" << &head << std::endl;
  // std::cout << "Head after populate:\t" << head << std::endl;
  // std::cout << "Head after populate next:\t" << head->next << std::endl;
  // std::cout << "index:\t" << head->index << std::endl;
  // std::cout << "flag:\t" << head->flag << std::endl;
  // std::cout << "size:\t" << head->size << std::endl;
  // std::cout << "char:\t" << head->sym << std::endl;
  // std::cout << "msg:\t" << head->message << std::endl;
  // std::cout << "code:\t" << head->code << std::endl;
  // for (int i = 0; i < symbol.size(); i++)
  // {
  //   serverCall(head);
  // }
  
  // std::cout << "\nHead after server call:\t" << &head << std::endl;
  // std::cout << "Head after populate:\t" << head << std::endl;
  // std::cout << "Head after populate next:\t" << head->next << std::endl;
  // std::cout << "index:\t" << head->index << std::endl;
  // std::cout << "flag:\t" << head->flag << std::endl;
  // std::cout << "size:\t" << head->size << std::endl;
  // std::cout << "char:\t" << head->sym << std::endl;
  // std::cout << "msg:\t" << head->message << std::endl;
  // std::cout << "code:\t" << head->code << std::endl;

  // print compression steps
  std::cout << "\n\n\nprint compression" << std::endl;
  printCompression(symbol, count, head);


  return 0;
}