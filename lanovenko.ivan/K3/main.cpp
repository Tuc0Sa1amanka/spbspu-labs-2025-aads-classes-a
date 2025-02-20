#include <iostream>
#include <string>

struct List
{
  size_t data;
  List* next;
};

struct ListOfLists
{
  List* data;
  ListOfLists* next;
};

void deleteListOfLists(List* head)
{
  while (head != nullptr)
  {
    List* temporary = head;
    head = head->next;
    delete temporary;
  }
}

void deleteListOfLists(ListOfLists* head)
{
  while (head != nullptr)
  {
    ListOfLists* temporary = head;
    deleteListOfLists(temporary->data);
    head = head->next;
    delete temporary;
  }
}

List* convert(const size_t* array, size_t n)
{
  if (n == 0)
  {
    return nullptr;
  }
  List* head = nullptr;
  try
  {
    head = new List{ array[0], nullptr };
    List* tail = head;
    for (size_t i = 1; i < n; i++)
    {
      tail->next = new List{ array[i], nullptr };
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

ListOfLists* convert(const size_t* const* array, size_t m, size_t* n)
{
  if (m == 0)
  {
    return nullptr;
  }
  ListOfLists* head = nullptr;
  try
  {
    head = new ListOfLists{ convert(array[0], n[0]), nullptr };
    ListOfLists* tail = head;
    for (size_t i = 1; i < m; i++)
    {
      tail->next = new ListOfLists{ convert(array[i], n[i]), nullptr };
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

size_t oddCount(List* head)
{
  List* temporary = head;
  size_t res = 0;
  while (temporary != nullptr)
  {
    if (temporary->data % 2 == 0)
    {
      res++;
    }
    temporary = temporary->next;
  }
  return res;
}


size_t evenCount(List* head)
{
  List* temporary = head;
  size_t res = 0;
  while (temporary != nullptr)
  {
    if (temporary->data % 2 != 0)
    {
      res++;
    }
    temporary = temporary->next;
  }
  return res;
}

size_t oddCount(ListOfLists* head)
{
  ListOfLists* tail = head;
  size_t res = 0;
  while (tail != nullptr)
  {
    res += oddCount(tail->data);
    tail = tail->next;
  }
  return res;
}

size_t evenCount(ListOfLists* head)
{
  ListOfLists* tail = head;
  std::size_t res = 0;
  while (tail != nullptr)
  {
    res += evenCount(tail->data);
    tail = tail->next;
  }
  return res;
}

size_t count(ListOfLists* head, const std::string& condition)
{
  if (condition.find("odd")!= std::string::npos)
  {
    return oddCount(head);
  }
  else if (condition.find("even") != std::string::npos)
  {
    return evenCount(head);
  }
  else
  {
    return evenCount(head) + oddCount(head);
  }
}

void deleteMatrix(size_t** arr, size_t arrayQuantity)
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
  size_t** matrix = nullptr;
  size_t* sizes = nullptr;
  if (!(std::cin >> arrayQuantity))
  {
    std::cerr << "Input fail!\n";
    return 1;
  }
  try
  {
    matrix = new size_t * [arrayQuantity];
    sizes = new size_t[arrayQuantity];
  }
  catch (const std::bad_alloc& e)
  {
    return 1;
  }
  for (size_t i = 0; i < arrayQuantity; i++)
  {
    if (!(std::cin >> sizes[i]))
    {
      delete[] sizes;
      delete[] matrix;
      std::cerr << "Inptut fail\n";
      return 1;
    }
    try
    {
      matrix[i] = new size_t[sizes[i]];
    }
    catch (const std::bad_alloc& e)
    {
      deleteMatrix(matrix, sizes[i]);
    }
    for (size_t j = 0; j < sizes[i]; j++)
    {
      if (!(std::cin >> matrix[i][j]))
      {
        delete[] sizes;
        deleteMatrix(matrix, arrayQuantity);
        return 1;
      }
    }
  }
  ListOfLists* buf = nullptr;
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
  std::cout << count(buf, str);
  deleteMatrix(matrix, arrayQuantity);
  delete[] sizes;
  deleteListOfLists(buf);
}
