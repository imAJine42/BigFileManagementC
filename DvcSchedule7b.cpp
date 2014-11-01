// Lab 7b, The "DvcSchedule7b" Program
// Name: AJ Bernabeo
// Editor: Notepad++
// Compiler: VC++ 2012

#include <fstream>
#include <iostream>
#include <string>
#include <cassert>
#include <ctime>
#include <vector>
using namespace std;

#include <cstring>

struct SubjectCode
{
  string name;
  int count;
};

struct Node
{
  SubjectCode data;
  Node* next;
};

int main()
{
  cout << endl;
  
  cout << "Lab 7b, The \"DvcSchedule7b\" Program \n";
  cout << "Name: AJ Bernabeo\n";
  cout << "Editor: Notepad++\n";
  cout << "Compiler: VC++ 2012\n";
  cout << "File: " << __FILE__ << endl;
  cout << "Complied: " << __DATE__ << " at " << __TIME__ << endl;
  
  cout << endl;
  cout << "I'm checking file now";
  
  // for parsing the inputfile
  char* token;
  char buf[1000];
  const char* const tab = "\t";
  Node* start = 0;
  vector< vector<string> > duplicates;
  int dups = 0;
  int size = 0;

  // open the input file
  ifstream fin;
  fin.open("dvc-schedule.txt");
  if (!fin.good()) throw "I/O error";  

  // read the input file
  clock_t startTime = clock(); // Timing the performance
  int completion = 0;
  while (fin.good())
  {
    // read the line
    string line;
    getline(fin, line);
    strcpy(buf, line.c_str());
    if (buf[0] == 0) continue;
    
    // parse the line
    const string term(token = strtok(buf, tab));
    const string section(token = strtok(0, tab));
    const string course((token = strtok(0, tab)) ? token : "");
    const string instructor((token = strtok(0, tab)) ? token : "");
    const string whenWhere((token = strtok(0, tab)) ? token : "");
    if (course.find('-') == string::npos) continue; // invalid line
    const string subjectCode(course.begin(), course.begin() + course.find('-'));

    // if I get this far, then it's a valid record
    // Looks for duplicates
    bool found = false;
    int i, j;
    for(i = 0; i < duplicates.size(); i++)
    {
      if(duplicates[i][0] == term)
      {
        for(j = 1; j < duplicates[i].size(); j++)
        {
          if(duplicates[i][j] == section)
          {
            dups++;
            found = true;
            break;
          }
        }
        break;
      }
    }
    
    // If no Duplicates found continue
    if(found == false)
    {
      // For Duplicates
      if(i == duplicates.size()) // Adds new Vector<String> 
      {
        duplicates.push_back(vector<string>());
        duplicates[i].push_back(term);
        duplicates[i].push_back(section);
      }
      else if(j == duplicates[i].size()) // Pushes section to the current vector
        duplicates[i].push_back(section);
      
      // For Data
      Node* p;
      for(p = start; p; p = p->next)
        if(subjectCode == p->data.name) break;
      
      if(p) p->data.count++; // Adds to existing data
      else
      { // Creates new vector and sorts the data
        Node* prev;
        for(prev = 0, p = start; p; prev = p, p = p->next)
          if(subjectCode < p->data.name) break;
        
        Node* subjectCodes = new Node;
        subjectCodes->data.name = subjectCode;
        subjectCodes->data.count = 1;
        subjectCodes->next = p;
        if(prev) prev->next = subjectCodes;
        else start = subjectCodes;
        
        size++;
      }
    }
    
    // For Buffering
    if(completion % 10000 == 0) cout << '.';
    completion++;
  }
  fin.close();
  double elapsedSeconds = (double)(clock() - startTime) / CLOCKS_PER_SEC;
  cout << "finish" << endl;
  
  // Output
  Node* p;
  Node* prev;
  for(prev = start, p = start->next; p; prev = p, p = p->next)
    assert(prev->data.name < p->data.name);
  
  cout << endl;
  cout << "Number of Classes: " << size << endl;
  cout << "Duplicates: " << dups << endl;
  cout << "--------------------" << endl;
  for(p = start; p; p = p->next)
    cout << p->data.name << ", " << p->data.count << endl;
  
  cout << endl;
  cout << "Runtime: " << elapsedSeconds << endl;
  
  // Deallocation
  while(start)
  {
    Node* p = start->next;
    delete p;
    start = p;
  }
  
  cout << endl;
}