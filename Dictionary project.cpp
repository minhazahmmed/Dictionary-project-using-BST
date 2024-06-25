#include <bits/stdc++.h>
#include <fstream>
#include <string>
using namespace std;

class Node
{
public:
    string word;
    string meaning;
    Node *left;
    Node *right;
    Node(string s, string m)
    {
        word = s;
        meaning = m;
        left = nullptr;
        right = nullptr;
    }
};

void Insert(Node *&root, const string &fileWord, const string &fileMeaning)
{
    if (root == nullptr)
    {
        root = new Node(fileWord, fileMeaning);
        return;
    }

    Node *mover = root;
    Node *prev = nullptr;
    while (mover)
    {
        prev = mover;
        if (fileWord < mover->word)
        {
            mover = mover->left;
        }
        else if (fileWord > mover->word)
        {
            mover = mover->right;
        }
        else
        {
            cout <<  "Word already exists\n" ;
            return;
        }
    }

    if (fileWord < prev->word)
    {
        prev->left = new Node(fileWord, fileMeaning);
    }
    else
    {
        prev->right = new Node(fileWord, fileMeaning);
    }
}

void ReadFromFile(Node *&root)
{
    ifstream infile("dictionary.txt");
    string line;

    while (getline(infile, line))
    {
        if (line == "###")
            break;

        size_t pos = line.find(':');
        if (pos != string::npos)
        {
            string fileWord = line.substr(0, pos);
            string fileMeaning = line.substr(pos + 1);
            Insert(root, fileWord, fileMeaning);
        }
    }

    infile.close();
}

void Inorder(Node *root, ofstream &outfile)
{
    if (root)
    {
        Inorder(root->left, outfile);
        outfile << root->word << ":" << root->meaning << endl;
        Inorder(root->right, outfile);
    }
}

void WriteInFile(Node *root)
{
    ofstream outfile("dictionary.txt");
    Inorder(root, outfile);
    outfile.close();
    cout << "File write successful.\n";
}

void AddWord(Node *&root)
{
    string s, meaning;
    cin.ignore();
    cout << "Enter a word to add: ";
    getline(cin, s);
    cout << "Enter the meaning of this word: ";
    getline(cin, meaning);

    Insert(root, s, meaning);
    WriteInFile(root);
}

Node* SearchWord(Node *root, const string &word)
{
    Node *mover = root;
    while (mover)
    {
        if (mover->word == word)
        {
            return mover;
        }
        else if (word < mover->word)
        {
            mover = mover->left;
        }
        else
        {
            mover = mover->right;
        }
    }
    return nullptr;
}

void SearchWordInFile(Node *root)
{
    string word;
    cout << "Enter a word to search: ";
    cin.ignore();
    getline(cin, word);

    Node *result = SearchWord(root, word);
    if (result)
    {
        cout << "The meaning of this word is: " << result->meaning << endl;
    }
    else
    {
        cout << "Word not found in the dictionary.\n";
    }
}

void show(Node *root)
{
    if (root)
    {
        show(root->left);
        cout << root->word << " " << root->meaning << endl;
        show(root->right);
    }
}

void DeleteWord(Node *&root)
{
    string del;
    cout << "Enter the word you want to delete: ";
    cin >> del;

    Node *parent = nullptr;
    Node *mover = root;

    while (mover && mover->word != del)
    {
        parent = mover;
        if (del < mover->word)
        {
            mover = mover->left;
        }
        else
        {
            mover = mover->right;
        }
    }

    if (!mover)
    {
        cout << "Word not found\n";
        return;
    }

    if (!mover->left || !mover->right)
    {
        Node *child = (mover->left) ? mover->left : mover->right;
        if (!parent)
        {
            root = child;
        }
        else if (parent->left == mover)
        {
            parent->left = child;
        }
        else
        {
            parent->right = child;
        }
        delete mover;
    }
    else
    {
        Node *succParent = mover;
        Node *successor = mover->right;
        while (successor->left)
        {
            succParent = successor;
            successor = successor->left;
        }

        mover->word = successor->word;
        mover->meaning = successor->meaning;

        if (succParent->left == successor)
        {
            succParent->left = successor->right;
        }
        else
        {
            succParent->right = successor->right;
        }
        delete successor;
    }

    WriteInFile(root);
}

int main()
{
    Node *root = nullptr;
    ReadFromFile(root);

    int loop = 1;
    while (loop)
    {
        cout << "Choose an option: \n1. Add word\n2. Search word in file\n3. Delete Word\n4. Show all\n0. Exit\n";
        cin >> loop;
        loop = int(loop);

        if (loop == 0)
            break;

        if (loop == 1)
        {
            AddWord(root);
        }
        else if (loop == 2)
        {
            SearchWordInFile(root);
        }
        else if (loop == 3)
        {
            DeleteWord(root);
        }
        else if(loop == 4)
        {
            show(root);
        }
        else
        {
            cout << "Invalid option. Try again.\n";
        }
    }

    return 0;
}
