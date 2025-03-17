#include <iostream>
#include <string>

template< class T >
struct List {
  T data;
  List< T > * next;
};

void deleteList(List< List< int > * > * head)
{
  List< List< int > * > * listOfLists = head;
  while (listOfLists)
  {
    List< int > * currentList = listOfLists->data;
    while (currentList)
    {
      List< int > * tempNode = currentList;
      currentList = currentList->next;
      delete tempNode;
    }
    List< List< int > * > * tempList = listOfLists;
    listOfLists = listOfLists->next;
    delete tempList;
  }
}

List< List< int > * > * convert(const int * const * d, size_t m, const size_t * n)
{
  if (m == 0)
  {
    return nullptr;
  }
  List< List< int > * > * head = new List< List< int > * > ();
  List< List< int > * > * tail = head;
  try
  {
    for (size_t i = 0; i < m; i++)
    {
      List< int > * subhead = new List< int > {d[i][0], nullptr};
      List< int > * current = subhead;
      for (size_t j = 1; j < n[i]; j++)
      {
        List< int > * newNode = new List< int > {d[i][j], nullptr};
        current->next = newNode;
        current = newNode;
      }
      tail->next = new List< List< int > * > {subhead, nullptr};
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

bool isEven(int num)
{
  return num % 2 == 0;
}

bool isOdd(int num)
{
  return num % 2 != 0;
}

template< class T, class C >
size_t count(const List< List< T > * > * head, C condition)
{
  size_t k = 0;
  while (head)
  {
    List< int > * current = head->data;
    while (current)
    {
      if (condition(current->data))
      {
        k++;
      }
      current = current->next;
    }
    head = head->next;
  }
  return k;
}

void deleteArr(int ** arr, size_t count)
{
  for (size_t i = 0; i < count; ++i)
  {
    delete[] arr[i];
  }
  delete[] arr;
}

int main()
{
  size_t M = 0;
  if (!(std::cin >> M))
  {
    std::cerr << "incorrect input\n";
    return 1;
  }
  int ** arr = nullptr;
  size_t i = 0;
  size_t * n = nullptr;
  try
  {
    arr = new int * [M];
    n = new size_t[M];
    for (; i < M; ++i)
    {
      size_t N = 0;
      if (!(std::cin >> N))
      {
        delete[] n;
        deleteArr(arr, i);
        std::cerr << "incorrect input\n";
        return 1;
      }
      arr[i] = new int[N];
      n[i] = N;
      for (size_t j = 0; j < N; ++j)
      {
        if (!(std::cin >> arr[i][j]))
        {
          delete[] n;
          deleteArr(arr, i + 1);
          std::cerr << "incorrect input\n";
          return 1;
        }
      }
    }
  }
  catch (const std::exception & e)
  {
    delete[] n;
    deleteArr(arr, i);
    std::cerr << e.what() << "\n";
    return 1;
  }

  List< List< int > * > * head = nullptr;
  try
  {
    head = convert(arr, M, n);
  }
  catch(const std::bad_alloc &)
  {
    delete[] n;
    deleteArr(arr, i);
    std::cerr << "Out of memory\n";
    return 1;
  }
  delete[] n;
  deleteArr(arr, i);

  std::string cond;
  std::cin >> cond;
  if (cond == "odd")
  {
    std::cout << count(head, isOdd) << "\n";
  }
  else if (cond == "even")
  {
    std::cout << count(head, isEven) << "\n";
  }
  else
  {
    std::cout << count(head, isOdd) << " " << count(head, isEven) << "\n";
  }
  deleteList(head);
}
