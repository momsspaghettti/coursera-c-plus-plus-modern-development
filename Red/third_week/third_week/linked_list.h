#pragma once


template <typename T>
class LinkedList
{
public:
	struct Node
	{
		T value;
		Node* next = nullptr;
	};

	LinkedList() = default;

	~LinkedList();

	void PushFront(const T& value);

	void InsertAfter(Node* node, const T& value);

	void RemoveAfter(Node* node);

	void PopFront();

	Node* GetHead();

	const Node* GetHead() const;

private:
	Node* head_ = nullptr;
};


template <typename T>
void LinkedList<T>::PushFront(const T& value)
{
	Node* new_head = new Node;
	new_head->value = value;
	new_head->next = head_;

	head_ = new_head;
}


template <typename T>
void LinkedList<T>::InsertAfter(Node* node, const T& value)
{
    if (node == nullptr)
    {
		PushFront(value);
    }
    else
    {
		Node* insert_node = new Node;
		insert_node->value = value;
		insert_node->next = node->next;
		node->next = insert_node;
    }
}



template <typename T>
void LinkedList<T>::PopFront()
{
    if (head_ == nullptr)
    {
        return;
    }

	Node* tmp = head_;
	head_ = head_->next;
	tmp->next = nullptr;
	delete tmp;
}


template <typename T>
void LinkedList<T>::RemoveAfter(Node* node)
{
    if (node == nullptr)
    {
		PopFront();
    }
    else
    {
        if (node->next == nullptr)
            return;

		Node* tmp = node->next;
		node->next = node->next->next;
		tmp->next = nullptr;
		delete tmp;
    }
}


template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::GetHead()
{
	return head_;
}


template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::GetHead() const
{
	return head_;
}


template <typename T>
LinkedList<T>::~LinkedList()
{
    auto node_to_delete = GetHead();

    if (node_to_delete == nullptr)
    {
		delete node_to_delete;
		return;
    }

	auto iter_node = node_to_delete->next;

    if (iter_node == nullptr)
    {
		delete node_to_delete;
		delete iter_node;
        return;
    }

    while (iter_node != nullptr)
    {
		delete node_to_delete;
		node_to_delete = iter_node;
		iter_node = iter_node->next;
    }

	delete node_to_delete;
	delete iter_node;
}


void TestLinkedList();