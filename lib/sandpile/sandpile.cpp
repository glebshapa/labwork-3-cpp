#include "sandpile.h"
#include "../image/image.h"

#include <iostream>
#include <vector>
#include <set>

void Field::ExpandUp() {
	Node* new_start = new Node;
	new_start->down = start;
	start->up = new_start;
	start = new_start;
	height++;
	Node* last = start;
	for (int j = 1; j < width; j++) {
		Node* temp = new Node;
		temp->value = 0;
		temp->left = last;
		temp->left->right = temp;
		temp->down = temp->left->down->right;
		temp->down->up = temp;
		last = temp;
	}
}

void Field::ExpandLeft() {
	Node* new_start = new Node;
	new_start->right = start;
	start->left = new_start;
	start = new_start;
	width++;
	Node* last = start;
	for (int i = 1; i < height; i++) {
		Node* temp = new Node;
		temp->value = 0;
		temp->up = last;
		temp->up->down = temp;
		temp->right = temp->up->right->down;
		temp->right->left = temp;
		last = temp;
	}
}

void Field::ExpandDown() {
	Node* first_temp = new Node;
	Node* up_first_temp = start;
	while (up_first_temp->down != nullptr) {
		up_first_temp = up_first_temp->down;
	}
	first_temp->up = up_first_temp;
	up_first_temp->down = first_temp;
	height++;
	Node* last = first_temp;
	for (int j = 1; j < width; j++) {
		Node* temp = new Node;
		temp->value = 0;
		temp->left = last;
		temp->left->right = temp;
		temp->up = temp->left->up->right;
		temp->up->down = temp;
		last = temp;
	}
}

void Field::ExpandRight() {
	Node* first_temp = new Node;
	Node* left_first_temp = start;
	while (left_first_temp->right != nullptr) {
		left_first_temp = left_first_temp->right;
	}
	first_temp->left = left_first_temp;
	left_first_temp->right = first_temp;
	width++;
	Node* last = first_temp;
	for (int i = 1; i < height; i++) {
		Node* temp = new Node;
		temp->value = 0;
		temp->up = last;
		temp->up->down = temp;
		temp->left = temp->up->left->down;
		temp->left->right = temp;
		last = temp;
	}
}

void Field::Spill(uint64_t max_iter, uint64_t freq, const char* path_output) {
	std::set<Node*> all_spilling_nodes;
	std::vector<Node*> current_spilling_nodes;
	for (Node* temp_i = start; temp_i != nullptr; temp_i = temp_i->down) {
		for (Node* temp_j = temp_i; temp_j != nullptr; temp_j = temp_j->right) {
			if (temp_j->value >= 4) {
				all_spilling_nodes.insert(temp_j);
			}
		}
	}
	uint64_t iter_counter = 0;
	while (iter_counter <= max_iter && !all_spilling_nodes.empty()) {
		for (auto node : all_spilling_nodes) {
			current_spilling_nodes.emplace_back(node);
		}
		for (auto node : current_spilling_nodes) {
			node->value -= 4;
			if (node->value < 4)
				all_spilling_nodes.erase(node);

			if (node->up == nullptr)
				ExpandUp();
			node->up->value++;
			if (node->up->value >= 4)
				all_spilling_nodes.insert(node->up);

			if (node->right == nullptr)
				ExpandRight();
			node->right->value++;
			if (node->right->value >= 4)
				all_spilling_nodes.insert(node->right);

			if (node->down == nullptr)
				ExpandDown();
			node->down->value++;
			if (node->down->value >= 4)
				all_spilling_nodes.insert(node->down);

			if (node->left == nullptr)
				ExpandLeft();
			node->left->value++;
			if (node->left->value >= 4)
				all_spilling_nodes.insert(node->left);
		}
		iter_counter++;
		if (freq != 0 && iter_counter % freq == 0) {
			std::cout << iter_counter << " iterations\n";
			Output(path_output);
		}
		current_spilling_nodes.clear();
	}
}

void Field::Output(const char* path) const {
	const Color WHITE = Color(255, 255, 255);
	const Color GREEN = Color(0, 255, 0);
	const Color PURPLE = Color(150, 0, 255);
	const Color YELLOW = Color(255, 255, 0);
	const Color BLACK = Color(0, 0, 0);
	Image image(width, height);
	int i = 0;
	for (Node* temp_i = start; temp_i != nullptr; temp_i = temp_i->down, i++) {
		int j = 0;
		for (Node* temp_j = temp_i; temp_j != nullptr; temp_j = temp_j->right, j++) {
			if (temp_j->value == 0)
				image.SetColor(WHITE, j, i);
			if (temp_j->value == 1)
				image.SetColor(GREEN, j, i);
			if (temp_j->value == 2)
				image.SetColor(PURPLE, j, i);
			if (temp_j->value == 3)
				image.SetColor(YELLOW, j, i);
			if (temp_j->value > 3)
				image.SetColor(BLACK, j, i);
		}
	}
	image.Export(path);
}