#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <string>


typedef struct symbol_struct
{
    std::string symbol;
    std::string code = "";
    double p;
} SYMBOL;

class Node
{
private:
    SYMBOL* s;

public:
    Node* left = 0;
    Node* right = 0;

    SYMBOL* get_s()
    {
        return s;
    }
    std::string get_symbol()
    {
        return s->symbol;
    }
    std::string get_code()
    {
        return s->code;
    }
    double get_p()
    {
        return s->p;
    }

    Node(Node* l, Node* r)
    {
        s = new SYMBOL;
        this->s->symbol = r->s->symbol + l->s->symbol;
        this->s->p = l->s->p + r->s->p;
        this->left = l;
        this->right = r;
    }
    Node(SYMBOL* s)
    {
        this->s = s;
        left = right = 0;
    }
    ~Node() { delete s; }

    bool compare(Node* b)
    {
        return this->s->p > b->s->p;
    }

    void traversal_code(std::string code)
    {
        this->s->code = code;
        if (left)
        {
            left->traversal_code(code + "0");
            right->traversal_code(code + "1");
        }
    }

    void print(int wide)
    {
        if (!this)
            return;
        using namespace std;
        cout.width(wide + 3);
        cout << s->symbol + "\t\t";
        cout.width(8);
        cout << s->p << "\t\t" << s->code << "\n";
    }
};

class Analiser
{
private:
    int amount;
    std::vector<SYMBOL*> arr;

public:
    Analiser() : amount(0) {}
    Analiser(std::string path)
    {
        amount = 0;
        std::ifstream in(path);

        if (in.is_open())
        {
            while (1)
            {
                std::string s;
                in >> s;
                if (s == "0")
                {
                    break;
                }
                SYMBOL* add = new SYMBOL;
                add->symbol = s;
                in >> add->p;
                arr.push_back(add);
                ++amount;
            }

            in.close();
        }
        else
            std::cout << "Unable to open file";
    }
    ~Analiser()
    {
        arr.clear();
    }

    void print()
    {
        std::cout << "Table\n";
        for (int i = 0; i < amount; ++i)
        {
            std::cout << arr[i]->symbol.c_str() << "  " << arr[i]->p << '\t' << arr[i]->code << '\n';
        }
    }

    std::vector<Node*> create_node_array()
    {
        std::vector<Node*> n_arr;
        for (int i = 0; i < amount; ++i)
        {
            Node* add = new Node(arr[i]);
            n_arr.push_back(add);
        }
        return n_arr;
    }

    int get_amount()
    {
        return arr.size();
    }
};

bool comparator(Node* a, Node* b)
{
    return a->compare(b);
}

void sort_arr(std::vector<Node*>& arr)
{
    std::sort(arr.begin(), arr.end(), comparator);
}

void print_arr(std::vector<Node*>& arr, int iteration)
{
    std::cout << "\nTable iteration " << iteration << "\n";
    int wide = arr[0]->get_symbol().length();
    for (int i = 1; i < arr.size(); ++i)
    {
        int n_wide = arr[i]->get_symbol().length();
        if (n_wide > wide)
        {
            wide = n_wide;
        }
    }
    for (int i = 0; i < arr.size(); ++i)
    {
        arr[i]->print(wide);
    }
    std::cout << "\n";
}

void delete_tree(Node* root)
{
    if (!root)
        return;
    delete_tree(root->left);
    delete_tree(root->right);
    delete root;
}

void print_code(Node* root, int wide)
{
    if (!root)
        return;
    //root->print(wide);
    root->right->print(wide);
    root->left->print(wide);
}

Node* create_graph(std::vector<Node*> arr, int iteration)
{
    if (arr.size() == 1)
    {
        arr[0]->traversal_code("");
        return arr[0];
    }
    sort_arr(arr);
    Node* l = arr.back();
    arr.pop_back();
    Node* r = arr.back();
    arr.pop_back();
    Node* n = new Node(l, r);
    arr.push_back(n);
    if (arr[0]->get_p() != 1)
        print_arr(arr, iteration);
    return create_graph(arr, iteration + 1);
}

std::vector<Node*> delete_node(std::vector<Node*> arr, Node* dell)
{
    std::vector<Node*> ret;
    int i = 0;
    for (; i < arr.size(); ++i)
    {
        if (arr[i] == dell)
            break;
        ret.push_back(arr[i]);
    }
    for (++i; i < arr.size(); ++i)
    {
        ret.push_back(arr[i]);
    }
    return ret;
}

void print_code_table(Node* root, int size)
{
    std::vector<Node*> arr;
    arr.push_back(root);
    for (int i = size - 1; i > 0; --i)
    {
        Node* dell = NULL;
        for (int j = arr.size(); j != 0; --j)
        {
            if (arr[j - 1]->left)
            {
                dell = arr[j - 1];
            }
        }
        arr = delete_node(arr, dell);
        arr.push_back(dell->right);
        arr.push_back(dell->left);
        sort_arr(arr);
        if (i != 1)
            print_arr(arr, i - 1);
    }
}

int main(int argc, char* argv[])
{
    std::string name;
    if (argc < 2)
    {

        using namespace std;
        cout << "Please, enter file name: ";
        getline(std::cin, name, '\n');
    }
    else
    {
        name = argv[1];
    }
    Analiser* a = new Analiser(name);
    a->print();
    Node* top = create_graph(a->create_node_array(), 1);
    top->traversal_code("");

    print_code_table(top, a->get_amount());

    a->print();

    delete_tree(top);
    delete a;
    return 0;
}