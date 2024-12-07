#include <iostream>
#include <memory>
#include <deque>
#include <algorithm>
#include <chrono>

template<typename T>
class Stack {
    struct Node {
        T value;
        std::shared_ptr<Node> next;

        Node(T val, std::shared_ptr<Node> nxt) : value(val), next(nxt) {}
    };
    std::shared_ptr<Node> topNode;

public:
    Stack() : topNode(nullptr) {}

    Stack push(T value) const {
        return Stack(std::make_shared<Node>(value, topNode));
    }

    Stack pop() const {
        if (!topNode) return *this;
        return Stack(topNode->next);
    }

    size_t getMemorySize() const {
        size_t size = 0;
        auto node = topNode;
        while (node) {
            size += sizeof(*node);
            node = node->next;
        }
        return size;
    }

private:
    Stack(std::shared_ptr<Node> node) : topNode(node) {}
};

template <typename Func>
void measureTime(const std::string& operation, Func&& func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << operation << " took " << duration.count() << " ms" << std::endl;
}

void testStackOperations(size_t numElements) {
    Stack<int> stack;

    std::cout << "\nTesting " << numElements << " elements:\n";

      measureTime("Push operation", [&]() {
        for (size_t i = 0; i < numElements; ++i) {
            stack = stack.push(i);
        }
    });
    std::cout << "Memory usage after push: " << stack.getMemorySize() << " bytes" << std::endl;

        measureTime("Pop operation", [&]() {
        for (size_t i = 0; i < numElements; ++i) {
            stack = stack.pop();
        }
    });

}

int main() {
    testStackOperations(10000);
    testStackOperations(20000);
    testStackOperations(30000);

    return 0;
}
