#include <State/BST.hpp>

BST::Node* BST::insert(Node *&root, Node* parent, int data) {
    if (root == nullptr) {
        if (parent == nullptr) {
            root = new Node{
                data, 1, (int)mNodeList.size(), false, 
                sf::Vector2f(Constant::WINDOW_WIDTH/2 - Size::NODE_RADIUS, 150 * Constant::SCALE_Y), 
                nullptr, nullptr, nullptr, 1
            };
        }
        else {
            bool isLeft = data < parent->val;
            root = new Node{
                data, parent->height + 1, (int)mNodeList.size(), isLeft, 
                sf::Vector2f(parent->position.x + (isLeft ? -NODE_DISTANCE_HORIZONTAL : NODE_DISTANCE_HORIZONTAL), parent->position.y + NODE_DISTANCE_VERTICAL), 
                nullptr, nullptr, parent, 1
            };
        }
        mNodeList.push_back(root);
        return root;
    }
    if (data < root->val) {
        if (root->left != nullptr && data > root->left->val) moveTree(root->left, true);
        root->left = insert(root->left, root, data);
    }
    else if (data > root->val) {
        if (root->right != nullptr && data < root->right->val) moveTree(root->right, false);
        root->right = insert(root->right, root, data);
    }
    else {
        root->duplicate++;
    }
    return root;
}

void BST::moveTree(Node* root, bool isLeft) {
    if (root == nullptr) return;
    sf::Vector2f dis = sf::Vector2f(isLeft ? -NODE_DISTANCE_HORIZONTAL : NODE_DISTANCE_HORIZONTAL, 0);
    root->position += dis;
    moveTree(root->left, isLeft);
    moveTree(root->right, isLeft);
}

void BST::clear(Node *&root) {
    if (root == nullptr) return;
    clear(root->left);
    clear(root->right);
    delete root;
    root = nullptr;
}

void BST::createRandomTree() {
    mInputData.clear();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 99);

    for (int i = 0; i < mInputSize; i++) {
        mInputData.push_back(dis(gen));
    }

    for (int i = 0; i < mInputData.size(); i++) {
        insert(mRoot, nullptr, mInputData[i]);
    }

    createTree();
}