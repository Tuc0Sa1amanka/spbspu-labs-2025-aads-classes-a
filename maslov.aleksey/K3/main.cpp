#include <iostream>
#include <cstddef>
#include <string>

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
    List< List< int > * > * outHead = head->next;
    List< int > * inHead = head->data;
    while (inHead)
    {
      List< int > * node = inHead->next;
      delete inHead;
      inHead = node;
    }
    delete head;
    head = outHead;
  }
}
List< List< int > * > * convert(const int * const * d, size_t m, const size_t * n)
{
  List< List< int > * > * head = new List< List< int > * >{nullptr, nullptr};
  List< List< int > * > * tail = head;
  for (size_t i = 0; i < m; ++i)
  {
    List< int > * inHead = nullptr;
    List< int > * inTail = nullptr;
    try
    {
      inHead = new List< int >{d[i][0], nullptr};
      inTail = inHead;
      for (size_t j = 1; j < n[i]; ++j)
      {
        List< int > * newElement = new List< int >{d[i][j], nullptr};
        inTail->next = newElement;
        inTail = newElement;
      }
      tail->next = new List< List< int > * >{inHead, nullptr};
      tail = tail->next;
    }
    catch (const std::bad_alloc &)
    {
      deleteList(head);
      throw;
    }
  }
  return head;
}
void deleteArray(int ** array, size_t numArray)
{
  for (size_t i = 0; i < numArray; ++i)
  {
    delete[] array[i];
  }
  delete[] array;
}
bool isEven(int number)
{
  return number % 2 == 0;
}
bool isOdd(int number)
{
  return number % 2 != 0;
}
template< class T, class C >
size_t count(const List< List< T > * > * head, C condition)
{
  size_t count = 0;
  while (head)
  {
    List< int > * subhead = head->data;
    while (subhead)
    {
      if (condition(subhead->data))
      {
        count++;
      }
      subhead = subhead->next;
    }
    head = head->next;
  }
  return count;
}

int main()
{
  size_t numArray = 0;
  if (!(std::cin >> numArray))
  {
    std::cerr << "error input: number of arrays\n";
    return 1;
  }
  int ** array = nullptr;
  size_t * numElements = nullptr;
  try
  {
    array = new int * [numArray];
    numElements = new size_t[numArray];
  }
  catch (const std::bad_alloc &)
  {
    std::cerr << "memory error\n";
    delete[] array;
    return 1;
  }
  for (size_t i = 0; i < numArray; i++)
  {
    if (!(std::cin >> numElements[i]))
    {
      std::cerr << "error input: number of size\n";
      deleteArray(array, i);
      delete[] numElements;
      return 1;
    }
    try
    {
      array[i] = new int[numElements[i]];
    }
    catch (const std::bad_alloc &)
    {
      std::cerr << "memory error\n";
      deleteArray(array, i);
      delete[] numElements;
      return 1;
    }
    for (size_t j = 0; j < numElements[i]; j++)
    {
      if (!(std::cin >> array[i][j]))
      {
        std::cerr << "error input: element\n";
        deleteArray(array, i + 1);
        delete[] numElements;
        return 1;
      }
    }
  }
  List< List< int > * > * head = nullptr;
  try
  {
    head = convert(array, numArray, numElements);
  }
  catch (const std::bad_alloc &)
  {
    std::cerr << "memory error\n";
    deleteArray(array, numArray);
    delete[] numElements;
    return 1;
  }
  deleteArray(array, numArray);
  delete[] numElements;
  std::string name;
  std::cin >> name;
  if (name == "even")
  {
    std::cout << count(head, isEven) << "\n";
  }
  else if (name == "odd")
  {
    std::cout << count(head, isOdd) << "\n";
  }
  else
  {
    std::cout << count(head, isOdd) << " " << count(head, isEven) << "\n";
  }
  deleteList(head);
}
