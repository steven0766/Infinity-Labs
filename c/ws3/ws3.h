#ifndef __WS_H__
#define __WS_H__
 
 /* summerizer the rows or matrix and saving it
    int result array.
    Note: rows must be the same as matrix rows
          and result size. */
void SumOfArrayRows(int (*matrix)[4], size_t rows, int* result);

/* for any size of soldiers, who will stay alive
   after each soldier stab the one stand from
   the right side. return the index of the soldier
   who stayed alive. */
int JosephusProblem(size_t soldiers_size);

/* printing all data types and their size */
void DataTypes();

/* print all environment variables in envp in lower case */
void PrintEnvp(char** envp);

#endif /* __WS_H__ */
