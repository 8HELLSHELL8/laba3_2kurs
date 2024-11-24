#include "DynamicArray.cpp"


struct node 
{
    int32_t key;
    node* leftChild;
    node* rightChild;

    node(int value)
    {
        key = value;
        leftChild = nullptr;
        rightChild = nullptr;
    }
};

template <typename T>
struct Queue 
{
private:

    struct Node 
    {
        T data;
        Node* next;
        Node(T data)
        {
            this->data = data;
            next = nullptr;
        }
    };

    Node* head;
    Node* tail;

public:

    Queue()
    {
        head = nullptr;
        tail = nullptr;
    }

    Queue(const Queue& other)
    {
        head = nullptr;
        tail = nullptr;
        Node* current = other.head;
        while (current != nullptr) 
        {
            enqueue(current->data);  // We copy the elements one by one
            current = current->next;
        }
    }

    // ~Queue() 
    // {
    //     while (head != nullptr) 
    //     {
    //         Node* temp = head;
    //         head = head->next;
    //         delete temp;
    //     }
    // }

    bool is_empty() const 
    { 
        return head == nullptr; 
    }

    void enqueue(T data) 
    {
        Node* new_node = new Node(data);
        if (is_empty()) 
        {
            head = tail = new_node;
        }
        else 
        {
            tail->next = new_node;
            tail = new_node;
        }
    }

    void dequeue() 
    {
        if (is_empty()) 
        {
            throw runtime_error("Queue is empty");
        }
        Node* temp = head;
        head = head->next;
        delete temp;
        if (head == nullptr) 
        {
            tail = nullptr;
        }
    }

    T front() const 
    {
        if (is_empty()) 
        {
            throw runtime_error("Queue is empty");
        }
        return head->data;
    }
};

struct FBT 
{
private:

    node* root;
    Queue<node*> waitlist;  // Queue for adding new nodes
    Queue<node*> allNodes;  // Queue for storing all nodes

public:

    FBT(int value) 
    {
        root = new node(value);
        allNodes.enqueue(root);
        waitlist.enqueue(root);
    }

    // ~FBT() 
    // {
    //     // Freeing all nodes
    //     Queue<node*> tempQueue = allNodes;
    //     while (!tempQueue.is_empty()) 
    //     {
    //         node* currentNode = tempQueue.front();
    //         tempQueue.dequeue();
    //         delete currentNode;  // Deleting the node
    //     }
    // }



    void Add(int value) 
    {

        // Проверяем, существует ли узел с таким значением
        try 
        {
            findPlace(value); 
            return; 
        } 
        catch (const runtime_error&) 
        {
            // Узел не найден, продолжаем добавление
        }

        if (waitlist.is_empty()) 
        {
            throw runtime_error("The parent node was not found to which a new child node can be added!!!");
        }

        node* parent = waitlist.front();
        node* newNode = new node(value);
        allNodes.enqueue(newNode); // Добавляем новый узел в очередь

        if (parent->leftChild == nullptr) {
            parent->leftChild = newNode; 
        } 
        else if (parent->rightChild == nullptr) {
            parent->rightChild = newNode; 
            waitlist.dequeue();
        } 
        else 
        {
            throw runtime_error("Parent node already has two children.");
        }

        waitlist.enqueue(newNode); // Добавляем новый узел в очередь для дальнейшего использования
    }

    void PrintNodes() 
    {
        Queue<node*> tempQueue = allNodes;
        while (!tempQueue.is_empty()) 
        {
            node* currentNode = tempQueue.front();
            cout << currentNode->key << " ";
            tempQueue.dequeue();
        }
        cout << "\n";
    }

