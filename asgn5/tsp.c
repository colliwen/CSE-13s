#include "graph.h"
#include "path.h"
#include "stack.h"
#include "vertices.h"

#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "i:o:dh"

bool dfs(uint32_t n, Graph *g, Path *p, Path *best_path, bool *ham) {
    /*
	mark n as visited
	for every one of n's edges:
		if (edge is not visited):
			dfs(edge, g)
	mark n as unvisited
	*/
    graph_visit_vertex(g, n);
    path_add(p, n, g);
    if (path_vertices(p) == graph_vertices(g)) {
        if (graph_get_weight(g, n, START_VERTEX) != 0) {
            path_add(p, START_VERTEX, g);
            if (path_distance(p) < path_distance(best_path)) {
                *ham = true;
                path_copy(best_path, p);
            }
            graph_unvisit_vertex(g, n);
            path_remove(p, g);
            return *ham;
        }
    }
    for (uint32_t edge = 0; edge < graph_vertices(g); edge++) {
        if (graph_visited(g, edge) == false && graph_get_weight(g, n, edge) != 0) {
            dfs(edge, g, p, best_path, ham);
        }
    }
    graph_unvisit_vertex(g, n);
    path_remove(p, g);
    return *ham;
    /*
	}
	else if (n + 1 <= graph_vertices(g) && graph_get_weight(g, n, n + 1) != 0) {
		n++;
		dfs(n, g, p, best_path, ham);
	}
	else {
		graph_unvisit_vertex(g, n);
		path_remove(p, g);
		n--;
		dfs(n, g, p, best_path, ham);
	}
	return *ham;

	*/

    /*	
    if (path_vertices(p) < graph_vertices(g)) {
        graph_visit_vertex(g, n);
        path_add(p, n, g);    
	n++;
        if (graph_visited(g, n) == false) {
            dfs(n, g, p, best_path, ham);
        }
    } else if (graph_get_weight(g, n, START_VERTEX) != 0) {
        path_add(p, START_VERTEX, g);
        if (path_distance(p) < path_distance(best_path)) {
            *ham = true;
	    path_copy(best_path, p);
	    printf("%u\n", path_distance(best_path));
	    
	    printf("works?\n");

            graph_unvisit_vertex(g, n);
	    path_remove(p, g);
        }
    } else {
        graph_unvisit_vertex(g, n);
	path_remove(p, g);
    }
    return *ham;
    */
}

int main(int argc, char **argv) {
    int opt;

    char *usage = "Usage: tsp [options]\n\
\n\
-i infile    Specify the input file path containing the cities and edges\n\
             of a graph. If not specified, the default input should be\n\
             set as stdin.\n\
\n\
-o outfile   Specify the output file path to print to. If not specified,\n\
             the default output should be set as stdout.\n\
\n\
-d           Specifies the graph to be directed.\n\
\n\
-h           Prints out a help message describing the purpose of the\n\
             graph and the command-line options it accepts, exiting the\n\
             program afterwards.\n";
    FILE *infile = stdin;
    FILE *outfile = stdout;
    bool user_infile = false;
    bool user_outfile = false;
    bool directed = false;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            user_infile = true;
            char *infile_name = optarg;
            infile = fopen(infile_name, "r");
            if (infile == NULL) {
                fprintf(stderr, "tsp:  error reading input file %s\n%s", infile_name, usage);
                return 1;
            }
            break;
        case 'o':
            user_outfile = true;
            char *outfile_name = optarg;
            outfile = fopen(outfile_name, "w");
            break;
        case 'd': directed = true; break;
        case 'h': printf("%s", usage); return 0;
        default: fprintf(stderr, "%s", usage); return 1;
        }
    }

    (void) user_infile;
    (void) user_outfile;

    uint32_t numVerts;
    uint32_t numEdges;
    uint32_t start;
    uint32_t end;
    uint32_t weight;

    if (fscanf(infile, "%u\n", &numVerts) != 1) {
        fprintf(stderr, "tsp:  error reading number of vertices\n");
        return 1;
    }
    Graph *g = graph_create(numVerts, directed);

    char *vertName = calloc(100, sizeof(char));
    for (uint32_t i = 0; i < numVerts; i++) {
        vertName = calloc(100, sizeof(char));
        fgets(vertName, 100, infile);
        graph_add_vertex(g, vertName, i);
        free(vertName);
    }

    if (fscanf(infile, "%u", &numEdges) != 1) {
        fprintf(stderr, "tsp:  must provide number of edges\n");
        return 1;
    }
    for (uint32_t i = 0; i < numEdges; i++) {
        if (fscanf(infile, "%u %u %u", &start, &end, &weight) != 3) {
            fprintf(stderr, "tsp:  error reading edge\n");
            return 1;
        }
        graph_add_edge(g, start, end, weight);
    }
    fclose(infile);
    graph_print(g);

    Path *p = path_create(numVerts + 1);
    Path *best_path = path_create(numVerts + 1);

    bool ham = false;
    printf("%d\n", dfs(START_VERTEX, g, p, best_path, &ham));
    if (dfs(START_VERTEX, g, p, best_path, &ham) == true) {
        fprintf(outfile, "Alissa starts at:\n");
        path_print(best_path, outfile, g);
        fprintf(outfile, "Total Distance: %u\n", path_distance(best_path));
        fclose(outfile);
    }

    else {
        fprintf(outfile, "No path found! Alissa is lost!\n");
        fclose(outfile);
    }

    return 0;
}
