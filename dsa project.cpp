#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <sstream>
using namespace std;

// Logger class for timing and logging (for performance)
class Logger {
public:
    static void log(const string& msg) {
        ofstream fout("log.txt", ios::app);
        if (fout.is_open()) {
            fout << msg << endl;
            fout.close();
        }
    }
};

// Product class
class Product {
public:
    int id;
    string name;
    double price;
    int stock;

    Product(int id, string name, double price, int stock)
        : id(id), name(name), price(price), stock(stock) {}

    void display() const {
        cout << left << setw(6) << id
             << setw(25) << name
             << "$" << setw(10) << fixed << setprecision(2) << price
             << "Stock: " << stock << endl;
    }
};

// ProductList - Uses vector (array)
class ProductList {
private:
    vector<Product> products;

public:
    void addProduct(const Product& p) {
        products.push_back(p);
    }

    Product* findProductById(int id) {
        for (auto& p : products)
            if (p.id == id) return &p;
        return nullptr;
    }

    int size() const {
        return (int)products.size();
    }

    Product& getAt(int index) {
        return products[index];
    }

    void displayAll() const {
        cout << "ID    Name                     Price      Stock\n";
        cout << "--------------------------------------------------\n";
        for (const auto& p : products)
            p.display();
    }

    void updateStock(int id, int qty) {
        for (auto& p : products) {
            if (p.id == id) {
                p.stock -= qty;
                if (p.stock < 0) p.stock = 0;
            }
        }
    }

    void removeProductById(int id) {
        auto it = remove_if(products.begin(), products.end(),
                            [id](const Product& p) { return p.id == id; });
        if (it != products.end()) {
            products.erase(it, products.end());
            cout << "Product with ID " << id << " removed.\n";
        } else {
            cout << "Product not found.\n";
        }
    }

    void updateProduct(int id, const string& newName, double newPrice, int newStock) {
        Product* p = findProductById(id);
        if (p) {
            p->name = newName;
            p->price = newPrice;
            p->stock = newStock;
            cout << "Product updated successfully.\n";
        } else {
            cout << "Product not found.\n";
        }
    }

    // Bubble Sort by Name
    void bubbleSortByName() {
        auto start = chrono::high_resolution_clock::now();
        int n = (int)products.size();
        for (int i = 0; i < n - 1; ++i) {
            for (int j = 0; j < n - i - 1; ++j) {
                if (products[j].name > products[j + 1].name) {
                    swap(products[j], products[j + 1]);
                }
            }
        }
        auto end = chrono::high_resolution_clock::now();
        auto diff = chrono::duration_cast<chrono::microseconds>(end - start);
        Logger::log("Bubble sort by name time (microseconds): " + to_string(diff.count()));
    }

    // Quick Sort by Price
    void quickSortByPrice(int low, int high) {
        if (low < high) {
            int pi = partition(low, high);
            quickSortByPrice(low, pi - 1);
            quickSortByPrice(pi + 1, high);
        }
    }

    int partition(int low, int high) {
        double pivot = products[high].price;
        int i = low - 1;
        for (int j = low; j <= high - 1; ++j) {
            if (products[j].price < pivot) {
                ++i;
                swap(products[i], products[j]);
            }
        }
        swap(products[i + 1], products[high]);
        return i + 1;
    }

    // Linear Search by ID
    Product* linearSearch(int id) {
        for (auto& p : products) {
            if (p.id == id) return &p;
        }
        return nullptr;
    }

    // Binary Search by ID (Assuming sorted by ID)
    int binarySearch(int id) {
        int left = 0, right = (int)products.size() - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (products[mid].id == id) return mid;
            else if (products[mid].id < id) left = mid + 1;
            else right = mid - 1;
        }
        return -1;
    }
};

// CartNode (for Linked List Cart)
class CartNode {
public:
    Product product;
    int quantity;
    CartNode* next;

    CartNode(const Product& p, int qty) : product(p), quantity(qty), next(nullptr) {}
};

// Cart implemented as singly linked list
class Cart {
private:
    CartNode* head;

public:
    Cart() : head(nullptr) {}

    void addToCart(const Product& p, int qty) {
        CartNode* newNode = new CartNode(p, qty);
        if (!head) {
            head = newNode;
        } else {
            CartNode* temp = head;
            while (temp->next)
                temp = temp->next;
            temp->next = newNode;
        }
        cout << qty << " x " << p.name << " added to cart.\n";
    }