    void PrintTree() 
    {
        int totalNodes = 0;  // The total number of nodes in the tree
        Queue<node*> tempQueue = allNodes;

        while (!tempQueue.is_empty()) 
        {
            tempQueue.dequeue();
            totalNodes++;
        }

        int level = 0;  // Current tree level
        int printedNodes = 0;  // The number of nodes already printed
        int maxNodesAtLevel = pow(2, floor(log2(totalNodes)) + 1);

        tempQueue = allNodes;  // Returning the queue to its original state for printing the tree

        while (printedNodes < totalNodes) 
        {
            int nodesInLevel = pow(2, level);
            level++;

            int leadingSpaces = maxNodesAtLevel / nodesInLevel - 1;
            int spacingBetweenNodes = maxNodesAtLevel / nodesInLevel - 1;
            string leadSep(leadingSpaces + 3, ' ');
            string endSep(spacingBetweenNodes, ' ');

            for (int i = 0; i < nodesInLevel && printedNodes < totalNodes; i++) 
            {
                cout << leadSep << tempQueue.front()->key << endSep;
                tempQueue.dequeue();
                printedNodes++;
            }

            cout << "\n";
        }
    }

    node* findPlace(int value) 
    {
        Queue<node*> tempQueue = allNodes;

        while (!tempQueue.is_empty()) 
        {
            node* currentNode = tempQueue.front();
            if (currentNode->key == value) 
            {
                return currentNode;
            }
            tempQueue.dequeue();
        }

        throw runtime_error("Value was not found!!!");
        return nullptr;
    }

    bool isFull() {
        Queue<node*> tempQueue = allNodes;

        while (!tempQueue.is_empty()) 
        {
            node* currentNode = tempQueue.front();
            if ((currentNode->leftChild == nullptr && currentNode->rightChild != nullptr) ||
                (currentNode->leftChild != nullptr && currentNode->rightChild == nullptr)) 
            {
                return false;
            }
            tempQueue.dequeue();
        }
        return true;
    }

    void load_from_file(const string& filename, const string& name_structure) 
    {

        ifstream file(filename);
        
        if (file.is_open()) 
        {

            string line;
            bool structure_found = false;

            while (getline(file, line)) 
            {

                if (line.find(name_structure + ":") != string::npos) 
                {

                    structure_found = true; 
                    size_t pos = line.find(':');
                    string values = line.substr(pos + 1); 
                    stringstream ss(values);
                    string value;

                    // Парсим значения, разделенные запятыми
                    while (getline(ss, value, ',')) 
                    {
                        int key = stoi(value); 
                        try 
                        {
                            Add(key); // Добавляем узел в дерево
                        } 
                        catch (const runtime_error& e) 
                        {
                            cerr << "Error when adding a value " << key << ": " << e.what() << endl;
                        }
                    }
                    break; 
                }
            }

            file.close();
        } 
        else 
        {
            cerr << "Error opening the file for reading.\n";
        }
    }


    void save_to_file(const string& filename, const string& name_structure) const 
    {

        ifstream read_file(filename);
        DynamicArray lines; // Используем вектор для хранения строк
        string line;
        bool structure_found = false;

        if (read_file.is_open()) 
        {
            while (getline(read_file, line)) 
            {
                lines.MPUSH(line); 
            }
            read_file.close();
        } 
        else 
        {
            cerr << "Error opening the file for reading.\n";
            return;
        }

        // Открываем файл для записи
        ofstream write_file(filename);
        if (!write_file.is_open()) 
        {
            cerr << "Error opening the file for writing.\n";
            return;
        }

        for (const auto& existing_line : lines) 
        {
            if (existing_line.find(name_structure + ":") == 0) 
            {
                structure_found = true;

                // Заменяем строку структуры новыми значениями
                write_file << name_structure << ": ";
                Queue<node*> tempQueue = allNodes; // Используем очередь для сохранения узлов

                while (!tempQueue.is_empty()) 
                {
                    node* currentNode = tempQueue.front();
                    tempQueue.dequeue();

                    // Сохраняем ключ узла
                    write_file << currentNode->key;
                    if (!tempQueue.is_empty()) 
                    {
                        write_file << ", "; 
                    }
                }
                write_file << "\n"; 
            } 
            else 
            {
                write_file << existing_line << "\n"; 
            }
        }

        // Если структура не найдена, добавляем её
        if (!structure_found) 
        {
            write_file << name_structure << ": ";
            Queue<node*> tempQueue = allNodes; // Повторно используем очередь для сохранения узлов

            while (!tempQueue.is_empty()) 
            {
                node* currentNode = tempQueue.front();
                tempQueue.dequeue();

                // Сохраняем ключ узла
                write_file << currentNode->key;
                if (!tempQueue.is_empty()) 
                {
                    write_file << ", "; 
                }
            }
            write_file << "\n"; 
        }

        write_file.close(); 
    }

