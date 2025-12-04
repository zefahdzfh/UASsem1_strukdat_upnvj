#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ======================
//  STRUCT NODE
// ======================
typedef struct Node {
    char name[100];
    int isFile;               
    struct Node* parent;
    struct Node* firstChild;
    struct Node* nextSibling;
} Node;

// ======================
//  CREATE NODE
// ======================
Node* createNode(const char* name, int isFile) {
    Node* newNode = (Node*) malloc(sizeof(Node));
    strcpy(newNode->name, name);
    newNode->isFile = isFile;
    newNode->parent = NULL;
    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;
    return newNode;
}

Node* root;
Node* currentFolder;

// ======================
// INSERT CHILD
// ======================
void insertChild(Node* parent, Node* child) {
    if (parent == NULL) return;

    child->parent = parent;

    if (parent->firstChild == NULL) {
        parent->firstChild = child;
        return;
    }

    Node* current = parent->firstChild;
    while (current->nextSibling != NULL) {
        current = current->nextSibling;
    }

    current->nextSibling = child;
}

// ======================
// FIND CHILD
// ======================
Node* findChild(Node* parent, const char* name) {
    Node* current = parent->firstChild;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0)
            return current;
        current = current->nextSibling;
    }
    return NULL;
}

// ======================
// COMMANDS
// ======================
void command_mkdir(const char* name) {
    if (findChild(currentFolder, name)) {
        printf("Nama sudah dipakai.\n");
        return;
    }

    Node* folder = createNode(name, 0);
    insertChild(currentFolder, folder);
    printf("Folder '%s' dibuat.\n", name);
}

void command_touch(const char* name) {
    if (findChild(currentFolder, name)) {
        printf("Nama sudah dipakai.\n");
        return;
    }

    Node* file = createNode(name, 1);
    insertChild(currentFolder, file);
    printf("File '%s' dibuat.\n", name);
}

void command_ls() {
    Node* child = currentFolder->firstChild;

    if (!child) {
        printf("(kosong)\n");
        return;
    }

    while (child) {
        if (child->isFile)
            printf("- %s\n", child->name);
        else
            printf("> %s\n", child->name);

        child = child->nextSibling;
    }
}

void command_cd(const char* name) {
    if (strcmp(name, "..") == 0) {
        if (currentFolder->parent)
            currentFolder = currentFolder->parent;
        else
            printf("Sudah di root.\n");
        return;
    }

    Node* target = findChild(currentFolder, name);

    if (!target) {
        printf("Folder tidak ditemukan.\n");
        return;
    }

    if (target->isFile) {
        printf("Tidak bisa masuk ke file.\n");
        return;
    }

    currentFolder = target;
}

// ======================
// PRINT TREE
// ======================
void printTree(Node* root, int depth) {
    if (!root) return;

    for (int i = 0; i < depth; i++)
        printf("   ");

    if (root->isFile)
        printf("- %s\n", root->name);
    else
        printf("> %s\n", root->name);

    printTree(root->firstChild, depth + 1);
    printTree(root->nextSibling, depth);
}

// ======================
// INPUT & COMMAND PARSER
// ======================
void getInput(char* buffer) {
    fgets(buffer, 200, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

void processCommand(char* input) {
    char cmd[50], arg[100];
    int count = sscanf(input, "%s %s", cmd, arg);

    if (count == -1) return;

    if (strcmp(cmd, "mkdir") == 0) command_mkdir(arg);
    else if (strcmp(cmd, "touch") == 0) command_touch(arg);
    else if (strcmp(cmd, "ls") == 0) command_ls();
    else if (strcmp(cmd, "cd") == 0) command_cd(arg);
    else if (strcmp(cmd, "tree") == 0) printTree(currentFolder, 0);
    else if (strcmp(cmd, "exit") == 0) exit(0);
    else printf("Perintah tidak dikenal.\n");
}

// ======================
// MAIN
// ======================
int main() {
    root = createNode("/", 0);
    currentFolder = root;

    char input[200];

    while (1) {
        printf("%s> ", currentFolder->name);
        getInput(input);
        processCommand(input);
    }

    return 0;
}
