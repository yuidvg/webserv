#include ".hpp"

void deleteCStrArray(char *const *array)
{
    for (int i = 0; array[i]; i++)
    {
        delete array[i];
    }
    delete[] array;
}
