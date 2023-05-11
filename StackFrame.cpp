#include "StackFrame.h"
#include <iostream>
#include <fstream>
#include "errors.h"
#include "constants.h"
using namespace std;

StackFrame::StackFrame() : opStackMaxSize(OPERAND_STACK_MAX_SIZE), localVarSpaceSize(LOCAL_VARIABLE_SPACE_SIZE) {}

void StackFrame::run(string filename) {
    ifstream iFs;
    iFs.open(filename);
    string s; // storing the input of a line;
    while (!iFs.eof()) {
        line++;
        getline(iFs, s);
        int n = s.length();
        int i = 0;
        string re;
        for (; i < n; i++) {
            if (s[i] == ' ')
                break;
            re += s[i];
        }
        float data = 0;
        string data_string;
        s.erase(0, size_t(i + 1));
        n = s.length();
        if (n != 0) {
            if (re == "iconst" || re == "fconst") {
                data = stof(s);
                data_string = '0';
            }
            else {
                data_string = s;
                data = 0;
            }
        }
        implementation(re, data, data_string);
        catch_error();
    }
    iFs.close();
}

void StackFrame::implementation(string re, float data, string data_string) {
    if (re == "iadd") iadd();
    else if (re == "fadd") fadd();
    else if (re == "isub") isub();
    else if (re == "fsub") fsub();
    else if (re == "imul") imul();
    else if (re == "fmul") fmul();
    else if (re == "idiv") idiv();
    else if (re == "fdiv") fdiv();
    else if (re == "irem") irem();
    else if (re == "ineg") ineg();
    else if (re == "fneg") fneg();
    else if (re == "iand") iand();
    else if (re == "ior")  ior();
    else if (re == "ieq")  ieq();
    else if (re == "feq")  feq();
    else if (re == "ineq") ineq();
    else if (re == "fneq") fneq();
    else if (re == "ilt")  ilt();
    else if (re == "flt")  flt();
    else if (re == "igt")  igt();
    else if (re == "fgt")  fgt();
    else if (re == "ibnot")  ibnot();
    else if (re == "iconst") iconst(data);
    else if (re == "fconst") fconst(data);
    else if (re == "iload")  iload(data_string);
    else if (re == "fload")  fload(data_string);
    else if (re == "istore") istore(data_string);
    else if (re == "fstore") fstore(data_string);
    else if (re == "i2f")  i2f();
    else if (re == "f2i")  f2i();
    else if (re == "top")  top();
    else if (re == "val")  val(data_string);
    else if (re == "par")  par(data_string);
}


void StackFrame::catch_error() {
    if (code_error == 0) return;
    else {
        clear_stack();
        clear_avl();
        if (code_error == 1) throw TypeMisMatch(line);
        else if (code_error == 2) throw DivideByZero(line);
        else if (code_error == 3) throw StackFull(line);
        else if (code_error == 4) throw StackEmpty(line);
        else if (code_error == 5) throw LocalSpaceFull(line);
        else throw UndefinedVariable(line);
    }
    
}

int StackFrame::get_height(Node* root) {
    if (root == NULL)
        return -1;
    int l_height = get_height(root->left);
    int r_height = get_height(root->right);
    return (l_height > r_height) ? l_height + 1 : r_height + 1;
}

int StackFrame::balance_tree(Node* root) {
    if (root == NULL)
        return 0;
    return get_height(root->left) - get_height(root->right);
}

int StackFrame::compare(std::string a, std::string root) {
    if (a == root)
        return 0;
    int m = a.length();
    int i = 0;
    int ans=0;
    while (i < m) {
        if (a[i] < root[i] || a[i] == '\0') {
            ans = 1; //string a is smaller than string root
            break;
        }
        else if (a[i] > root[i] || root[i] == '\0') {
            ans = 2; //string a is larger than string root
            break;
        }
        i++;
    }
    return ans;
}

