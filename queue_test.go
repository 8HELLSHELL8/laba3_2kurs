package main

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestQueueOperations(t *testing.T) {
	queue := newQueue()

	// Проверяем начальное состояние очереди
	assert.True(t, queue.isEmpty(), "Queue should be empty initially")
	assert.Equal(t, 0, queue.getSize(), "Queue size should be 0 initially")
	assert.Equal(t, "", queue.front(), "Front should return empty string on empty queue")

	// Добавляем элементы в очередь
	queue.push("10")
	queue.push("20")
	queue.push("30")

	assert.False(t, queue.isEmpty(), "Queue should not be empty after adding elements")
	assert.Equal(t, 3, queue.getSize(), "Queue size should be 3 after adding 3 elements")
	assert.Equal(t, "10", queue.front(), "Front should return '10' as the first element")

	// Удаляем элементы из очереди
	assert.Equal(t, "10", queue.pop(), "Pop should return '10'")
	assert.Equal(t, 2, queue.getSize(), "Queue size should be 2 after popping 1 element")
	assert.Equal(t, "20", queue.front(), "Front should return '20' after popping '10'")

	// Удаляем оставшиеся элементы
	assert.Equal(t, "20", queue.pop(), "Pop should return '20'")
	assert.Equal(t, "30", queue.pop(), "Pop should return '30'")
	assert.True(t, queue.isEmpty(), "Queue should be empty after removing all elements")

	// Попытка удалить из пустой очереди
	assert.Equal(t, "", queue.pop(), "Pop on empty queue should return empty string")
}

func TestPrintAll(t *testing.T) {
	queue := newQueue()
	queue.push("10")
	queue.push("20")
	queue.push("30")

	queue.printAll()
}
