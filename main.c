#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100

struct graph {
    int nodes[MAX_NODES][MAX_NODES];
    int num_nodes;
};

void read_graph(FILE* file, struct graph* g) {
    char line[256];
    int row = 0;

    // Read each line of the file
    while (fgets(line, sizeof(line), file)) {
        int col = 0;
        char* token = strtok(line, " ");
        while (token) {
            // Convert token to integer
            int value = atoi(token);
            // Store value in graph
            g->nodes[row][col++] = value;
            // Store value in opposite cell to create a loop
            if (col-1 <= row) {
                g->nodes[col-1][row] = value;
            }
            // Move to next token
            token = strtok(NULL, " ");
        }
        // Move to next row
        row++;
    }

    // Set num_nodes to the number of rows
    g->num_nodes = row;
}

void write_dot(FILE* file, struct graph* g) {
    fprintf(file, "digraph G {\n");
    // Add all nodes
    for (int i = 0; i < g->num_nodes; i++) {
        fprintf(file, "  %d;\n", i);
    }
    // Add edges with labels
    for (int i = 0; i < g->num_nodes; i++) {
        for (int j = i; j < g->num_nodes; j++) {
            if (g->nodes[i][j]) {
                fprintf(file, "  %d -> %d [dir=none, label=%d];\n", i, j, g->nodes[i][j]);
            }
        }
    }
    fprintf(file, "}\n");
}

int main() {
    char filename[100];
    struct graph g = {0};

    printf("Enter the filename:\n");
    scanf("%s", filename);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error. Unable to open file.\n");
        return 1;
    }
    read_graph(file, &g);
    fclose(file);
    file = fopen("graph.dot", "w");
    if (file == NULL) {
        fprintf(stderr, "Error. Unable to open file.\n");
        return 1;
    }
    write_dot(file, &g);
    fclose(file);
    system("dot -Tpng graph.dot -o graph.png");
    return 0;
}