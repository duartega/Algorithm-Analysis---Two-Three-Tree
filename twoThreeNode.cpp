/** A Index Generator implemented using a two-three b-tree
 * Authors: Jorge Bautista & Gabriel Durante */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

struct word {
	std::string value, lineOccurences;
};

class twoThreeNode { 
	public:
		twoThreeNode(word *key): leftNode(nullptr), rightNode(nullptr), leftVal(key), centerNode(nullptr), rightVal(nullptr), height(0) {}

		/** Accessors **/
		twoThreeNode *insertVal(twoThreeNode *, word *);
		twoThreeNode *add(twoThreeNode *);
		word *find(twoThreeNode *, std::string );

		/** Adders **/
		void setLeftNode(twoThreeNode *node)   { leftNode = node;    }
		void setRightNode(twoThreeNode *node)  { rightNode = node;   }
		void setCenterNode(twoThreeNode *node) { centerNode = node;  }
		void setHeight(int h)									 { height = h;         }	

		/** Getters **/
		bool isLeaf() 			 					{ return leftNode == nullptr; }
		twoThreeNode *leftChild()     { return leftNode;     }
		twoThreeNode *rightChild() 	  { return rightNode;    }
		twoThreeNode *centerChild() 	{ return centerNode;   }
		word  *getLeftVal()		 				{ return leftVal;      }
		word  *getRightVal()		 			{ return rightVal;     }
		int getHeight() 							{ return height;       }

		/** Pre Order Traversal Printer **/
		void printVals(twoThreeNode *);

	private:
		twoThreeNode *leftNode, *rightNode, *centerNode;
		word *leftVal, *rightVal;
		int  height;
};

void twoThreeNode::printVals(twoThreeNode *node) {
  if(node == nullptr )
		return ;

	std::cout << "node values: ";
	if(node->getLeftVal() != nullptr) {
  	word *leftWord = node->getLeftVal();
		std::cout << leftWord->value << " at lines: " << leftWord->lineOccurences ;
	}

	if(node->getRightVal() != nullptr) {
 		word *rightWord = node->getRightVal();
		std::cout << " " << rightWord->value << " at lines: " << rightWord->lineOccurences;
	}
	std::cout << "\nleft values: ";
	printVals(node->leftChild());
	std::cout << "center values: ";
	printVals(node->centerChild());
	std::cout << "right values:";
	printVals(node->rightChild());
	std::cout << "\n \n";
}

twoThreeNode *twoThreeNode::insertVal(twoThreeNode *node, word *val) {
	if(node == nullptr)
		return new twoThreeNode(val);

	if(node->isLeaf()) { //no children
 		twoThreeNode *newNode = new twoThreeNode(val);
		return node->add(newNode);
	}

   /** Value must be added to one of the child Nodes  **/
   word *leftWord = node->getLeftVal();
   word *rightWord = node->getRightVal();	

   if(val->value < leftWord->value) { //insert on the left node
   	twoThreeNode *newNode = insertVal(node->leftChild(), val);
	twoThreeNode *child = node->leftChild();
	word *newWordL = newNode->getLeftVal();
	word *newWordR = newNode->getRightVal();
	word *childL = child->getLeftVal();
	word *childR = child->getRightVal();
	if( newWordL == childL && newWordR == childR) 
		return node;
	return node->add(newNode);
   } 

   else if( node->getRightVal() == nullptr || val->value < rightWord->value) { //traverse through the center
	twoThreeNode *newNode = insertVal(node->centerChild(),val);
	twoThreeNode *child = node->centerChild();	
	word *newWordL = newNode->getLeftVal();
	word *newWordR = newNode->getRightVal();
	word *childL = child->getLeftVal();
	word *childR = child->getRightVal();
	if( newWordL == childL && newWordR == childR) 
		return node;
	return node->add(newNode);
   }
   
   else { //insert on a right node
	twoThreeNode *newNode = insertVal(node->rightChild(),val);
	twoThreeNode *child = node->rightChild();	
	word *newWordL = newNode->getLeftVal();
	word *newWordR = newNode->getRightVal();
	word *childL = child->getLeftVal();
	word *childR = child->getRightVal();
	if( newWordL == childL && newWordR == childR) 
		return node;
	return node->add(newNode);
 
   }
   std::cout << "sad times fam\n";
   return nullptr; //this should never happen so if it does sad times fam
}

twoThreeNode *twoThreeNode::add(twoThreeNode *node) {
   if(getLeftVal() == nullptr ) //empty tree
	return node;

   else if(rightVal == nullptr ) {  //only left is present, add right value
	if((node->getLeftVal())->value < (getLeftVal())->value) {
		rightVal = getLeftVal();
		rightNode = centerNode;
		leftVal  = node->getLeftVal();
		leftNode = node->leftChild();
		centerNode = node->centerChild();
	}

 	else {
	   rightVal = node->getLeftVal();
	   centerNode = node->leftChild();
	   rightNode = node->centerChild();
	}

        return this;
    }

    else if((node->getLeftVal())->value < (getLeftVal())->value) { //add as a left child
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

    else if((node->getLeftVal())->value < (getRightVal())->value) { //add as a center child
	twoThreeNode *newNode = new twoThreeNode(getRightVal());
	newNode->setLeftNode(node->centerChild());
	newNode->setCenterNode(rightNode);
	node->setCenterNode(newNode);
	node->setLeftNode(this);
	rightVal = nullptr;
	rightNode = nullptr;
	return node;
	
    }
    
    else { //add as a right child
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

word *twoThreeNode::find(twoThreeNode *node, std::string val) {
	if(node == nullptr) 
		return nullptr;

	if( node->getLeftVal() != nullptr && (node->getLeftVal())->value == val )
		return node->getLeftVal();

	if( node->getRightVal() != nullptr && (node->getRightVal())->value == val )
		return node->getRightVal();

	if( node->getLeftVal() != nullptr && val < (node->getLeftVal())->value ) //search left
		return find(node->leftChild(), val);

	else if ( node->getRightVal() == nullptr || val < (node->getRightVal())->value) //search center
		return find(node->centerChild(), val);

	else  //search right
		return find(node->rightChild(), val);
}

void getFile(std::ifstream & file) {
	std::string fileLocation;
	std::cout << "Please enter the file location: ";
	std::getline(std::cin,fileLocation);
	file.open(fileLocation);

	while(file.fail()) {
		fileLocation = "";
		std::cout << "The file was not found. Enter a new file location:\n";
		getline(std::cin,fileLocation);
		file.open(fileLocation);
  }
}

twoThreeNode *generateTree() {
	twoThreeNode *tree = new twoThreeNode(nullptr);
	std::ifstream file;

	getFile(file);
	std::string line = "";
	int lineNum = 1;

	while(std::getline(file,line)) {
		std::string tempWord = "";
		for(int i = 0; i < line.length(); i++) {
			while(line[i] != ' ' && line[i] != '\n' && i < line.length()) {
				tempWord.insert(tempWord.end(), line[i]);
				i++;
			}
		}

		word *exist = tree->find(tree,tempWord);
		if(exist != nullptr) 
			exist->lineOccurences = exist->lineOccurences + " " + std::to_string(lineNum);

		else {
 			word *val = new word();
			val->value = tempWord;
			val->lineOccurences = val->lineOccurences + " " + std::to_string(lineNum);
			tree = tree->insertVal(tree, val);
		}
			lineNum ++;
	} 
	return tree;
}

int main() { 
	twoThreeNode *tree = generateTree();
 	tree->printVals(tree);
 	return 0;
}
