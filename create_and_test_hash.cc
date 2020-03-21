// YOUR NAME.

#include "quadratic_probing.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


template <typename HashTableType>
void TestFunctionForHashTable(HashTableType &hash_table, const string &words_filename, const string &query_filename) {
  cout << "TestFunctionForHashTables..." << endl;
  cout << "Words filename: " << words_filename << endl;
  cout << "Query filename: " << query_filename << endl;
  hash_table.MakeEmpty();  
  //..Insert your own code

  //Here I am gonna populate the hash table hash_table with the words from words_filename
  ifstream wordsfile(words_filename);
  string line;
  while(getline(wordsfile,line)){ //save the word on each line in the string "line"
    hash_table.Insert(line);      //insert that string the hash table
  }

  cout << "Collisions: " << hash_table.getCollisions() << endl;
  cout << "Number of items: " << hash_table.getNumberOfItems() << endl;
  cout << "Size of hash table: " << hash_table.getSizeArray() << endl;
  cout << "Load factor: " << hash_table.loadFactor() << endl;
  cout << "Avg. number of collisions: " << hash_table.avgNumberOfCollisions() << endl << endl;

  //This is where we will deal with the queried words
  ifstream queryfile(query_filename);
  while(getline(queryfile,line)){
    if(hash_table.Contains(line)){
      cout <<line << " Found " << hash_table.getProbes() << endl;
    }
    else
      cout <<line << " Not_Found " << hash_table.getProbes() << endl;
  }
}

int testFunctionWrapper(int argument_count, char **argument_list) {
  const string words_filename(argument_list[1]);
  const string query_filename(argument_list[2]);
  const string param_flag(argument_list[3]);

  if (param_flag == "linear") {
    // HashTableLinear<string> linear_probing_table;
    // TestFunctionForHashTable(linear_probing_table, words_filename, query_filename);    
  } else if (param_flag == "quadratic") {
    HashTable<string> quadratic_probing_table;
    TestFunctionForHashTable(quadratic_probing_table, words_filename, query_filename);    
  } else if (param_flag == "double") {
    // HashTableDouble<string> double_probing_table;
    // TestFunctionForHashTable(double_probing_table, words_filename, query_filename);    
  } else {
    cout << "Unknown tree type " << param_flag << " (User should provide linear, quadratic, or double)" << endl;
  }
  return 0;
}

// Sample main for program create_and_test_hash.
// WE WILL NOT USE YOUR MAIN IN TESTING. DO NOT CODE FUNCTIONALITY INTO THE MAIN.
// WE WILL DIRECTLY CALL testFunctionWrapper. ALL FUNCTIONALITY SHOULD BE THERE.
// This main is only here for your own testing purposes.
int main(int argc, char **argv) {

  if (argc != 4) {
    cout << "Usage: " << argv[0] << " <wordsfilename> <queryfilename> <flag>" << endl;
    return 0;
  }
  
  testFunctionWrapper(argc, argv);
  return 0;
}
