#include "library.h"

int main(int argc, char* argv[])
{
    struct MainArray array = creatMainArray(10);

    findDifferencesInFilesAndSaveResults("../labs1/result.txt", 4,
            "../labs1/file1.txt", "../labs1/file2.txt", "../labs1/file3.txt", "../labs1/file4.txt");

    creatNewBlockFrom(array, "../labs1/result.txt");
//    printf("%s", array.blocks[0]->arrayOfOperations[0]);
//    deleteOperationBlock(array, 1);
    deleteOperationInBlockNr(array, 1, 0);
//    printf("%d", howManyOperationsInBlockNr(array, 1));
    printf("%d", howManyOperationsInBlockNr(array, 0));
//    printf("%s", array.blocks[1]->arrayOfOperations[0]);

    return 0;
}