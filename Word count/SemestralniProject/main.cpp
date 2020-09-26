#include <map>
#include <chrono>
#include <vector>
#include <thread>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

//globalni promenna pro multi thread
int i;

template<typename TimePoint>
chrono::milliseconds to_ms(TimePoint tp) {
    return chrono::duration_cast<chrono::milliseconds>(tp);
}

/**
 * Metoda pro pruchod vsech souboru s textem jednym vlaknem
 * @param str - vector s nazvem vsech souboru s textem
 */
void oneThread(vector<string> str) {
    string strings;
    ifstream inputFile;
    ofstream outputFile;
    map<string, int> map;
    std::map<string, int>::iterator it;

    outputFile.open("output.txt");

    for (int ind = 0; ind < str.size(); ind++) {
        inputFile.open(str[ind]);
        if (!inputFile) {
            cout << "Unable to open input file";
            exit(1);
        } else if (!outputFile) {
            cout << "Unable to open output file";
            exit(1);
        }
        for (auto itr = map.begin(); getline(inputFile, strings); itr++) {
            string word;
            for (auto x : strings) {
                if (x == ',' || x == ' ' || x == '.' || x == '?' || x == ':' || x == '"' || x == ']' || x == ')' || x == '-' || x == ';' || x == '!') {
                    if (!word.empty()) {
                        it = map.find(word);
                        if (it != map.end())
                            map.at(word) += 1;
                        else
                            map.insert({word, 1});
                    }
                    word = "";
                } else if (x == '(' || x == '[')
                    continue;
                else {
                    char small = tolower(x);
                    word = word + small;
                }
            }
            map.end();
            inputFile.close();
        }
    }

    for (auto itr = map.begin(); itr != map.end(); itr++) {
        outputFile << itr->first << ": " << itr->second << '\n';
        cout << itr->first << ": " << itr->second << '\n';
    }

    outputFile.close();
    map.clear();
}

/**
 * Metoda pro pruchod vsech souboru s textem a bude potreba tolik vlaken kolik mam souboru(pro kazdy soubor jedno vlakno)
 * @param str - vector s nazvem vsech souboru s textem
 */
void multiThread(vector<string> str) {
    string strings;
    ifstream inputFile;
    ofstream outputFile;
    map<string, int> map;
    std::map<string, int>::iterator it;

    outputFile.open("output.txt");

    i = i;
    inputFile.open(str[i]);
    if (!inputFile) {
        cout << "Unable to open input file";
        exit(1);
    } else if (!outputFile) {
        cout << "Unable to open output file";
        exit(1);
    }
    for (auto itr = map.begin(); getline(inputFile, strings); itr++) {
        string word;
        for (auto x : strings) {
            if (x == ',' || x == ' ' || x == '.' || x == '?' || x == ':' || x == '"' || x == ']' || x == ')' || x == '-' || x == ';' || x == '!') {
                if (!word.empty()) {
                    it = map.find(word);
                    if (it != map.end())
                        map.at(word) += 1;
                    else
                        map.insert({word, 1});
                }
                word = "";
            } else if (x == '(' || x == '[')
                continue;
            else {
                char small = tolower(x);
                word = word + small;
            }
        }
        map.end();
        inputFile.close();
    }
    i++;
    for (auto itr = map.begin(); itr != map.end(); itr++) {
        outputFile << itr->first << ": " << itr->second << '\n';
        cout << itr->first << ": " << itr->second << '\n';
    }

    outputFile.close();
    map.clear();
}

int main(int argc, char *argv[]) {
    string line;
    string argvFirst;
    string argvSecond;
    ifstream inputFile;
    vector<string> files;
    vector<string> files1;
    vector<string> outPrint = {
        "\nFirst argument: \"-one\" for One Thread run or \"-multi\" for Multi Thread run\n\nSecond argument: filename with his format\n\nExample of input: \"-one input.txt \" or \"-multi input.txt\"\n",
        "Please write help information: \"--help\" without second argument\n",
        "Please write filename with his format\nExample:\"filename.txt\"\n",
        "Please write correct thread mode in first argument\nOne Thread run: -one \nMulti Thread run: -multi\nHelp information: --help\nPlease write filename with his format in second argument\nExample:\"filename.txt\"\n\n",
        "Please write correct thread mode in first argument\nOne Thread run: -one \nMulti Thread run: -multi\nHelp information: --help\n"
    };

    //Jestli argumenty byly vlozeny pri spusteni
    if(argc > 1) {
        argvFirst = argv[1];
        if(argv[2] != nullptr)
            argvSecond = argv[2];
    } else { //tento pripad jestli argumenty nebyly uvedeny pri spusteni, tak bude potreba napsat rucne
        cout << outPrint[3] << "INPUT: ";
        cin >> argvFirst;
        if(argvFirst == "--help")
            argv[2] = nullptr;
        else
            cin >> argvSecond;
    }

    //Pripad jestli bylo uvedeno pouzit jedno vlakno
    if (argvFirst == "-one" && argvSecond == "input.txt") {
        cout << "One Thread is running\n";

        auto start = chrono::high_resolution_clock::now();

        inputFile.open(argvSecond);

        while (getline(inputFile, line))
            if (!line.empty())
                files.push_back(line);
        oneThread(files);

        auto end = chrono::high_resolution_clock::now();
        cout << "Needed in One Thread " << to_ms(end - start).count() << " ms to finish.\n";
    }

    //Pripad jestli bylo uvedeno pouzit nekolik vlaken
    else if (argvFirst == "-multi" && argvSecond == "input.txt") {
        cout << "Multi Thread is running\n";
        auto start = chrono::high_resolution_clock::now();

        inputFile.open(argvSecond);

        while (getline(inputFile, line))
            if (!line.empty())
                files.push_back(line);

        i = 0;
        for(int ind = 0; ind < files.size(); ind++) {
            thread thread(multiThread, files);
            thread.join();
        }

        auto end = chrono::high_resolution_clock::now();
        cout << "Needed in Multi Thread " << to_ms(end - start).count() << " ms to finish.\n";
    }

    //Pripad jestli bylo uvedeno pomocni informace a nebyl uveden druhy argument
    else if (argvFirst == "--help" && argvSecond.empty()) {
        cout << outPrint[0];
        return 0;
    }

    //Pripad jestli bylo uvedeno pomocni informace a byl uveden druhy argument
    else if (argvFirst == "--help" && !argvSecond.empty()) {
        cerr << outPrint[1];
        return 1;
    }

    //Pripad jestli bylo uvedeno spravny prvni argument vlaken a byla nalezena chyba s druhym argumentem
    else if ((argvFirst == "-one" || argvFirst == "-multi") && (argvSecond.empty() || argvSecond != "input.txt")) {
        cerr << outPrint[2] << endl;
        return 1;
    }

    //Pripad jestli byla nalezena chyba s prvnim argumentem vlaken a byla nalezena chyba s druhym argumentem
    else if((argvFirst != "-one" || argvFirst != "-multi") && argvSecond != "input.txt") {
        cerr << outPrint[3] << endl;
        return 1;
    }

    //Pripad jestli byla nalezena chyba jenom s prvnim argumentem vlaken
    else {
        cerr << outPrint[4] << endl;
        return 1;
    }
    return 0;
}
