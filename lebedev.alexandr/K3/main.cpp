#include <iostream>
#include <string>

template< class T >
struct List
{
  T data;
  List< T > * next;
};

void deleteList(List< List< int >* >* head)
{
  List< List< int >* >* listOfLists = head;
  while (listOfLists)
  {
    List< int >* currentList = listOfLists->data;
    while (currentList)
    {
      List< int >* tempNode = currentList->next;
      delete currentList;
      currentList = tempNode;
    }
    List< List< int >* >* tempNode = listOfLists->next;
    delete listOfLists;
    listOfLists = tempNode;
  }
}

List< List< int >* >* convert(const int* const* d, size_t m, const size_t* n)
{
  if (m == 0)
  {
    return nullptr;
  }
  List< List< int >* >* head = nullptr;
  List< List< int >* >* tail = nullptr;
  try
  {
    head = new List< List< int >* >{ nullptr, nullptr };
    tail = head;
    for (size_t i = 0; i < m; i++)
    {
      List< int >* subhead = new List< int >{ d[i][0], nullptr };
      List< int >* subtail = subhead;
      for (size_t j = 1; j < n[i]; j++)
      {
        List< int >* temp = new List< int >{ d[i][j], nullptr };
        subtail->next = temp;
        subtail = temp;
      }
      tail->data = subhead;
      tail->next = new List< List< int >* >{ nullptr, nullptr };
      tail = tail->next;
    }
  }
  catch (...)
  {
    deleteList(head);
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
size_t count(const List< List< T >* >* head, C condition)
{
  size_t c = 0;
  while (head)
  {
    List< int >* subhead = head->data;
    while (subhead)
    {
      if (condition(subhead->data))
      {
        c++;
      }
      subhead = subhead->next;
    }
    head = head->next;
  }
  return c;
}

void deleteArray(int** t, size_t created)
{
  for (size_t i = 0; i < created; i++)
  {
    delete[] t[i];
  }
  delete[] t;
}

int main()
{
  size_t m = 0, n = 0, created = 0;
  if (!(std::cin >> m))
  {
    std::cerr << "Incorrect input!\n";
    return 1;
  }

  int** ptr = nullptr;
  size_t* sizes = nullptr;
  try
  {
    ptr = new int*[m];
    sizes = new size_t[m];
  }
  catch (const std::bad_alloc& e)
  {
    std::cerr << "Memory allocation error!\n";
    deleteArray(ptr, created);
    delete[] sizes;
    return 2;
  }

  while (created < m)
  {
    if (!(std::cin >> n))
    {
      std::cerr << "Incorrect input!\n";
      deleteArray(ptr, created);
      delete[] sizes;
      return 1;
    }
    try
    {
      ptr[created] = new int[n];
      sizes[created] = n;
    }
    catch (const std::bad_alloc& e)
    {
      deleteArray(ptr, created);
      delete[] sizes;
      std::cerr << "Memory allocation error!\n";
      return 2;
    }

    for (size_t i = 0; i < n; i++)
    {
      if (!(std::cin >> ptr[created][i]))
      {
        std::cerr << "Incorrect input!\n";
        deleteArray(ptr, created + 1);
        delete[] sizes;
        return 1;
      }
    }
    created++;
  }

  List< List< int >* >* head = nullptr;
  try
  {
    head = convert(ptr, m, sizes);
  }
  catch (const std::bad_alloc& e)
  {
    deleteArray(ptr, m);
    delete[] sizes;
    std::cerr << "Memory allocation error!\n";
    return 2;
  }
  deleteArray(ptr, m);
  delete[] sizes;

  std::string condition;
  std::cin >> condition;
  if (condition == "odd")
  {
    std::cout << count(head, isOdd) << '\n';
  }
  else if (condition == "even")
  {
    std::cout << count(head, isEven) << '\n';
  }
  else
  {
    std::cout << count(head, isOdd) << ' ' << count(head, isEven) << '\n';
  }

  deleteList(head);
  return 0;
}
