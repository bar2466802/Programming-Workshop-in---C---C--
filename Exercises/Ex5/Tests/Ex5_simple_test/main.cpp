#include "RecommenderSystem.h"
#include <iostream>
#include <fstream>
#include <string>

#define ILLEGAL_OUT "your_res_1.txt"

using std::cout;
using std::endl;
using std::ofstream;


string UNAMES[2] = {"Miles", "Michaela"};
string MOVIES[2] = {"TopHat", "TropicalMalady"};
string UNAMES_SEEN[2] = {"Reina", "Celine"};

void printIllegals(RecommenderSystem& rc)
{
    // look for non existing user/ movie

    string uname = "a523sgf";
    cout << rc.recommendByContent(uname) << endl;
    cout << rc.recommendByCF(uname, 1) << endl;
    cout << rc.predictMovieScoreForUser("TheKid", uname, 1) << endl;
    cout << rc.predictMovieScoreForUser(uname, "Reina", 1) << endl;

    cout << "\nlegal vals:" << endl;
    for (const string& n : UNAMES)
    {
        cout << "recommend by content: " << rc.recommendByContent(n) << endl;
        for (int k = 1; k < 4; k++)
        {
            cout << "recommend by cf, k = " << k << ": " << rc.recommendByCF(n, k) << endl;
            for (const string& m: MOVIES)
            {
                cout << "predict, k = " << k << ": " << rc.predictMovieScoreForUser(m, n, k) << endl;
            }
        }
    }

    cout << "\nillegal vals:" << endl;
    for (const string& n : UNAMES_SEEN)
    {
        cout << "recommend by content: " << rc.recommendByContent(n) << endl;
        for (int k = 1; k < 2; k++)
        {
            cout << "recommend by cf, k = " << k << ": " << rc.recommendByCF(n, k) << endl;
            for (const string& m: MOVIES)
            {
                cout << "predict, k = " << k << ": " << rc.predictMovieScoreForUser(m, n, k) << endl;
            }
        }
    }
}


int main()
{
    // init rc obj
    cout << "started" << endl;
    RecommenderSystem rc = RecommenderSystem();
    // illegal path try
    rc.loadData("iasvf", "ranks_big.txt");
    cout << "next" << endl;
    cout<< "loaded: " << rc.loadData("movies_big.txt", "ranks_big.txt") << endl;
    printIllegals(rc);
}