void StackFrame::traverse_tree(Node* root, Node* &is_find, std::string key) {
    Node* tmp = root;
    if (tmp == NULL)
        return;
    if (tmp->key == key) {
        is_find = root;
        return;
    }
    traverse_tree(tmp->left, is_find, key);
    traverse_tree(tmp->right, is_find, key);
}

void StackFrame::searching(Node* root, std::string key, bool& ans) {
    Node* tmp = root;
    if (tmp == NULL)
        return;
    if (tmp->key == key) {
        ans = 1;
        return;
    }
    searching(tmp->left, key, ans);
    searching(tmp->right, key, ans);
}

void StackFrame::clear_stack(){
    while (stack.empty() != 1) {
        stack.pop();
    }
}

void StackFrame::clear_avl(){
    while (root != NULL) {
        root = remove_tree(root, root->key);
    }
}

void StackFrame::insert(float data, int code, std::string key) {
    root = insert_tree(root, data, code, key);
}
bool StackFrame::search(std::string key) {
    bool ans = 0;
    searching(root, key, ans);
    return ans;
}
float StackFrame::get_data(std::string key) {
    Node* is_find = new Node(0, 0, "\0");
    traverse_tree(root, is_find, key);
    return is_find->data;
}
int StackFrame::get_code(std::string key) {
    Node* is_find = new Node(0, 0, "\0");
    traverse_tree(root, is_find, key);
    return is_find->code;
}
void StackFrame::iadd() {
    if (stack.size() < 4) {
        code_error = 4;
        return;
    }
    float num1 = stack.top();
    stack.pop();
    int code_num1 = int(stack.top());
    stack.pop();
    float num2 = stack.top();
    stack.pop();
    int code_num2 = int(stack.top());
    stack.pop();
    if (code_num1 == 0 && code_num2 == 0) {
        stack.push(0);
        stack.push(float(int(num1 + num2)));
    }
    else {
        code_error = 1;
        return;
    }
}

void StackFrame::fadd() {
    if (stack.size() < 4) {
        code_error = 4;
        return;
    }
    float num1 = stack.top();
    stack.pop();
    stack.pop();
    float num2 = stack.top();
    stack.pop();
    stack.pop();
    stack.push(1);
    stack.push(num1 + num2);

}

void StackFrame::isub() {
    if (stack.size() < 4) {
        code_error = 4;
        return;
    }
    float num1 = stack.top();
    stack.pop();
    int code_num1 = int(stack.top());
    stack.pop();
    float num2 = stack.top();
    stack.pop();
    int code_num2 = int(stack.top());
    stack.pop();
    if (code_num1 == 0 && code_num2 == 0) {
        stack.push(0);
        stack.push(float(int(num2 - num1)));
    }
    else {
        code_error = 1;
        return;
    }
}

void StackFrame::fsub() {
    if (stack.size() < 4) {
        code_error = 4;
        return;
    }
    float num1 = stack.top();
    stack.pop();
    stack.pop();
    float num2 = stack.top();
    stack.pop();
    stack.pop();
    stack.push(1);
    stack.push(num2 - num1);
}

void StackFrame::imul() {
    if (stack.size() < 4) {
        code_error = 4;
        return;
    }
    float num1 = stack.top();
    stack.pop();
    int code_num1 = int(stack.top());
    stack.pop();
    float num2 = stack.top();
    stack.pop();
    int code_num2 = int(stack.top());
    stack.pop();
    if (code_num1 == 0 && code_num2 == 0) {
        stack.push(0);
        stack.push(float(int(num1 * num2)));
    }
    else {
        code_error = 1;
        return;
    }
}

void StackFrame::fmul() {
    if (stack.size() < 4) {
        code_error = 4;
        return;
    }
    float num1 = stack.top();
    stack.pop();
    stack.pop();
    float num2 = stack.top();
    stack.pop();
    stack.pop();
    stack.push(1);
    stack.push(num1 * num2);

}

