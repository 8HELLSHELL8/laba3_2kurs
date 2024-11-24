#include "DynamicArray.cpp"

struct Stack 
{
private:

    struct Node 
    {
        string person;
        Node* next;
        Node* prev;

        Node(string person)
        {
            this->person = person;
            next = nullptr;
            prev = nullptr;
        }
    };

    Node* head;
    Node* tail;

public:

    Stack()
    {
        head = nullptr;
        tail = nullptr;
    }

    ~Stack() 
    {
        while (head != nullptr) 
        {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
    }                          

    bool is_empty() 
    {
        return head == nullptr;
    }             

    void push(string name) 
    {
        Node* new_node = new Node(name);
        new_node->next = head;  

        if (head != nullptr) 
        {
            head->prev = new_node; 
        } 
        else 
        {
            tail = new_node;
        }

        head = new_node;  
    }

    void pop() 
    {
        if (is_empty()) 
        {
            throw runtime_error("Stack is empty");
        }

        Node* temp = tail;  

        if (tail->prev != nullptr) 
        {
            tail = tail->prev;  
            tail->next = nullptr;  
        }
        else 
        {
            // Если это был последний элемент
            head = nullptr; 
            tail = nullptr;
        }

        delete temp;  
    }
  
                         
    void print() 
    {
        Node* current = head;
        while (current != nullptr) 
        {
            cout << current->person << " ";
            current = current->next;
        }
        cout << "\n";
    }  

    void save_to_file(const string& filename, const string& name_structure) const 
    {
        ifstream read_file(filename);
        DynamicArray lines;
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

        ofstream write_file(filename);
        if (!write_file.is_open()) 
        {
            cerr << "Error opening the file for writing.\n";
            return;
        }

        for (auto& existing_line : lines) 
        {
            if (existing_line.find(name_structure + " : ") == 0) {
                structure_found = true;
                // Заменяем строку структуры новыми значениями
                write_file << name_structure << " : ";

                Node* current = head;
                bool first = true;
                while (current != nullptr) 
                {
                    if (!first) write_file << ", ";
                    write_file << current->person; 
                    current = current->next;
                    first = false; // Сбрасываем первый элемент
                }
                write_file << endl;  
            } 
            else 
            {
                write_file << existing_line << endl;  
            }
        }

        // Если структура не найдена, добавляем её в конец файла
        if (!structure_found) 
        {
            write_file << name_structure << " : ";
            Node* current = head;
            bool first = true;

            while (current != nullptr) 
            {
                if (!first) write_file << ", ";
                write_file << current->person; 
                current = current->next;
                first = false; 
            }
            write_file << endl;
        }

        write_file.close();  
    }

    void load_from_file(const string& filename, const string& name_structure) 
    {
        ifstream file(filename);
        
        if (!file.is_open()) 
        {
            cerr << "Error opening the file for reading.\n";
            return;
        }

        string line;
        bool structure_found = false;

        // Читаем строки файла
        while (getline(file, line)) 
        {
            // Ищем строку, которая начинается с имени структуры
            if (line.find(name_structure + " : ") == 0) 
            {
                structure_found = true;
                size_t pos = line.find(':');
                string values = line.substr(pos + 1);  // Получаем строку с элементами после " : "
                stringstream ss(values);
                string value;

                // Используем временный стек для загрузки элементов в правильном порядке
                Stack temp_stack;
                while (getline(ss, value, ',')) 
                {
                    // Удаляем лишние пробелы
                    value.erase(0, value.find_first_not_of(" "));
                    value.erase(value.find_last_not_of(" ") + 1);

                    // Загружаем элементы в стек
                    temp_stack.push(value);
                }

                // Перемещаем элементы из временного стека в текущий стек
                while (!temp_stack.is_empty()) 
                {
                    push(temp_stack.head->person);
                    temp_stack.pop();
                }
                break;
            }
        }

        file.close();
    }

    int size()
    {
        Node* current = head;
        int i = 0;
        while(current)
        {
            i++;
            current = current->next;
        }
        return i;
    }

    void serialize(const string& fileName)
    {
        ofstream fileOut(fileName, ios::binary);
        if (!fileOut.is_open())
        {
            cerr << "Error opening file for serialization!" << endl;
        }

        int stackSize = this->size();
        fileOut.write(reinterpret_cast<char*>(&stackSize), sizeof(stackSize));

        Node* current = head;
        for (int i = 0; i < stackSize; i++, current = current->next)
        {
            size_t wordSize = current->person.size();
            fileOut.write(reinterpret_cast<char*>(&wordSize), sizeof(wordSize));
            fileOut.write(current->person.c_str(), wordSize);
        }
        cout << "Serialization done!" << endl;
        fileOut.close();
    }

     void deserialize(const string& fileName)
    {
        ifstream fileIn(fileName, ios::binary);
        if (!fileIn.is_open())
        {
            cerr << "Error opening file for deserialization" << endl;
            return;
        }

        int newSize;
        fileIn.read(reinterpret_cast<char*>(&newSize), sizeof(newSize));

        for (int i = 0; i < newSize; i++)
        {
            size_t wordSize;
            fileIn.read(reinterpret_cast<char*>(&wordSize), sizeof(wordSize));
            char* word = new char[wordSize + 1];
            fileIn.read(word, wordSize);
            word[wordSize] = '\0';
            push(string(word));
            delete[] word;
        }

        cout << "Deserialization done!" << endl;
        fileIn.close();
    }

};

int main()
{
    Stack init;
    init.push("1");
    init.push("2");
    init.push("1");
    init.serialize("binary/stack.bin");

    Stack restore;
    restore.deserialize("binary/stack.bin");
    restore.print();

    return 0;
}