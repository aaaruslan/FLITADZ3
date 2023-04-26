#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100

struct graph {
    int nodes[MAX_NODES][MAX_NODES];
    int num_nodes;
};

void dfs(struct graph* g, int v, int visited[]) {
    visited[v] = 1;
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->nodes[v][i] && !visited[i]) {
            dfs(g, i, visited);
        }
    }
}

int is_connected(struct graph* g) {
    int visited[MAX_NODES] = {0};
    dfs(g, 0, visited);
    for (int i = 0; i < g->num_nodes; i++) {
        if (!visited[i]) {
            return 0;
        }
    }
    return 1;
}

void read_graph(FILE* file, struct graph* g) {
    char line[256];
    int row = 0;

    while (fgets(line, sizeof(line), file)) {
        int col = 0;
        char* token = strtok(line, " ");
        while (token) {
            int value = atoi(token);
            g->nodes[row][col++] = value;
            if (col-1 <= row) {
                g->nodes[col-1][row] = value;
            }
            token = strtok(NULL, " ");
        }
        row++;
    }

    g->num_nodes = row;
}

void write_dot(FILE* file, struct graph* g) {
    fprintf(file, "digraph G {\n");
    for (int i = 0; i < g->num_nodes; i++) {
        fprintf(file, "  %d;\n", i);
    }
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
    scanf("%s",filename);
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error. Unable to open file.\n");
        return 1;
    }
    read_graph(file, &g);
    fclose(file);

    if (!is_connected(&g)) {
        fprintf(stderr, "Error. Graph is not connected.\n");
        return 1;
    }

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
