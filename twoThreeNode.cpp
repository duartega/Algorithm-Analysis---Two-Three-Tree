/** A Index Generator implemented using a two-three b-tree
 * Authors: Jorge Bautista & Gabriel Durante */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>

#include "time.h"
#include <iomanip>
#include <sstream>
#include "bst.cpp"


struct word
{
  std::string value, lineOccurences;
};

class twoThreeNode
{
public:
  twoThreeNode(word *key) : leftNode(nullptr), rightNode(nullptr), leftVal(key), centerNode(nullptr), rightVal(nullptr), height(0) {}

  /** Accessors **/
  twoThreeNode *insertVal(twoThreeNode *, word *);
  twoThreeNode *add(twoThreeNode *);
  word *find(twoThreeNode *, std::string);
  int getNumNodes(twoThreeNode *);
  int getHeight(twoThreeNode *);

  /** Adders **/
  void setLeftNode(twoThreeNode *node) { leftNode = node; }
  void setRightNode(twoThreeNode *node) { rightNode = node; }
  void setCenterNode(twoThreeNode *node) { centerNode = node; }

  /** Getters **/
  bool isLeaf() { return leftNode == nullptr; }
  twoThreeNode *leftChild() { return leftNode; }
  twoThreeNode *rightChild() { return rightNode; }
  twoThreeNode *centerChild() { return centerNode; }
  word *getLeftVal() { return leftVal; }
  word *getRightVal() { return rightVal; }

  /** Overloaders **/
  friend bool operator==(const twoThreeNode &, const twoThreeNode &);

  /** Pre Order Traversal Printer **/
  void printVals(twoThreeNode *, std::ostream &);

private:
  twoThreeNode *leftNode, *rightNode, *centerNode;
  word *leftVal, *rightVal;
  int height;
};

bool operator==(twoThreeNode &nOne, twoThreeNode &nTwo)
{
  return (nOne.getLeftVal() == nTwo.getLeftVal() && nOne.getRightVal() == nTwo.getRightVal());
}

int twoThreeNode::getNumNodes(twoThreeNode *node)
{
  if (node == nullptr)
    return 0;
  return 1 + getNumNodes(node->leftChild()) + getNumNodes(node->centerChild()) + getNumNodes(node->rightChild());
}

int twoThreeNode::getHeight(twoThreeNode *node)
{
  if (node == nullptr)
    return 0;
  return 1 + getHeight(node->leftChild());
}

void twoThreeNode::printVals(twoThreeNode *node, std::ostream &ostr)
{
  if (node == nullptr)
    return;

  ostr << "node values: ";
  if (node->getLeftVal() != nullptr)
  {
    word *leftWord = node->getLeftVal();
    ostr << leftWord->value << " at lines: " << leftWord->lineOccurences;
  }

  if (node->getRightVal() != nullptr)
  {
    word *rightWord = node->getRightVal();
    ostr << " " << rightWord->value << " at lines: " << rightWord->lineOccurences;
  }

  ostr << "\nleft values: ";
  printVals(node->leftChild(), ostr);
  ostr << "center values: ";
  printVals(node->centerChild(), ostr);
  ostr << "right values:";
  printVals(node->rightChild(), ostr);
  ostr << "\n \n";
}

twoThreeNode *twoThreeNode::insertVal(twoThreeNode *node, word *val)
{
  if (node == nullptr)
    return new twoThreeNode(val);

  if (node->isLeaf())
  { //no children
    twoThreeNode *newNode = new twoThreeNode(val);
    return node->add(newNode);
  }

  /** Value must be added to one of the child Nodes  **/
  word *leftWord = node->getLeftVal();
  word *rightWord = node->getRightVal();

  if (val->value < leftWord->value)
  { //insert on the left node
    twoThreeNode *newNode = insertVal(node->leftChild(), val);
    if (newNode == node->leftChild())
      return node;
    return node->add(newNode);
  }

  else if (node->getRightVal() == nullptr || val->value < rightWord->value)
  { //traverse through the center
    twoThreeNode *newNode = insertVal(node->centerChild(), val);
    if (newNode == node->centerChild())
      return node;
    return node->add(newNode);
  }

  else
  { //insert on a right node
    twoThreeNode *newNode = insertVal(node->rightChild(), val);
    if (newNode == node->rightChild())
      return node;
    return node->add(newNode);
  }

  std::cout << "sad times fam\n";
  return nullptr; //this should never happen so if it does sad times fam
}

