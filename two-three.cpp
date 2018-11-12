#include "two-three.h"
#include "time.h"
#include <iomanip>
#include <sstream>

// constructor
TTT::TTT()
{
	root = NULL;
}

//Returns true if there are no nodes in the tree
bool TTT::isEmpty()
{
	return root = NULL;
}
//Used to implement the search function in the main
//program.
void TTT::contains() const
{
	string input;
	node *foundNode = NULL;
	cout << "Search word: ";
	cin >> input;
	if (containsHelper(input, root, foundNode))
	{
		cout << "Line Numbers: " << foundNode->lines[0];
		for (int i = 1; i < foundNode->lines.size(); i++)
			cout << ", " << foundNode->lines[i];
		cout << '\n';
	}
	else
		cout << '\"' << input << "\" is not in the document\n";
}
void TTT::printTreeTTT(ostream &out) const
{
	out << "Two Three Tree Index:\n-------------------------\n";
	printTreeHelper(root, out);
}


void TTT::buildTreeTTT(ifstream &input)
{
	int line = 1, numWords = 0, distWords = 0, treeHeight = 0;
	stringstream tempWord;
	double totalTime, finishTime, startTime = clock();
	while (!input.eof())
	{
		string tempLine, tempWord; // Templine is where the string will be stored, char by char

		//Read a whole line of text from the file
		getline(input, tempLine);
		for (int i = 0; i < tempLine.length(); i++)
		{
			//Insert valid chars into tempWord until a delimiter( newline or space) is found
			while (tempLine[i] != ' ' && tempLine[i] != '\n' && i < tempLine.length())
			{
				tempWord.insert(tempWord.end(), tempLine[i]);
				i++;
			}

			//Trim any punctuation off end of word. Will leave things like apostrophes
			//and decimal points
			while (tempWord.length() > 0 && !isalnum(tempWord[tempWord.length() - 1]))
				tempWord.resize(tempWord.size() - 1);

			if (tempWord.length() > 0)
			{
				//Once word is formatted, call insert with the word, the line of the input
				//file it came from, the root of our tree, and the distinct word counter
				insertHelper(tempWord, line, root, distWords);
				//Increment our total number of words inserted
				numWords++;
				//Clear out tempWord so we can use it again
				tempWord.clear();
			}
		}
		line++;
	}
	//Do time and height calculation
	finishTime = clock();
	totalTime = (double)(finishTime - startTime) / CLOCKS_PER_SEC;
	treeHeight = findHeight(root);

	//Print output
	cout << setw(40) << std::left;
	cout << "Total number of words: " << numWords << endl;

	cout << setw(40) << std::left
		 << "Total number of distinct words: " << distWords << endl;

	cout << setw(40) << std::left
		 << "Total time spent building index: " << totalTime << endl;

	cout << setw(40) << std::left
		 << "Height of 2-3 is : " << treeHeight << endl;
}