void StackFrame::idiv() {
    if (stack.size() < 4) {
        code_error = 4;
        return;
    }
    float num1 = stack.top();
    stack.pop();
    int code_num1 = int(stack.top());
    stack.pop();
    float num2 = stack.top();
    stack.pop();
    int code_num2 = int(stack.top());
    stack.pop();
    if (code_num1 == 0 && code_num2 == 0) {
        if (num1 == 0) {
            code_error = 2;
            return;
        }
        stack.push(0);
        stack.push(float(int(num2 / num1)));
    }
    else {
        code_error = 1;
        return;
    }
}

void StackFrame::fdiv() {
    if (stack.size() < 4) {
        code_error = 4;
        return;
    }
    float num1 = stack.top();
    stack.pop();
    stack.pop();
    float num2 = stack.top();
    if (num1 == 0) {
        code_error = 2;
        return;
    }
    stack.pop();
    stack.pop();
    stack.push(1);
    stack.push(num2 / num1);
}

void StackFrame::irem() {
    if (stack.size() < 4) {
        code_error = 4;
        return;
    }
    float num1 = stack.top();
    stack.pop();
    int code_num1 = int(stack.top());
    stack.pop();
    float num2 = stack.top();
    stack.pop();
    int code_num2 = int(stack.top());
    stack.pop();
    if (code_num1 == 0 && code_num2 == 0) {
        if (num1 == 0) {
            code_error = 2;
            return;
        }
        num2 = float(int(num2) - int((num2 / num1)) * int(num1));
        stack.push(0);
        stack.push(num2);
    }
    else {
        code_error = 1;
        return;
    }
}

void StackFrame::ineg() {
    if (stack.size() < 2) {
        code_error = 4;
        return;
    }
    float num = stack.top();
    stack.pop();
    int code = int(stack.top());
    stack.pop();
    if (code == 0) {
        if (num == 0) {
            stack.push(0);
            stack.push(num);
        }
        else {
            stack.push(0);
            stack.push(-num);
        }
    }
    else {
        code_error = 1;
        return;
    }
}

void StackFrame::fneg() {
    if (stack.size() < 2) {
        code_error = 4;
        return;
    }
    float num = stack.top();
    stack.pop();
    stack.pop();
    stack.push(1);
    stack.push(-num);
}

void StackFrame::iand() {
    if (stack.size() < 4) {
        code_error = 4;
        return;
    }
    float num1 = stack.top();
    stack.pop();
    int code_num1 = int(stack.top());
    stack.pop();
    float num2 = stack.top();
    stack.pop();
    int code_num2 = int(stack.top());
    stack.pop();
    if (code_num1 == 0 && code_num2 == 0) {
        num1 = float((int(num1) & int(num2)));
        stack.push(0);
        stack.push(num1);
    }
    else {
        code_error = 1;
        return;
    }
}

void StackFrame::ior() {
    if (stack.size() < 4) {
        code_error = 4;
        return;
    }
    float num1 = stack.top();
    stack.pop();
    int code_num1 = int(stack.top());
    stack.pop();
    float num2 = stack.top();
    stack.pop();
    int code_num2 = int(stack.top());
    stack.pop();
    if (code_num1 == 0 && code_num2 == 0) {
        num1 = float((int(num1) | int(num2)));
        stack.push(0);
        stack.push(num1);
    }
    else {
        code_error = 1;
        return;
    }
}

void StackFrame::ieq() {
    if (stack.size() < 4) {
        code_error = 4;
        return;
    }
    float num1 = stack.top();
    stack.pop();
    int code_num1 = int(stack.top());
    stack.pop();
    float num2 = stack.top();
    stack.pop();
    int code_num2 = int(stack.top());
    stack.pop();
    if (code_num1 == 0 && code_num2 == 0) {
        num1 = (num1 == num2) ? float(1) : float(0);
        stack.push(0);
        stack.push(num1);
    }
    else {
        code_error = 1;
        return;
    }
}

