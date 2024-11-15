#include <nfd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <string>

int main( void )
{
    int colorcount;
    std::cout << "Enter number of colours\n";
    std::cin >> colorcount;

    NFD_Init();
    nfdu8char_t* outPath;
    nfdu8filteritem_t filters[1] = { { "Gcodes", "gcode" } };
    nfdopendialogu8args_t args = { 0 };
    args.filterList = filters;
    args.filterCount = 1;
    std::ifstream fileIn;
    for (int i = 0; i < colorcount; i++) 
    {    
        nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);
        if (result == NFD_OKAY) 
        {
            std::cout << "File #" << colorcount << " opened successfully!\n";
            fileIn.open(outPath);
            NFD_FreePathU8(outPath);
            
        }
        else if (result == NFD_CANCEL) 
        {
            puts("User pressed cancel.");
            break;
        }
        else 
        {
            printf("Error: %s\n", NFD_GetError());
            break;
        }
    }
    
    nfdchar_t *savePath = NULL;
    nfdresult_t result = NFD_SaveDialogU8(&savePath, filters, 1, NULL, NULL);
    if ( result == NFD_OKAY )
    {
        puts("Success!");
        std::ofstream fileOut (savePath);
        std::istreambuf_iterator<char> begin_file(fileIn);
        std::copy(std::istreambuf_iterator<char>(fileIn), 
        std::istreambuf_iterator<char>(), 
        std::ostreambuf_iterator<char>(fileOut));
        fileIn.close();
        fileOut.close();
        std::cout << savePath << std::endl;
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