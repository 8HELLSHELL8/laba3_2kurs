#include "library.h"

struct LinkedList 
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

    LinkedList()
    {
        head = nullptr;
        tail = nullptr;
    }

    ~LinkedList() 
    {
        Node* current = head;

        while (current != nullptr) 
        {
            Node* next_node = current->next;
            delete current;
            current = next_node;
        }
        head = tail = nullptr;
    }                         

    bool is_empty() 
    {
        return head == nullptr;
    }           

    int size() const 
    {
        int counter = 0;
        Node* current = head;
        while (current) 
        {
            ++counter;
            current = current->next;
        }
        return counter;
    }

    void addhead(string name) 
    {
        Node* newnode = new Node(name);

        if (head == nullptr) 
        {
            head = newnode;
            tail = newnode;
        }
        else 
        {
            newnode->next = head;
            head->prev = newnode;
            head = newnode;
        }
    } 

    void addtail(string name) 
    {
        Node* newnode = new Node(name);

        if (head == nullptr) 
        {
            head = newnode;
            tail = newnode;
        }
        else 
        {
            tail->next = newnode;
            newnode->prev = tail;
            tail = newnode;
        }
    } 

    void delhead() 
    {
        if (is_empty()) 
        {
            throw runtime_error("LinkedList is empty");
        }
        Node* next_node = head->next;

        if (next_node != nullptr) 
        {
            next_node->prev = nullptr;
        }
        else 
        {
            tail = nullptr;
        }

        delete head;
        head = next_node;
    }   

    void deltail() 
    {
        if (is_empty()) 
        {
            throw runtime_error("LinkedList is empty");
        }

        Node* prev_node = tail->prev;

        if (prev_node != nullptr) 
        {
            prev_node->next = nullptr;
        }
        else 
        {
            head = nullptr;
        }

        delete tail;
        tail = prev_node;
    }             

    void remove(string name) 
    {
        if (is_empty()) 
        {
            throw runtime_error("LinkedList is empty");
        }

        Node* current = head;
        bool found = false;

        while (current != nullptr) 
        {
            if (current->person == name) 
            {
                if (current == head) 
                {
                    head = current->next;
                    if (head != nullptr) 
                    {
                        head->prev = nullptr;
                    } 
                    else 
                    {
                        tail = nullptr;
                    }
                } 
                else if (current == tail) 
                {
                    tail = current->prev;
                    tail->next = nullptr;
                } 
                else 
                {
                    current->prev->next = current->next; // The next node for the previous one
                    current->next->prev = current->prev; // The previous node for the next one
                }

                delete current; // Deleting the node
                found = true;
                break;
            }
            current = current->next;
        }

        if (!found) 
        {
            //cout << "Element \"" << name << "\" not found in LinkedList!!!" << "\n";
            throw runtime_error("Element not found in LinkedList!!!");
        }
    }      

    string get(int index) 
    {
        if (index < 0 || is_empty() || index >= size()) 
        {
            throw out_of_range("Index out of range!");
        }

        Node* current = head;
        for (int i = 0; i < index; ++i) 
        {
            current = current->next;
        }
        return current->person;
    }

    bool search(string name) 
    {
        if (is_empty()) 
        {
            throw runtime_error("LinkedList is empty");
        }

        Node* current = head;

        while (current != nullptr) 
        {
            if (current->person == name) 
            {
                //cout << "Element \"" << current->person << "\" was found in LinkedList!!!" << "\n";
                return 1;
            }
            current = current->next;
        }
        throw runtime_error("Element not found in LinkedList!!!");
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

    
    void load_from_file(const string& filename, const string& name_structure) 
    {
        ifstream file(filename);
        
        if (!file.is_open()) 
        {
            cerr << "Ошибка открытия файла для чтения.\n";
            return;
        }

        string line;
        bool structure_found = false;

        while (!is_empty()) 
        {
            delhead();
        }

        while (getline(file, line)) 
        {
            // Проверяем, является ли строка началом новой структуры
            if (line.find(name_structure + " : ") != string::npos) 
            {
                structure_found = true; // Найдена структура с указанным именем
                size_t pos = line.find(':');
                string values = line.substr(pos + 1); 
                stringstream ss(values);
                string value;

                // Парсим значения, разделенные запятыми, и добавляем их в список
                while (getline(ss, value, ',')) 
                {
                    value.erase(0, value.find_first_not_of(" \t"));  
                    addtail(value);  
                }
                break;  
            }
        }

        file.close();
    }


    void save_to_file(const string& filename, const string& name_structure) const 
    {
        ifstream read_file(filename);
        LinkedList lines;  
        string line;
        bool structure_found = false;

        if (read_file.is_open()) 
        {
            while (getline(read_file, line)) 
            {
                lines.addtail(line);
            }
            read_file.close();
        } 
        else 
        {
            cerr << "Ошибка открытия файла для чтения.\n";
            return;
        }

        ofstream write_file(filename);
        if (!write_file.is_open()) 
        {
            cerr << "Ошибка открытия файла для записи.\n";
            return;
        }

        Node* current = lines.head;

        while (current != nullptr) 
        {
            string& existing_line = current->person;  // Получаем строку из временного списка
            if (existing_line.find(name_structure + " : ") == 0) 
            {
                structure_found = true;

                // Заменяем строку структуры новыми значениями из списка
                write_file << name_structure << " : ";
                Node* data_node = head;  
                bool first = true;
                while (data_node != nullptr) 
                {
                    if (!first) 
                    {
                        write_file << ", "; 
                    }
                    write_file << data_node->person;  // Записываем элемент
                    first = false;
                    data_node = data_node->next; 
                }
                write_file << endl; 
            } 
            else 
            {
                // Записываем все остальные строки как есть
                write_file << existing_line << endl;
            }
            current = current->next;  // Переходим к следующему элементу во временном списке
        }

        // Если структура не найдена, добавляем её в конец файла
        if (!structure_found) 
        {
            write_file << name_structure << " : ";
            Node* data_node = head;  
            bool first = true;
            while (data_node != nullptr) {
                if (!first) {
                    write_file << ", "; 
                }
                write_file << data_node->person; 
                first = false;
                data_node = data_node->next;  // Переходим к следующему элементу
            }
            write_file << endl; 
        }

        write_file.close();  
    }
    
    void serialize(const string& fileName)
    {
        ofstream fileOut(fileName, ios::binary);
        if (!fileOut.is_open())
        {
            cerr << "Error opening for serialization" << endl;
            return;
        }
        size_t listSize = this->size();
        fileOut.write(reinterpret_cast<char*>(&listSize), sizeof(listSize));

        Node* current = head;
        for (int i = 0; i < listSize; i++)
        {
            size_t wordSize = current->person.size();
            fileOut.write(reinterpret_cast<char*>(&wordSize), sizeof(wordSize));
            fileOut.write(current->person.c_str(), wordSize);

            current = current->next;
        }
        cout << "Serialization done!" << endl;
        fileOut.close();
    }

    void deserialize(const string& fileName)
    {
        ifstream fileIn(fileName, ios::binary);
        if (!fileIn.is_open())
        {
            cerr << "Error opening for deserialization" << endl;
            return;
        }

        size_t listSize;
        fileIn.read(reinterpret_cast<char*>(&listSize), sizeof(listSize));
        for (int i = 0; i < listSize; i++)
        {
            size_t wordSize;
            fileIn.read(reinterpret_cast<char*>(&wordSize), sizeof(wordSize));
            char* word = new char[wordSize + 1];
            fileIn.read(word, wordSize);
            word[wordSize] = '\0';

            addtail(string(word));

            delete[] word;
        }


        cout << "Deserialization done!" << endl;
        fileIn.close();
    }
    



};