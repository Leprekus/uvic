import { expect, test } from 'vitest';
import Graph, { type GraphItems } from './graph';

const items: GraphItems<number> = [
    [1, 2, 1],
    [1, 3, 1],
    [2, 3, 4],
    [3, 4, 5]
];

const G = new Graph<number>(items);
test('adjacent edges are correctly retrieved', () => {

    const adjA = G.adjacent(1);
    expect(adjA.length).toBe(2);

    const adjB = G.adjacent(2);
    expect(adjB.length).toBe(1);

    const adjC = G.adjacent(3);
    expect(adjC.length).toBe(1);

    const adjD = G.adjacent(-1);
    expect(adjD.length).toBe(0);
})

