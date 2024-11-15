#include <nfd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

int main( void )
{
    int colorcount;
    cout << "Enter number of colours.\n";
    cin >> colorcount;
    /*while (cout << "Enter number of colours.\n" && !(cin >> colorcount) && colorcount < 1)
    {
        std::cin.clear();
        if (colorcount < 1)
        {
            std::cout << "Number of colors must be 1 or more, please try again.\n";
        }
        else
        {
            std::string line;
            std::getline(std::cin, line);
            std::cout << line << "\nI am sorry, but '" << line << "' is not a number\n";
        }
    }
    const char *code = "M160";*/

    NFD_Init();
    nfdu8char_t* outPath;
    nfdu8filteritem_t filters[1] = { { "Gcodes", "gcode" } };
    nfdopendialogu8args_t args = { 0 };
    args.filterList = filters;
    args.filterCount = 1;
    ifstream fileIn;
    vector<string> buff;
    for (int i = 0; i < colorcount; i++) 
    {    
        nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);
        if (result == NFD_OKAY) 
        {
            cout << "File #" << i + 1 << " opened successfully!\n";
            vector<string> buffToBuff;
            fileIn.open(outPath);
            NFD_FreePathU8(outPath);
            string line;
            while (getline(fileIn, line))
            {
                buffToBuff.push_back(line);
            }
            if (i < colorcount - 1)
            {
                buffToBuff.erase(find(buffToBuff.rbegin(), buffToBuff.rend(), "M107").base(), buffToBuff.end());
                buffToBuff.push_back("M600");
            }
            if (i > 0)
            {
                buffToBuff.erase(buffToBuff.begin(), find(buffToBuff.begin(), buffToBuff.end(), "M107"));
            }
            buff.insert(buff.end(), buffToBuff.begin(), buffToBuff.end());
            fileIn.close();
        }
        else if (result == NFD_CANCEL) 
        {
            puts("User pressed cancel.");
            return 0;
        }
        else 
        {
            printf("Error: %s\n", NFD_GetError());
            return 0;
        }
    }
    
    nfdchar_t *savePath = NULL;
    nfdresult_t result = NFD_SaveDialogU8(&savePath, filters, 1, NULL, NULL);
    if ( result == NFD_OKAY )
    {
        puts("Success!");
        ofstream fileOut (savePath);
        for (const auto& item : buff)
        {
            fileOut << item << endl;
        }
        fileOut.close();
        free(savePath);
    }
    else if ( result == NFD_CANCEL )
    {
        puts("User pressed cancel.");
    }
    else 
    {
        printf("Error: %s\n", NFD_GetError() );
    }
    NFD_Quit();
    return 0;
}