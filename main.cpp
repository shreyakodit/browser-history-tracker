#include <iostream>
#include <bits/stdc++.h>
#include <regex>
#include <ctime>
#include <string>
#include <vector>
using namespace std;

// function which returns a pair with url and current date
pair<string, string> stp(string url)
{
    time_t t = time(0);                    // returns the current time of the system as time since epoch
    struct tm *timeStruct = localtime(&t); // converts time since epoch to calendar time expressed as local time
    auto date = std::to_string(timeStruct->tm_mday) + std::to_string(1 + timeStruct->tm_mon) + std::to_string(1900 + timeStruct->tm_year);
    return make_pair(url, date);
}

class BrowserHistory
{
private:
    stack<pair<string, string>> s_backward; // stores the sequence of URLs accessed when the bardward button is pressed
    stack<pair<string, string>> s_forward;  // stores the sequence of URLs accessed when the forward button is pressed
    vector<pair<string, string>> history;   // stores the brwosing history

    map<string, int> freqList; // stores the frequency of each page when a user visits it

public:
    BrowserHistory()
    {
    }
    void setBrowserHistory(string homepage)
    {

        newVisit(homepage);
    }

    void newVisit(string url)
    {
        while (!s_forward.empty())
            s_forward.pop();
        s_backward.push(stp(url));
        history.push_back(stp(url));
        updateFreq(url);
    }

    void visit(string url)
    {
        history.push_back(stp(url));
        updateFreq(url);
    }

    //  function to update the frequency of each page whenever a user visits it
    void updateFreq(string url)
    {
        map<string, int>::iterator search = freqList.find(url);
        if (search == freqList.end())
        {
            freqList[url] = 1;
        }
        else
        {
            search->second++;
        }
    }

    vector<string> getFreqVisitedPage()
    {
        vector<string> freq;
        int max = 0;
        for (auto itr = freqList.begin(); itr != freqList.end(); itr++)
        {
            if (itr->second > max)
            {
                max = itr->second;
                freq.clear();
                freq.push_back(itr->first);
            }
            else if (itr->second == max)
            {
                freq.push_back(itr->first);
            }
        }
        return freq;
    }

    // function  which returns the current URL after moving back n steps
    string back(int steps)
    {
        while (s_backward.size() >= 2 && steps-- > 0)
        {
            s_forward.push(s_backward.top());
            s_backward.pop();
        }
        visit(s_backward.top().first);
        return s_backward.top().first;
    }

    // function  which returns the current URL after moving forward n steps
    string forward(int steps)
    {
        while (!s_forward.empty() && steps-- > 0)
        {
            s_backward.push(s_forward.top());
            s_forward.pop();
        }
        visit(s_backward.top().first);
        return s_backward.top().first;
    }

    // function to view full web browsing history
    void ViewBrowsingHistory()
    {
        for (auto x : history)
        {
            cout << x.first << "\t" << x.second[0] << "-" << x.second[1] << "-" << x.second.substr(2) << endl;
        }
    }

    //  function which returns the browsing history of a particular page
    vector<pair<string, string>> ViewByPage(string url)
    {
        vector<pair<string, string>> v;
        for (auto x : history)
        {
            if (x.first == url)
            {
                v.push_back(x);
            }
        }
        return v;
    }

    //  function which returns the browsing history of a particular date
    vector<pair<string, string>> ViewByDate(string date)
    {
        vector<pair<string, string>> v;
        for (auto x : history)
        {
            if (x.second == date)
            {
                v.push_back(x);
            }
        }
        return v;
    }
};

class User
{
public:
    string email = "";
    bool isLoggedIn = false;
    BrowserHistory bh;

    User() {}

    void setUser(string e, string home)
    {
        email = e;
        bh.setBrowserHistory(home);
    }

    void ViewByDate(string date)
    {
        bh.ViewByDate(date);
    }

    void ViewByPage(string url)
    {
        bh.ViewByPage(url);
    }

    void FreqVisitedPage()
    {
        vector<string> vect = bh.getFreqVisitedPage();

        for (auto x : vect)
        {
            cout << x << endl;
        }
    }
};

vector<User> userdata = {}; //(a global variable) a vector that stores all the users.

bool checkUserExistence(string email)
{
    vector<User>::iterator it;
    for (it = userdata.begin(); it != userdata.end(); it++)
    {
        if ((*it).email == email)
            break;
    }

    if (it == userdata.end())
        return false;
    else
        return true;
}
bool Email_check(string email)
{
    const regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    return regex_match(email, pattern);
}

