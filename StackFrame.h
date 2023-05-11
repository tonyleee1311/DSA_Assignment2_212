#ifndef __STACK_FRAME_H__
#define __STACK_FRAME_H__

#include <string>
#include <stack>

/*
StackFrame declaration
*/
class StackFrame {
    int opStackMaxSize; // max size of operand stack
    int localVarSpaceSize; // size of local variable array
public:
    class AVL; //forward declaration
    class Node; //forward declaration
private:
    std::stack<float> stack;
    Node* root=NULL;
    int avl_size=0;
    int code_error=0;
    int line = 0;

    /*
    Run the method written in the testcase
    @param filename name of the file
    */
public:
    /*
    Constructor of StackFrame
    */
    StackFrame();
    void run(std::string filename);
    void implementation(std::string, float, std::string);
    void catch_error();
    int get_height(Node* root);
    int balance_tree(Node* root);
    int compare(std::string a, std::string root);
    Node* left_rotation(Node* root) {
        Node* new_root = root->right;
        Node* new_root_leaf = new_root->left;
        //start rotation
        new_root->left = root;
        root->right = new_root_leaf;
        return new_root;
    }
    Node* right_rotation(Node* root) {
        Node* new_root = root->left;
        Node* new_root_leaf = root->right;
        //start rotation
        new_root->right = root;
        root->left = new_root_leaf;
        return new_root;
    }
    void traverse_tree(Node* root, Node* &is_find, std::string key);
    void searching(Node* root, std::string key, bool& ans);
    Node* insert_tree(Node* root, float data, int code, std::string key) {

        if (root == NULL) {
            root = new Node(data, code, key);
            avl_size+=2;
            return root;
        }
        else {
            int check = compare(key, root->key); //compare two strings
             //insert to the left
            if (check == 1) {
                root->left = insert_tree(root->left, data, code, key);
            }
            else if (check == 2) {
                root->right = insert_tree(root->right, data, code, key);
            }
            else if (check == 0) {
                root->data = data;
                root->code = code;
                return root;
            }
        }
        // Check if the tree is AVL tree, if not, rotate the tree
        int balance = balance_tree(root);
        if (balance >= 2) {
            //imbalance on the left
            int new_balance = balance_tree(root->left);
            //LL case
            if (new_balance==1)
                return right_rotation(root);
            //LR case
            else{
                root->left = left_rotation(root->left);
                return right_rotation(root);
            }
        }
        else if (balance <= -2) {
            //imbalance on the right
            int new_balance = balance_tree(root->right);
            //RR case
            if (new_balance == -1)
                return left_rotation(root);
            //RL case
            else{
                root->right = right_rotation(root->right);
                return left_rotation(root);
            }
        }
        return root;
    }
    Node* Min_Right(Node* root) {
        Node* tmp = root;
        while (tmp->left != NULL) {
            tmp = tmp->left;
        }
        return tmp;
    }
    Node* remove_tree(Node* root, std::string key) {
        if (root == NULL)
             return root;
        int check = compare(key, root->key);
        if (check == 1)
            root->left = remove_tree(root->left, key);
        else if (check == 2)
            root->right = remove_tree(root->right, key);
        else {
            // case one: No child
            if (root->left == NULL && root->right == NULL) {
                delete root;
                root = NULL;
                return root;
            }
            // case two: One child
            if (root->left == NULL) {
                Node* tmp = root;
                root = root->right;
                delete tmp;
                return root;
            }
            else if (root->right == NULL) {
                Node* tmp = root;
                root = root->left;
                delete tmp;
                return root;
            }
            // case three: Two childrem
            else {
                Node* tmp = Min_Right(root->right);
                root->data = tmp->data;
                root->code = tmp->code;
                root->key = tmp->key;
                root->right = remove_tree(root->right, tmp->key);
                return root;
            }
        }
        //Check if the tree balance or not
        int balance = balance_tree(root);
        if (balance >= 2) {
            //imbalance on the left
            balance = balance_tree(root->left);
            if (balance >= 1) {
                //LL case
                return right_rotation(root);
            }
            else if (balance <= -1) {
                // LR case
                root->left = left_rotation(root->left);
                return right_rotation(root);
            }
        }
        else if (balance <= -2) {
            balance = balance_tree(root->right);
            if (balance >= 1) {
                //RR case
                return left_rotation(root);
            }
            else if (balance <= -1) {
                // RL case
                root->right = right_rotation(root->right);
                return left_rotation(root);
            }
        }
        return root;
    }
    void clear_stack();
    void clear_avl();
    void insert(float data, int code, std::string key);
    bool search(std::string key);
    float get_data(std::string key);
    int get_code(std::string key);
    void iadd();
    void fadd();
    void isub();
    void fsub();
    void imul();
    void fmul();
    void idiv();
    void fdiv();
    void irem();
    void ineg();
    void fneg();
    void iand();
    void ior();
    void ieq();
    void feq();
    void ineq();
    void fneq();
    void ilt();
    void flt();
    void igt();
    void fgt();
    void ibnot();
    void iconst(float );
    void fconst(float );
    void iload(std::string );
    void fload(std::string );
    void istore(std::string );
    void fstore(std::string );
    void i2f();
    void f2i();
    void top();
    void val(std::string );
    void find_par(Node* root, std::string key, bool& is_find);
    void par(std:: string );
    public:
        class Node {
            friend class StackFrame;
        private:
            float data;
            int code;
            std::string key;
            Node* left;
            Node* right;
        public:
            Node(float data, int code, std::string key) : data(data), code(code), key(key), left(NULL), right(NULL) {}
        };
};

#endif // !__STACK_FRAME_H__#pragma once
