#pragma once
#include <string>
#include <vector>


#define SAVE_FILENAME "config.ini"
#define MAX_PEOPLE 64


using std::string;
using std::vector;


typedef struct{
  string name;
  int index; // index in people array
  float credit;
} person;


typedef struct{
  float amount;
  int payee;
  vector<int> debtor;
} bill;


typedef struct{
  person* from;
  person* to;
  float amount;
}transaction;


class splitter {
  public: 
    // constructor: 
    splitter(string filename=SAVE_FILENAME); // construct from file
    ~splitter(); // destructor saves into file

    int add_person(string name);

    // returns the number of people tracked
    int get_count() const;

    vector<string> get_people() const;

    // this function should split the bill amon debters and add credit to payee
    void add_bill(bill b);

    // calculates who should transaction how much to who, returns vector of transactions.
    // ideally in the least amount of transactions possible but meh...
    vector<transaction> summarize();

    // transactions debt to credit between two people
    void transfer(int from, int to, float amount);

    // clears all debts and credits
    void clear(); 

  private:
    int people_count;
    person people[MAX_PEOPLE];
};
