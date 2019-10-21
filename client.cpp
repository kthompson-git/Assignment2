#include <bits/stdc++.h>  
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

// set port and ip address as global
unsigned int portNum;
char *ipAddress;

// structure for linked list
struct Data
{
	public:
    int index;
    int flag;
    int size;
    char sym;
    char message[1000];
    char code[1000];
    Data *next;
};

// append new node to end of linked list
void append(Data** head, int i, int flag, int size, char sym, std::string msg)  
{      
  Data *newNode = new Data(); // create new node
  Data *tail = *head; // set tail reference

  // enter new node data
  newNode->index = i; 
  newNode->flag = flag;
  newNode->size = size;
  newNode->sym = sym;
  for (int j = 0; j < size; j++)
  {
    newNode->message[j] = msg[j];
    newNode->code[j] = msg[j]; 
  }
  newNode->next = NULL; // set next to NULL for end of list

  if (*head == NULL) // set new node to head if no head
  {  
    *head = newNode; 
    return;  
  }  

  while (tail->next != NULL) // cycle through list to tail node
    tail = tail->next;  

  tail->next = newNode; // set new node as tail of list
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
  struct Data *head = (struct Data *) headRef;
  while (true)
  {
    if (head->flag == 0)
    {
      head->flag = 1;   
      int sockfd, n;
      struct sockaddr_in serv_addr;
      struct hostent *server;
      char buffer[256];
      sockfd = socket(AF_INET, SOCK_STREAM, 0);
      if (sockfd < 0) 
        std::cout << "ERROR opening socket\n";
      server = gethostbyname(ipAddress);
      if (server == NULL)
      {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
      }
      bzero((char *) &serv_addr, sizeof(serv_addr));
      serv_addr.sin_family = AF_INET;
      bcopy((char *)server->h_addr, 
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
      serv_addr.sin_port = htons(portNum);
      if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        std::cout << "ERROR connecting\n";
      bzero(buffer, 256);
      buffer[0] = head->sym;
      for (int i = 0; i < head->size; i++)
      {
        buffer[i+1] = head->message[i];
      }
      n = write(sockfd, buffer, strlen(buffer));
      if (n < 0) 
          std::cout << "ERROR writing to socket\n";
      bzero(buffer, 256);
      n = read(sockfd,buffer,255);
      if (n < 0) 
          std::cout << "ERROR reading from socket\n";
      for (int i = 0; i < head->size; i++)
      {
        head->code[i] = buffer[i];
      }
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

// populate linked lists
void populateList(std::string &fileIn, std::vector<char> &sym, Data **head)
{
  if (fileIn.empty()) // exit function if string is empty
    return;
  for (int i = 0; i < sym.size(); i++)
  {
    append(head, i, 0, fileIn.length(), sym[i], fileIn);
    removeChar(sym[i], fileIn); 
  }
  return;
}

// prints the vectors
void printVectors(std::vector<char> &sym, std::vector<int> &cnt)
{
  for (int i = 0; i < sym.size(); i++)
  {
    if (sym[i] == '\n') // special case for newline character
      printf("<EOL> frequency = %d\n", cnt[i]);
    else
      printf("%c frequency = %d\n", sym[i], cnt[i]);
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
  portNum = atoi(argv[2]);
  ipAddress = argv[1];

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

  // create linked list
  populateList(fileIn, symbol, &head);

  // create thread
  pthread_t tid[symbol.size()];
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  void *status;
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  for (int i = 0; i < symbol.size(); i++)
  {
    if (pthread_create(&tid[i], NULL, serverCall, head))
    {
      fprintf(stderr, "Error creating thread.\n");
      exit(1);
    }
  }
  pthread_attr_destroy(&attr);
  for (int i = 0; i < symbol.size(); i++ )
  {
    if (pthread_join(tid[i], &status)) 
    {
      std::cout << "Error:unable to join thread" << std::endl;
      exit(-1);
    }
  }
  
  printCompression(symbol, count, head);


  return 0;
}