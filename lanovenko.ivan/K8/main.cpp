#include <functional>
#include <iostream>
#include <ostream>

template < class T, class Cmp >
struct BiTree
{
  T data;
  Cmp cmp;
  BiTree< T, Cmp >* left, * right, * parent;
};

template< class T, class Cmp >
void clearBiTree(BiTree< T, Cmp>** root)
{
  if (!(*root))
  {
    return;
  }
  clearBiTree(&(*root)->left);
  clearBiTree(&(*root)->right);
  delete (*root);
  *root = nullptr;
}

template< class T, class Cmp >
void printBiTree(BiTree< T, Cmp >* root, std::ostream& out)
{
  if (!root)
  {
    return;
  }
  static bool first = true;
  printBiTree(root->left, out);
  if (!first)
  {
    out << " ";
  }
  out << root->data;
  first = false;
  printBiTree(root->right, out);
}

template< class T, class Cmp >
void insert(BiTree< T, Cmp >* root, BiTree< T, Cmp >* node)
{
  BiTree< T, Cmp >* yaroot = root;
  BiTree< T, Cmp >* subroot = nullptr;
  while (yaroot)
  {
    subroot = yaroot;
    if (node->cmp(node->data, yaroot->data))
    {
      yaroot = yaroot->left;
    }
    else
    {
      yaroot = yaroot->right;
    }
  }
  if (node->cmp(node->data, subroot->data))
  {
    subroot->left = node;
  }
  else
  {
    subroot->right = node;
  }
  node->parent = subroot;
}

template< class T, class Cmp >
BiTree< T, Cmp >* find(BiTree< T, Cmp >* root, const T& value)
{
  BiTree< T, Cmp >* yaroot = root;
  while (yaroot && yaroot->data != value)
  {
    if (yaroot->cmp(value, yaroot->data))
    {
      yaroot = yaroot->left;
    }
    else
    {
      yaroot = yaroot->right;
    }
  }
  if (!yaroot)
  {
    throw std::invalid_argument("<INVALID NODE>");
  }
  else
  {
    return yaroot;
  }
}

template< class T, class Cmp >
BiTree< T, Cmp >* treeMinimum(BiTree< T, Cmp >* root)
{
  BiTree< T, Cmp >* temp = root;
  while (temp->left)
  {
    temp = temp->left;
  }
  return temp;
}

template< class T, class Cmp >
BiTree< T, Cmp >* extract(BiTree< T, Cmp >* root, const T& value, BiTree< T, Cmp >** result)
{
  *result = find(root, value);
  if ((*result)->left == nullptr && (*result)->right == nullptr)
  {
    if ((*result)->parent == nullptr)
    {
      return nullptr;
    }
    if ((*result)->parent->right == (*result))
    {
      (*result)->parent->right = nullptr;
    }
    else if ((*result)->parent->left == (*result))
    {
      (*result)->parent->left = nullptr;
    }
    return root;
  }
  if ((*result)->left != nullptr && (*result)->right == nullptr)
  {
    (*result)->left->parent = (*result)->parent;
    if ((*result)->parent)
    {
      if ((*result)->parent->left == (*result))
      {
        (*result)->parent->left = (*result)->left;
      }
      else if ((*result)->parent->right == (*result))
      {
        (*result)->parent->right = (*result)->left;
      }
    }
    else
    {
      root = (*result)->left;
    }
    (*result)->left = (*result)->parent = nullptr;
    return root;
  }
  if ((*result)->right != nullptr && (*result)->left == nullptr)
  {
    (*result)->right->parent = (*result)->parent;
    if ((*result)->parent)
    {
      if ((*result)->parent->right == (*result))
      {
        (*result)->parent->right = (*result)->right;
      }
      else if ((*result)->parent->left == (*result))
      {
        (*result)->parent->left = (*result)->right;
      }
    }
    else
    {
      root = (*result)->right;
    }
    (*result)->right = (*result)->parent = nullptr;
    return root;
  }
  if ((*result)->right != nullptr && (*result)->left != nullptr)
  {
    BiTree< T, Cmp >* rightMin = treeMinimum((*result)->right);
    (*result)->left->parent = rightMin;
    rightMin->left = (*result)->left;
    if (rightMin->parent)
    {
      if (rightMin->parent->left == rightMin)
      {
        if (rightMin->right)
        {
          rightMin->parent->left = rightMin->right;
          rightMin->right->parent = rightMin->parent;
        }
        else
        {
          rightMin->parent->left = nullptr;
        }
        rightMin->right = (*result)->right;
        rightMin->right->parent = rightMin;
      }
    }
    rightMin->parent = (*result)->parent;
    if ((*result)->parent)
    {
      if ((*result)->parent->left == (*result))
      {
        (*result)->parent->left = rightMin;
      }
      else if ((*result)->parent->right == (*result))
      {
        (*result)->parent->right = rightMin;
      }
    }
    else
    {
      root = rightMin;
    }
    (*result)->parent = (*result)->left = (*result)->right = nullptr;
    return root;
  }
  return root;
}

template< class T, class Cmp >
BiTree< T, Cmp >* convert(const T* data, size_t s)
{
  if (s == 0)
  {
    return nullptr;
  }
  BiTree< T, Cmp >* root = new BiTree< T, Cmp >{ data[0], std::less< T >{}, nullptr, nullptr, nullptr };
  try
  {
    for (size_t i = 1; i < s; i++)
    {
      BiTree< T, Cmp >* node = new BiTree< T, Cmp >{ data[i], std::less< T >{}, nullptr, nullptr, nullptr };
      insert(root, node);
    }
  }
  catch (const std::bad_alloc& e)
  {
    throw;
    clearBiTree(&root);
  }
  return root;
}

int main()
{
  size_t n = 0;
  if (!(std::cin >> n))
  {
    std::cerr << "Input fail!";
    return 1;
  }
  int* array = nullptr;
  try
  {
    array = new int[n] {};
  }
  catch (const std::bad_alloc& e)
  {
    std::cerr << "Out of memmory";
    return 1;
  }
  for (size_t i = 0; i < n; i++)
  {
    if (!(std::cin >> array[i]))
    {
      std::cerr << "Input fail";
      delete[] array;
      return 1;
    }
  }
  BiTree < int, std::less< int > >* root = nullptr;
  try
  {
    root = convert< int, std::less < int > >(array, n);
    delete[] array;
  }
  catch (const std::bad_alloc& e)
  {
    delete[] array;
    std::cerr << "Out of memmory";
    return 1;
  }
  int current = 0;
  bool errors = false;
  while (!std::cin.eof())
  {
    if (!(std::cin >> current))
    {
      errors = true;
      break;
    }
    BiTree< int, std::less< int > >* extracted = nullptr;
    try
    {
      root = extract(root, current, &extracted);
    }
    catch (const std::invalid_argument& e)
    {
      std::cout << e.what() << "\n";
    }
    delete extracted;
  }
  if (errors && !std::cin.eof())
  {
    printBiTree(root, std::cout);
    std::cout << "\n";
    std::cerr << "Input fail" << "\n";
    clearBiTree(&root);
    return 1;
  }
  printBiTree(root, std::cout);
  clearBiTree(&root);
  std::cout << "\n";
}
