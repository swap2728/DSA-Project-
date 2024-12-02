#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Node for the linked list

string file = "history.txt";

struct Node
{
    int value;
    Node *next;
    Node(int val) : value(val), next(nullptr) {}
};

// Root class to handle input and linked list creation
class Root
{
public:
    Node *head;

    Root(Node *root)
    {
        head = root;
    }

    // Constructor to create linked list from string
    Root(const string &num) : head(nullptr)
    {
        Node *tail = nullptr;

        for (char c : num)
        {
            Node *newNode = new Node(c - '0');
            if (!head)
            {
                head = tail = newNode;
            }
            else
            {
                tail->next = newNode;
                tail = newNode;
            }
        }
    }

    // Print the linked list
    void printList() const
    {
        Node *current = head;
        while (current)
        {
            cout << current->value;
            current = current->next;
        }
        cout << endl;
    }

    void reverseList()
    {
        // Node*& head
        Node *prev = nullptr; // To keep track of the previous node
        Node *curr = head;    // The current node being processed
        Node *next = nullptr; // To keep track of the next node

        // Traverse the linked list and reverse the direction of the next pointers
        while (curr != nullptr)
        {
            next = curr->next; // Save the next node
            curr->next = prev; // Reverse the direction of the current node
            prev = curr;       // Move the previous node to the current node
            curr = next;       // Move to the next node in the original list
        }

        head = prev; // After the loop, prev will point to the new head of the reversed list
    }
};

// Abstract base class
class Calculator
{
public:
};

class A : public Calculator
{
public:
    virtual void add(double num1, double num2) = 0;
    virtual void subtract(double num1, double num2) = 0;
    virtual void multiply(double num1, double num2) = 0;
    virtual void divide(double num1, double num2) = 0;
};

class B : public Calculator
{
public:
    virtual Node* add(const Root &num1, const Root &num2,int cnt=0) = 0;
    virtual Node* subtract(const Root &num1, const Root &num2,int cnt=0) = 0;
    virtual Node* multiply(const Root &num1, const Root &num2,int cnt=0) = 0;
    virtual void divide(const Root &num1, const Root &num2) = 0;
};

// BasicCalculator implementation (for simple numeric operations)
class BasicCalculator : public A
{
public:
    void add(double a, double b) override
    {
        cout << "Addition: " << (a + b) << endl;
    }

    void subtract(double a, double b) override
    {
        cout << "Subtraction: " << (a - b) << endl;
    }

    void multiply(double a, double b) override
    {
        cout << "Multiplication: " << (a * b) << endl;
    }

    void divide(double a, double b) override
    {
        if (b != 0)
            cout << "Division: " << (a / b) << endl;
        else
            cout << "Error: Division by zero!" << endl;
    }
};

// SuperCalculator class for linked list-based arithmetic
class SuperCalculator : public B
{
public:
    Node* add(const Root &num1, const Root &num2 , int cnt=0) override
    {
        Root A(num1.head);
        Root B(num2.head);
        // Root* node2 = new Root(num2.head);
        A.reverseList();
        B.reverseList();
        Node *node1 = A.head;
        Node *node2 = B.head;
        // Root C(nullptr) ;
        Node *result = nullptr;
        Node *tail = nullptr;
        int carry = 0;

        // Traverse both lists
        while (node1 || node2 || carry)
        {
            int sum = carry; // Start with carry

            if (node1)
            {
                sum += node1->value; // Add value of first list node if available
                node1 = node1->next; // Move to next node in the first list
            }

            if (node2)
            {
                sum += node2->value; // Add value of second list node if available
                node2 = node2->next; // Move to next node in the second list
            }

            // Calculate new digit and carry
            carry = sum / 10;
            int digit = sum % 10;

            // Create new node with the resulting digit
            Node *newNode = new Node(digit);
            if (!result)
            {
                result = tail = newNode; // Initialize result list with the first node
            }
            else
            {
                tail->next = newNode; // Add node to the end of result list
                tail = newNode;       // Move tail pointer
            }
        }
        A.reverseList();
        B.reverseList();
        Root C(result);
        C.reverseList();


        if(cnt){
             ofstream obj;

        obj.open(file,ios::app);

        obj<<"Addition Result : ";
        Node* fileptr = C.head;
        while(fileptr){
            obj<<fileptr->value<<" ";
            fileptr=fileptr->next;
        }
        obj<<endl;
        obj.close();


        cout << "Addition Result: ";
        printList(C.head);
        }

        
        // printList(C.head); // Helper function to print the linked list

        

        return C.head;
    }

