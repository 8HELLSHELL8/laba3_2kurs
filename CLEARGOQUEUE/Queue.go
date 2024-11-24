package main

import (
	"bytes"
	"encoding/gob"
	"fmt"
	"os"
)

type Node struct {
	content string
	next    *Node
}

type Queue struct {
	head *Node
	tail *Node
	size int // Поле для хранения размера очереди
}

func newQueue() *Queue {
	return &Queue{}
}

func (q *Queue) push(value string) {
	newNode := &Node{content: value}
	if q.tail != nil {
		q.tail.next = newNode
	}
	q.tail = newNode
	if q.isEmpty() {
		q.head = newNode
	}
	q.size++
}

func (q *Queue) pop() string {
	if q.isEmpty() {
		fmt.Println("Queue is empty!")
		return ""
	}
	value := q.head.content
	q.head = q.head.next
	if q.head == nil {
		q.tail = nil // Если очередь опустела, сбрасываем tail
	}
	q.size--
	return value
}

func (q *Queue) getSize() int {
	return q.size
}

func (q *Queue) isEmpty() bool {
	return q.head == nil
}

func (q *Queue) front() string {
	if !q.isEmpty() {
		return q.head.content
	} else {
		fmt.Println("Queue is empty!")
		return ""
	}
}

// Сериализация очереди в бинарный файл
func (q *Queue) serializeToFile(filename string) error {
	// Создаем файл для записи
	file, err := os.Create(filename)
	if err != nil {
		return fmt.Errorf("failed to create file: %v", err)
	}
	defer file.Close()

	// Создаем буфер и энкодер
	var buffer bytes.Buffer
	encoder := gob.NewEncoder(&buffer)

	// Составляем массив строк из очереди
	var elements []string
	currentNode := q.head
	for currentNode != nil {
		elements = append(elements, currentNode.content)
		currentNode = currentNode.next
	}

	// Кодируем массив строк в бинарный формат
	if err := encoder.Encode(elements); err != nil {
		return fmt.Errorf("failed to encode queue: %v", err)
	}

	// Записываем бинарные данные в файл
	if _, err := file.Write(buffer.Bytes()); err != nil {
		return fmt.Errorf("failed to write to file: %v", err)
	}

	return nil
}

// Десериализация очереди из бинарного файла
func (q *Queue) deserializeFromFile(filename string) error {
	// Открываем файл для чтения
	file, err := os.Open(filename)
	if err != nil {
		return fmt.Errorf("failed to open file: %v", err)
	}
	defer file.Close()

	// Читаем данные из файла в буфер
	var buffer bytes.Buffer
	if _, err := buffer.ReadFrom(file); err != nil {
		return fmt.Errorf("failed to read from file: %v", err)
	}

	// Декодируем бинарные данные в массив строк
	var elements []string
	decoder := gob.NewDecoder(&buffer)
	if err := decoder.Decode(&elements); err != nil {
		return fmt.Errorf("failed to decode queue: %v", err)
	}

	// Восстанавливаем очередь из массива строк
	*q = *newQueue() // Очистка текущей очереди
	for _, value := range elements {
		q.push(value)
	}

	return nil
}

// Функция для работы с очередью и сериализацией
func processQueue() (*Queue, string, error) {
	queue := newQueue()
	queue.push("10")
	queue.push("20")
	queue.push("30")

	// Сериализация в файл
	filename := "queue.bin"
	err := queue.serializeToFile(filename)
	if err != nil {
		return nil, "", fmt.Errorf("Error serializing queue: %v", err)
	}

	// Десериализация из файла
	newQueue := newQueue()
	err = newQueue.deserializeFromFile(filename)
	if err != nil {
		return nil, "", fmt.Errorf("Error deserializing queue: %v", err)
	}

	return newQueue, filename, nil
}

func (q *Queue) printAll() {
	currentNode := q.head
	for currentNode != nil {
		fmt.Printf("%s ", currentNode.content)
		currentNode = currentNode.next
	}
	fmt.Println()
}

// Сериализация очереди в текстовый файл
func (q *Queue) serializeToTxtFile(filename string) error {
	// Создаем файл для записи
	file, err := os.Create(filename)
	if err != nil {
		return fmt.Errorf("failed to create file: %v", err)
	}
	defer file.Close()

	// Записываем содержимое очереди построчно
	currentNode := q.head
	for currentNode != nil {
		_, err := file.WriteString(currentNode.content + "\n")
		if err != nil {
			return fmt.Errorf("failed to write to file: %v", err)
		}
		currentNode = currentNode.next
	}

	return nil
}

// Десериализация очереди из текстового файла
func (q *Queue) deserializeFromTxtFile(filename string) error {
	// Открываем файл для чтения
	file, err := os.Open(filename)
	if err != nil {
		return fmt.Errorf("failed to open file: %v", err)
	}
	defer file.Close()

	// Очищаем текущую очередь
	*q = *newQueue()

	// Сканируем файл построчно и добавляем элементы в очередь
	var line string
	for {
		_, err := fmt.Fscanf(file, "%s\n", &line)
		if err != nil {
			if err.Error() == "EOF" { // Конец файла
				break
			}
			return fmt.Errorf("failed to read from file: %v", err)
		}
		q.push(line)
	}

	return nil
}

func main() {
	// Создаем очередь и добавляем элементы
	queue := newQueue()
	queue.push("10")
	queue.push("20")
	queue.push("30")

	// Сериализация в текстовый файл
	txtFilename := "queue.txt"
	binFilename := "queue.bin"
	err1 := queue.serializeToTxtFile(txtFilename)
	err2 := queue.serializeToFile(binFilename)
	if err1 != nil {
		fmt.Printf("Error serializing to txt file: %v\n", err1)
		return
	}
	if err2 != nil {
		fmt.Printf("Error serializing to bin file: %v\n", err2)
		return
	}

	fmt.Printf("Queue serialized to txt file: %s\n", txtFilename)

	// Десериализация из текстового файла
	newQueueTxt := newQueue()
	newQueueBin := newQueue()

	err3 := newQueueTxt.deserializeFromTxtFile(txtFilename)
	err4 := newQueueBin.deserializeFromFile(binFilename)
	if err3 != nil {
		fmt.Printf("Error deserializing from txt file: %v\n", err3)
		return
	}
	fmt.Println("Queue deserialized from txt file:")
	newQueueTxt.printAll()
	if err4 != nil {
		fmt.Printf("Error deserializing from bin file: %v\n", err4)
		return
	}

	newQueueBin.printAll()
}