twoThreeNode *twoThreeNode::add(twoThreeNode *node)
{
  if (getLeftVal() == nullptr) //empty tree
    return node;

  else if (rightVal == nullptr)
  { //only left is present, add right value
    if ((node->getLeftVal())->value < (getLeftVal())->value)
    {
      rightVal = getLeftVal();
      rightNode = centerNode;
      leftVal = node->getLeftVal();
      leftNode = node->leftChild();
      centerNode = node->centerChild();
    }

    else
    {
      rightVal = node->getLeftVal();
      centerNode = node->leftChild();
      rightNode = node->centerChild();
    }
    return this;
  }

  else if ((node->getLeftVal())->value < (getLeftVal())->value)
  { //add as a left child
    twoThreeNode *newNode = new twoThreeNode(getLeftVal());
    newNode->setLeftNode(node);
    newNode->setCenterNode(this);
    leftNode = centerNode;
    centerNode = rightNode;
    rightNode = nullptr;

    leftVal = rightVal;
    rightVal = nullptr;
    return newNode;
  }

  else if ((node->getLeftVal())->value < (getRightVal())->value)
  { //add as a center child
    twoThreeNode *newNode = new twoThreeNode(getRightVal());
    newNode->setLeftNode(node->centerChild());
    newNode->setCenterNode(rightNode);
    node->setCenterNode(newNode);
    node->setLeftNode(this);
    rightVal = nullptr;
    rightNode = nullptr;
    return node;
  }

  else
  { //add as a right child
    twoThreeNode *newNode = new twoThreeNode(getRightVal());
    newNode->setLeftNode(this);
    newNode->setCenterNode(node);
    node->setLeftNode(rightNode);
    rightVal = nullptr;
    rightNode = nullptr;
    return newNode;
  }
  return nullptr; //no no no hunny
}

word *twoThreeNode::find(twoThreeNode *node, std::string val)
{
  if (node == nullptr)
    return nullptr;

  if (node->getLeftVal() != nullptr && (node->getLeftVal())->value == val)
    return node->getLeftVal();

  if (node->getRightVal() != nullptr && (node->getRightVal())->value == val)
    return node->getRightVal();

  if (node->getLeftVal() != nullptr && val < (node->getLeftVal())->value) //search left
    return find(node->leftChild(), val);

  else if (node->getRightVal() == nullptr || val < (node->getRightVal())->value) //search center
    return find(node->centerChild(), val);

  else //search right
    return find(node->rightChild(), val);
}

void getFile(std::ifstream &file)
{
  std::string fileLocation;
  std::cout << "Please enter the file location: ";
  std::getline(std::cin, fileLocation);
  file.open(fileLocation);

  while (file.fail())
  {
    fileLocation = "";
    std::cout << "The file was not found. Enter a new file location:\n";
    getline(std::cin, fileLocation);
    file.open(fileLocation);
  }
}

twoThreeNode *generateTree(ifstream &file)
{
  twoThreeNode *tree = new twoThreeNode(nullptr);
  stringstream tempWord;
  //std::ifstream file;

  //getFile(file);
  std::string line = "";
  int lineNum = 1;

  std::clock_t start;
  start = std::clock();
  while (!file.eof())
  {
    std::string tempWord, line;
    getline(file, line);

    for (int i = 0; i < line.length(); i++)
    {
      while (line[i] != ' ' && line[i] != '\n' && line[i] != '\r' && i < line.length())
      {
        tempWord.insert(tempWord.end(), line[i]);
        i++;
      }

      //Trim any punctuation off end of word. Will leave things like apostrophes
      //and decimal points
      while (tempWord.length() > 0 && !isalnum(tempWord[tempWord.length() - 1]))
        tempWord.resize(tempWord.size() - 1);

      word *exist = tree->find(tree, tempWord);
      if (exist != nullptr)
        exist->lineOccurences = exist->lineOccurences + " " + std::to_string(lineNum);

      else
      {
        word *val = new word();
        val->value = tempWord;
        val->lineOccurences = val->lineOccurences + " " + std::to_string(lineNum);
        tree = tree->insertVal(tree, val);
      }
      tempWord = "";
      lineNum++;
    }
  }
  std::cout << (std::clock() - start) / double(CLOCKS_PER_SEC / 1000) << std::endl;
  return tree;
}

int main(int argc, char *argv[])
{
  	if (argc != 2)
	{
		cout << "Incorrect input. Correct format: ./<exectuable.out> <inputtext.txt>\n";
		return 1;
	}
  	ifstream input(argv[1]);
  	ifstream input2(argv[1]);
    	BST myBSTTree;
	if (input.is_open())
	{
		myBSTTree.buildTree(input);
		twoThreeNode *tree = generateTree(input2);

		input.close();
		input2.close();

  //tree->printVals(tree);

  while (1)
  {
    char t = '0';
    short option = 0;
    std::cout << "Options: (a) BST, (b) 2-3 Tree, (c) Compare BST and 2-3 Tree\n";
    std::cin >> t;
    std::cout << "Options: (1) display index, (2) search, (3) save index, (4) quit\n";
    std::cin >> option;

    if (t == 'a' && option == 1)
      myBSTTree.printTree(std::cout);
    if (t == 'b' && option == 1)
    {
      tree->printVals(tree, std::cout);
      std::cout << tree->getHeight(tree) << std::endl;
      std::cout << tree->getNumNodes(tree) << std::endl;
    }

    //Search index for a word
    if (option == 2)
    {
      if (t == 'b')
      {
        std::string sw;
        std::cout << "Search word: ";
        std::cin >> sw;
        if (tree->find(tree, sw) == nullptr)
        {
          std::cout << "value does not exist\n";
        }
        else
        {
          std::cout << "value does exist fam\n";
        }
      }
    }
    //Save index
    else if (option == 3)
    {
      std::string outputFile;
      std::cout << "Enter a filename to save your index to (Suggested: <filename>.txt) : ";
      std::cin >> outputFile;
      std::ofstream output;
      output.open(outputFile);
      if (t == 'b')
        tree->printVals(tree, output);
      output.close();
      std::cout << "Saved\n";
    }

    //Quit
    else if (option == 4)
      break;
  }
  return 0;
}
}
