#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for usleep

#define RESET "\033[0m"              // Resets the text formatting back to the default after printing the highlighted text.
#define HIGHLIGHT "\033[46m\033[30m" // Bright cyan background with black text
#define VISITED "\033[42m\033[30m"   // Bright green background with black text

// Define the structure for a tree node
typedef struct TreeNode
{
    char name[50];
    struct TreeNode *firstChild;
    struct TreeNode *nextSibling;
    int visited; // Field to mark if the node has been visited
} TreeNode;

// Function to create a new tree node
TreeNode *createNode(const char *name)
{
    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
    strcpy(newNode->name, name);
    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;
    newNode->visited = 0; // Initialize visited to 0 (false)
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

// Function to reset the visited field of all nodes in the tree
void resetVisited(TreeNode *root)
{
    if (root == NULL)
        return;
    root->visited = 0;               // Reset visited field of current node
    resetVisited(root->firstChild);  // Recursively reset visited field of children
    resetVisited(root->nextSibling); // Recursively reset visited field of siblings
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
        printf("%s ✅\n", root->name);
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
        clearScreen();                  // Clear the screen
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
            cursor->visited = 1;            // Mark the current node as visited
            clearScreen();                  // Clear the screen
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
                cursor->visited = 1;            // Mark the current node as visited
                clearScreen();                  // Clear the screen
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
void runningTraversalMethod(TreeNode *root, void (*traversalMethod)(TreeNode *), const char *name)
{
    printf("Running %s traversal...\n\n", name);
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
    TreeNode *root = createNode("Animalia");

    // Add children to the root node
    TreeNode *chordata = createNode("Chordata");
    TreeNode *arthropoda = createNode("Arthropoda");
    TreeNode *mollusca = createNode("Mollusca");
    addChild(root, chordata);
    addChild(root, arthropoda);
    addChild(root, mollusca);

    // Add children to the Chordata node
    TreeNode *mammalia = createNode("Mammalia");
    TreeNode *aves = createNode("Aves");
    TreeNode *reptilia = createNode("Reptilia");
    addChild(chordata, mammalia);
    addChild(chordata, aves);
    addChild(chordata, reptilia);

    // Add children to the Mammalia node
    TreeNode *primates = createNode("Primates");
    TreeNode *carnivora = createNode("Carnivora");
    addChild(mammalia, primates);
    addChild(mammalia, carnivora);

    // Add children to the Aves node
    TreeNode *passeriformes = createNode("Passeriformes");
    TreeNode *psittaciformes = createNode("Psittaciformes");
    addChild(aves, passeriformes);
    addChild(aves, psittaciformes);

    // Add children to the Reptilia node
    TreeNode *squamata = createNode("Squamata");
    TreeNode *testudines = createNode("Testudines");
    addChild(reptilia, squamata);
    addChild(reptilia, testudines);

    // Add children to the Arthropoda node
    TreeNode *insecta = createNode("Insecta");
    TreeNode *arachnida = createNode("Arachnida");
    TreeNode *crustacea = createNode("Crustacea");
    addChild(arthropoda, insecta);
    addChild(arthropoda, arachnida);
    addChild(arthropoda, crustacea);

    // Add children to the Insecta node
    TreeNode *coleoptera = createNode("Coleoptera");
    TreeNode *diptera = createNode("Diptera");
    addChild(insecta, coleoptera);
    addChild(insecta, diptera);

    // Add children to the Arachnida node
    TreeNode *araneae = createNode("Araneae");
    TreeNode *scorpiones = createNode("Scorpiones");
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
    runningTraversalMethod(root, preOrderTraversal, "pre-order");
    suspense(2);
    printf("\nNext: Post-Order Traversal n\n");
    clearScreen();

    // Traverse the tree with cursor highlighting
    runningTraversalMethod(root, postOrderTraversal, "post-order");
    suspense(2);
    clearScreen();

    // Traverse the tree with cursor highlighting
    runningTraversalMethod(root, inOrderTraversal, "in-order");
    printf("\nNext: nothing. Close the program \n");
    suspense(2);
    clearScreen();

    // Free the memory allocated for the tree
    freeTree(root);

    return 0;
}