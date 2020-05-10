#include "calculator.h"
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;
using std::cin;


void list_people(splitter &s){
  auto people = s.get_people();
  int i=0;
  for (auto p : people){
    cout << i << ") " << p << endl;
    i++;
  }
}


int main(){
  
  splitter s;

  cout << "WARNING: Minimal error checking!" << endl;
  std::string input;
  while (1){
    cout << "1) add person" << endl
         << "2) add bill" << endl
         << "3) calculate transfers" << endl
         << "4) transfer money" << endl
         << "5) clear history" << endl 
         << "6) exit" << endl << endl
         << ">";

    cin >> input;
    int option = std::stoi(input);

    switch (option) {
      case 1: // add person
        cout << "TODO" << endl;
        break;

      case 2: // add bill
      {
        bill b;
        cout << "Bill amount?" << endl
          << "amount=";
        cin >> input;
        b.amount = std::stof(input);
        
        cout << "Who paid?" << endl;
        list_people(s);

        cout << ">";
        cin >> input;
        b.payee = std::stoi(input);
        
        while (1){
          cout << "Who else is responsible for bill? (q to end selection)" << endl;
          list_people(s);

          cin >> input;
          if (input[0] != 'q')
            b.debtor.push_back(std::stoi(input));
          else break;
        }
        s.add_bill(b);
        break;
      }

      case 3: // calculate transfers
      {
        auto ts = s.summarize();
        for (auto t : ts){
          cout << t.from->name << " == " << t.amount << " => " << t.to->name << endl;
        }
        cout << endl << "Press any key to continue...";
        cin.get();
        cin.get();
        break;
      }

      case 4: // transfer money
        cout << "TODO" << endl;
        break;
      case 5: // clear history
        s.clear();
        break;
      case 6: // exit
        return 0;

      default: 
        cout << "Invalid selection!" << endl;
    }
  }

}