int loginUser(string email)
{
    int i = 0;
    for (i = 0; i < userdata.size(); i++)
    {

        if (userdata[i].email == email)
        {
            userdata[i].isLoggedIn = true;
            break;
        }
    }
    if (i == userdata.size())
    {

        return -1;
    }
    else
        return i;
}
void command()
{
    int i = 0, userid = 0;
    User name1;

    while (true)
    {

        cout << "1.New User\n2.Existing user\n";
        cout << "Enter: ";
        string mail, homepage;
        cin >> i;
        cout << "Please enter email: ";
        cin >> mail;
        if (Email_check(mail))
        {
            if (i == 1)
            {
                cout << "Please enter the homepage: ";
                cin >> homepage;
                name1.setUser(mail, homepage);
                userdata.push_back(name1);
                userid = loginUser(mail);
                name1 = userdata[userid];
            }
        }
        else
        {
            cout << "Wrong Format of Mail\n";
            command();
            break;
        }

        if (i == 2)
        {
            // login
            userid = loginUser(mail);
            if (userid == -1)
            {
                cout << "Invalid creds or user does not exist" << endl;
            }
            else
                cout << "Welcome User!!" << endl;
            name1 = userdata[userid];
        }

        bool flag = true;

        if (name1.isLoggedIn == true)
        {
            do
            {
                int fn;
                cout << "Select one:\n";
                cout << "1. Visit a new URL\n";
                cout << "2. Move Forward\n";
                cout << "3. Move Backward\n";
                cout << "4. View Browsing history\n";
                cout << "5. View the most frequently visited page\n";
                cout << "6. View page by date\n";
                cout << "7. View by page\n";
                cout << "8. Logout from here\n";
                cout << "9. Press -1 to exit\n";

                cin >> fn;

                string URL;
                int steps;
                switch (fn)
                {

                case 1:
                {
                    cout << "Enter URL: ";
                    cin >> URL;
                    name1.bh.newVisit(URL);
                    cout << "\nThe current webpage is- " << URL << "\n";
                    break;
                }

                case 2:
                {
                    cout << "Number of steps to move forward: ";
                    cin >> steps;
                    cout << "The curent webpage is- " << name1.bh.forward(steps) << "\n";
                    break;
                }

                case 3:
                {
                    cout << "Number of steps to move backward: ";
                    cin >> steps;
                    cout << "The curent webpage is- " << name1.bh.back(steps) << "\n";
                    break;
                }

                case 4:
                {
                    cout << "Browsing history: " << endl;
                    name1.bh.ViewBrowsingHistory();
                    cout << endl;
                    break;
                }

                case 5:
                {
                    cout << "Most frequently visited page is/are: " << endl;
                    vector<string> freq = name1.bh.getFreqVisitedPage();
                    for (int i = 0; i < freq.size(); i++)
                        cout << freq[i] << endl;
                    cout << endl;
                    break;
                }

                case 6:
                {
                    string date;
                    cout << "Enter the date of which you want to get the history(in the format [dmyyyy]: ";
                    cin >> date;
                    vector<pair<string, string>> v = name1.bh.ViewByDate(date);
                    if (v.empty())
                        cout << "Please visit a URL before this command\n";
                    else
                    {
                        cout << "View page by date: \n";
                        for (int i = 0; i < v.size(); i++)
                            cout << v[i].first << "\t" << v[i].second[0] << "-" << v[i].second[1] << "-" << v[i].second.substr(2) << "\n";
                    }
                    break;
                }
                case 7:
                {
                    string pageURL;
                    cout << "Enter the page whose history you want: ";
                    cin >> pageURL;
                    vector<pair<string, string>> v = name1.bh.ViewByPage(pageURL);
                    if (v.empty())
                        cout << "Please visit a URL before this command\n";
                    else
                    {
                        cout << "View page by page: \n";
                        for (int i = 0; i < v.size(); i++)
                            cout << v[i].first << "\t" << v[i].second[0] << "-" << v[i].second[1] << "-" << v[i].second.substr(2) << "\n";
                    }
                    break;
                }
                case 8:
                {
                    cout << "You have been Logged out successfully\n";
                    command();
                    break;
                }

                case -1:
                {
                    exit(0);
                }

                default:
                    cout << "Not a valid number\n";
                }

            } while (flag == true);
        }
    }
}
int main()
{
    command();
    return 0;
}