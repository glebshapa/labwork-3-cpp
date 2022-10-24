#pragma once

#include <iostream>
#include <vector>
#include <set>

struct Node {
	uint64_t value{};
	Node *up = nullptr;
	Node *right = nullptr;
	Node *down = nullptr;
	Node *left = nullptr;
};

struct Field {
	Node *start = new Node;
	int width, height;

	Field(std::vector<uint64_t> matrix, int height, int width) : height(height), width(width) {
		start->value = matrix[0];
		Node* last = start;
		for (int i = 1; i < height; i++) {
			Node* temp = new Node;
			temp->value = matrix[i * height];
			temp->up = last;
			temp->up->down = temp;
			last = temp;
		}
		last = start;
		for (int j = 1; j < width; j++) {
			Node* temp = new Node;
			temp->value = matrix[j];
			temp->left = last;
			temp->left->right = temp;
			last = temp;
		}
		if (height > 1 && width > 1) {
			Node* last_i = start->down;
			for (int i = 1; i < height; i++) {
				last = last_i;
				for (int j = 1; j < width; j++) {
					Node* temp = new Node;
					temp->value = matrix[i * height + j];
					temp->left = last;
					temp->up = temp->left->up->right;
					temp->left->right = temp;
					temp->up->down = temp;
					last = temp;
				}
				last_i = last_i->down;
			}
		}
	}

	~Field();

	void ExpandUp();

	void ExpandLeft();

	void ExpandDown();

	void ExpandRight();

	void Spill(std::set<Node*>& all_spilling_nodes, std::vector<Node*>& current_spilling_nodes);

	void Output(const std::string& path) const;
};