    void serialize(const string& filename) const
    {
        ofstream fileOut(filename, ios::binary);
        if (!fileOut.is_open()) 
        {
            cerr << "Error opening file for serialization!" << endl;
            return;
        }

        Queue<node*> tempQueue = allNodes; // Используем очередь для обхода всех узлов
        while (!tempQueue.is_empty()) 
        {
            node* currentNode = tempQueue.front();
            tempQueue.dequeue();

            // Сохраняем ключ узла
            fileOut.write(reinterpret_cast<char*>(&currentNode->key), sizeof(currentNode->key));

            // Сохраняем информацию о детях
            bool hasLeftChild = (currentNode->leftChild != nullptr);
            bool hasRightChild = (currentNode->rightChild != nullptr);
            fileOut.write(reinterpret_cast<char*>(&hasLeftChild), sizeof(hasLeftChild));
            fileOut.write(reinterpret_cast<char*>(&hasRightChild), sizeof(hasRightChild));
        }

        cout << "Serialization complete." << endl;
        fileOut.close();
    }

    void deserialize(const string& filename)
    {
        ifstream fileIn(filename, ios::binary);
        if (!fileIn.is_open()) 
        {
            cerr << "Error opening file for deserialization!" << endl;
            return;
        }

        // Очищаем текущее дерево
        while (!waitlist.is_empty())
        {
            waitlist.dequeue();
        }
        while (!allNodes.is_empty())
        {
            node* currentNode = allNodes.front();
            allNodes.dequeue();
            delete currentNode;
        }
        root = nullptr;

        // Считываем данные из файла
        Queue<node*> tempQueue; // Очередь для восстановления дерева
        int key;
        while (fileIn.read(reinterpret_cast<char*>(&key), sizeof(key))) 
        {
            bool hasLeftChild, hasRightChild;
            fileIn.read(reinterpret_cast<char*>(&hasLeftChild), sizeof(hasLeftChild));
            fileIn.read(reinterpret_cast<char*>(&hasRightChild), sizeof(hasRightChild));

            node* newNode = new node(key);
            allNodes.enqueue(newNode);

            if (root == nullptr) 
            {
                root = newNode; // Устанавливаем корень дерева
            } 
            else 
            {
                node* parent = tempQueue.front();
                if (parent->leftChild == nullptr) 
                {
                    parent->leftChild = newNode;
                } 
                else if (parent->rightChild == nullptr) 
                {
                    parent->rightChild = newNode;
                    tempQueue.dequeue(); // Убираем родителя из очереди, если оба ребенка установлены
                }
            }

            if (hasLeftChild || hasRightChild) 
            {
                tempQueue.enqueue(newNode);
            }
        }

        cout << "Deserialization complete." << endl;
        fileIn.close();
    }
};


int main()
{
    FBT tree(1);
    tree.Add(2);
    tree.Add(3);
    tree.Add(4);
    tree.Add(5);
    tree.Add(6);
    tree.Add(7);
    tree.Add(8);
    tree.Add(9);
    tree.Add(0);
    tree.Add(10);
    tree.serialize("binary/tree.bin");

    FBT init(1000);
    init.deserialize("binary/tree.bin");
    init.PrintTree();

    


    return 0;
}

