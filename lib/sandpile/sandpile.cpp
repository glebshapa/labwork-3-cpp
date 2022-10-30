#include "sandpile.h"
#include "../image/image.h"

#include <iostream>
#include <vector>
#include <set>

Field::~Field() {
	Node* temp_i = start;
	while (temp_i != nullptr) {
		Node* temp2_i = temp_i->down;
		for (Node* temp_j = temp_i->right; temp_j != nullptr; temp_j = temp_j->right) {
			delete temp_j->left;
			if (temp_j->right == nullptr) {
				delete temp_j;
				break;
			}
		}
		temp_i = temp2_i;
	}
};

void Field::InitSpilling() {
	for (Node* temp_i = start; temp_i != nullptr; temp_i = temp_i->down) {
		for (Node* temp_j = temp_i; temp_j != nullptr; temp_j = temp_j->right) {
			if (temp_j->value >= 4) {
				all_spilling_nodes.insert(temp_j);
			}
		}
	}
}

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

void Field::Spill() {
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
	current_spilling_nodes.clear();
}

void Field::Output(const std::string& path) const {
	Image image(width, height);
	int i = 0;
	for (Node* temp_i = start; temp_i != nullptr; temp_i = temp_i->down, i++) {
		int j = 0;
		for (Node* temp_j = temp_i; temp_j != nullptr; temp_j = temp_j->right, j++) {
			if (temp_j->value > 3)
				image.SetColor(4, j, i);
			else
				image.SetColor(static_cast<uint8_t>(temp_j->value), j, i);
		}
	}
	image.Export(path);
}