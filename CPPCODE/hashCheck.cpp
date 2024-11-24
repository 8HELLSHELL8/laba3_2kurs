#include "DynamicArray.cpp"


using namespace std;

struct Node 
{
    string key;
    string value;
    Node* next;

    Node(const string& key, const string& value) 
    {
        this->key = key;
        this->value = value;
        this->next = nullptr;
    }
};

struct Hashtable 
{
private:

    Node** table; // An array of pointers to the beginning of the chains
    int size;

    int hashfunction(const string& key) 
    {
        uint32_t hash = 5381;
        for (char c : key) 
        {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return hash % size;
    }

public:

    Hashtable(int size)
    {
        this->size = size;
        table = new Node * [size];
        for (int i = 0; i < size; i++) 
        {
            table[i] = nullptr;
        }
    }

    ~Hashtable() 
    {
        for (int i = 0; i < size; i++) 
        {
            Node* current = table[i];
            while (current != nullptr) 
            {
                Node* prev = current;
                current = current->next;
                delete prev;
            }
        }
        delete[] table;
    }        

    void add(const string& key, const string& value) 
    {
        int index = hashfunction(key);
        Node* current = table[index];


        // Проходим по цепочке, чтобы найти существующий ключ
        while (current != nullptr)
        {
            if (current->key == key) 
            {
                current->value = value; // Обновляем значение, если ключ уже существует
                return;
            }
            current = current->next;
        }

        // Если ключ не найден, создаем новый узел и добавляем его в начало цепочки
        Node* new_node = new Node(key, value);
        new_node->next = table[index];
        table[index] = new_node;
    }

    string get(const string& key) 
    {
        int index = hashfunction(key);
        Node* current = table[index];

        while (current != nullptr) 
        {
            if (current->key == key) 
            {
                return current->value;
            }
            current = current->next;
        }

        throw runtime_error("The key was not found");
    }   

    void remove(const string& key) 
    {
        int index = hashfunction(key);
        Node* current = table[index];
        Node* prev = nullptr;

        while (current != nullptr) // We go through the chain and look for an item to delete
        { 
            if (current->key == key) 
            {
                if (prev == nullptr) 
                {
                    table[index] = current->next; // If the first in the chain
                }
                else 
                {
                    prev->next = current->next;
                }
                delete current;
                return;
            }
            prev = current;
            current = current->next;
        }
        throw runtime_error("The key was not found");
    } 

    void print() 
    {
        for (int i = 0; i < size; ++i) 
        {
            cout << i << ": ";
            Node* current = table[i];
            if (current != nullptr) 
            {
                while (current != nullptr) 
                {
                    cout << "[" << current->key << ": " << current->value << "]";
                    current = current->next;
                    if (current != nullptr) 
                    {
                        cout << " -> ";
                    }
                }
            }
            cout << "\n";
        }
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

        while (getline(file, line)) 
        {
            // Проверяем, является ли строка началом структуры хеш-таблицы
            if (line.find(name_structure + " : ") != string::npos) 
            {
                structure_found = true; // Нашли хеш-таблицу с указанным именем
                size_t pos = line.find(':');
                string values = line.substr(pos + 1); 
                stringstream ss(values);
                string pair;

                // Парсим ключ-значение, разделенные запятыми
                while (getline(ss, pair, ',')) 
                {
                    size_t start = pair.find('[');
                    size_t middle = pair.find(':');
                    size_t end = pair.find(']');

                    if (start != string::npos && middle != string::npos && end != string::npos) 
                    {
                        string key = pair.substr(start + 1, middle - start - 1);  // Получаем ключ
                        string value = pair.substr(middle + 2, end - middle - 2); // Получаем значение

                        // Добавляем пару ключ-значение в хеш-таблицу
                        add(key, value);
                    }
                }
                break;
            }
        }

        file.close();

    }

    void save_to_file(const string& filename, const string& name_structure) const 
    {
        // Читаем существующие данные из файла
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
            cerr << "Ошибка открытия файла для записи.\n";
            return;
        }

        // Проверяем наличие структуры с именем name_structure
        for (auto& existing_line : lines) 
        {
            if (existing_line.find(name_structure + " : ") == 0) 
            {
                structure_found = true;

                // Заменяем строку структуры новыми значениями
                write_file << name_structure << " : ";
                
                // Проходим по всем элементам хеш-таблицы
                bool first_entry = true;
                for (int i = 0; i < size; ++i) 
                {
                    Node* current = table[i];
                    while (current != nullptr) 
                    {
                        if (!first_entry) 
                        {
                            write_file << ", "; // Добавляем разделитель
                        }
                        write_file << "[" << current->key << ": " << current->value << "]";
                        first_entry = false;
                        current = current->next;
                    }
                }
                write_file << endl;
            } 
            else 
            {
                write_file << existing_line << endl; // Сохраняем остальные строки
            }
        }

        // Если структура не найдена, добавляем её в конец файла
        if (!structure_found) 
        {
            write_file << name_structure << " : ";
            bool first_entry = true;
            for (int i = 0; i < size; ++i) 
            {
                Node* current = table[i];
                while (current != nullptr) 
                {
                    if (!first_entry) 
                    {
                        write_file << ", "; // Добавляем разделитель
                    }
                    write_file << "[" << current->key << ": " << current->value << "]";
                    first_entry = false;
                    current = current->next;
                }
            }
            write_file << endl;
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

        // Сохраняем размер таблицы
        fileOut.write(reinterpret_cast<const char*>(&size), sizeof(size));

        // Сохраняем ключ-значение
        for (int i = 0; i < size; ++i) 
        {
            Node* current = table[i];
            while (current != nullptr) 
            {
                // Сохраняем длину ключа и сам ключ
                int keyLength = current->key.size();
                fileOut.write(reinterpret_cast<const char*>(&keyLength), sizeof(keyLength));
                fileOut.write(current->key.c_str(), keyLength);

                // Сохраняем длину значения и само значение
                int valueLength = current->value.size();
                fileOut.write(reinterpret_cast<const char*>(&valueLength), sizeof(valueLength));
                fileOut.write(current->value.c_str(), valueLength);

                current = current->next;
            }
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

        // Очищаем текущую хеш-таблицу
        for (int i = 0; i < size; ++i) 
        {
            Node* current = table[i];
            while (current != nullptr) 
            {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
            table[i] = nullptr;
        }

        // Читаем размер таблицы
        int newSize;
        fileIn.read(reinterpret_cast<char*>(&newSize), sizeof(newSize));

        // Если размер таблицы изменился, пересоздаём массив
        if (newSize != size) 
        {
            delete[] table;
            size = newSize;
            table = new Node * [size];
            for (int i = 0; i < size; ++i) 
            {
                table[i] = nullptr;
            }
        }

        // Читаем ключ-значение
        while (fileIn.peek() != EOF) 
        {
            // Читаем длину ключа и сам ключ
            int keyLength;
            fileIn.read(reinterpret_cast<char*>(&keyLength), sizeof(keyLength));
            string key(keyLength, '\0');
            fileIn.read(&key[0], keyLength);

            // Читаем длину значения и само значение
            int valueLength;
            fileIn.read(reinterpret_cast<char*>(&valueLength), sizeof(valueLength));
            string value(valueLength, '\0');
            fileIn.read(&value[0], valueLength);

            // Добавляем ключ-значение в хеш-таблицу
            add(key, value);
        }

        cout << "Deserialization complete." << endl;
        fileIn.close();
    }

};


int main()
{
    Hashtable init(5);
    init.add("lol", "kek");
    init.add("check", "check");
    init.add("proverka", "test");
    init.serialize("binary/hash.bin");

    Hashtable restore(1);
    restore.deserialize("binary/hash.bin");
    restore.print();

    return 0;
}