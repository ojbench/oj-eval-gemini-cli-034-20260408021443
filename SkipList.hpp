#ifndef SKIP_LIST_HPP
#define SKIP_LIST_HPP

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <climits>

template<typename T>
class SkipList {
private:
    struct Node {
        T* value;
        std::vector<Node*> forward;
        Node(const T& val, int level) : value(new T(val)), forward(level, nullptr) {}
        Node(int level) : value(nullptr), forward(level, nullptr) {}
        ~Node() { delete value; }
    };

    int max_level;
    float p;
    int current_level;
    Node* head;

    int randomLevel() {
        int lvl = 1;
        while (((float)std::rand() / RAND_MAX) < p && lvl < max_level) {
            lvl++;
        }
        return lvl;
    }

public:
    SkipList() : max_level(20), p(0.5), current_level(1) {
        head = new Node(max_level);
        std::srand(std::time(0));
    }

    ~SkipList() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->forward[0];
            delete current;
            current = next;
        }
    }

    // Insert a value into the skip list. If the value already exists, do nothing.
    void insert(const T & item) {
        std::vector<Node*> update(max_level, nullptr);
        Node* current = head;

        for (int i = current_level - 1; i >= 0; i--) {
            while (current->forward[i] != nullptr && *(current->forward[i]->value) < item) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];

        if (current != nullptr && !(*(current->value) < item) && !(item < *(current->value))) {
            // Item already exists
            return;
        }

        int lvl = randomLevel();

        if (lvl > current_level) {
            for (int i = current_level; i < lvl; i++) {
                update[i] = head;
            }
            current_level = lvl;
        }

        Node* newNode = new Node(item, lvl);

        for (int i = 0; i < lvl; i++) {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
    }

    // Search for a value in the skip list
    bool search(const T & item) {
        Node* current = head;

        for (int i = current_level - 1; i >= 0; i--) {
            while (current->forward[i] != nullptr && *(current->forward[i]->value) < item) {
                current = current->forward[i];
            }
        }

        current = current->forward[0];

        if (current != nullptr && !(*(current->value) < item) && !(item < *(current->value))) {
            return true;
        }

        return false;
    }

    // Delete a value from the skip list. If the value does not exist in the skip list, do nothing.
    void deleteItem(const T & item) {
        std::vector<Node*> update(max_level, nullptr);
        Node* current = head;

        for (int i = current_level - 1; i >= 0; i--) {
            while (current->forward[i] != nullptr && *(current->forward[i]->value) < item) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];

        if (current != nullptr && !(*(current->value) < item) && !(item < *(current->value))) {
            for (int i = 0; i < current_level; i++) {
                if (update[i]->forward[i] != current) {
                    break;
                }
                update[i]->forward[i] = current->forward[i];
            }

            delete current;

            while (current_level > 1 && head->forward[current_level - 1] == nullptr) {
                current_level--;
            }
        }
    }
};

#endif