void StackFrame::feq() {
    if (stack.size() < 4) {
        code_error = 4;
        return;
    }
    float num1 = stack.top();
    stack.pop();
    stack.pop();
    float num2 = stack.top();
    stack.pop();
    stack.pop();
    num1 = (num1 == num2) ? float(1) : float(0);
    stack.push(0);
    stack.push(num1);

}

void StackFrame::ineq() {
    if (stack.size() < 4) {
        code_error = 4;
        return;
    }
    float num1 = stack.top();
    stack.pop();
    int code_num1 = int(stack.top());
    stack.pop();
    float num2 = stack.top();
    stack.pop();
    int code_num2 = int(stack.top());
    stack.pop();
    if (code_num1 == 0 && code_num2 == 0) {
        num1 = (num1 != num2) ? float(1) : float(0);
        stack.push(0);
        stack.push(num1);
    }
    else {
        code_error = 1;
        return;
    }
}

void StackFrame::fneq() {
    if (stack.size() < 4) {
        code_error = 4;
        return;
    }
    float num1 = stack.top();
    stack.pop();
    stack.pop();
    float num2 = stack.top();
    stack.pop();
    stack.pop();
    num1 = (num1 != num2) ? float(1) : float(0);
    stack.push(0);
    stack.push(num1);
}

void StackFrame::ilt() {
    if (stack.size() < 4) {
        code_error = 4;
        return;
    }
    float num1 = stack.top();
    stack.pop();
    int code_num1 = int(stack.top());
    stack.pop();
    float num2 = stack.top();
    stack.pop();
    int code_num2 = int(stack.top());
    stack.pop();
    if (code_num1 == 0 && code_num2 == 0) {
        num1 = (num2 < num1) ? float(1) : float(0);
        stack.push(0);
        stack.push(num1);
    }
    else {
        code_error = 1;
        return;
    }
}

void StackFrame::flt() {
    if (stack.size() < 4) {
        code_error = 4;
        return;
    }
    float num1 = stack.top();
    stack.pop();
    stack.pop();
    float num2 = stack.top();
    stack.pop();
    stack.pop();
    num1 = (num2 < num1) ? float(1) : float(0);
    stack.push(0);
    stack.push(num1);
}

void StackFrame::igt() {
    if (stack.size() < 4) {
        code_error = 4;
        return;
    }
    float num1 = stack.top();
    stack.pop();
    int code_num1 = int(stack.top());
    stack.pop();
    float num2 = stack.top();
    stack.pop();
    int code_num2 = int(stack.top());
    stack.pop();
    if (code_num1 == 0 && code_num2 == 0) {
        num1 = (num2 > num1) ? float(1) : float(0);
        stack.push(0);
        stack.push(num1);
    }
    else {
        code_error = 1;
        return;
    }
}

void StackFrame::fgt() {
    if (stack.size() < 4) {
        code_error = 4;
        return;
    }
    float num1 = stack.top();
    stack.pop();
    stack.pop();
    float num2 = stack.top();
    stack.pop();
    stack.pop();
    num1 = (num2 > num1) ? float(1) : float(0);
    stack.push(0);
    stack.push(num1);
}

void StackFrame::ibnot() {
    if (stack.size() < 2) {
        code_error = 4;
        return;
    }
    float num = stack.top();
    stack.pop();
    int code_num = int(stack.top());
    stack.pop();
    if (code_num == 0) {
        num = (num != 0) ? float(0) : float(1);
        stack.push(0);
        stack.push(num);
    }
    else {
        code_error = 1;
        return;
    }
}

void StackFrame::iconst(float data) {
    if (stack.size() == 32) {
        code_error = 3;
        return;
    }
    stack.push(0);
    stack.push(data);
}

void StackFrame::fconst(float data) {
    if (stack.size() == 32) {
        code_error = 3;
        return;
    }
    stack.push(1);
    stack.push(data);
}

void StackFrame::iload(string data_string) {
    bool is_search = search(data_string);
    if (is_search == 0) {
        code_error = 6;
        return;
    }
    int code = get_code(data_string);
    if (code == 1) {
        code_error = 1;
        return;
    }
    if (stack.size() == 32) {
        code_error = 3;
        return;
    }
    float num = get_data(data_string);
    stack.push(float(code));
    stack.push(num);
}

