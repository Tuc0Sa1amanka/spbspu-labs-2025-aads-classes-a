#include <utility>
#include <iostream>
#include <string>
#include <limits>

namespace lanovenko
{
  std::istream& operator>>(std::istream& in, std::pair< int, int >& current)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    if (!(in >> current.first))
    {
      in.setstate(std::ios::failbit);
    }
    if (!(in >> current.second))
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }
}

template< class T, class Cmp >
struct TriTree
{
  std::pair< T, T > data;
  TriTree< T, Cmp >* left;
  TriTree< T, Cmp >* middle;
  TriTree< T, Cmp >* right;
  TriTree< T, Cmp >* parent;
};

template< class T, class Cmp >
void clearTriTree(TriTree< T, Cmp >** root)
{
  if (!(*root))
  {
    return;
  }
  clearTriTree(&(*root)->left);
  clearTriTree(&(*root)->middle);
  clearTriTree(&(*root)->right);
  delete (*root);
  *root = nullptr;
}

template< class T, class Cmp >
struct TriTreeIterator
{
  using this_t = TriTreeIterator< T, Cmp >;
  ~TriTreeIterator() = default;
  TriTreeIterator(const TriTree< T, Cmp >* node);
  TriTreeIterator(const TriTreeIterator& rhs) = default;
  TriTreeIterator& operator=(const TriTreeIterator& rhs) = default;
  this_t fall_left(this_t root) const;
  bool is_root(this_t root) const;
  bool is_left(this_t root) const;
  bool is_middle(this_t root) const;
  bool hasNext() const;
  this_t next() const;
  const std::pair< T, T >& data() const;
private:
  const TriTree< T, Cmp >* node_;
};

template< class T, class Cmp >
TriTreeIterator< T, Cmp >::TriTreeIterator(const TriTree< T, Cmp >* node):
  node_(node)
{}

template< class T, class Cmp >
typename TriTreeIterator< T, Cmp >::this_t TriTreeIterator< T, Cmp >::fall_left(this_t root) const
{
  return root.node_->left ? fall_left(this_t{ root.node_->left }) : root;
}

template< class T, class Cmp >
bool TriTreeIterator< T, Cmp >::is_root(this_t root) const
{
  return !(root.node_->parent);
}

template< class T, class Cmp >
bool TriTreeIterator< T, Cmp >::is_left(this_t root) const
{
  return (root.node_->parent->left == root.node_);
}

template< class T, class Cmp >
bool TriTreeIterator< T, Cmp >::hasNext() const
{
  if (!node_)
  {
    return false;
  }
  return true;
}

template< class T, class Cmp >
bool TriTreeIterator< T, Cmp >::is_middle(this_t root) const
{
  return (root.node_->parent->middle == root.node_);
}

template< class T, class Cmp >
const std::pair< T, T >& TriTreeIterator< T, Cmp >::data() const
{
  return node_->data;
}

template< class T, class Cmp >
TriTreeIterator< T, Cmp > TriTreeIterator< T, Cmp >::next() const
{

  if (node_->middle)
  {
    return fall_left(node_->middle);
  }
  if (node_->right)
  {
    return fall_left(node_->right);
  }
  auto current = node_;
  while (true)
  {
    if (is_root(current))
    {
      return nullptr;
    }
    if (is_left(current))
    {
      return current->parent;
    }
    else if (is_middle(current))
    {
      if (current->parent->right)
      {
        return fall_left(current->parent->right);
      }
      current = current->parent;
    }
    else
    {
      current = current->parent;
    }
  }
}

template< class T, class Cmp >
TriTreeIterator< T, Cmp > begin(TriTree< T, Cmp >* root)
{
  TriTreeIterator < T, Cmp > root_it{ root };
  return root_it.fall_left(root_it);
}

template< class T, class Cmp >
void insert(TriTree< T, Cmp >* root, TriTree< T, Cmp >* node, Cmp cmp)
{
  TriTree< T, Cmp >* yaroot = root;
  TriTree< T, Cmp >* subroot = nullptr;
  while (yaroot)
  {
    subroot = yaroot;
    if (cmp(node->data.first, yaroot->data.first) && cmp(node->data.second, yaroot->data.first))
    {
      yaroot = yaroot->left;
    }
    else if (cmp(yaroot->data.first, node->data.first) && cmp(yaroot->data.first, node->data.second))
    {
      if (cmp(node->data.first, yaroot->data.second) && cmp(node->data.second, yaroot->data.second))
      {
        yaroot = yaroot->middle;
      }
      else if (cmp(yaroot->data.second, node->data.first) && cmp(yaroot->data.second, node->data.second))
      {
        yaroot = yaroot->right;
      }
      else
      {
        throw std::logic_error("< INVALID PAIR >");
      }
    }
    else
    {
      throw std::logic_error("< INVALID PAIR >");
    }
  }
  if (cmp(node->data.first, subroot->data.first) && cmp(node->data.second, subroot->data.first))
  {
    subroot->left = node;
  }
  else if (cmp(subroot->data.first, node->data.first) && cmp(subroot->data.first, node->data.second))
  {
    if (cmp(node->data.first, subroot->data.second) && cmp(node->data.second, subroot->data.second))
    {
      subroot->middle = node;
    }
    else if (cmp(subroot->data.second, node->data.first) && cmp(subroot->data.second, node->data.second))
    {
      subroot->right = node;
    }
  }
  node->parent = subroot;
}

