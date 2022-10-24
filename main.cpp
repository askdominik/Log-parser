#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>  
#include <iomanip>


using namespace std;


struct Session
{
    string time;
    string sessionid;
    string other;
    int duration;
    friend istream& operator>>(istream& input, Session& session);
};

istream& operator>>(istream& input,Session& session)
{
  input >> session.time;
  input >> session.sessionid;
  input >> session.other;
  return input;
}

int main()
{ 
  vector<Session> sessions;
  map<string, vector<Session> > session;
  Session record;
  ifstream read("email.log");
  
  // reading from file
  while (read >> record)  
  {
    session[record.sessionid].push_back(record);
  }

  int hrs;
  int mins;
  int sec;

  for (auto& [sessionid, sessions] : session) 
  {
    if(sessions.size() == 5) // if session is complete
    {
      cout << "[" << endl;
      cout << "\t\ {" << endl;
      cout << "\t\t\"time:\" : {" << endl;
      for (int i = 0; i < sessions.size(); i++)
      {
        struct tm tm;
        istringstream firstElement;
        istringstream lastElement;

        // first time in seconds
        firstElement.str(sessions[0].time);
        firstElement >> get_time(&tm, "%Y-%m-%dT%H:%M:%S");
        time_t time1 = mktime(&tm);

        // last time in seconds
        lastElement.str(sessions[sessions.size() - 1].time);
        lastElement >> get_time(&tm, "%Y-%m-%dT%H:%M:%S");
        time_t time2 = mktime(&tm);   

        // duration in seconds
        sessions[i].duration = time2 - time1;

        // duration in hrs, mins, sec
        hrs = sessions[i].duration / 3600;
        mins = sessions[i].duration / 60;
        sec = sessions[i].duration;

        if(i == 0)
        {
          cout << "\t\t\t\"start:\" : " << "\"" << sessions[i].time << "\" ," << endl;
          cout << "\t\t\t\"duration:\" : " << "\"" << hrs << ":" << mins << ":" << sec << endl;
          cout << "\t\t\ }," << endl;
          cout << "\t\t\"sessionid:\" : " << "\"" << sessions[i].sessionid << "\" ," << endl;
          sessions[i].other.erase(0,7);
          cout << "\t\t\"client:\" : " << "\"" << sessions[i].other << "\" ," << endl;
        }  
        if(i == 1)
        {
          sessions[i].other.erase(0,11);
          cout << "\t\t\"messageid:\" : " << "\"" << sessions[i].other << "\" ," << endl;
        }
        if(i == 2)
        {
          sessions[i].other.erase(0,5);
          cout << "\t\t\"address:\" : {" << endl;
          cout << "\t\t\t\"from:\" : " << "\"" << sessions[i].other << "\" ," << endl;
        }
        if(i == 3)
        {
          sessions[i].other.erase(0,3);
          cout << "\t\t\t\"to:\" : " << "\"" << sessions[i].other << endl;
          cout << "\t\t\ }," << endl;
        }
        if(i == 4)
        {
          sessions[i].other.erase(0,7);
          cout << "\t\t\"status:\" : " << "\"" << sessions[i].other << endl;
        }

      } 
      cout << "\t\ }" << endl;
      cout << "]" << endl;
    }
  }
  return 0;
} 
