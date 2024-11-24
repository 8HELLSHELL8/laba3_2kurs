#include "library.h"

struct ForwardList 
{
private:

    struct Node 
    {
        string person;
        Node* next;

        Node(string string_per)
        {
            person = string_per;
            next = nullptr;
        }
    };

    Node* head;
    Node* tail;

public:

    ForwardList() 
    {
        head = nullptr;
        tail = nullptr;
    }

    ~ForwardList() 
    {
        Node* current = head;
        while (current != nullptr) 
        {
            Node* next_node = current->next;
            delete current;
            current = next_node;
        }
        head = nullptr;
    }

    bool is_empty() const 
    {
        return head == nullptr;
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

    void addtail(string name) 
    {
        Node* newNode = new Node(name);
        if (is_empty()) 
        {
            head = newNode;
            tail = newNode;
            return;
        }
        tail->next = newNode;
        tail = newNode;
    }

    void addhead(string name) 
    {
        Node* newNode = new Node(name);
        if (is_empty()) 
        {
            head = newNode;
            tail = newNode;
            return;
        }
        newNode->next = head;
        head = newNode;
    }

    void delhead() 
    {
        if (is_empty()) 
        {
            throw runtime_error("ForwardList is empty");
        }
        if (head == tail) 
        {
            delete tail;
            head = tail = nullptr;
            return;
        }
        Node* node = head;
        head = node->next;
        delete node;
    }

    void deltail() 
    {
        if (is_empty()) 
        {
            throw runtime_error("ForwardList is empty");
        }
        if (head == tail) 
        {
            delete tail;
            head = tail = nullptr;
            return;
        }
        Node* node = head;
        while (node->next != tail) 
        {
            node = node->next;
        }
        delete tail;
        tail = node;
        tail->next = nullptr;
    }

    void remove(string name) 
    {
        if (is_empty()) 
        {
            throw runtime_error("ForwardList is empty");
        }
        // Если удаляем голову
        if (head->person == name) 
        {
            Node* temp = head;
            head = head->next;
            if (head == nullptr) 
            {
                tail = nullptr;
            }
            delete temp;
            return;
        }
        // Поиск узла с данным значением
        Node* current = head;
        Node* prev = nullptr;
        while (current != nullptr && current->person != name) 
        {
            prev = current;
            current = current->next;
        }

        if (current == nullptr) 
        {
            cout << "Value \"" << name << "\" not found in the list." << endl;
            return;
        }

        if (current == tail) 
        {
            tail = prev;
        }

        if (prev != nullptr) 
        {
            prev->next = current->next;
        }
        delete current;
    }

    bool search(string name) 
    {
        if (is_empty()) 
        {
            throw runtime_error("ForwardList is empty");
        }

        Node* current = head;
        while (current != nullptr) 
        {
            if (current->person == name) 
            {
                cout << "Value \"" << name << "\" found in the list." << endl;
                return true;
            }
            current = current->next;
        }

        cout << "Value \"" << name << "\" not found in the list." << endl;
        return false;
    }

    void print() const 
    {
        if (is_empty()) 
        {
            cout << "List is empty" << endl;
            return;
        }
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
                    value.erase(0, value.find_first_not_of(" \t"));  // Удаляем пробелы перед значениями
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
        ForwardList lines;
        string line;
        bool structure_found = false;

        // Читаем файл построчно и сохраняем все строки во временный список
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

        // Открываем файл для записи (перезапись)
        ofstream write_file(filename);
        if (!write_file.is_open()) 
        {
            cerr << "Ошибка открытия файла для записи.\n";
            return;
        }

        Node* current = lines.head;

        while (current != nullptr)
        {
            string& existing_line = current->person;
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
                    write_file << data_node->person;
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
            current = current->next;
        }

        // Если структура не найдена, добавляем её в конец файла
        if (!structure_found) 
        {
            write_file << name_structure << " : ";
            Node* data_node = head;
            bool first = true;
            while (data_node != nullptr) 
            {
                if (!first) 
                {
                    write_file << ", ";
                }
                write_file << data_node->person;
                first = false;
                data_node = data_node->next;
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
            cerr << "Error opening file for serialization" << endl;
            return;
        }
        size_t listSize = this->size();
        fileOut.write(reinterpret_cast<char*>(&listSize), sizeof(listSize));
        Node* current = head;
        for (int i = 0; i < listSize; i++)
        {
            size_t wordLenght = current->person.size();
            fileOut.write(reinterpret_cast<char*>(&wordLenght), sizeof(wordLenght));
            fileOut.write(current->person.c_str(), wordLenght);

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
            cerr << "Error opening file for deserialization" << endl;
            return;
        }
        
        size_t listSize;
        fileIn.read(reinterpret_cast<char*>(&listSize), sizeof(listSize));

        for (size_t i = 0; i < listSize; i++)
        {
            size_t wordSize;
            fileIn.read(reinterpret_cast<char*>(&wordSize), sizeof(wordSize));

            char* word = new char[wordSize + 1];
            fileIn.read(word, wordSize);
            word[wordSize] = '\0';

            addtail(string(word));
            delete[] word; // Освобождаем память
        }

        cout << "Deserialization done!" << endl;
        fileIn.close();
    }


};