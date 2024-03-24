#include "TetrisMini.h"
#include "TimeCheck.h"

int main()
{
    int inputTetris = 0;

    tableInit();
    timeInit();
    
    while (gameCheck())
    {
        if (inputTetris = gameInputCatch())
        {
            if (inputTetris == 'q')
            {
                break;
            }
            
            blockChange(inputTetris);
        }
        
        if (timeCheck())
        {
            blockChange('s');
        }
    }
    
    tableClose();
    return 0;
}
