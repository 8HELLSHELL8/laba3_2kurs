package main

import (
	"fmt"
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

func (q *Queue) printAll() {
	currentNode := q.head
	for currentNode != nil {
		fmt.Printf("%s ", currentNode.content)
		currentNode = currentNode.next
	}
	fmt.Println()
}
