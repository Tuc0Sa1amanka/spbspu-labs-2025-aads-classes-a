#include <iostream>
#include <string>

template< class T >
struct List
{
  T data;
  List< T > * next;
};

template< class T, class C >
size_t count(List< List< T > * > * head, C condition)
{
  size_t count = 0;
  while (head)
  {
    List< int > * subhead = head->data;
    while (subhead)
    {
      condition(subhead->data) ? count++ : count = count;
      subhead = subhead->next;
    }
    head = head->next;
  }
  return count;
}

void clearAllMassives(int ** ptr_massives, size_t * ptr_numbers_of_elements, size_t number_of_massives);
List< List< int > * > * convert(const int * const * d, size_t m, const size_t * n);
void clearAllLists(List< List< int > * > * head);
bool isOdd(int element);
bool isEven(int element);

int main()
{
  List< List< int > * > * head = nullptr;
  int ** ptr_massives = nullptr;
  size_t number_of_massives = 0;
  if (!(std::cin >> number_of_massives))
  {
    std::cerr << "ERROR: Invalid argument" << "\n";
    return 1;
  }
  size_t * ptr_numbers_of_elements = nullptr;
  size_t number_of_elements = 0;
  size_t created = 0;
  try
  {
    ptr_massives = new int * [number_of_massives];
    ptr_numbers_of_elements = new size_t [number_of_massives];
    for (size_t i = 0; i < number_of_massives; i++)
    {
      std::cin >> number_of_elements;
      ptr_massives[i] = new int [number_of_elements];
      for (size_t j = 0; j < number_of_elements; j++)
      {
        std::cin >> ptr_massives[i][j];
      }
      created++;
      ptr_numbers_of_elements[i] = number_of_elements;
      if (!std::cin || std::cin.eof())
      {
        throw std::logic_error("ERROR: Invalid argument");
      }
    }
    head = convert(ptr_massives, number_of_massives, ptr_numbers_of_elements);
  }
  catch (const std::logic_error & e)
  {
    clearAllMassives(ptr_massives, ptr_numbers_of_elements, created);
    std::cerr << e.what() << "\n";
    return 2;
  }
  catch(const std::bad_alloc & e)
  {
    clearAllMassives(ptr_massives, ptr_numbers_of_elements, created);
    std::cerr << "ERROR: Out of memory" << "\n";
    return 3;
  }
  clearAllMassives(ptr_massives, ptr_numbers_of_elements, created);
  std::string condition;
  std::cin >> condition;
  if (!condition.compare("odd"))
  {
    std::cout << count(head, isOdd);
  }
  else if (!condition.compare("even"))
  {
    std::cout << count(head, isEven);
  }
  else
  {
    std::cout << count(head, isOdd);
    std::cout << " ";
    std::cout << count(head, isEven);
  }
  std::cout << "\n";
  clearAllLists(head);
}

void clearAllMassives(int ** ptr_massives, size_t * ptr_numbers_of_elements, size_t created)
{
  for (size_t i = 0; i < created; i++)
  {
    delete[] ptr_massives[i];
  }
  delete[] ptr_massives;
  delete[] ptr_numbers_of_elements;
}

List< List< int > * > * convert(const int * const * d, size_t m, const size_t * n)
{
  List< List< int > * > * head = new List< List< int > * >{ nullptr, nullptr };
  List< List< int > * > * subhead = head;
  try
  {
    for (size_t i = 0; i < m; i++)
    {
      (i != m - 1) ? subhead->next = new List< List< int > * >{ nullptr, nullptr } : subhead = subhead;
      subhead->data = new List< int >{ d[i][0], nullptr };
      List< int > * subsubhead = subhead->data;
      for (size_t j = 1; j < n[i]; j++)
      {
        subsubhead->next = new List< int >{ d[i][j], nullptr };
        subsubhead = subsubhead->next;
      }
      subhead = subhead->next;
    }
  }
  catch(const std::bad_alloc & e)
  {
    clearAllLists(head);
    throw;
  }
  return head;
}

void clearAllLists(List< List< int > * > * head)
{
  if (!head)
  {
    return;
  }
  while (head)
  {
    List< List< int > * > * subhead = head->next;
    while (head->data)
    {
      List< int > * subsubhead = head->data->next;
      delete head->data;
      head->data = subsubhead;
    }
    delete head;
    head = subhead;
  }
}

bool isOdd(int element)
{
  return element % 2 == 1;
}

bool isEven(int element)
{
  return element % 2 == 0;
}
