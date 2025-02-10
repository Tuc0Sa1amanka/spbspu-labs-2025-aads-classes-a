#include <iostream>

struct FwdList {
  int value;
  FwdList * next;
};

void deleteList(FwdList * head)
{
  FwdList * temp = head;
  while (temp)
  {
    FwdList * nextHead = temp->next;
    delete temp;
    temp = nextHead;
  }
}

int main()
{
  FwdList * head = nullptr;
  FwdList * current = nullptr;
  for (int i = 0; i < 10; i++)
  {
    FwdList * subhead = nullptr;
    try
    {
      subhead = new FwdList {i, nullptr};
    }
    catch (const std::bad_alloc &)
    {
      deleteList(head);
      throw;
    }
    current->next = subhead;
    current = subhead;
  }
}