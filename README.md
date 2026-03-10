# Contact List with BST-Based TreeMap

A contact management system implemented in C++ using a self-balancing Binary Search Tree as the underlying data structure.

## Overview

This project implements a phone book / contact list application backed by a custom TreeMap built on top of a Binary Search Tree. Contacts are stored and retrieved efficiently, and can be iterated in alphabetical order.

## Data Structures Implemented from Scratch

- `BinarySearchTree<T>` - Generic BST with height and subtree size tracking
- `TreeMap<K, V>` - Key-value map built on top of BST
- `Node<T>` - BST node with height and subtree size fields

## System Components

- **ContactInfo** - Stores phones, email, company, and notes for a contact
- **ContactList** - Manages contacts using TreeMap; supports add, delete, update, search
- **TreeMap** - Provides put, get, delete, ceiling/floor, first/last entry operations
- **KeyValuePair** - Generic key-value pair used as BST elements

## How to Run
```bash
g++ -o contacts main.cpp ContactList.cpp
./contacts
```

## Key Concepts

- Binary Search Tree implementation
- Generic template-based data structures in C++
- Alphabetical iteration using ceiling/floor operations
- Key-value map design pattern
- Custom exception handling

## Tech Stack

Language: C++ | University: METU
