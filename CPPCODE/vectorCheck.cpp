#include "library.h"

class DynamicArray 
{
private:

    string* arr;  
    int size_of_Vec;  
    int memory_size; 

public:

    DynamicArray() 
    {
        arr = new string[1];
        size_of_Vec = 0;
        memory_size = 1;
    }  

    DynamicArray(const DynamicArray& other) 
    {
        size_of_Vec = other.size_of_Vec;
        memory_size = other.memory_size;
        arr = new string[memory_size];
        for (size_t i = 0; i < size_of_Vec; i++) 
        {
            arr[i] = other.arr[i];
        }
    }   

    DynamicArray(initializer_list<string> init_list) 
    {
        size_of_Vec = init_list.size();  // Размер массива соответствует количеству элементов
        memory_size = size_of_Vec * 2;  
        arr = new string[memory_size];

        int i = 0;
        for (const string& item : init_list) 
        {
            arr[i++] = item;  // Копируем элементы из списка
        }
    }

    DynamicArray& operator=(const DynamicArray& other) 
    {
        if (this != &other) 
        { // Don't we assign the object to ourselves
            delete[] arr;
            size_of_Vec = other.size_of_Vec;
            memory_size = other.memory_size;
            arr = new string[memory_size];
            for (size_t i = 0; i < size_of_Vec; i++) 
            {
                arr[i] = other.arr[i];
            }
        }
        return *this;
    }
    
    ~DynamicArray() 
    {
        delete[] arr;
    }                            

    string* begin() 
    {
        return arr; // Возвращаем указатель на первый элемент
    }

    string* end() 
    {
        return arr + size_of_Vec; // Возвращаем указатель на элемент после последнего
    }

    void MPUSH(string name) 
    {
        if (size_of_Vec == memory_size) 
        {
            string* arr_copy = new string[memory_size * 2];

            for (size_t i = 0; i < size_of_Vec; i++) 
            {
                arr_copy[i] = arr[i];
            }
            delete[] arr;
            memory_size *= 2;
            arr = arr_copy;
        }
        arr[size_of_Vec] = name;
        size_of_Vec++;
    }

    void MPUSH(string name, int index) 
    {
        if (index < 0 || index > size_of_Vec) 
        {
            throw std::out_of_range("Index out of range");
        }

        if (size_of_Vec == memory_size) 
        {
            string* arr_copy = new string[memory_size * 2];

            for (size_t i = 0; i < size_of_Vec; i++) 
            {
                arr_copy[i] = arr[i];
            }
            delete[] arr;
            memory_size *= 2;
            arr = arr_copy;
        }

        for (size_t i = size_of_Vec; i > index; i--) 
        {
            arr[i] = arr[i - 1];
        }

        arr[index] = name;
        size_of_Vec++;
    } 

    void MDEL(int index) 
    {
        if (index < 0 || index >= size_of_Vec) 
        {
            throw out_of_range("Index out of range!!!");
        }

        for (size_t i = index; i < size_of_Vec - 1; i++) 
        {
            arr[i] = arr[i + 1];
        }

        size_of_Vec--;
    }  

    string MGET(int index) const 
    {
        if (index <= size_of_Vec && index >= 0) 
        {
            return arr[index];
        }
        throw out_of_range("Index out of range!!!");
    }  

    void MSET(string name, int index) 
    {
        if (index < 0 || index >= size_of_Vec) 
        {
            throw out_of_range("Index out of range!!!");
        }
        arr[index] = name;  
    }   

    void resize(int new_size) 
    {
        string* arr_copy = new string[new_size];
        for (size_t i = 0; i < size_of_Vec; i++) 
        {
            arr_copy[i] = arr[i];
        }
        delete[] arr;
        arr = arr_copy;
        memory_size = new_size;
    }     

    int size() const 
    {
        return size_of_Vec;
    }  

    int memory_use() const 
    {
        return memory_size;
    }  

    void print() const 
    {
        for (int i = 0; i < size_of_Vec; i++) 
        {
            cout << arr[i] << " ";
        }
        cout << "\n";
    }  

    void load_from_file(const string& filename, const string& name_structure) 
    {
        ifstream file(filename);
        
        if (file.is_open()) 
        {
            string line;
            size_of_Vec = 0;  
            bool structure_found = false;

            while (getline(file, line)) 
            {
                // Проверяем, является ли строка началом нового массива
                if (line.find(name_structure + " : ") != string::npos) 
                {
                    structure_found = true; // Нашли массив с указанным именем
                    size_t pos = line.find(':');
                    string values = line.substr(pos + 1); 
                    stringstream ss(values);
                    string value;

                    // Парсим значения, разделенные запятыми
                    while (getline(ss, value, ',')) 
                    {
                        if (size_of_Vec >= memory_size) 
                        {
                            resize(memory_size * 2); // Увеличиваем размер при необходимости
                        }
                        arr[size_of_Vec++] = value;  
                    }
                    break; 
                }
            }


            file.close();
        } 
        else 
        {
            cerr << "Ошибка открытия файла для чтения.\n";
        }
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
            cerr << "Ошибка открытия файла для чтения.\n";
            return;
        }

        ofstream write_file(filename);
        if (!write_file.is_open()) 
        {
            cerr << "Ошибка открытия файла для записи.\n";
            return;
        }

        for (auto& existing_line : lines) 
        {
            if (existing_line.find(name_structure + " : ") == 0) 
            {
                structure_found = true;

                // Заменяем строку структуры новыми значениями
                write_file << name_structure << " : ";
                for (int i = 0; i < size_of_Vec; ++i) { 
                    if (i > 0) write_file << ", ";
                    write_file << arr[i];
                }
                write_file << endl;
            } 
            else 
            {
                write_file << existing_line << endl;
            }
        }

        // Если структура не найдена, добавляем её
        if (!structure_found) {
            write_file << name_structure << " : ";
            for (int i = 0; i < size_of_Vec; ++i) 
            { 
                if (i > 0) write_file << ", ";
                write_file << arr[i]; 
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

        // Записываем размер массива
        fileOut.write(reinterpret_cast<const char*>(&size_of_Vec), sizeof(size_of_Vec));

        // Записываем строки
        for (int i = 0; i < size_of_Vec; i++)
        {
            size_t length = arr[i].size();
            // Записываем длину строки
            fileOut.write(reinterpret_cast<const char*>(&length), sizeof(length));
            // Записываем строку как массив символов
            fileOut.write(arr[i].c_str(), length);
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

        // Читаем размер массива
        fileIn.read(reinterpret_cast<char*>(&this->size_of_Vec), sizeof(this->size_of_Vec));

        // Выделяем память для массива
        delete[] arr; // На случай, если в массиве уже были данные
        arr = new string[size_of_Vec];
        memory_size = size_of_Vec;

        // Читаем строки
        for (int i = 0; i < size_of_Vec; i++)
        {
            size_t wordLength;
            fileIn.read(reinterpret_cast<char*>(&wordLength), sizeof(wordLength));

            // Используем временный буфер для строки
            string temp(wordLength, '\0'); // Создаем строку с нужной длиной
            fileIn.read(&temp[0], wordLength); // Читаем данные прямо в строку
            arr[i] = temp;
        }

        cout << "Deserialization done!" << endl;
        fileIn.close();
    }

};

int main() 
{
    DynamicArray arr{"check", "for", "work","newword","XDD"};
    arr.serialize("binary/vector.bin");
    
    DynamicArray newArr;
    newArr.deserialize("binary/vector.bin");
    newArr.print();  

    return 0;
}