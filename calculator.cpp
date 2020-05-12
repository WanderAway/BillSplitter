#include "calculator.h"

#include <fstream>
#include <sstream>
#include <iostream>

using std::fstream;
using std::stringstream;
using std::getline;
using std::stof;
using std::endl;


// constructor for bill splitter
splitter::splitter(string filename){
  this->people_count = 0;

  fstream fs(filename, fstream::in);

  // if file successfully opened
  if (fs.is_open()){
    string temp;
    while (getline(fs, temp)){
      if (temp.length() > 4){
        stringstream ss(temp);
        
        // populate person object
        getline(ss, people[people_count].name, ',');
        people[people_count].index = people_count;
        
        getline(ss, temp);
        people[people_count].credit = stof(temp);

        people_count++;
      }
    }
    fs.close();
  }
}


// destructor for bill splitter, saves contents to file
splitter::~splitter(){
  fstream fs(SAVE_FILENAME, fstream::out);
  
  for (int i=0; i<this->people_count; i++){
    fs << people[i].name << ',' << people[i].credit << endl;
  }

  fs.close();
}


int splitter::add_person(string name){
  people[people_count].name = name;
  people[people_count].index = people_count;
  people[people_count].credit = 0;
  people_count++;

  return 0;
}


int splitter::get_count() const{
  return this->people_count;
}


vector<string> splitter::get_people() const{
  vector<string> results;
  for (int i=0; i<people_count; i++){
    results.push_back(people[i].name);
  }
  return results;
}


void splitter::add_bill(bill b){

  int debtors = b.debtor.size();
  float debt = b.amount / (debtors + 1);

  people[b.payee].credit += debt * debtors;

  for (auto p : b.debtor)
    people[p].credit -= debt;
}


void splitter::transfer(int from, int to, float amount){
  people[from].credit += amount;
  people[to].credit -= amount;
}


void splitter::clear(){
  for (int i=0; i<this->people_count; i++)
    people[i].credit = 0;
}


// the actual complicated bit
vector<transaction> splitter::summarize()  {
  vector<transaction> results;

  vector<person*> debters;
  vector<person*> creditors;

  // put people into either debter or creditor
  // ignore people with a credit of 0
  for (int i=0; i<people_count; i++){
    if (people[i].credit > 0)
      creditors.push_back(&(people[i]));
    else if (people[i].credit < 0)
      debters.push_back(people + i);
  }

  // sort the two vectors in descending order based on credit owed or owned
  // since the list of people is fixed to a small number (MAX_PEOPLE=64), 
  // not going to worry too much about sorting efficiency.
  // using insertion sort here
  for (int i=0; i<debters.size(); i++){
    bool swap = false;
    for (int j=debters.size()-1; j > i; j--){
      if (debters[j]->credit < debters[j-1]->credit){ // since negative, larger debts are smaller in value
        swap = true;
        person *temp = debters[j];
        debters[j] = debters[j-1];
        debters[j-1] = temp;
      }
    }

    // if no swap occurred, vector is sorted.
    if (!swap) break;

    swap = false;
  }
  
  // same thing but for creditors
  for (int i=0; i<creditors.size(); i++){
    bool swap = false;
    for (int j=creditors.size()-1; j > i; j--){
      if (creditors[j]->credit > creditors[j-1]->credit){  
        swap = true;
        person *temp = creditors[j];
        creditors[j] = creditors[j-1];
        creditors[j-1] = temp;
      }
    }
    if (!swap) break;
    swap = false;
  }

  // typically there's only one or two big creditors (for us anyway)
  // so we'll just work down the list
  
  // debter index
  int didx = 0;
  float credcarry = 0;
  for (auto p : creditors){
    float tempcred;
    if (credcarry == 0) {
      credcarry = p->credit;
      while (credcarry > 0){
        tempcred = credcarry;
        credcarry += debters[didx]->credit;
        
        transaction t;
        t.from = debters[didx];
        t.to = p;
        
        if (credcarry > 0){
          // subtract debter's debt from creditor's credit, add to transaction
          t.amount = t.from->credit * -1;
          // move on to next debter
          didx++;
        }
        else
          t.amount = tempcred;
        
        results.push_back(t);
      }
    }
    else if (credcarry < 0){
      tempcred = credcarry;
      credcarry += p->credit;
      
      transaction t;
      if (credcarry > 0){
        t.from = debters[didx];
        t.to = p;
        t.amount = tempcred * -1;
        results.push_back(t);
        
        didx++;
        
        while (credcarry > 0){
          tempcred = credcarry;
          credcarry += debters[didx]->credit;
          
          t.from = debters[didx];
          
          if (credcarry > 0){
            t.amount = t.from->credit * -1;
            didx++;
          }
          else
            t.amount = tempcred;
          
          results.push_back(t);
        }
        
      }
      else { // this debter owes a lot of money... subtract credit from this creditor and move to next one
        t.from = debters[didx];
        t.to = p;
        t.amount = p->credit;
        
        results.push_back(t);
      }
    }

    else {
      std::cerr << "[DEBUG]ERROR: summarize() loop resulted in positive credcarry!" << endl;
    }
  }

  return results;
}