//x is the word to insert, line is the line in the text file
//the word was found at, node is the node of the tree being
//examined, and distWord is incremented if a new word is created
//and used by buildTree
void TTT::insertHelper(const string &x, int line, node *&t, int &distWord)
{
	if (t == NULL) // Create a new node when there is none
	{
		t = new node(x, "", "", NULL, NULL, NULL, NULL);
		t->lines.push_back(line);
		distWord++;
	}

	else if (t->lval != "" && t->rval == "" && (t->left == NULL && t->right == NULL))
	{
			leftInsertHelper(t, x);
			t->lines.push_back(line);
			distWord++;
	}
	else if (x < t->lval && t->left != NULL) // Has left child
		insertHelper(x, line, t->left, distWord);
	else if (x > t->rval && t->right != NULL) // Has right child
		insertHelper(x, line, t->right, distWord);

	else if (t->lval != "" && t->rval != "") // t lval and rval are both taken
	{
		splitInsertHelper(t,x);
	}
		else if (x < t->lval) // Left case
		{
			if (t->parent->rval == "")
			{
				node *m = new node(t->rval, "", "", NULL, NULL, NULL, t);
				t->parent->rval = t->parent->lval;
				t->parent->lval = t->lval;
				t->parent->middle = m;
				t->rval = "";
				t->lines.push_back(line);
				distWord++;
			}
			else if (t->parent->rval != "")
			{
				node *l = new node(x, "", "", NULL, NULL, NULL, t);
				node *r = new node(t->rval, "", "", NULL, NULL, NULL, t);
				node *l2 = new node(t->parent->middle->lval, "", "", NULL, NULL, NULL, t);
				node *r2 = new node(t->parent->right->lval, "", "", NULL, NULL, NULL, t);
				node *m = new node(t->parent->rval, "", "", NULL, NULL, NULL, t);

				t->left = l;
				t->right = r;
				t->rval = "";
				t->parent->right = m;
				m->right = r2;
				m->left = l2;
				m->right->parent = m;
				m->left->parent = m;
				t->parent->rval = "";
				free(t->parent->middle);
				t->lines.push_back(line);
				distWord++;
			}
		}
		else if (x > t->rval && t->right == NULL) // Right case
		{
			if (t->right == NULL){
				node *l = new node(t->lval, "", "", NULL, NULL, NULL, t);
				node *r = new node(x, "", "", NULL, NULL, NULL, t);

				t->left = l;
				t->right = r;				
				t->lval = t->rval;
				t->rval = "";
				t->lines.push_back(line);
				distWord++;
			}
			else if (t->parent->rval == "")
			{
				node *m = new node(t->lval, "", "", NULL, NULL, NULL, t);
				t->parent->rval = t->rval;
				t->lval = x;
				t->parent->middle = m;
				t->rval = "";
				t->lines.push_back(line);
				distWord++;
			}
		
		else if (t->parent->rval != "")
		{
			node *l = new node(t->lval, "", "", NULL, NULL, NULL, t);
			node *r = new node(x, "", "", NULL, NULL, NULL, t);
			node *m = new node(t->parent->rval, "", "", NULL, NULL, NULL, t);

			node *l2 = new node(t->parent->middle->lval, "", "", NULL, NULL, NULL, t);
			node *r2 = new node(t->parent->right->lval, "", "", NULL, NULL, NULL, t);

			t->left = l;
			t->right = r;
			t->parent->right = m;
			t->lval = t->rval;
			t->rval = "";
			//t->parent->right = m;
			m->right = r2;
			m->left = l2;
			m->right->parent = m;
			m->left->parent = m;
			t->parent->rval = "";
			free(t->parent->middle);
			t->lines.push_back(line);
			distWord++;
		}
		}
	}



void TTT::leftInsertHelper(node *&t, const string &x)
{
	if (x < t->lval)
	{   //x should be on left
        t->rval = t->lval;
        t->lval = x;
    }
	else
	{    //x should be on right
        t->rval = x;
    }
}


