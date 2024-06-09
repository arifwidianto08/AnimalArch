#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to get the operating system path separator
char getOSPathSeparator() {
    // Define the operating system type based on predefined macros
    #if defined(_WIN32) || defined(__CYGWIN__)
        return '\\';
    #elif defined(__linux__)
        return '/';
    #elif defined(__APPLE__) && defined(__MACH__)
        return '/';
    #elif defined(unix) || defined(__unix__) || defined(__unix)
        return '/';
    #else
        #error Unknown environment!
    #endif
}

// Function to adjust the path separator based on the operating system
char *adjustPathSeparator(const char *path) {
    char separator = getOSPathSeparator();
    size_t len = strlen(path);

    // Create a new string to store the adjusted path
    char *adjusted_path = (char *)malloc(len + 1); // +1 for the null terminator

    if (adjusted_path == NULL) {
        // Handle memory allocation failure
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Copy the characters from the original path, adjusting the separator
    for (size_t i = 0; i < len; i++) {
        adjusted_path[i] = (path[i] == '\\' || path[i] == '/') ? separator : path[i];
    }

    // Add the null terminator
    adjusted_path[len] = '\0';

    return adjusted_path;
}

// Define the structure for a tree node with additional data fields
typedef struct TreeNode {
    char name[50];
    char scientificName[100];
    char commonName[100];
    char classification[100];
    char habitat[100];
    struct TreeNode *firstChild;
    struct TreeNode *nextSibling;
} TreeNode;

// Function to create a new tree node
TreeNode *createNode(const char *name, const char *scientificName, const char *commonName, const char *classification, const char *habitat) {
    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
    strcpy(newNode->name, name);
    strcpy(newNode->scientificName, scientificName);
    strcpy(newNode->commonName, commonName);
    strcpy(newNode->classification, classification);
    strcpy(newNode->habitat, habitat);
    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;
    return newNode;
}

// Function to add a child to a parent node
void addChild(TreeNode *parent, TreeNode *child) {
    if (parent->firstChild == NULL) {
        parent->firstChild = child;
    } else {
        TreeNode *temp = parent->firstChild;
        while (temp->nextSibling != NULL) {
            temp = temp->nextSibling;
        }
        temp->nextSibling = child;
    }
}

// Function to search for a node by name
TreeNode *search(TreeNode *root, const char *name) {
    if (root == NULL)
        return NULL;
    if (strcmp(root->name, name) == 0)
        return root;

    TreeNode *result = search(root->firstChild, name);
    if (result != NULL)
        return result;

    return search(root->nextSibling, name);
}

// Function for in-order traversal with specific rule
void inOrderTraversal(TreeNode *root)
{
    if (root == NULL)
        return;

    TreeNode *child = root->firstChild;

    // Visit the left children (first two child nodes from the left)
    for (int i = 0; i < 2 && child != NULL; i++) {
        inOrderTraversal(child);
        child = child->nextSibling;
    }

    // Visit the root node
    printf("%s\n", root->name);

    // Skip to the last child
    if (child != NULL) {
        while (child->nextSibling != NULL) {
            child = child->nextSibling;
        }
        // Visit the right child (single last node)
        inOrderTraversal(child);
    }
}

// Function for pre-order traversal
void preOrderTraversal(TreeNode *root) {
    if (root == NULL)
        return;
    printf("%s\n", root->name);
    preOrderTraversal(root->firstChild);
    preOrderTraversal(root->nextSibling);
}

// Function for post-order traversal
void postOrderTraversal(TreeNode *root) {
    if (root == NULL)
        return;
    postOrderTraversal(root->firstChild);
    postOrderTraversal(root->nextSibling);
    printf("%s\n", root->name);
}

// Function to save parent-child edges to a CSV file
void saveParentChildEdges(TreeNode *root, FILE *file) {
    if (root == NULL)
        return;

    TreeNode *child = root->firstChild;
    while (child != NULL) {
        fprintf(file, "%s,%s\n", root->name, child->name);
        child = child->nextSibling;
    }

    // Recursively save parent-child edges for children nodes
    child = root->firstChild;
    while (child != NULL) {
        saveParentChildEdges(child, file);
        child = child->nextSibling;
    }
}

// Function to save sibling edges to a CSV file
void saveSiblingEdges(TreeNode *root, FILE *file) {
    if (root == NULL)
        return;

    TreeNode *child = root->firstChild;
    while (child != NULL && child->nextSibling != NULL) {
        fprintf(file, "%s,%s\n", child->name, child->nextSibling->name);
        child = child->nextSibling;
    }

    // Recursively save sibling edges for children nodes
    child = root->firstChild;
    while (child != NULL) {
        saveSiblingEdges(child, file);
        child = child->nextSibling;
    }
}

// Function to print the tree structure
void printTree(TreeNode *root, int level) {
    if (root == NULL)
        return;

    for (int i = 0; i < level; i++) {
        if (i == level - 1) {
            printf("├── ");
        } else {
            printf("│   ");
        }
    }
    printf("%s\n", root->name);

    TreeNode *child = root->firstChild;
    while (child != NULL) {
        printTree(child, level + 1);
        child = child->nextSibling;
    }
}

int main() {
    // Create the root of the tree
    TreeNode *root = createNode("1", "Animalia", "Animals", "Kingdom", "Various");

    // Add children to the root node
    TreeNode *n5 = createNode("5", "Chordata", "Chordates", "Phylum", "Various");
    TreeNode *n6 = createNode("6", "Arthropoda", "Arthropods", "Phylum", "Various");
    TreeNode *n3 = createNode("3", "Mollusca", "Mollusks", "Phylum", "Aquatic/Terrestrial");
    TreeNode *n4 = createNode("4", "Annelida", "Annelids", "Phylum", "Various");
    addChild(root, n5);
    addChild(root, n6);
    addChild(root, n3);
    addChild(root, n4);

    // Add children to the node 5
    TreeNode *n14 = createNode("14", "Mammalia", "Mammals", "Class", "Various");
    TreeNode *n15 = createNode("15", "Aves", "Birds", "Class", "Various");
    TreeNode *n16 = createNode("16", "Reptilia", "Reptiles", "Class", "Various");
    TreeNode *n17 = createNode("17", "Amphibia", "Amphibians", "Class", "Various");
    addChild(n5, n14);
    addChild(n5, n15);
    addChild(n5, n16);
    addChild(n5, n17);

    // Add children to the node 6
    TreeNode *n18 = createNode("18", "Insecta", "Insects", "Class", "Various");
    TreeNode *n19 = createNode("19", "Arachnida", "Arachnids", "Class", "Various");
    TreeNode *n20 = createNode("20", "Crustacea", "Crustaceans", "Class", "Various");
    TreeNode *n21 = createNode("21", "Myriapoda", "Myriapods", "Class", "Various");
    addChild(n6, n18);
    addChild(n6, n19);
    addChild(n6, n20);
    addChild(n6, n21);

    // Add children to the node 3
    TreeNode *n8 = createNode("8", "Cephalopoda", "Cephalopods", "Class", "Various");
    TreeNode *n9 = createNode("9", "Gastropoda", "Gastropods", "Class", "Various");
    TreeNode *n10 = createNode("10", "Bivalvia", "Bivalves", "Class", "Aquatic");
    addChild(n3, n8);
    addChild(n3, n9);
    addChild(n3, n10);

    // Add children to the node 4
    TreeNode *n11 = createNode("11", "Polychaeta", "Polychaetes", "Class", "Marine");
    TreeNode *n12 = createNode("12", "Oligochaeta", "Oligochaetes", "Class", "Various");
    TreeNode *n13 = createNode("13", "Hirudinea", "Leeches", "Class", "Aquatic/Terrestrial");
    addChild(n4, n11);
    addChild(n4, n12);
    addChild(n4, n13);

    // Print the tree structure
    printf("Tree Structure:\n");
    printTree(root, 0);

    // Perform in-order traversal
    printf("\nIn-Order Traversal:\n");
    inOrderTraversal(root);

    // Perform pre-order traversal
    printf("\nPre-Order Traversal:\n");
    preOrderTraversal(root);

    // Perform post-order traversal
    printf("\nPost-Order Traversal:\n");
    postOrderTraversal(root);

    // Save parent-child edges to CSV file
    char *parentChildPath = adjustPathSeparator("edges_parent_child.csv");
    FILE *parentChildFile = fopen(parentChildPath, "w");
    if (parentChildFile != NULL) {
        saveParentChildEdges(root, parentChildFile);
        fclose(parentChildFile);
        printf("\nParent-child edges saved to %s\n", parentChildPath);
    } else {
        perror("Error opening parent-child CSV file");
    }

    // Save sibling edges to CSV file
    char *siblingPath = adjustPathSeparator("edges_sibling.csv");
    FILE *siblingFile = fopen(siblingPath, "w");
    if (siblingFile != NULL) {
        saveSiblingEdges(root, siblingFile);
        fclose(siblingFile);
        printf("Sibling edges saved to %s\n", siblingPath);
    } else {
        perror("Error opening sibling CSV file");
    }

    // Free allocated memory for adjusted paths
    free(parentChildPath);
    free(siblingPath);

    return 0;
}
