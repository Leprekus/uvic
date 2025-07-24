
interface PointInterface {
    data: number[];
    distance: (b: PointInterface) => number
    compare: (b: PointInterface) => boolean
}
const Point = (p: number[]): PointInterface => {
    const  distance = (b: PointInterface) => {
        const [xi, yi] = p;
        const [xj, yj ] = b.data;
        return Math.abs(xi - xj) + Math.abs(yi - yj);
    }

    const compare = (b: PointInterface) => {
        const [xi, yi] = p;
        const [xj, yj] = b.data;
        return  xi === xj && yi === yj;
    }
    return {
        data: p,
        distance,
        compare
    };
}

type Edge = { u: PointInterface, v: PointInterface, w: number };

function minCostConnectPoints(points: number[][]): number {
    if(points.length < 2) return 0;
    let pts = points.map(p => Point(p));
    const tree: Edge[] = [];
    let cost = 0;

    while(pts.length) {
        //get vertex not in tree
        const curr = pts.pop()!;
        let min: Edge = {u: Point([]), v: Point([]), w: Infinity};
        //iterate through vertices not in tree
        pts.map(p => {
            console.log(pts.length + 'th iteration: ' + curr.distance(p), 'curr: ', curr.data, 'p: ', p.data);
            if(curr.distance(p) < min.w) min = {u: curr, v: p, w: curr.distance(p)};
        });
        //iterate through vertices in tree (u vertices are in tree) v(may or may not be)
        tree.map(e => {
            if(curr.distance(e.u) < min.w) min = {u: curr, v: e.u, w: curr.distance(e.u)};
        });
        //add vertex to try with minimum weighted edge
        tree.push(min);
        //remove adj vertex v from pts (vertices not in tree)
        pts = pts.filter(p => p.compare(min.v) === false);
        if(min.w < Infinity) cost += min.w;
    };

    console.log(JSON.stringify(tree));
    return cost;
    
};

console.log(minCostConnectPoints([[0,0],[1,1],[1,0],[-1,1]]));
