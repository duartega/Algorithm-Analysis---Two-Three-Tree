//Description: Takes a text file supplied by the user
//             and turns it into a word index, implemented
//             through the use of a BST

#include <iostream>
#include <fstream>
#include "time.h"
#include <iomanip>
#include <sstream>
#include "bst.cpp"
#include "23node.cpp"
#include <string>

using namespace std;

twoThreeNode *generateTree(ifstream &file)
{
	twoThreeNode *tree = new twoThreeNode(nullptr);
	stringstream tempWord;
	int lineNum = 1;

	while (!file.eof())
	{
		string tempWord, line;
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
			lineNum++;
		}
	}
	return tree;
}

int main(int argc, char *argv[])
{
	int tree;
	char option;
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
		twoThreeNode *trees = generateTree(input2);

		input.close();
		input2.close();
		while (1)
		{
			char tree = '0';
			short option = 0;
			cout << "Options: (a) BST, (b) 2-3 Tree, (c) Compare BST and 2-3 Tree\n";
			cin >> tree;
			cout << "Options: (1) display index, (2) search, (3) save index, (4) quit\n";
			cin >> option;

			//Print index
			if (tree == 'a' && option == 1)
				myBSTTree.printTree(cout);
			if (tree == 'b' && option == 1)
				trees->printVals(trees);

			//Search index for a word
			if (option == 2)
			{
				if (tree == 'a')
					myBSTTree.contains();
				if (tree == 'b')
				{
					string sw;
					cout << "Search word: ";
					cin >> sw;
					tree->find(tree, sw)
				}
			}
			//Save index
			else if (option == 3)
			{
				string outputFile;
				cout << "Enter a filename to save your index to (Suggested: <filename>.txt) : ";
				cin >> outputFile;
				ofstream output(outputFile.c_str());
				if (tree == 'a')
					myBSTTree.printTree(output);
				if (tree == 'b')
					trees->printVals(trees);
				output.close();
				cout << "Saved\n";
			}

			//Quit
			else if (option == 4)
				break;
		}
	}
	else
	{
		cout << "Invalid File Name. Restart Program.\n";
		return 2;
	}
	return 0;
}
