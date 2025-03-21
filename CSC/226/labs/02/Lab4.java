import edu.princeton.cs.algs4.*;


public class Lab4 {
	private IndexMinPQ<Double> mstPQ;
	private IndexMinPQ<Double> dijkstraPQ;
	
	private Edge[] mstEdgeTo;
	private Edge[] dijkstraEdgeTo;
	
	private double[] mstDistTo;
	private double[] dijkstraDistTo;
	
	private boolean[] marked;
	
	private EdgeWeightedGraph G;
	
	private double[] xcoord;
	private double[] ycoord;
	
	public Lab4(EdgeWeightedGraph G) {
		this.G = G;
		mstEdgeTo = new Edge[G.V()];
        mstDistTo = new double[G.V()];
        marked = new boolean[G.V()];
        mstPQ = new IndexMinPQ<Double>(G.V());
        
        dijkstraEdgeTo = new Edge[G.V()];
        dijkstraDistTo = new double[G.V()];
        dijkstraPQ = new IndexMinPQ<Double>(G.V());
        
        for (int v = 0; v < G.V(); v++) {
            mstDistTo[v] = Double.POSITIVE_INFINITY;
            dijkstraDistTo[v] = Double.POSITIVE_INFINITY;
        }
	}
	
	public Lab4(EdgeWeightedGraph G, double[] x, double[] y) {
		this(G);
		xcoord = x;
		ycoord = y;
	}
	
	public void setCoordinates(double[] x, double[] y) {
		this.xcoord = x;
		this.ycoord = y;
	}
	
	
	public void primAndDijkstra(int s, int n) {
		mstDistTo[s] = 0.0;
		mstPQ.insert(s, mstDistTo[s]);
		dijkstraDistTo[s] = 0.0;
		dijkstraPQ.insert(s, dijkstraDistTo[s]);
		int mstEdgeCount = 0;
		int dijkstraEdgeCount = 0;
		while (mstEdgeCount != (n - 1)  || dijkstraEdgeCount != (n - 1) ){
			if (mstEdgeCount < n - 1) {
				int mstVertex = mstPQ.delMin();
				marked[mstVertex] = true;
				for (Edge e : G.adj(mstVertex)) {
					int u = e.other(mstVertex);
					if (marked[u]) continue;
					if (mstDistTo[u] > e.weight()) {
						mstDistTo[u] = e.weight();
						mstEdgeTo[u] = e;
						if (mstPQ.contains(u)) {
							mstPQ.decreaseKey(u, mstDistTo[u]);
						}else {
							mstPQ.insert(u, mstDistTo[u]);
						}
					}
				}
				Edge currentMstEdge = mstEdgeTo[mstVertex];
				if (currentMstEdge != null) {
					mstEdgeCount++;
					StdDraw.setPenColor(StdDraw.RED);
					drawEdge(currentMstEdge, 0);
					StdDraw.pause(1000);
				}
			}
			if (dijkstraEdgeCount < n - 1) {
				int dijkstraVertex = dijkstraPQ.delMin();
				for (Edge e : G.adj(dijkstraVertex)){
					int u = e.other(dijkstraVertex);
					int z = e.other(u);
					if(dijkstraDistTo[u] + e.weight() < dijkstraDistTo[z]){
						dijkstraDistTo[z] = dijkstraDistTo[u] + e.weight();
						dijkstraPQ.changeKey(z, dijkstraDistTo[z]);
					}
				}

				Edge currentDijkstraEdge = dijkstraEdgeTo[dijkstraVertex];
				if (currentDijkstraEdge != null) {
					dijkstraEdgeCount++;
					StdDraw.setPenColor(StdDraw.YELLOW);
					drawEdge(currentDijkstraEdge, 2*n+1);
					StdDraw.pause(1000);
				}
			}
		}
	}
	
	public void drawEdge(Edge e, double x_translation) {
		int u = e.either();
		int v = e.other(u);
		StdDraw.line(x_translation + xcoord[u], ycoord[u], x_translation + xcoord[v], ycoord[v]);
		StdDraw.text(x_translation + 0.5*(xcoord[u] + xcoord[v]) + 0.1, 0.5*(ycoord[u] + ycoord[v]) + 0.1, Double.toString(e.weight()));
	}
	

	public static void main(String[] args) {
		String[] vertices = {"A", "B", "C", "D", "E", "F", "G"};
		int source = 0;
		int n = 7;
		EdgeWeightedGraph G = new EdgeWeightedGraph(n);
		Edge e1 = new Edge(0, 1, 6);
		Edge e2 = new Edge(0, 2, 12);
		Edge e3 = new Edge(1, 2, 3);
		Edge e4 = new Edge(1, 4, 4);
		Edge e5 = new Edge(1, 5, 7);
		Edge e6 = new Edge(1, 6, 4);
		Edge e7 = new Edge(2, 3, 9);
		Edge e8 = new Edge(2, 4, 5);
		Edge e9 = new Edge(4, 5, 2);
		G.addEdge(e1);
		G.addEdge(e2);
		G.addEdge(e3);
		G.addEdge(e4);
		G.addEdge(e5);
		G.addEdge(e6);
		G.addEdge(e7);
		G.addEdge(e8);
		G.addEdge(e9);
		
		double[] x = new double[n];
		double[] y = new double[n];
		
		x[0] = 1;
		y[0] = 18;
		x[1] = 3;
		y[1] = 14;
		x[2] = 6;
		y[2] = 17;
		x[3] = 10;
		y[3] = 16;
		x[4] = 8;
		y[4] = 11;
		x[5] = 4;
		y[5] = 9;
		x[6] = 0;
		y[6] = 12;
		
		Lab4 lab4 = new Lab4(G, x, y);
		StdDraw.setCanvasSize(500, 500);
		StdDraw.clear(StdDraw.BLACK);
		StdDraw.setScale(-1, 28);
		StdDraw.setPenColor(StdDraw.GREEN);
		
		for (int i = 0; i < 7; i++) {
			StdDraw.circle(x[i], y[i], 1);
			StdDraw.text(x[i], y[i], vertices[i]);
			StdDraw.circle(x[i]+15, y[i], 1);
			StdDraw.text(x[i]+15, y[i], vertices[i]);
		}
		
		StdDraw.text(6, 25, "Prim MST");
		StdDraw.text(20, 25, "Dijkstra SPT");
		
		lab4.primAndDijkstra(source, n);

	}

}