char TTT::directionHelper(node *& t, node *& p)
{
	if (p->left == t)
		return 'l';
	else if (p->middle == t)
		return 'm';
	else if (p->right = t)
		return 'r';
}
void TTT::splitInsertHelper(node *&t, const string &x)
{
	if (x < t->lval) // Left case (curr lval promoted)
	{
		if (t->parent != NULL && t->parent->rval == "")
		{
			node *m = new node(t->rval, "", "", NULL, NULL, NULL, t);
			t->parent->rval = t->parent->lval;
			t->parent->lval = t->lval;
			t->parent->middle = m;
			t->lval = x; t->rval = "";
		}
		else if (t->parent != NULL && t->parent->rval != "")
		{
			node *l = new node(x, "", "", NULL, NULL, NULL, t);
			node *r = new node(t->rval, "", "", NULL, NULL, NULL, t);
			node *l2 = new node(t->parent->middle->lval, "", "", NULL, NULL, NULL, t);
			node *r2 = new node(t->parent->right->lval, "", "", NULL, NULL, NULL, t);
			node *m = new node(t->parent->rval, "", "", NULL, NULL, NULL, t);

			t->left = l;
			t->right = r;
			t->rval = "";
			t->parent->right = m;
			m->right = r2;
			m->left = l2;
			m->right->parent = m;
			m->left->parent = m;
			t->parent->rval = "";
			free(t->parent->middle);
		}
		else
		{
			node *l = new node(x, "", "", NULL, NULL, NULL, t); t->left = l;
			node *r = new node(t->rval, "", "", NULL, NULL, NULL, t); t->right = r;
			t->rval = "";
		}
	}

	else if (x > t->rval) // Right case (curr rval promoted)
	{
		if (t->parent != NULL && t->parent->rval == ""){
			node *m = new node(t->lval, "", "", NULL, NULL, NULL, t);
			t->parent->rval = t->rval;
			t->parent->middle = m;
			t->lval = x; t->rval = "";
		}
		else if (t->parent != NULL && t->parent->rval != "")
		{
			node *l = new node(t->lval, "", "", NULL, NULL, NULL, t);
			node *r = new node(x, "", "", NULL, NULL, NULL, t);
			node *m = new node(t->parent->rval, "", "", NULL, NULL, NULL, t);

			node *l2 = new node(t->parent->middle->lval, "", "", NULL, NULL, NULL, t);
			node *r2 = new node(t->parent->right->lval, "", "", NULL, NULL, NULL, t);

			t->left = l;
			t->right = r;
			t->parent->right = m;
			t->lval = t->rval;
			t->rval = "";
			//t->parent->right = m;
			m->right = r2;
			m->left = l2;
			m->right->parent = m;
			m->left->parent = m;
			t->parent->rval = "";
			free(t->parent->middle);
		}
		else
		{
			node *l = new node(t->lval, "", "", NULL, NULL, NULL, t); t->left = l;
			node *r = new node(x, "", "", NULL, NULL, NULL, t); t->right = r;
			t->lval = t->rval;
			t->rval = "";
		}
	}

	else if (x > t->lval && x < t->rval) // Middle case (x promoted)
	{
		if (t->parent != NULL)
		{
			node *m = new node(t->rval, "", "", NULL, NULL, NULL, t);
			t->parent->rval = t->parent->lval;
			t->parent->lval = x;
			t->parent->middle = m;
			t->rval = "";
		}
		else
		{
			node *l = new node(t->lval, "", "", NULL, NULL, NULL, t); t->left = l;
			node *r = new node(t->rval, "", "", NULL, NULL, NULL, t); t->right = r;
			t->lval = x;
			t->rval = "";
		}
	}
}

//Used by contains() to see if a words is present or not. Will
//give contains() a pointer to the found node so that contains()
//can prints the lines the word was found on.
bool TTT::containsHelper(const string &x, node *t, node *&result) const
{
	if (t == NULL)
		return false;
	if (t->lval.compare(x) == 0)
	{
		result = t;
		return true;
	}
	else if (x > t->lval)
		return containsHelper(x, t->right, result);
	else
		return containsHelper(x, t->left, result);
}

void TTT::printTreeHelper(node *t, ostream &out) const
{
	if (t == NULL)
		return;
	else
	{
		printTreeHelper(t->left, out);
		out << setw(30) << std::left;
		out << t->lval << " " << t->lines[0];
		for (int i = 1; i < t->lines.size(); i++)
			out << ", " << t->lines[i];
		out << endl;
		printTreeHelper(t->right, out);
	}
}

//Returns height of tree. If tree has only one node, height is 1
int TTT::findHeight(node *t)
{
	if (t == NULL)
		return 0;
	else
	{
		int leftHeight = findHeight(t->left), rightHeight = findHeight(t->right);
		if (leftHeight > rightHeight)
			return (leftHeight + 1);
		else
			return (rightHeight + 1);
	}
}