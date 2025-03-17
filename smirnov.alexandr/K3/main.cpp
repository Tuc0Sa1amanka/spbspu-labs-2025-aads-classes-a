#include <iostream>
#include <cstddef>
#include <stdexcept>

template< class T >
struct List
{
  T data;
  List< T > * next;
};

void deleteList(List< List< int > * > * head)
{
  while (head)
  {
    List< List< int > * > * temp = head;
    head = head->next;
    List< int > * subList = temp->data;
    while (subList)
    {
      List< int > * subTemp = subList;
      subList = subList->next;
      delete subTemp;
    }
    delete temp;
  }
}

List< List< int > * > * convert(const int * const * d, size_t m, const size_t * n)
{
  if (m == 0)
  {
    return nullptr;
  }
  List< List< int > * > * head = nullptr;
  List< List< int > * > * current = nullptr;
  try
  {
    for (size_t i = 0; i < m; ++i)
    {
      List< int > * subHead = nullptr;
      List< int > * subCurrent = nullptr;
      for (size_t j = 0; j < n[i]; ++j)
      {
        List< int > * newNode = new List< int >;
        newNode->data = d[i][j];
        newNode->next = nullptr;
        if (!subHead)
        {
          subHead = newNode;
          subCurrent = subHead;
        }
        else
        {
          subCurrent->next = newNode;
          subCurrent = subCurrent->next;
        }
      }
      List< List< int > * > * newList = new List< List< int > * >;
      newList->data = subHead;
      newList->next = nullptr;
      if (!head)
      {
        head = newList;
        current = head;
      }
      else
      {
        current->next = newList;
        current = current->next;
      }
    }
  }
  catch (const std::bad_alloc & e)
  {
    deleteList(head);
    throw;
  }
  return head;
}

template< class T, class C >
size_t count(const List< List< T > * > * head, C condition)
{
  size_t count = 0;
  while (head)
  {
    List< T > * subList = head->data;
    while (subList)
    {
      if (condition(subList->data))
      {
        ++count;
      }
      subList = subList->next;
    }
    head = head->next;
  }
  return count;
}

bool isOdd(int x)
{
  return x % 2 != 0;
}

bool isEven(int x)
{
  return x % 2 == 0;
}

void deleteArrays(int ** arrays, size_t numArrays)
{
  for (size_t i = 0; i < numArrays; ++i)
  {
    if (arrays[i])
    {
      delete[] arrays[i];
    }
  }
  delete[] arrays;
}

int main()
{
  int ** arrays = nullptr;
  size_t * sizes = nullptr;
  size_t numArrays = 0;
  try
  {
    std::cin >> numArrays;
    if (!std::cin)
    {
      std::cerr << "Input error\n";
      return 1;
    }
    arrays = new int * [numArrays];
    sizes = new size_t[numArrays];
    for (size_t i = 0; i < numArrays; ++i)
    {
      size_t arraySize;
      std::cin >> arraySize;
      if (!std::cin)
      {
        std::cerr << "Input error\n";
        deleteArrays(arrays, i);
        delete[] sizes;
        return 1;
      }
      arrays[i] = new int[arraySize];
      sizes[i] = arraySize;
      for (size_t j = 0; j < arraySize; ++j)
      {
        std::cin >> arrays[i][j];
        if (!std::cin)
        {
          std::cerr << "Input error\n";
          deleteArrays(arrays, i + 1);
          delete[] sizes;
          return 1;
        }
      }
    }
  }
  catch (const std::exception & e)
  {
    std::cerr << e.what() << "\n";
    deleteArrays(arrays, numArrays);
    delete[] sizes;
    return 1;
  }
  List< List< int > * > * head = nullptr;
  try
  {
    head = convert(arrays, numArrays, sizes);
  }
  catch(const std::bad_alloc &)
  {
    std::cerr << "Out of memory\n";
    deleteArrays(arrays, numArrays);
    delete[] sizes;
    return 1;
  }
  std::string type;
  std::cin >> type;
  if (type == "odd")
  {
    std::cout << count(head, isOdd) << "\n";
  }
  else if (type == "even")
  {
    std::cout << count(head, isEven) << "\n";
  }
  else
  {
    std::cout << count(head, isOdd) << " " << count(head, isEven) << "\n";
  }
  deleteList(head);
  deleteArrays(arrays, numArrays);
  delete[] sizes;
}
