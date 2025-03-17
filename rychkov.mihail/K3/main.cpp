#include <iostream>
#include <string>

namespace rychkov
{
  template< class T >
  struct List
  {
    T data;
    List< T >* next = nullptr;
  };

  void destroy(List< int >* head)
  {
    while (head != nullptr)
    {
      auto temp = head;
      head = head->next;
      delete temp;
    }
  }
  void destroy(List< List< int >* >* head)
  {
    while (head != nullptr)
    {
      destroy(head->data);
      auto temp = head;
      head = head->next;
      delete temp;
    }
  }
  void destroy(int** arr, size_t height, size_t* widthArr)
  {
    while (height > 0)
    {
      delete[] arr[--height];
    }
    delete[] arr;
    delete[] widthArr;
  }

  List< int >* convert(const int* arr, size_t size)
  {
    if (size == 0)
    {
      return nullptr;
    }
    auto head = new List< int >{arr[0]};
    auto tail = head;
    for (size_t i = 1; i < size; i++)
    {
      try
      {
        tail->next = new List< int >{arr[i]};
        tail = tail->next;
      }
      catch (...)
      {
        destroy(head);
        throw;
      }
    }
    return head;
  }
  List< List< int >* >* convert(const int*const* arr, size_t height, const size_t* widthArr)
  {
    if (height == 0)
    {
      return nullptr;
    }
    List< List< int >* >* head = new List< List< int >* >;
    auto tail = head;
    try
    {
      tail->data = convert(arr[0], widthArr[0]);
      for (size_t i = 1; i < height; i++)
      {
        tail->next = new List< List< int >* >;
        tail = tail->next;
        tail->data = convert(arr[i], widthArr[i]);
      }
    }
    catch (...)
    {
      destroy(head);
      throw;
    }
    return head;
  }

  template< class T, class C >
  size_t count(const List< List< T >* >* head, C condition)
  {
    size_t result = 0;
    for (; head != nullptr; head = head->next)
    {
      for (auto i = head->data; i != nullptr; i = i->next)
      {
        if (condition(i->data))
        {
          result++;
        }
      }
    }
    return result;
  }

  bool isEven(int number)
  {
    return number % 2 == 0;
  }
  bool isOdd(int number)
  {
    return number % 2 == 1;
  }
}

int main()
{
  size_t height = 0;
  if (!(std::cin >> height))
  {
    std::cerr << "failed to read arrays count\n";
    return 1;
  }
  int** arr = nullptr;
  size_t* widthArr = nullptr;
  try
  {
    arr = new int*[height]{};
    widthArr = new size_t[height]{};
  }
  catch (...)
  {
    delete[] arr;
    return 2;
  }

  for (size_t i = 0; i < height; i++)
  {
    if (!(std::cin >> widthArr[i]))
    {
      std::cerr << "failed to read array (#" << i + 1 << ") length\n";
      rychkov::destroy(arr, i, widthArr);
      return 1;
    }

    try
    {
      arr[i] = new int[widthArr[i]];
    }
    catch (...)
    {
      rychkov::destroy(arr, i, widthArr);
      return 2;
    }

    for (size_t j = 0; j < widthArr[i]; j++)
    {
      if (!(std::cin >> arr[i][j]))
      {
        std::cerr << "failed to read value[" << i << "][" << j << "]\n";
        rychkov::destroy(arr, i + 1, widthArr);
        return 1;
      }
    }
  }

  rychkov::List< rychkov::List< int >* >* head = nullptr;
  try
  {
    head = rychkov::convert(arr, height, widthArr);
  }
  catch (...)
  {
    rychkov::destroy(arr, height, widthArr);
    return 2;
  }
  rychkov::destroy(arr, height, widthArr);

  std::string task;
  std::cin >> task;
  if (task == "odd")
  {
    std::cout << rychkov::count(head, rychkov::isOdd) << '\n';
  }
  else if (task == "even")
  {
    std::cout << rychkov::count(head, rychkov::isEven) << '\n';
  }
  else
  {
    std::cout << rychkov::count(head, rychkov::isOdd) << ' ';
    std::cout << rychkov::count(head, rychkov::isEven) << '\n';
  }
  rychkov::destroy(head);
}
