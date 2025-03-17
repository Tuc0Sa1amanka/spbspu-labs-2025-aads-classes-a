#include <iostream>
#include <string>
#include <cstddef>

namespace
{
  template< class T >
  struct List {
    T data;
    List< T > * next;
  };

  void deleteMtx(int** matrix, size_t m)
  {
    for (size_t i = 0; i < m; ++i)
    {
      delete[] matrix[i];
    }
    delete[] matrix;
  }

  template< class T >
  void deleteList(List< T >* head)
  {
    while (head != nullptr)
    {
      List< T >* next_head = head->next;
      delete head;
      head = next_head;
    }
  }

  template< class T >
  void deleteListOfLists(List< List< T >* > * head)
  {
    while (head != nullptr)
    {
      List< List< T >* > * next_head = head->next;
      deleteList(head->data);
      delete head;
      head = next_head;
    }
  }

  List< List< int > * > * convert(const int * const * d, const size_t m, const size_t * n)
  {
    if (m == 0)
    {
      return nullptr;
    }
    List< List< int > * > * head = new List< List< int > * >{nullptr, nullptr};
    List< List< int > * > * tail = head;
    for (size_t i = 0; i < m; ++i)
    {
      if (n[i] == 0)
      {
        continue;
      }
      List< int > * innhead = nullptr;
      try
      {
        innhead = new List< int >{d[i][0], nullptr};
        List< int > * inntail = innhead;
        for (size_t j = 1; j < n[i]; j++)
        {
          inntail->next = new List< int >{d[i][j],nullptr};
          inntail = inntail->next;
        }
        tail->next = new List< List< int > * >{innhead, nullptr};
        tail = tail->next;
      }
      catch (const std::bad_alloc&)
      {
        deleteList(innhead);
        deleteListOfLists(head);
        throw;
      }
    }
    List< List< int > * > * temp = head->next;
    delete head;
    head = temp;
    return head;
  }

  template< class T, class C >
  size_t count(const List< List< T > * > * head, C condition)
  {
    size_t counter = 0;
    for (const List< List< T > * > * i = head; i != nullptr; i = i->next)
    {
      for (List< T > * j = i->data; j != nullptr; j = j->next)
      {
        if (condition(j->data))
        {
          counter++;
        }
      }
    }
    return counter;
  }

  bool isOdd(int n)
  {
    return n % 2 != 0;
  }

  bool isEven(int n)
  {
    return n % 2 == 0;
  }
}

int main()
{
  int arrays_size = 0;
  if ((!(std::cin >> arrays_size)) || arrays_size < 0)
  {
    std::cerr << "No arrays\n";
    return 1;
  }
  int** arrays = nullptr;
  size_t* sizes = nullptr;
  try
  {
    arrays = new int*[arrays_size];
    sizes = new size_t[arrays_size];
  }
  catch (const std::bad_alloc&)
  {
    delete[] arrays;
    delete[] sizes;
    std::cerr << "Not enough memory\n";
    return 1;
  }
  size_t created = 0;
  for (;created < size_t(arrays_size);)
  {
    if (!(std::cin >> sizes[created]))
    {
      delete[] sizes;
      deleteMtx(arrays, created);
      std::cerr << "Input error\n";
      return 1;
    }
    try
    {
      arrays[created] = new int[sizes[created]];
    }
    catch (const std::bad_alloc&)
    {
      std::cerr << "Not enough memory\n";
      delete[] sizes;
      deleteMtx(arrays, created);
      return 1;
    }
    created++;
    for (size_t j = 0; j < sizes[created - 1]; ++j)
    {
      if (!(std:: cin >> arrays[created - 1][j]))
      {
        delete[] sizes;
        deleteMtx(arrays, created);
        std::cerr << "Input error\n";
        return 1;
      }
    }
  }
  List< List< int > * > * head = nullptr;
  try
  {
    head = convert(arrays, arrays_size, sizes);
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "Not enough memory\n";
    delete[] sizes;
    deleteMtx(arrays, created);
    return 1;
  }
  std::string s = "";
  std::cin >> s;
  if (s == "odd")
  {
    std::cout << count(head, isOdd) << "\n";
  }
  else if (s == "even")
  {
    std::cout << count(head, isEven) << "\n";
  }
  else
  {
    std::cout << count(head, isOdd) << " " << count(head, isEven) << "\n";
  }
  deleteListOfLists(head);
  deleteMtx(arrays, created);
  delete[] sizes;
  return 0;
}