    void removeFromCart(int productId) {
        if (!head) {
            cout << "Cart is empty.\n";
            return;
        }
        if (head->product.id == productId) {
            CartNode* toDelete = head;
            head = head->next;
            delete toDelete;
            cout << "Product removed from cart.\n";
            return;
        }
        CartNode* temp = head;
        while (temp->next && temp->next->product.id != productId) {
            temp = temp->next;
        }
        if (temp->next && temp->next->product.id == productId) {
            CartNode* toDelete = temp->next;
            temp->next = temp->next->next;
            delete toDelete;
            cout << "Product removed from cart.\n";
        } else {
            cout << "Product not found in cart.\n";
        }
    }

    void updateQuantity(int productId, int newQty) {
        CartNode* temp = head;
        while (temp) {
            if (temp->product.id == productId) {
                temp->quantity = newQty;
                cout << "Quantity updated.\n";
                return;
            }
            temp = temp->next;
        }
        cout << "Product not found in cart.\n";
    }

    void displayCart() const {
        if (!head) {
            cout << "Cart is empty.\n";
            return;
        }
        cout << "Your Cart:\n";
        double total = 0.0;
        CartNode* temp = head;
        while (temp) {
            cout << left << setw(25) << temp->product.name
                 << " x " << setw(3) << temp->quantity << " = $"
                 << fixed << setprecision(2) << temp->quantity * temp->product.price << endl;
            total += temp->quantity * temp->product.price;
            temp = temp->next;
        }
        cout << "Total: $" << total << endl;
    }

    void clearCart() {
        while (head) {
            CartNode* temp = head;
            head = head->next;
            delete temp;
        }
        cout << "Cart cleared.\n";
    }

    bool isEmpty() const {
        return head == nullptr;
    }
};

// Recently viewed products stack
class RecentlyViewed {
private:
    stack<Product> viewed;

public:
    void viewProduct(const Product& p) {
        viewed.push(p);
    }

    void display() {
        if (viewed.empty()) {
            cout << "No recently viewed products.\n";
            return;
        }
        cout << "Recently Viewed Products:\n";
        stack<Product> copy = viewed;
        while (!copy.empty()) {
            copy.top().display();
            copy.pop();
        }
    }
};

// Order Processing Queue
class OrderQueue {
private:
    queue<Product> orders;

public:
    void addOrder(const Product& p) {
        orders.push(p);
    }

    void processOrders() {
        if (orders.empty()) {
            cout << "No orders to process.\n";
            return;
        }
        cout << "Processing Orders:\n";
        while (!orders.empty()) {
            Product p = orders.front();
            orders.pop();
            cout << "Order processed for product: " << p.name << endl;
        }
    }
};

// Graph for product recommendations (Adjacency List)
class ProductGraph {
private:
    unordered_map<int, vector<int>> adjList;  // productID -> list of recommended productIDs
    unordered_map<int, string> productNames;

public:
    void addEdge(int fromId, int toId) {
        adjList[fromId].push_back(toId);
    }

    void addProductName(int id, const string& name) {
        productNames[id] = name;
    }

    void dfsUtil(int start, unordered_map<int, bool>& visited) {
        visited[start] = true;
        cout << productNames[start] << " ";

        for (int neighbor : adjList[start]) {
            if (!visited[neighbor])
                dfsUtil(neighbor, visited);
        }
    }

    void dfs(int start) {
        unordered_map<int, bool> visited;
        if(productNames.find(start) == productNames.end()){
            cout << "Product ID not found for recommendations.\n";
            return;
        }
        cout << "Recommended Products from " << productNames[start] << ": ";
        dfsUtil(start, visited);
        cout << endl;
    }
};
   
