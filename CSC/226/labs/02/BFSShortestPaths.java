import edu.princeton.cs.algs4.*;

public class BFSShortestPaths {

    public static void main(String[] args) {

        In in = new In(args[0]);

        Graph G = new Graph(in);

        

        int s = 0; // start vertex

        // TODO: instantiate a new BFS object
        BreadthFirstPaths bfs = new BreadthFirstPaths(G, s);

        // For each vertex v in G, if there is an undirected path from s to v, print distance of the shortest undirected path from s to v.
        for(int i = 0; i < G.V(); i++){
            if(G.adj(i))

        } 

        // Otherwise, if there is no path from startVertex to v, print -1
        

        // TODO:
        
        // extend this idea to create an n x n array of shortest paths sp, where sp[u][v] equals the distance of shortest path from u to v

        // or s[u][v] equals -1 if there is no path from u to v
  


        for (int u = 0; u < sp.length; u++) {

            System.out.print(u + ": ");

            for (int v = 0; v < sp.length; v++) {

                System.out.print(" " + v + " (" + sp[u][v] + ") ");

            }

            System.out.println();

        }

    }

}