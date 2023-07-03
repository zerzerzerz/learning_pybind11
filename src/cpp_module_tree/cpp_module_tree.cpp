#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <tuple>
#include <map>
#include <string>
#include <cassert>
#include <sstream>

using namespace std;
namespace py = pybind11;

class Node{
    public:
        string name;
        Node* prev;
        Node* left;
        Node* right;
        Node(string _name, Node* _prev, Node* _left, Node* _right): name(_name), prev(_prev), left(_left), right(_right){
            cout << "[Create] node " << name << endl;
        }
        ~Node(){
            cout << "[Delete] node " << name << endl;
        }
        string __repr__(){
            string s = "<Node, name=";
            string other_name;
            s += name;

            s += ", prev=";
            other_name = (prev==nullptr)? "None": prev->name;
            s += other_name;

            s += ", left=";
            other_name = (left==nullptr)? "None": left->name;
            s += other_name;

            s += ", right=";
            other_name = (right==nullptr)? "None": right->name;
            s += other_name;

            s += ">";
            return s;
        }

        string get_address(){
            stringstream ss;
            ss << this;
            return ss.str();
        }
};

Node create_static_node(const string& s){
    Node n(s, nullptr, nullptr, nullptr);
    return n;
}

Node* create_dynamic_node_take_ownership(const string& s){
    Node* n = new Node(s, nullptr, nullptr, nullptr);
    return n;
}

Node* create_dynamic_node_reference(const string& s){
    Node* n = new Node(s, nullptr, nullptr, nullptr);
    return n;
}

Node* create_crush_node(const string& name){
    
    Node n = Node(name, nullptr, nullptr, nullptr);
    return &n;
}

class Tree{
    public:
        Node* root;
        int num_node;
        vector<Node*> node_list;
        map<string, Node*> name2node;  

        Node* get_root(){
            return root;
        } 

        vector<Node*> get_root_via_vector(){
            return {root};
        }

        
        ~Tree(){
            for(const auto& i: node_list){
                delete i;
            }
        }

        Tree(int _num_node){
            assert (_num_node > 0);
            num_node = _num_node;
            for(int i=0; i<num_node; ++i){
                auto node = new Node("name_" + to_string(i), nullptr, nullptr, nullptr);
                name2node[node->name] = node;
                node_list.push_back(node);
            }

            root = node_list[0];

            for(int i=0; i<num_node; ++i){
                auto c = 2*i+1;
                if(c<num_node){
                    node_list[i]->left = node_list[c];
                    node_list[c]->prev = node_list[i];
                }
                c++;
                if(c<num_node){
                    node_list[i]->right = node_list[c];
                    node_list[c]->prev = node_list[i];
                }
            }
        }     
        void traverse(){
            _traverse(root);
        }

        void display(){
            for(const auto& p: name2node){
                cout << p.first << " -> " << p.second->__repr__() << endl;
            }
        }

    private:
        void _traverse(Node* n){
            if(n!=nullptr){
                _traverse(n->left);
                cout << n->__repr__() << endl;
                _traverse(n->right);
            }
        }
};


PYBIND11_MODULE(cpp_module_tree, m){
    m.def("create_static_node", &create_static_node, py::return_value_policy::take_ownership);
    m.def("create_dynamic_node_take_ownership", &create_dynamic_node_take_ownership, py::return_value_policy::take_ownership);
    m.def("create_dynamic_node_reference", &create_dynamic_node_reference, py::return_value_policy::reference);
    m.def("create_crush_node", &create_crush_node);

    py::class_<Node>(m, "Node")
        .def(py::init<string, Node*, Node*, Node*>())
        // .def("__repr__", &Node::__repr__)
        .def("get_address", &Node::get_address)
        .def_readwrite("name", &Node::name)
        .def_readwrite("prev", &Node::prev)
        .def_readwrite("left", &Node::left)
        .def_readwrite("right", &Node::right)
    ;

    py::class_<Tree>(m, "Tree")
        .def(py::init<int>())
        .def("display", &Tree::display)
        .def("get_root", &Tree::get_root)
        .def("get_root_via_vector_move", &Tree::get_root_via_vector, py::return_value_policy::move)
        .def("get_root_via_vector_copy", &Tree::get_root_via_vector, py::return_value_policy::copy)
        .def("get_root_via_vector_take_ownership", &Tree::get_root_via_vector, py::return_value_policy::take_ownership)
        .def("get_root_via_vector_reference", &Tree::get_root_via_vector, py::return_value_policy::reference)
        .def("traverse", &Tree::traverse)
        .def_readwrite("root", &Tree::root)
        .def_readwrite("num_node", &Tree::num_node)
        .def_readwrite("node_list", &Tree::node_list)
        .def_readwrite("name2node", &Tree::name2node)
    ;
}