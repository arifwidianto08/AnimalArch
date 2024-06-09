#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // for usleep

#define RESET "\033[0m"              // Resets the text formatting back to the default after printing the highlighted text.
#define HIGHLIGHT "\033[46m\033[30m" // Bright cyan background with black text
#define VISITED "\033[42m\033[30m"   // Bright green background with black text

// Define the structure for a tree node
typedef struct TreeNode
{
    char name[50];
    char scientificName[100];
    char commonName[100];
    char classification[100];
    char habitat[100];
    struct TreeNode *firstChild;
    struct TreeNode *nextSibling;
    int visited; // Field to mark if the node has been visited
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

// Sleep function for a specified number of seconds
void suspense(float second)
{
    usleep(second * 1000000);
}

// Function to clear the screen using ANSI escape codes
void clearScreen()
{
    printf("\033[H\033[J"); // Clear the screen
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

// Function to reset the visited field of all nodes in the tree
void resetVisited(TreeNode *root)
{
    if (root == NULL)
        return;
    root->visited = 0;               // Reset visited field of current node
    resetVisited(root->firstChild);  // Recursively reset visited field of children
    resetVisited(root->nextSibling); // Recursively reset visited field of siblings
}

// Function to count the number of nodes in the tree
int countNodes(TreeNode *root)
{
    if (root == NULL)
    {
        return 0;
    }
    int count = 1;
    TreeNode *child = root->firstChild;
    while (child != NULL)
    {
        count += countNodes(child);
        child = child->nextSibling;
    }
    return count;
}

// Function to traverse the tree and store all names in an array
void storeNames(TreeNode *root, char **names, int *index)
{
    if (root == NULL)
    {
        return;
    }
    // Store the current node's name in the array
    names[(*index)++] = root->name;
    TreeNode *child = root->firstChild;
    while (child != NULL)
    {
        storeNames(child, names, index);
        child = child->nextSibling;
    }
}

// Function to randomly generate a name from the tree
const char *getRandomName(TreeNode *root)
{
    if (root == NULL)
    {
        return NULL;
    }

    // Count the number of nodes in the tree
    int numNodes = countNodes(root);

    // Allocate memory for storing names
    char **names = (char **)malloc(numNodes * sizeof(char *));
    int index = 0;

    // Traverse the tree and store all names in the array
    storeNames(root, names, &index);

    // Initialization, should only be called once.
    srand(time(NULL));

    // Generate a random index to select a name
    int randomIndex = rand() % numNodes;

    // Get the randomly selected name
    const char *randomName = names[randomIndex];

    // Free the dynamically allocated memory
    free(names);

    return randomName;
}

// Function to print the tree structure with a cursor on a specific node
void printTree(TreeNode *root, int level, TreeNode *cursor, const char *markedLabel)
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

    if (root == cursor)
    {
        printf(HIGHLIGHT "%s" RESET "\n", root->name);
    }
    else if (root->visited)
    {
        printf("%s ( Scientific Name: %s, Common Name: %s, Classification: %s, Habitat: %s ) ✅\n",
               root->name, root->scientificName, root->commonName, root->classification, root->habitat);
    }
    else
    {
        printf("%s\n", root->name);
    }

    TreeNode *child = root->firstChild;
    while (child != NULL)
    {
        printTree(child, level + 1, cursor, markedLabel);
        child = child->nextSibling;
    }
}

// Function to perform pre-order traversal and mark visited nodes
void preOrderTraversal(TreeNode *root)
{
    if (root == NULL)
        return;

    TreeNode *cursor = root;
    TreeNode *stack[100]; // Assume a maximum depth of 100 for simplicity
    int stackSize = 0;

    while (1)
    {

        // Print the tree with the current cursor
        clearScreen();                                // Clear the screen
        printf("Running pre-order traversal...\n\n"); // Print the traversal method

        printTree(root, 0, cursor, ""); // No marked node initially
        suspense(0.5);                  // Wait for 500 milliseconds

        // Mark the current node as visited
        cursor->visited = 1;

        // Push the cursor's first child to the stack if it exists
        if (cursor->firstChild != NULL)
        {
            if (stackSize < 100)
            {
                stack[stackSize++] = cursor;
            }
            cursor = cursor->firstChild;
        }
        else if (cursor->nextSibling != NULL)
        {
            cursor = cursor->nextSibling;
        }
        else
        {
            // Backtrack to find the next sibling
            while (stackSize > 0 && cursor->nextSibling == NULL)
            {
                cursor = stack[--stackSize];
            }
            if (cursor->nextSibling == NULL)
                break;
            cursor = cursor->nextSibling;
        }
    }
}

// Function to perform in-order traversal and mark visited nodes
void inOrderTraversal(TreeNode *root)
{
    if (root == NULL)
        return;

    TreeNode *stack[100]; // Assume a maximum depth of 100 for simplicity
    TreeNode *cursor = root;
    int stackSize = 0;

    while (stackSize > 0 || cursor != NULL)
    {
        if (cursor != NULL)
        {
            stack[stackSize++] = cursor;
            cursor = cursor->firstChild;
        }
        else
        {
            cursor = stack[--stackSize];
            cursor->visited = 1;                         // Mark the current node as visited
            clearScreen();                               // Clear the screen
            printf("Running in-order traversal...\n\n"); // Print the traversal method

            printTree(root, 0, cursor, ""); // No marked node initially
            suspense(0.5);                  // Wait for 500 milliseconds
            cursor = cursor->nextSibling;
        }
    }
}

// Function to perform post-order traversal and mark visited nodes
void postOrderTraversal(TreeNode *root)
{
    if (root == NULL)
        return;

    TreeNode *stack[100]; // Assume a maximum depth of 100 for simplicity
    TreeNode *lastVisited = NULL;
    TreeNode *cursor = root;
    int stackSize = 0;

    while (stackSize > 0 || cursor != NULL)
    {
        if (cursor != NULL)
        {
            stack[stackSize++] = cursor;
            cursor = cursor->firstChild;
        }
        else
        {
            TreeNode *peekNode = stack[stackSize - 1];
            if (peekNode->nextSibling != NULL && lastVisited != peekNode->nextSibling)
            {
                cursor = peekNode->nextSibling;
            }
            else
            {
                cursor = stack[--stackSize];
                cursor->visited = 1; // Mark the current node as visited

                clearScreen();                                 // Clear the screen
                printf("Running post-order traversal...\n\n"); // Print the traversal method

                printTree(root, 0, cursor, ""); // No marked node initially
                suspense(0.5);                  // Wait for 500 milliseconds
                lastVisited = cursor;
                cursor = NULL;
            }
        }
    }
}
// Function to free the memory allocated for the tree
void freeTree(TreeNode *root)
{
    if (root == NULL)
        return;
    freeTree(root->firstChild);
    freeTree(root->nextSibling);
    free(root);
}

// Function to run a traversal method and display the progress
void runningTraversalMethod(TreeNode *root, void (*traversalMethod)(TreeNode *))
{
    suspense(2);

    // Run the traversal method
    traversalMethod(root);

    // Reset visited field of all nodes after traversal
    resetVisited(root);

    printf("\nFinished...\n");
    suspense(3);
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

    clearScreen();

    // Print the tree structure
    printf("Printing the Tree Structure...\n\n");
    suspense(1);
    printTree(root, 0, NULL, "");

    printf("\nNext: Pre-Order Traversal \n\n");
    suspense(5);
    clearScreen();

    // Traverse the tree with cursor highlighting
    runningTraversalMethod(root, preOrderTraversal);
    suspense(5);
    printf("\nNext: Post-Order Traversal n\n");
    clearScreen();

    // Traverse the tree with cursor highlighting
    runningTraversalMethod(root, postOrderTraversal);
    suspense(5);
    clearScreen();

    // Traverse the tree with cursor highlighting
    runningTraversalMethod(root, inOrderTraversal);
    printf("\nNext: nothing. Close the program \n");
    suspense(5);
    clearScreen();

    // Generate a random name from the tree
    const char *randomName = getRandomName(root);

    // Print the randomly generated name
    printf("Random Name: %s\n", randomName);

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

    // Free the memory allocated for the tree
    freeTree(root);

    return 0;
}