#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to get the operating system path separator
char getOSPathSeparator()
{
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
char *adjustPathSeparator(const char *path)
{
    char separator = getOSPathSeparator();
    size_t len = strlen(path);

    // Create a new string to store the adjusted path
    char *adjusted_path = (char *)malloc(len + 1); // +1 for the null terminator

    if (adjusted_path == NULL)
    {
        // Handle memory allocation failure
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Copy the characters from the original path, adjusting the separator
    for (size_t i = 0; i < len; i++)
    {
        adjusted_path[i] = (path[i] == '\\' || path[i] == '/') ? separator : path[i];
    }

    // Add the null terminator
    adjusted_path[len] = '\0';

    return adjusted_path;
}

// Define the structure for a tree node with additional data fields
typedef struct TreeNode
{
    char name[50];
    char scientificName[100];
    char commonName[100];
    char classification[100];
    char habitat[100];
    struct TreeNode *firstChild;
    struct TreeNode *nextSibling;
} TreeNode;

// Function to create a new tree node
TreeNode *createNode(const char *name, const char *scientificName, const char *commonName, const char *classification, const char *habitat)
{
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
void addChild(TreeNode *parent, TreeNode *child)
{
    if (parent->firstChild == NULL)
    {
        parent->firstChild = child;
    }
    else
    {
        TreeNode *temp = parent->firstChild;
        while (temp->nextSibling != NULL)
        {
            temp = temp->nextSibling;
        }
        temp->nextSibling = child;
    }
}

// Function to search for a node by name
TreeNode *search(TreeNode *root, const char *name)
{
    if (root == NULL)
        return NULL;
    if (strcmp(root->name, name) == 0)
        return root;

    TreeNode *result = search(root->firstChild, name);
    if (result != NULL)
        return result;

    return search(root->nextSibling, name);
}

// Function for in-order traversal
void inOrderTraversal(TreeNode *root)
{
    if (root == NULL)
        return;

    TreeNode *child = root->firstChild;

    // Visit the left children
    if (child != NULL)
    {
        inOrderTraversal(child);
    }

    // Visit the root node
    printf("%s\n", root->name);

    // Visit the remaining siblings of the first child
    if (child != NULL)
    {
        child = child->nextSibling;
        while (child != NULL)
        {
            inOrderTraversal(child);
            child = child->nextSibling;
        }
    }
}

// Function for pre-order traversal
void preOrderTraversal(TreeNode *root)
{
    if (root == NULL)
        return;
    printf("%s\n", root->name);
    preOrderTraversal(root->firstChild);
    preOrderTraversal(root->nextSibling);
}

// Function for post-order traversal
void postOrderTraversal(TreeNode *root)
{
    if (root == NULL)
        return;

    // Recursively visit all child nodes
    TreeNode *child = root->firstChild;
    while (child != NULL)
    {
        postOrderTraversal(child);
        child = child->nextSibling;
    }

    // Visit the root node
    printf("%s\n", root->name);
}

// Function to save parent-child edges to a CSV file
void saveParentChildEdges(TreeNode *root, FILE *file)
{
    if (root == NULL)
        return;

    TreeNode *child = root->firstChild;
    while (child != NULL)
    {
        fprintf(file, "%s,%s\n", root->name, child->name);
        child = child->nextSibling;
    }

    // Recursively save parent-child edges for children nodes
    child = root->firstChild;
    while (child != NULL)
    {
        saveParentChildEdges(child, file);
        child = child->nextSibling;
    }
}

// Function to save sibling edges to a CSV file
void saveSiblingEdges(TreeNode *root, FILE *file)
{
    if (root == NULL)
        return;

    TreeNode *child = root->firstChild;
    while (child != NULL && child->nextSibling != NULL)
    {
        fprintf(file, "%s,%s\n", child->name, child->nextSibling->name);
        child = child->nextSibling;
    }

    // Recursively save sibling edges for children nodes
    child = root->firstChild;
    while (child != NULL)
    {
        saveSiblingEdges(child, file);
        child = child->nextSibling;
    }
}

// Function to print the tree structure
void printTree(TreeNode *root, int level)
{
    if (root == NULL)
        return;

    for (int i = 0; i < level; i++)
    {
        if (i == level - 1)
        {
            printf("├── ");
        }
        else
        {
            printf("│   ");
        }
    }
    printf("%s\n", root->name);

    TreeNode *child = root->firstChild;
    while (child != NULL)
    {
        printTree(child, level + 1);
        child = child->nextSibling;
    }
}

int main()
{
    // Create the root of the tree
    TreeNode *root = createNode("Animalia", "Animalia", "Animals", "Kingdom", "Various");

    // Add children to the root node
    TreeNode *chordata = createNode("Chordata", "Chordata", "Chordates", "Phylum", "Various");
    TreeNode *arthropoda = createNode("Arthropoda", "Arthropoda", "Arthropods", "Phylum", "Various");
    TreeNode *mollusca = createNode("Mollusca", "Mollusca", "Mollusks", "Phylum", "Aquatic/Terrestrial");
    addChild(root, chordata);
    addChild(root, arthropoda);
    addChild(root, mollusca);

    // Add children to the Chordata node
    TreeNode *mammalia = createNode("Mammalia", "Mammalia", "Mammals", "Class", "Various");
    TreeNode *aves = createNode("Aves", "Aves", "Birds", "Class", "Various");
    TreeNode *reptilia = createNode("Reptilia", "Reptilia", "Reptiles", "Class", "Various");
    addChild(chordata, mammalia);
    addChild(chordata, aves);
    addChild(chordata, reptilia);

    // Add children to the Mammalia node
    TreeNode *primates = createNode("Primates", "Primates", "Primates", "Order", "Various");
    TreeNode *carnivora = createNode("Carnivora", "Carnivora", "Carnivores", "Order", "Various");
    addChild(mammalia, primates);
    addChild(mammalia, carnivora);

    // Add children to the Aves node
    TreeNode *passeriformes = createNode("Passeriformes", "Passeriformes", "Perching Birds", "Order", "Various");
    TreeNode *psittaciformes = createNode("Psittaciformes", "Psittaciformes", "Parrots", "Order", "Tropical");
    addChild(aves, passeriformes);
    addChild(aves, psittaciformes);

    // Add children to the Reptilia node
    TreeNode *squamata = createNode("Squamata", "Squamata", "Scaled Reptiles", "Order", "Various");
    TreeNode *testudines = createNode("Testudines", "Testudines", "Turtles", "Order", "Aquatic/Terrestrial");
    addChild(reptilia, squamata);
    addChild(reptilia, testudines);

    // Add children to the Arthropoda node
    TreeNode *insecta = createNode("Insecta", "Insecta", "Insects", "Class", "Various");
    TreeNode *arachnida = createNode("Arachnida", "Arachnida", "Arachnids", "Class", "Various");
    TreeNode *crustacea = createNode("Crustacea", "Crustacea", "Crustaceans", "Class", "Aquatic");
    addChild(arthropoda, insecta);
    addChild(arthropoda, arachnida);
    addChild(arthropoda, crustacea);

    // Add children to the Insecta node
    TreeNode *coleoptera = createNode("Coleoptera", "Coleoptera", "Beetles", "Order", "Various");
    TreeNode *diptera = createNode("Diptera", "Diptera", "Flies", "Order", "Various");
    addChild(insecta, coleoptera);
    addChild(insecta, diptera);

    // Add children to the Arachnida node
    TreeNode *araneae = createNode("Araneae", "Araneae", "Spiders", "Order", "Various");
    TreeNode *scorpiones = createNode("Scorpiones", "Scorpiones", "Scorpions", "Order", "Various");
    addChild(arachnida, araneae);
    addChild(arachnida, scorpiones);

    // Open files to save the CSV data
    const char *parentChildEdgesCSV = adjustPathSeparator("database/parent_child_edges.csv");
    const char *siblingEdgesCSV = adjustPathSeparator("database/sibling_edges.csv");

    FILE *parentChildFile = fopen(parentChildEdgesCSV, "w");
    FILE *siblingFile = fopen(siblingEdgesCSV, "w");

    if (parentChildFile == NULL || siblingFile == NULL)
    {
        perror("Unable to open file");
        return 1;
    }

    // Save the tree data to the CSV files
    saveParentChildEdges(root, parentChildFile);
    saveSiblingEdges(root, siblingFile);

    // Close the files
    fclose(parentChildFile);
    fclose(siblingFile);

    // Print the tree structure
    printTree(root, 0);

    // Perform various tree operations
    printf("\nIn-Order Traversal:\n");
    inOrderTraversal(root);

    printf("\nPre-Order Traversal:\n");
    preOrderTraversal(root);

    printf("\nPost-Order Traversal:\n");
    postOrderTraversal(root);

    // Search for a specific node
    TreeNode *searchResult = search(root, "Mammalia");
    if (searchResult != NULL)
    {
        printf("\nSearch Result:\n");
        printf("Name: %s, Scientific Name: %s, Common Name: %s, Classification: %s, Habitat: %s\n",
               searchResult->name, searchResult->scientificName, searchResult->commonName, searchResult->classification, searchResult->habitat);
    }
    else
    {
        printf("Node not found.\n");
    }

    // Free the allocated memory
    // Note: This example does not free the allocated memory for simplicity.
    // In a real application, you should free the memory to avoid memory leaks.

    return 0;
}