template< class T, class Cmp >
TriTree< T, Cmp >* convert(const std::pair< T, T >* data, size_t n, Cmp cmp)
{
  TriTree< T, Cmp >* root = new TriTree< T, Cmp >{ data[0], nullptr, nullptr, nullptr, nullptr };
  try
  {
    for (size_t i = 1; i < n; i++)
    {
      TriTree< T, Cmp >* node = new TriTree< T, Cmp >{ data[i], nullptr, nullptr, nullptr, nullptr };
      try
      {
        insert< T, Cmp >(root, node, cmp);
      }
      catch (const std::logic_error& e)
      {
        delete node;
        node = nullptr;
        continue;
      }
    }
  }
  catch (const std::bad_alloc& e)
  {
    clearTriTree(&root);
    throw;
  }
  return root;
}

template< class T, class Cmp >
size_t intersects(TriTree< T, Cmp >* root, const std::pair< T, T >& segment)
{
  size_t count = 0;
  for (auto it = begin(root); it.hasNext(); it = it.next())
  {
    if (segment.second > it.data().first && segment.first < it.data().second)
    {
      count++;
    }
  }
  return count;
}

template< class T, class Cmp >
size_t covers(TriTree< T, Cmp >* root, const std::pair< T, T >& segment)
{
  size_t count = 0;
  for (auto it = begin(root); it.hasNext(); it = it.next())
  {
    if (segment.first <= it.data().first && segment.second >= it.data().second)
    {
      count++;
    }
  }
  return count;
}

template< class T, class Cmp >
size_t avoids(TriTree< T, Cmp >* const root, const std::pair< T, T >& segment)
{
  size_t count = 0;
  for (auto it = begin(root); it.hasNext(); it = it.next())
  {
    if (segment.first > it.data().second || segment.second < it.data().first)
    {
      count++;
    }
  }
  return count;
}

enum class CommandType { COVERS, AVOIDS, INTERSECTS, INVALID};


CommandType parseCommand(const std::string& cmd)
{
  if (cmd == "covers")
  {
    return CommandType::COVERS;
  }
  if (cmd == "avoids")
  {
    return CommandType::AVOIDS;
  }
  if (cmd == "intersects")
  {
    return CommandType::INTERSECTS;
  }
  return CommandType::INVALID;
}

int main()
{
  size_t n = 0;
  if (!(std::cin >> n))
  {
    std::cerr << "Input fail\n";
    return 1;
  };

  std::pair< int, int >* arr = nullptr;
  try
  {
    arr = new std::pair< int, int >[n] {};
  }
  catch (const std::bad_alloc& e)
  {
    std::cerr << "Out of memmory!\n";
    return 1;
  }

  for (size_t i = 0; i < n; i++)
  {
    std::pair< int, int > current{};
    if (!lanovenko::operator>>(std::cin, current))
    {
      delete[] arr;
      arr = nullptr;
      std::cerr << "Input fail!\n";
      return 1;
    }
    if (current.first > current.second)
    {
      std::swap(current.first, current.second);
    }
    arr[i] = current;
  }

  TriTree< int, std::less< int > >* root = nullptr;
  try
  {
    root = convert< int, std::less< int > >(arr, n, std::less< int >{});
    delete[] arr;
    arr = nullptr;
  }
  catch (const std::bad_alloc& e)
  {
    delete[] arr;
    arr = nullptr;
    std::cerr << "Out of memmory\n";
    return 1;
  }

  std::string command = "";
  while (!(std::cin >> command).eof())
  {
    CommandType cmdType = parseCommand(command);
    if (cmdType == CommandType::INVALID)
    {
      clearTriTree(&root);
      std::cerr << "Invalid command\n";
      return 1;
    }

    std::pair< int, int > current;
    if (!(lanovenko::operator>>(std::cin, current)) || current.first > current.second)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
      continue;
    }

    switch (cmdType)
    {
      case CommandType::COVERS:
        std::cout << covers(root, current) << "\n";
        break;
      case CommandType::AVOIDS:
        std::cout << avoids(root, current) << "\n";
        break;
      case CommandType::INTERSECTS:
        std::cout << intersects(root, current) << "\n";
        break;
      default:
        break;
    }
  }
  clearTriTree(&root);
}
