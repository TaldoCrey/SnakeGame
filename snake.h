#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;

struct Node {
    sf::Vector2f position;
    Node* next;
};

bool coliding(sf::Vector2f p1, sf::Vector2f p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

class Snake {
    private:
        int size;
        Node* head;
        Node* tail;
    
    public:
        Snake(sf::Vector2f start_position) {
            this->size = 1;
            Node* first = new Node;
            first->position = start_position;
            first->next = NULL;
            this->head = first;
            this->tail = first;
        }

        void increase_size(sf::Vector2f new_body_pos) {
            this->size += 1;
            Node* body = new Node;
            body->position = new_body_pos;
            body->next = this->head;
            this->head = body;
        }

        bool is_safe() {
            if (this->size > 3) {
                Node* current = this->head->next;
                while (current->next != NULL) {
                    if (coliding(this->head->position, current->position)) {
                        return false;
                    } else {
                        current = current->next;
                        continue;
                    }
                }
                return true;
            } else {
                return true;
            }
        }

        void move(sf::Vector2f new_position) {
            sf::Vector2f new_pos = new_position;
            Node* current = this->head;
            sf::Vector2f aux_pos;
            while (current != NULL) {
                aux_pos = current->position;
                current->position = new_pos;
                new_pos = aux_pos;
                current = current->next;
            }
        }

        Node* get_head() {
            return this->head;
        }

        bool is_food_safe(sf::Vector2f food_pos) {
            Node* current = this->head;
            while (current != NULL) {
                if (coliding(food_pos, current->position)) {
                    return false;
                } else {
                    current = current->next;
                    continue;
                }
            }
            return true;
        }

        int get_size() {
            return size;
        }

        void reset() {
            while (this->head != this->tail) {
                Node* aux = this->head;
                this->head = aux->next;
                delete aux;
                this->size--;
            }
            this->head->position = sf::Vector2f(50.0f, 50.0f);
            this->tail->position = sf::Vector2f(50.0f, 50.0f);
        }
};