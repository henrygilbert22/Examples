#include <iostream>
#include <string>

#include "hashtable.h"
#include "pass_server.h"

void menu()
{
   cout << "\n\n";
   cout << "l - Load From File" << endl;
   cout << "a - Add User" << endl;
   cout << "r - Remove User" << endl;
   cout << "c - Change User Password" << endl;
   cout << "f - Find User" << endl;
   cout << "d - Dump HashTable" << endl;
   cout << "s - HashTable Size" << endl;
   cout << "w - Write to Password File" << endl;
   cout << "x - Exit program" << endl;
   cout << "\nEnter choice : ";
}

int main()
{

   PassServer<string, string> ps{};

   bool toexit = false;
   char line;
   string username;
   string password;
   string newpassword;
   pair<string,string> myPair;
   char * filename = new char[100];



   menu();

   while(line!='x')
   {
         menu();

      cin >> line;


      switch(line)                                                      //Simple switch statements for testing the input and utilzing the menu
      {
         case 'l':
            cout<< "Please enter a file name: ";
            cin >> filename;

            if(ps.load(filename))
            {
               cout<<"     Success loading file";
            }
            else
            {
               cout<<"     File was not loaded succsesfully";
            }
            
            break;
        
         case 'a':
            cout<<"Enter UserName: ";
            cin >> username;
            cout<<"Enter Password: ";
            cin >> password;

            myPair.first = username;
            myPair.second = password;
            if(ps.find(username))
            {
               cout<<"     Failure, sser alrady exists"<<endl;
               break;
            }


            if(ps.addUser(myPair))
            {
               cout<<"     Success adding " << username;
            }
            else
            {
                  cout<<"     Failture to insert, user already exists";
            } 

            break;
        
         case 'r':
            cout<<"Enter UserName: ";
            cin >> username;


            if(ps.removeUser(username))
            {
               cout<< "     Success removing " <<username;
            }
            else
            {
                  cout<<"     Failure removing " << username;
            } 

            break;
         
         case 'c':
            cout<<"Enter UserName: ";
            cin >> username;
            cout<<"Enter old Password: ";
            cin >> password;
            cout<<"Enter new Password: ";
            cin >> newpassword;

            myPair.first = username;
            myPair.second = password;

            if(ps.changePassword(myPair, newpassword))
            {
               cout<<"      Succses chaning " << username << "'s password";
            }
            else
            {
                  cout<<"     Failure changing password, wrong password";
            } 
            break;
         case 'f':
               cout<<"Enter username: ";
               cin >> username;

               if(ps.find(username))
               {
                  cout<< "       Succses finding " << username;
               }
               else
               {
                  cout<<"    Failure finding " << username;
               }
               
            break;
         case 'd':
            ps.dump();
            break;
         case 's':
            cout<<"Size: " << ps.size();

            break;
         case 'w':
            cout<<"Enter filename to write to ";
            cin >> filename;

            if(ps.write_to_file(filename))
            {
               cout<<"     Succsess writing " <<filename;
            }
            else
            {
               cout<<"     Failture writing " << filename;
            }
            
            break;
         case 'x':
               cout<<"     Exiting ...";
            break;

         default:
            cout<<"     Invalid selection.";
            break;

         
   }

   }

   return 0;
}