void StackFrame::fload(string data_string) {
    bool is_search = search(data_string);
    if (is_search == 0) {
        code_error = 6;
        return;
    }
    int code = get_code(data_string);
    if (code == 0) {
        code_error = 1;
        return;
    }
    if (stack.size() == 32) {
        code_error = 3;
        return;
    }
    float num = get_data(data_string);
    stack.push(float(code));
    stack.push(num);
}

void StackFrame::istore(string data_string) {
    if (stack.size() < 2) {
        code_error = 4;
        return;
    }
    float num = stack.top();
    stack.pop();
    int code = int(stack.top());
    stack.pop();
    if (code == 1) {
        code_error = 1;
        return;
    }
    if (avl_size == LOCAL_VARIABLE_SPACE_SIZE) {
        code_error = 5;
        return;
    }
    //store data to local
    //avl_size++;
    insert(num, code, data_string);
}

void StackFrame::fstore(string data_string) {
    if (stack.size() < 2) {
        code_error = 4;
        return;
    }
    float num = stack.top();
    stack.pop();
    int code = int(stack.top());
    stack.pop();
    if (code == 0) {
        code_error = 1;
        return;
    }
    if (avl_size == LOCAL_VARIABLE_SPACE_SIZE) {
        code_error = 5;
        return;
    }
    //store data to local
    //avl_size++;
    insert(num, code, data_string);
}

void StackFrame::i2f() {
    if (stack.size() < 2) {
        code_error = 4;
        return;
    }
    float num = stack.top();
    stack.pop();
    int code = int(stack.top());
    stack.pop();
    if (code == 1) {
        code_error = 1;
        return;
    }
    stack.push(1);
    stack.push(num);
}

void StackFrame::f2i() {
    if (stack.size() < 2) {
        code_error = 4;
        return;
    }
    float num = stack.top();
    stack.pop();
    int code = int(stack.top());
    stack.pop();
    if (code == 0) {
        code_error = 1;
        return;
    }
    stack.push(0);
    stack.push(float(int(num)));
}

void StackFrame::top() {
    if (stack.size() == 0) {
        code_error = 4;
        return;
    }
    cout << stack.top() << "\n";
}

void StackFrame::val(string data_string) {
    bool is_search = search(data_string);
    if (is_search == false) {
        code_error = 6;
        return;
    }
    float num = get_data(data_string);
    int code = get_code(data_string);
    code == 0 ? cout << int(num) << "\n" : cout << num << "\n";
}

void StackFrame::find_par(Node* root, std::string key, bool& is_find){
    Node* tmp = root;
    if (tmp == NULL)
        return;
    if (tmp->left == NULL && tmp->right==NULL)
        return;
    if (tmp->left == NULL) {
        //check right child
        if (tmp->right->key == key) {
            cout << tmp->key << endl;
            is_find = 1;
            return;
        }
    }
    else if (tmp->right == NULL) {
        // check left child
        if (tmp->left->key == key) {
            cout << tmp->key << endl;
            is_find = 1;
            return;
        }     
    }
    // Have left child and right child
    else if (tmp->left != NULL && tmp->right != NULL) {
        if (tmp->right->key == key) {
            cout << tmp->key << endl;
            is_find = 1;
            return;
        }
        else if (tmp->left->key == key) {
            cout << tmp->key << endl;
            is_find = 1;
            return;
        }
    }
    find_par(tmp->left, key, is_find);
    find_par(tmp->right, key, is_find);
}

void StackFrame::par(string data_string) {
    if (root == NULL) {
        code_error = 6;
       return;
    }
    if (root->key == data_string) {
        cout << "null\n";
        return;
    }
    else {
        bool is_find = 0;
        find_par(root, data_string, is_find);
        code_error = (is_find == 0) ? 6 : 0;
        return;
    }
}