    Node *reverse(Node *head)
    {
        if (head == NULL || head->next == NULL)
        {
            return head;
        }
        Node *curr = head;
        Node *prev = NULL;
        while (curr != NULL)
        {
            Node *next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        return prev;
    }

    Node* subtract(const Root &num1, const Root &num2,int cnt=0) override
    {
        Node *head1 = num1.head;
        Node *head2 = num2.head;
        // If head2 is NULL, return head1 as is.
        if (head2 == NULL)
        {
            printList(head1);
        }

        // If head1 is NULL, return head2 but with a negation in its value.
        if (head1 == NULL)
        {
            head2->value = -head2->value;
            printList(head2);
        }

        // Reverse both linked lists to start from the least significant digit.
        head1 = reverse(head1);
        head2 = reverse(head2);

        // Dummy node for the result linked list.
        Node *head = new Node(-1);
        Node *itr = head;

        Node *c1 = head1;
        Node *c2 = head2;
        int borrow = 0;

        // Subtract digits one by one.
        while (c1 != NULL)
        {
            int diff = borrow + c1->value - (c2 != NULL ? c2->value : 0);

            // Handle the borrow if the diff is negative.
            if (diff < 0)
            {
                borrow = -1;
                diff += 10;
            }
            else
            {
                borrow = 0;
            }

            // Add the current diff to the result linked list.
            itr->next = new Node(diff);
            itr = itr->next;
            c1 = c1->next;
            if (c2 != NULL)
            {
                c2 = c2->next;
            }
        }

        // Reverse the resulting linked list to get the correct order.
        Node *result = reverse(head->next);

        // Remove leading zeros from the result.
        while (result != NULL && result->value == 0)
        {
            result = result->next;
        }

        // If the result is empty (all digits were zero), return a single node with 0.
        if (result == NULL)
        {
            printList(new Node(0));
        }
        
        

        if(cnt){
             ofstream obj;

        obj.open(file,ios::app);

        obj<<"subtraction Result : ";
        Node* fileptr = result;
        while(fileptr){
            obj<<fileptr->value<<" ";
            fileptr=fileptr->next;
        }
        obj<<endl;
        obj.close();


        cout << "subtraction Result: ";
        printList(result);
        }

        return result;
    }

    Node* multiply(const Root &num1, const Root &num2 , int cnt=0) override
    {
        struct Node *result(0) ;
        printList(num1.head);
        Node* first = num1.head;
        Node* second = num2.head;
        first=reverse(first);
        second=reverse(second);
        
        struct Node *second_ptr = second,
                    *result_ptr1 = result, *first_ptr;
        int q = 0;
        while (second_ptr)
        {

            int carry = 0;

            Node* temp=new Node(0);
            Node* result_ptr2=temp;
            for(int i=0;i<q;i++){
                Node* p=new Node(0);
                result_ptr2->next=p;
                // if(i==cnt-1)break;
                result_ptr2=result_ptr2->next;
            }
            // Node* p=new Node(0);
            // result_ptr2->next=p;
            first_ptr = first;

            while (first_ptr)
            {    
                // cout<<first_ptr->value<<" "<<second_ptr->value<<endl;

                int mul = (first_ptr->value * second_ptr->value) + carry;
                Node* p = new Node(mul%10);
                result_ptr2->next=p;
                carry = (mul / 10);
                first_ptr = first_ptr->next;
                result_ptr2 = result_ptr2->next;
                // cnt++;
                
            }
            if (carry > 0)
            {   
                Node* p=new Node(carry);
                result_ptr2->next = p;
                result_ptr2 = result_ptr2->next;
            }
            temp=reverse(temp->next);
            // printList(temp);
            second_ptr=second_ptr->next;
            q++;
            result = add(result,temp);
        }

        if(cnt){
             ofstream obj;

        obj.open(file,ios::app);

        obj<<"multiply Result : ";
        Node* fileptr = result;
        while(fileptr){
            obj<<fileptr->value<<" ";
            fileptr=fileptr->next;
        }
        obj<<endl;
        obj.close();
        cout<<"multiply Result : ";
        printList(result);
        }

        
        return result;
    }

    void divide(const Root &num1, const Root &num2) override
    {
        int num1Value = listToNumber(num1.head);
        int num2Value = listToNumber(num2.head);

        if (num2Value == 0)
        {
            cout << "Error: Division by zero!" << endl;
            return;
        }

        int quotient = num1Value / num2Value;
        cout << "Division Result: ";
        printNumberAsList(quotient);
    }

private:
    void printList(Node *head) const
    {
        while (head)
        {
            cout << head->value;
            head = head->next;
        }
        cout << endl;
    }

    int listToNumber(Node *head) const
    {
        int num = 0;
        while (head)
        {
            num = num * 10 + head->value;
            head = head->next;
        }
        return num;
    }

    void printNumberAsList(int num) const
    {
        if (num == 0)
        {
            cout << "0" << endl;
            return;
        }

        Node *result = nullptr;
        Node *tail = nullptr;

        while (num)
        {
            int digit = num % 10;
            Node *newNode = new Node(digit);
            if (!result)
            {
                result = tail = newNode;
            }
            else
            {
                newNode->next = result;
                result = newNode;
            }
            num /= 10;
        }

        printList(result);
    }
};

void showHistory(){
    fstream obj;
    obj.open(file);
    while(obj){
        string p;
        getline(obj,p);
        cout<<p<<endl;
    }
    obj.close();
}

int main()
{
    // Input numbers as strings
    string strNum1, strNum2;
    cout << "Enter first number: ";
    cin >> strNum1;
    cout << "Enter second number: ";
    cin >> strNum2;

    // Create Root objects
    Root root1(strNum1);
    Root root2(strNum2);

    // Print linked lists
    cout << "First Number: ";
    root1.printList();
    cout << "Second Number: ";
    root2.printList();

    // Perform operations using SuperCalculator
    B *calc = new SuperCalculator(); // Use a pointer to the base class
    calc->add(root1, root2 , 1);       // Correct method call
    // calc->subtract(root1, root2);    // Correct method call
    calc->multiply(root1, root2,1);    // Correct method call
    // calc->divide(root1, root2);      // Correct method call
    calc->subtract(root1, root2,1);

    showHistory();

    // Clean up memory
    delete calc;

    return 0;
}