int main() {
    ProductList products;
    Cart cart;
    RecentlyViewed recentlyViewed;
    OrderQueue orderQueue;
    ProductGraph recommendationGraph;

    // Adding products
    products.addProduct(Product(1, "Laptop", 999.99, 10));
    products.addProduct(Product(2, "Mouse", 25.50, 100));
    products.addProduct(Product(3, "Keyboard", 45.75, 75));
    products.addProduct(Product(4, "Monitor", 199.99, 30));
    products.addProduct(Product(5, "USB Cable", 5.99, 200));
    products.addProduct(Product(6, "Charger", 15.49, 50));
    products.addProduct(Product(7, "Headphones", 89.99, 40));
    products.addProduct(Product(8, "Webcam", 55.25, 35));
    products.addProduct(Product(9, "Microphone", 60.00, 25));
    products.addProduct(Product(10, "Graphics Card", 399.95, 20));
    products.addProduct(Product(11, "Desk Lamp", 29.95, 45));
    products.addProduct(Product(12, "Phone Stand", 14.90, 100));
    products.addProduct(Product(13, "Gaming Chair", 249.99, 15));
    products.addProduct(Product(14, "Tablet", 320.00, 22));
    products.addProduct(Product(15, "Smartphone", 850.00, 18));

    // Add graph recommendations
    for (int i = 1; i <= products.size(); ++i) {
        Product& p = products.getAt(i - 1);
        recommendationGraph.addProductName(p.id, p.name);
    }

    recommendationGraph.addEdge(1, 2);
    recommendationGraph.addEdge(1, 3);
    recommendationGraph.addEdge(1, 10);
    recommendationGraph.addEdge(2, 5);
    recommendationGraph.addEdge(3, 5);
    recommendationGraph.addEdge(4, 10);
    recommendationGraph.addEdge(10, 14);

    int choice;
    do {
        cout << "\n===== E-Commerce System =====\n";
        cout << "1. Display Products\n";
        cout << "2. View Product Details\n";
        cout << "3. Add to Cart\n";
        cout << "4. View Cart\n";
        cout << "5. Remove from Cart\n";
        cout << "6. Update Cart Quantity\n";
        cout << "7. Checkout\n";
        cout << "8. View Recently Viewed\n";
        cout << "9. View Recommendations\n";
        cout << "10. Admin - Remove Product\n";
        cout << "11. Admin - Update Product\n";
        cout << "12. Sort Products by Name\n";
        cout << "13. Sort Products by Price\n";
        cout << "14. Search Product by ID\n";
        cout << "15. Process Orders\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                products.displayAll();
                break;

            case 2: {
                int id;
                cout << "Enter Product ID to view: ";
                cin >> id;
                Product* p = products.findProductById(id);
                if (p) {
                    p->display();
                    recentlyViewed.viewProduct(*p);
                } else {
                    cout << "Product not found.\n";
                }
                break;
            }

            case 3: {
                int id, qty;
                cout << "Enter Product ID to add to cart: ";
                cin >> id;
                cout << "Enter quantity: ";
                cin >> qty;
                Product* p = products.findProductById(id);
                if (p && p->stock >= qty) {
                    cart.addToCart(*p, qty);
                    products.updateStock(id, qty);
                } else {
                    cout << "Invalid ID or insufficient stock.\n";
                }
                break;
            }

            case 4:
                cart.displayCart();
                break;

            case 5: {
                int id;
                cout << "Enter Product ID to remove from cart: ";
                cin >> id;
                cart.removeFromCart(id);
                break;
            }

            case 6: {
                int id, qty;
                cout << "Enter Product ID to update quantity: ";
                cin >> id;
                cout << "Enter new quantity: ";
                cin >> qty;
                cart.updateQuantity(id, qty);
                break;
            }

            case 7: {
                if (cart.isEmpty()) {
                    cout << "Cart is empty.\n";
                } else {
                    cout << "Checking out...\n";
                    cart.displayCart();
                    cart.clearCart();
                }
                break;
            }

            case 8:
                recentlyViewed.display();
                break;

            case 9: {
                int id;
                cout << "Enter Product ID to get recommendations: ";
                cin >> id;
                recommendationGraph.dfs(id);
                break;
            }

            case 10: {
                int id;
                cout << "Enter Product ID to remove: ";
                cin >> id;
                products.removeProductById(id);
                break;
            }

            case 11: {
                int id;
                string newName;
                double newPrice;
                int newStock;
                cout << "Enter Product ID to update: ";
                cin >> id;
                cout << "Enter new name: ";
                cin.ignore();
                getline(cin, newName);
                cout << "Enter new price: ";
                cin >> newPrice;
                cout << "Enter new stock: ";
                cin >> newStock;
                products.updateProduct(id, newName, newPrice, newStock);
                break;
            }

            case 12:
                products.bubbleSortByName();
                cout << "Products sorted by name.\n";
                break;

            case 13:
                products.quickSortByPrice(0, products.size() - 1);
                cout << "Products sorted by price.\n";
                break;

            case 14: {
                int id;
                cout << "Enter Product ID to search: ";
                cin >> id;
                int index = products.binarySearch(id);
                if (index != -1) {
                    cout << "Product found:\n";
                    products.getAt(index).display();
                } else {
                    cout << "Product not found.\n";
                }
                break;
            }

            case 15:
                orderQueue.processOrders();
                break;

            case 0:
                cout << "Thank you for using the E-Commerce System!\n";
                break;

            default:
                cout << "Invalid choice. Try again.\n";
                break;
        }

    } while (choice != 0);

    return 0;
}

	



