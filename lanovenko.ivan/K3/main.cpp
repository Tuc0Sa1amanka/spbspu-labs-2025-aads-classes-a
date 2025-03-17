#include <iostream>
#include <string>

template< class T >
struct List {
  T data;
  List< T >* next;
};

void deleteListOfLists(List< int > * head)
{
  while (head != nullptr)
  {
    List<int>* temporary = head;
    head = head->next;
    delete temporary;
  }
}

void deleteListOfLists(List<List < int > * > * head)
{
  while (head != nullptr)
  {
    List<List < int > * > * temporary = head;
    deleteListOfLists(temporary->data);
    head = head->next;
    delete temporary;
  }
}

List< int > * convert(const int* array, size_t n)
{
  if (n == 0)
  {
    return nullptr;
  }
  List< int > * head = nullptr;
  try
  {
    head = new List<int>{ array[0], nullptr };
    List<int>* tail = head;
    for (size_t i = 1; i < n; i++)
    {
      tail->next = new List<int>{ array[i], nullptr };
      tail = tail->next;
    }
  }
  catch (const std::bad_alloc& e)
  {
    deleteListOfLists(head);
    throw;
  }
  return head;
}

List< List < int > * > * convert(const int* const* array, size_t m, size_t* n)
{
  if (m == 0)
  {
    return nullptr;
  }
  List < List < int > * > *head = nullptr;
  try
  {
    head = new List < List < int > * >{ convert(array[0], n[0]), nullptr };
    List <List < int >* >* tail = head;
    for (size_t i = 1; i < m; i++)
    {
      tail->next = new List < List < int > * >{ convert(array[i], n[i]), nullptr };
      tail = tail->next;
    }
  }
  catch (const std::bad_alloc& e)
  {
    deleteListOfLists(head);
    throw;
  }
  return head;
}

bool isEven(int a)
{
  return (a % 2 == 0);
}

bool isOdd(int a)
{
  return (a % 2 != 0);
}

template< class T, class C >
size_t count(const List< List< T > * > * head, C condition)
{
  size_t res = 0;
  const List< List< T >* >* temporary = head;
  while (temporary != nullptr)
  {
    List<T>* temporaryTwo = temporary->data;
    while (temporaryTwo != nullptr)
    {
      if (condition(temporaryTwo->data))
      {
        res++;
      }
      temporaryTwo = temporaryTwo->next;
    }
    temporary = temporary->next;
  }
  return res;
}

void deleteMatrix(int** arr, size_t arrayQuantity)
{
  for (size_t i = 0; i < arrayQuantity; i++)
  {
    delete[] arr[i];
  }
  delete[] arr;
}

int main()
{
  size_t arrayQuantity = 0;
  int** matrix = nullptr;
  size_t* sizes = nullptr;
  if (!(std::cin >> arrayQuantity))
  {
    std::cerr << "Input fail!\n";
    return 1;
  }
  try
  {
    matrix = new int* [arrayQuantity];
    sizes = new size_t[arrayQuantity];
  }
  catch (const std::bad_alloc& e)
  {
    delete[] matrix;
    return 1;
  }
  size_t created = 0;
  for (size_t i = 0; i < arrayQuantity; i++)
  {
    if (!(std::cin >> sizes[i]))
    {
      delete[] sizes;
      deleteMatrix(matrix, created);
      std::cerr << "Inptut fail\n";
      return 1;
    }
    try
    {
      matrix[i] = new int[sizes[i]];
    }
    catch (const std::bad_alloc& e)
    {
      delete[] sizes;
      deleteMatrix(matrix, created);
      std::cerr << "Out of memmory!\n";
      return 1;
    }
    created++;
    for (size_t j = 0; j < sizes[i]; j++)
    {
      if (!(std::cin >> matrix[i][j]))
      {
        delete[] sizes;
        deleteMatrix(matrix, created);
        return 1;
      }
    }
  }
  List<List < int > * > * buf = nullptr;
  try
  {
    buf = convert(matrix, arrayQuantity, sizes);
  }
  catch (const std::bad_alloc& e)
  {
    deleteMatrix(matrix, arrayQuantity);
    return 1;
  }
  std::string str = "";
  std::cin >> str;
  if (str == "odd")
  {
    std::cout << count(buf, isOdd) << '\n';
  }
  else if (str == "even")
  {
    std::cout << count(buf, isEven) << '\n';
  }
  else
  {
    std::cout << count(buf, isOdd) << " " << count(buf, isEven) << '\n';
  }
  deleteMatrix(matrix, arrayQuantity);
  delete[] sizes;
  deleteListOfLists(buf);
}
