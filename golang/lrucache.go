// +build !change

package lrucache

import "container/list"

type Cache interface {
	// Get returns value associated with the key.
	//
	// The second value is a bool that is true if the key exists in the cache,
	// and false if not.
	Get(key int) (int, bool)
	// Set updates value associated with the key.
	//
	// If there is no key in the cache new (key, value) pair is created.
	Set(key, value int)
	// Range calls function f on all elements of the cache
	// in increasing access time order.
	//
	// Stops earlier if f returns false.
	Range(f func(key, value int) bool)
	// Clear removes all keys and values from the cache.
	Clear()
}

type pair struct {
	key int
	val int
}

type LRU struct {
	mp map[int]*list.Element
	list *list.List
	cap int
}

func (l *LRU) Get(key int) (int, bool) {
	if val, ok := l.mp[key]; ok && val.Value.(*pair).key == key {
		l.list.MoveToFront(val)
		return l.list.Front().Value.(*pair).val, true
	}
	return 0, false
}

func (l *LRU) Set(key, value int) {
	if val, ok := l.mp[key]; ok {
		l.list.MoveToFront(val)
		l.list.Front().Value.(*pair).key = key
		l.list.Front().Value.(*pair).val = value
	} else {
		if l.list.Len() < l.cap {
			l.mp[key] = l.list.PushFront(&pair{key: key, val: value})
		} else if l.cap > 0 {
			l.list.MoveToFront(l.list.Back())
			l.list.Front().Value.(*pair).key = key
			l.list.Front().Value.(*pair).val = value
			l.mp[key] = l.list.Front()
		}
	}
}

func (l *LRU) Range(f func(key, value int) bool) {
	for e := l.list.Back(); e != nil; e = e.Prev() {
		if !f(e.Value.(*pair).key, e.Value.(*pair).val) {
			break
		}
	}
}

func (l *LRU) Clear() {
	l.mp = make(map[int]*list.Element, l.cap)
	l.list.Init()
}

func New(cap int) Cache {
	return &LRU{mp: make(map[int]*list.Element, cap), list: list.New(), cap: cap}
}
