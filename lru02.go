package main

import (
	"fmt"
)

func main() {
	lru := NewLRU(2)

	lru.Set(2, 1)
	lru.Print()

	lru.Set(2, 2)
	lru.Print()

	var v int
	b := lru.Get(1, &v)
	fmt.Printf("b=%v, 1=%d\n", b, v)

	lru.Set(1, 1)
	lru.Print()

	lru.Set(3, 3)
	lru.Print()

	b = lru.Get(1, &v)
	fmt.Printf("b=%v, 1=%d\n", b, v)

	b = lru.Get(2, &v)
	fmt.Printf("b=%v, 2=%d\n", b, v)

	b = lru.Get(3, &v)
	fmt.Printf("b=%v, 3=%d\n", b, v)
}

type LRUNode struct {
	key   int
	value int
	pre   *LRUNode
	next  *LRUNode
}

func NewLRUNode(k, v int) *LRUNode {
	node := &LRUNode{k, v, nil, nil}
	node.pre = node
	node.next = node
	return node
}

func (n *LRUNode) del() {
	n.pre.next = n.next
	n.next.pre = n.pre
}

// 带头结点的头插法
func (n *LRUNode) insert(node *LRUNode) {
	node.pre = n
	node.next = n.next
	n.next.pre = node
	n.next = node
}

////////////////////////////////////

type LRU struct {
	Cap int
	m   map[int]*LRUNode
	*LRUNode
}

func NewLRU(Cap int) *LRU {
	lru := &LRU{
		Cap:     Cap,
		m:       make(map[int]*LRUNode),
		LRUNode: NewLRUNode(0, 0),
	}
	return lru
}

func (lru *LRU) Get(key int, value *int) bool {
	node, ok := lru.m[key]
	if !ok {
		return false
	}

	node.del()
	lru.insert(node)
	*value = node.value

	return true
}

func (lru *LRU) Set(key, value int) {
	node, ok := lru.m[key]
	if ok {
		node.value = value
		node.del()
		lru.insert(node)
		return
	}

	if len(lru.m) == lru.Cap {
		delete(lru.m, lru.pre.key)
		lru.pre.del()
	}

	newNode := NewLRUNode(key, value)
	lru.insert(newNode)
	lru.m[key] = newNode
}

func (lru *LRU) Print() {
	fmt.Printf("size=%d\n", len(lru.m))

	for k, v := range lru.m {
		fmt.Printf("key=%d,value=%d ", k, v.value)
	}
	fmt.Println()

	for next := lru.next; next != lru.pre.next; next = next.next {
		fmt.Printf("key=%d,value=%d ", next.key, next.value)
	}
	fmt.Println()
}
